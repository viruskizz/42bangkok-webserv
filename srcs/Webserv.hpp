#pragma once

# include "../srcs/conf/Config.hpp"
# include "../srcs/utils/StringUtil.hpp"
# include <sys/socket.h>
# include <sys/signal.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <dirent.h>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <netdb.h>
# include <fcntl.h>
# include <cstdio>
# include "./server/HttpRequest.hpp"
# include <cstdlib>
# include <csignal>
# include <sys/types.h>
# include <sys/wait.h>
# include <vector>

// #include <iostream>
# define BREAK_LINE (char *)"\r\n"
# define READSIZE 100000
# define HTTP (char *)"HTTP/1.1 "
# define ROOT (char *)"/Users/shivarakii/Documents/42_coding/webserv_42/html"
// # define ROOT (char *)"/home/araiva/42bangkok/webserv/html"
/*
* marco for exitWithError() EE_PERR with perror() | EE_NONE just string*/
# define EE_PERR 1
# define EE_NONE 0

/*
* marco for stringJoin()
* SJ_NONE -> not free.
* SJ_FFIR -> free first argument.
* SJ_FSEC -> free secound argument.
* SJ_FFIR -> free both argument. */
# define SJ_NONE 0
# define SJ_FFIR 1
# define SJ_FSEC 2
# define SJ_BOTH 3

/*
* marco for stringAdd()
* SJ_NONE -> not free.
* SJ_FSTS -> free strings.
* SJ_FSTR -> free string.
* SJ_BOTH -> free both argument.*/
# define SA_NONE 0
# define SA_FSTS 1
# define SA_FSTR 2
# define SA_BOTH 3

/*
* marco for Method of RequestBody */
# define NONE	0
# define PASSIN	1
# define UPLOAD	2

char						*strjoin(char *str1, int lenStr1, char *str2, int lenStr2, int mode);
char						*strjoin(char *str1, char *str2, int mode);
std::string					intToString(int number);
int 						stringToint(std::string string);
char						*itoa(int number);
char						**stringAdd(char **strings, char *string, int mode);
char						*stringDuplicate(char *string);
int							stringsCount(char **strings);
void 						freeStrings2Star(char **strings);
std::string					stringTrim(std::string &string, std::string const &delimeters);
std::vector<std::string>	split(std::string const &string, char delimeter);
char						*stringTosChar(std::string const &string);
char						**vectorStringToChar(std::vector<std::string> vector);