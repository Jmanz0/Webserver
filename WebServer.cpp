#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ThreadPool.h"
#include "WebServer.h"

void WebServer::start() {
    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    loadFileContent();

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 2000);

    std::cout << "Server started at address: 127.0.0.1, port: 8080" << std::endl;

    ThreadPool pool(2000);

    while (true) {
        clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

        pool.enqueueTask([this, clientSocket]() {
            handleRequest(clientSocket);
        });
    }

    close(serverSocket);
}

void WebServer::handleRequest(int clientSocket) {
    std::string request;
    char buffer[512];
    ssize_t bytesRead;
    while ((bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        request += buffer;
        if (request.find("\r\n\r\n") != std::string::npos) {
            break; 
        }
    }

    std::istringstream iss(request);
    std::string requestLine;
    std::getline(iss, requestLine);

    std::istringstream requestLineStream(requestLine);
    std::string method, path, protocol;
    requestLineStream >> method >> path >> protocol;

    std::string filename;
    if (path == "/") {
        if (!fileContent.empty()) {
            std::ostringstream response;
            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: text/html\r\n";
            response << "Content-Length: " << fileContent.length() << "\r\n";
            response << "\r\n";

            response << fileContent;
            write(clientSocket, response.str().c_str(), response.str().length());
            close(clientSocket);
            return;
        }

        filename = "index.html"; 
    } else {
        filename = path.substr(1); 
    }

    std::ifstream file(filename, std::ios::binary);
    if (file) {
        std::ostringstream oss;
        oss << file.rdbuf();
        std::string content = oss.str();

        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << content.length() << "\r\n";
        response << "\r\n";

        response << content;
        write(clientSocket, response.str().c_str(), response.str().length());
    } else {
        std::string notFoundResponse =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 9\r\n"
            "\r\n"
            "Not Found";
        write(clientSocket, notFoundResponse.c_str(), notFoundResponse.length());
    }

    close(clientSocket);
}

void WebServer::loadFileContent() {
    std::ifstream file("index.html", std::ios::binary);
    if (file) {
        std::ostringstream oss;
        oss << file.rdbuf();
        fileContent = oss.str();
    } else {
        std::cerr << "Failed to load file content." << std::endl;
    }
}


int main() {
    WebServer server;
    server.start();

    return 0;
}
