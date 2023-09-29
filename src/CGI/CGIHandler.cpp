# include "CGIHandler.hpp"
CGI::CGI()
{
  _root = "www/";
  _redirect = "";
  _cgi_path = "";
  _compiler = "";
  _headersSent = false;
  _error_code = 0;
  _status_code = "";
  _isCgiDone = false;
  _cgiRan = false;
  _fd = 0;
  _pid = 0;
  _status = 0;
}

CGI::CGI(int clientSocket, std::vector<ServerConf> &servers) : _servers(servers)
{
  _root = "www/";
  _redirect = "";
  _cgi_path = "";
  _compiler = "";
  _clientSocket = clientSocket;
  _headersSent = false;
  _error_code = 0;
  _status_code = "";
  _isCgiDone = false;
  _cgiRan = false;
  _fd = 0;
  _cookies = std::vector<std::string>();
}

CGI::~CGI()
{

}
std::string getFileExtension(const std::string& filename)
{
    size_t dotPosition = filename.find_last_of(".");
    
    if (dotPosition != std::string::npos) {
        return filename.substr(dotPosition);
    }
    return "";
}

void CGI::initHeaders() {
  _headers["Server"] = "luna/1.0";
  _headers["Content-Type"] = "text/html";
  if (!(_redirect.empty())) {
    _headers["Location"] = _redirect;
    _error_code = 301;
  }
  _headers["Connection"] = "keep-alive";
}

void    CGI::findConfig(Request &req)
{
    const std::map<std::string, std::string> &headers = req.getHeaders();
    if (headers.find("Host") != headers.end())
    {
        int port = req.getPort();
        std::string host = headers.find("Host")->second;
        for (std::vector<ServerConf>::iterator it = _servers.begin(); it != _servers.end(); it++)
        {
            if (it->getString(SERVER_NAME) == host || it->getString(SERVER_NAME) + ":" + std::to_string(port) == host) {
                _config = *it;
                return;
            }
        }
    }
    _config = _servers[0];
}

int CGI::initializeCGIParameters(Request &req, Response &resp) {
  findConfig(req);
  _cgi_path = req.getPath();
  _filename = _cgi_path.substr(_cgi_path.find_last_of('/') + 1);
  std::string fileExtension = "";
  std::vector<Location>::iterator it = _config.location.begin();
  std::string extension = getFileExtension(_filename);
  if (extension.empty())
  {
    _error_code = 500;
    return 2;
  }
  while (it != resp._config.location.end())
  {
    if (it->getLocationName().empty())
    {
      _error_code = 500;
      return 2;
    }
    std::string cgiLocation = it->getLocationName().substr(1);
    if (cgiLocation == extension && it->getLocationName()[0] == '*')
    {
      _methods = it->getMethods();
      if (!it->getString(ROOT).empty())
        _root = it->getString(ROOT) + (it->getString(ROOT).back() != '/' ? "/" : "");
      _compiler = it->getCompiler();
      _redirect = it->getReturned();
      break;
    }
    else if (it == resp._config.location.end())
    {
      _error_code = 500;
      return 2;
    }
    it++;
  }
  std::vector<std::string>::iterator it_meth = _methods.begin();
  while (it_meth != _methods.end())
  {
    if (*it_meth == req.getMethod())
      break;
    it_meth++;
  }
  if (it_meth == _methods.end())
  {
    _error_code = 405;
    return 2;
  }
  if (_compiler.empty())
  {
    _error_code = 500;
    return 2;
  }
  initHeaders();
  return 0;
}

std::vector<std::string> CGI::getCookies() const
{
  return _cookies;
}

std::string CGI::parseCookies(std::string cookies)
{
  std::string setCookie = "Set-Cookie:";
  std::string::size_type pos = cookies.find(setCookie);
  while (pos != std::string::npos)
  {
    std::string::size_type pos2 = cookies.find("\r\n", pos);
    if (pos2 != std::string::npos)
    {
      std::string value = cookies.substr(pos + setCookie.length(), pos2 - pos - setCookie.length());
      _cookies.push_back(value);
      cookies.erase(pos, pos2 - pos);
    }
    pos = cookies.find(setCookie, pos2 + 2);
  }
  return cookies;
}

void CGI::parseHeaders(std::string headers)
{
  std::string headerKey[5] = {"Content-Type", "Content-Length", "Location", "Server", "Connection"};
  for (int i = 0; i < 5; i++)
  {
    if (headers.find(headerKey[i]) != std::string::npos)
    {
      std::string::size_type pos = headers.find(headerKey[i]);
      std::string::size_type pos2 = headers.find("\r\n", pos);
      std::string value = headers.substr(pos + headerKey[i].length() + 1, pos2 - pos - headerKey[i].length() - 1);
      _headers[headerKey[i]] = value;
    }
  }
}

int CGI::handlePostMethod(Request &req) {
  if (req.getMethod() == "POST")
  {
    std::string tmpfile = std::to_string(getpid()) + ".txt";
    std::string body = req.getBody();
    std::ofstream ofs(tmpfile);
		if (!ofs.is_open()) {
      _error_code = E500;
      return -1;
    }
		ofs << body;
		ofs.close();
		int fdf = open(tmpfile.c_str(), O_RDWR);
		if (fdf == -1) {
      _error_code = E500;
      return -1;
    }
		if (dup2(fdf, STDIN_FILENO) == -1) {
      _error_code = E500;
      return -1;
    }
		close(fdf);
    if (req.getHeaders().find("Content-Length") != req.getHeaders().end())
      setenv("CONTENT_LENGTH", req.getHeaders()["Content-Length"].c_str(), 1);
    unlink(tmpfile.c_str());
  }
  return 0;
}

int CGI::executeCGIScript(int clientSocket) {
    unused(clientSocket);
    std::string path = _root + _filename;
    if (access(path.c_str(), F_OK) == -1) {
        _error_code = E404;
        return -1;
    }
    std::string command = _compiler + _root + _filename;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        _error_code = E500;
        return -1;
    }
    
    char buffer[255];
    std::string body;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        body += buffer;
    std::string headers[] = {"Content-Type", "Content-Length", "Location", "Set-Cookie", "Server", "Connection"};
    for (int i = 0; i < 6; i++)
    {
      if (body.find("\r\n\r\n") != std::string::npos && body.find(headers[i]) != std::string::npos)
      {
        std::string::size_type pos = body.find("\r\n\r\n");
        if (pos != std::string::npos)
        {
          int fd = open(COOKIFILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
          if (fd == -1) {
            _error_code = E500;
            return -1;
          }
          std::string tmp = body.substr(0, pos);
          tmp += "\r\n\r\n";
          tmp +=  '\0';
          if (write(fd, tmp.c_str(), tmp.size()) <= 0) {
            _error_code = E500;
            return -1;
          }
          close(fd);
          body.erase(0, pos + 4);
        }
        break;
      }
    }
    pclose(pipe);
  
    if (_fd != -1) {
        if (write(_fd, body.c_str(), body.size()) <= 0)
        {
          _error_code = E500;
          return -1;
        }
    } else {
        _error_code = E500;
        return -1;
    }
    return 0;
}
int CGI::CGIHandler(Request &req, Response &resp, int clientSocket)
{
  if (_cgiRan == false) {
    int random = rand();
    _cgifd = "/tmp/" + std::to_string(random) + ".cgi";
    _pid = fork();
    int res = initializeCGIParameters(req, resp);
    if (res != 0)
    {
      _isCgiDone = true;
      return (res);
    }
    _fd = open(_cgifd.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (_pid == 0) {
      if (handlePostMethod(req) != 0)
        exit (_error_code);
      if (req.getCookies().size() > 0)
        setenv("HTTP_COOKIE", req.getCookies().c_str(), 1);
      std::map<std::string, std::string> headers = req.getHeaders();
      for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
      {
        if (it->first == "Content-Length")
          continue;
        std::string key = HTTP_ENV + it->first;
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        setenv(key.c_str(), it->second.c_str(), 1);
      }
      setenv("REQUEST_METHOD", req.getMethod().c_str(), 1);
      setenv("REQUEST_URI", req.getPath().c_str(), 1);
      setenv("QUERY_STRING", req.getQueries().c_str(), 1);
      setenv("SCRIPT_NAME", req.getPath().c_str(), 1);
      setenv("GATEWAY_INTERF", "CGI/1.1", 1);
      setenv("PATH_INFO", req.getPath().c_str(), 1);
      setenv("SCRIPT_NAME", _cgi_path.c_str(), 1);
      setenv("REDIRECT_STATUS", "", 1);
      if (executeCGIScript(clientSocket) != 0)
        exit (_error_code);
      exit(0);
    }
  }
  if (waitpid(_pid, &_status, WNOHANG) == -1)
  {
      if (WIFEXITED(_status)) {
        _status = WEXITSTATUS(_status);
      (_error_code = RESET_ERROR_CODE + _status) && _error_code == RESET_ERROR_CODE ? _error_code = 0 : _error_code;
      _isCgiDone = true;
      if (_fd == -1)
        _error_code = 500;
      int fd = open(COOKIFILE, O_RDONLY);
      if (access(COOKIFILE, F_OK) != -1)
      {
        if (fd == -1)
          _error_code = 500;
        char buffer[128];
        std::string body = "";
        while (read(fd, buffer, sizeof(buffer) - 1) > 0){
          buffer[sizeof(buffer) - 1] = '\0';
          body += buffer;
        }
        body = parseCookies(body);
        parseHeaders(body);
        close(fd);
        unlink(COOKIFILE);
      }
    }
    else
      return (CONTINUE);
  }
  _cgiRan = true;
  return (CONTINUE);
}
