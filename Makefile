# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/05 19:25:52 by zmoussam          #+#    #+#              #
#    Updated: 2023/08/05 19:33:58 by zmoussam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 
LDFLAGS =

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

TARGET = webserv

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(addprefix $(OBJ_DIR)/,$(patsubst $(SRC_DIR)/%,%,$(SRCS:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(*D)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all

.PHONY: all clean fclean re
