#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ThreadPool.h"

class WebServer {
public:
    void start();

private:
    void handleRequest(int clientSocket);
    void loadFileContent();

    int serverSocket;
    std::string fileContent;
    struct sockaddr_in serverAddress;
};
