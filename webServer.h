#ifndef HEADER_H
#define HEADER_H 

#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "logging.h"

#define GET 1
#define HEAD 2
#define POST 3

#define PORT 1701

inline int BUFFER_SIZE = 10;
const char* TERM_STRING = "CLOSE\r\n";
std::regex GET_REGEX("^(GET)\\s+([^\\s]+)\\s(HTTP/1.1)\r\n$");
std::regex FILE_REGEX("(^/file[0-9].html$)|(^/image[0-9].jpg$)");
// const char* GET_REQ_START = "GET /";
// int GET_REQ_START_LEN = strlen(GET_REQ_START);
// const char* GET_REQ_END = " HTTP/1.1\r\n";
// int GET_REQ_END_LEN = strlen(GET_REQ_END);

#endif
