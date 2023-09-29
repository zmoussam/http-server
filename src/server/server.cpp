# include "Server.hpp"
# include "CGIHandler.hpp"
# include <poll.h>
# include <map>
# include <ctime>
# include <fcntl.h>

std::map<std::string, std::string> mimeTypes = getMimeTypes();

/**
 * Constructor for the Server class.
 * Initializes the server socket and sets up the server address.
 *
 * @param address The server address to bind to.
 * @param port The port number to listen on.
 */
Server::Server(int port, std::vector<ServerConf> &servers) : _port(port), _serverConf(servers) {
    int reuse = 1;
    int set = 1;
    _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
    if (_serverSocket < 0) {
        return;
    }
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAddress.sin_port = htons(_port);

    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0) {
        return;
    }
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int)) < 0) {
        return;
    }
    if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0) {
        return;
    }
}

/**
 * Destructor for the Server class.
 * Closes the server socket.
 */
Server::~Server() {
}

Server::Server(const Server& other) : _requests(), _responses(), _clients(), _serverConf(other._serverConf)
{
    _port = other._port;
    _serverSocket = other._serverSocket;
    _serverAddress = other._serverAddress;
    _maxFd = other._maxFd;
}

Server& Server::operator=(const Server& other) {

    _port = other._port;
    _serverSocket = other._serverSocket;
    _serverAddress = other._serverAddress;
    _serverConf = other._serverConf;
    return *this;
}

void Server::setPort(int port) {
	_port = port;
}

int Server::getPort() const {
	return _port;
}

int Server::getSocket() const {
    return _serverSocket;
}

 unsigned int Server::getServerAddress() const {
    return _serverAddress.sin_addr.s_addr;
}

/**
 * Starts the server by listening for incoming client connections.
 * Handles client _requests in an infinite loop.
 */
int Server::start(void) {
    if (listen(_serverSocket, 1024) < 0) {
        return ERROR;
    }
    std::cout << "Server started on port: " << _port << std::endl;

    return OK;
}



int Server::addToSets(fd_set& masterSet) {
    for (size_t i = 0; i < _clients.size(); i++) {
        FD_SET(_clients[i].socket, &masterSet);
    }
    return OK;
}


int Server::handleClients(fd_set& readSet, fd_set& writeSet, fd_set& masterSet) {
    if (FD_ISSET(_serverSocket, &readSet)) {
        int clientSocket = accept(_serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            return ERROR;
        }
        acceptClientConnection(clientSocket, masterSet);
    }
    time_t currentTime = time(NULL);
    int res = 0;
    for (size_t i = 0; i < _clients.size(); i++) {
        int clientSocket = _clients[i].socket;

        if (FD_ISSET(clientSocket, &readSet)) {
            int req = _requests[clientSocket].handleRequest(_port);
            _clients[i].lastActivity = time(NULL);
            _clients[i].keepAlive = _requests[clientSocket].isKeepAlive();
            if (req == DISCONNECTED) {
                closeClientConnection(clientSocket, i, masterSet);
                res = 0;
            }
            
        }

        if (FD_ISSET(clientSocket, &writeSet) && _requests[clientSocket].isHeadersRead() && _requests[clientSocket].isBodyRead()) {
            if ((_requests[clientSocket].getPath().find(".py") != std::string::npos || _requests[clientSocket].getPath().find(".rb") != std::string::npos) && _requests[clientSocket].getError() == 0) {
                res = _cgis[clientSocket].CGIHandler(_requests[clientSocket], _responses[clientSocket], clientSocket);
                if (_cgis[clientSocket].isCgiDone()) {
                    res == -2 ? _cgis[clientSocket].setisCgiDone(false) : _cgis[clientSocket].setisCgiDone(true);
                    res = _responses[clientSocket].sendResp(_requests[clientSocket], &_cgis[clientSocket]);
                    unlink(_cgis[clientSocket].getCgiFd().c_str());
                }
            } else {
                res = _responses[clientSocket].sendResp(_requests[clientSocket], NULL);
            }
            _clients[i].lastActivity = time(NULL);

            if (res == DONE) {
                closeClientConnection(clientSocket, i, masterSet);
                res = 0; 
            }
        } 
        if (currentTime - _clients[i].lastActivity > MAX_IDLE_TIME) {
            closeClientConnection(clientSocket, i, masterSet);
        }
    }
    return OK;
}

void Server::closeClientConnection(int clientSocket, size_t index, fd_set& masterSet) {
    FD_CLR(clientSocket, &masterSet);
    _clients.erase(_clients.begin() + index);
    _responses.erase(clientSocket);
    _requests.erase(clientSocket);
    unlink(_cgis[clientSocket].getCgiFd().c_str());
    _cgis.erase(clientSocket);
    close(clientSocket);
}

void    Server::acceptClientConnection(int clientSocket, fd_set& masterSet) {
    FD_SET(clientSocket, &masterSet);
    Client client;
    client.socket = clientSocket;
    client.keepAlive = false;
    client.lastActivity = time(NULL);
    _clients.push_back(client);
    FD_SET(clientSocket, &masterSet);
    _requests[clientSocket] = Request(clientSocket, _serverConf);
    _responses[clientSocket] = Response(clientSocket, _serverConf);
    _cgis[clientSocket] = CGI(clientSocket, _serverConf);
}