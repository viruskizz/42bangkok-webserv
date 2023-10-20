NAME = webserv
CXX = c++ -g
CPPFLAGS = -Wall -Werror -Wextra -std=c++98
CPPFLAGS = -std=c++98

YELLOW ="\033[0;33m"
RESET = "\033[0m"

BUILD_DIR	= build
SRC_DIR		= srcs
# SRCS 		= $(shell find $(SRC_DIR) -name '*.cpp')

# For dev config
SRCS 		=	srcs/main.cpp \
				srcs/conf/Config.cpp \
				srcs/conf/ServerConf.cpp \
				srcs/utils/StringUtil.cpp \
				srcs/utils/WebservUtil.cpp \
				srcs/server/HttpRequest.cpp \
				srcs/server/RequestBody.cpp \
				srcs/server/Server.cpp \
				srcs/server/HttpRespond.cpp \
				srcs/server/CommonGatewayInterface.cpp \
				srcs/server/Cookie.cpp


OBJS 		= $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

INCLUDE_DIR	= ./includes
INCLUDES = -I$(INCLUDE_DIR)

all: $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) $(INCLUDES) -o $(NAME)
	@echo file: $(YELLOW)$(NAME)$(RESET) has created

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
