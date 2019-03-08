#pragma once
#include <cstdlib>
#include <string>

ssize_t readn(int fd,void* buffer,size_t n);
ssize_t writen(int fd,void* buffer,size_t n);
int setSocketNonBlocking(int fd);
int socket_bind_listen(int port);