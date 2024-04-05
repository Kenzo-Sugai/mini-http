#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <unistd.h>
#include <thread>
#include <ws2tcpip.h>

using namespace std;

void hello(){

    cout << "THREAD!!!" << endl;

}

int main() {

    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    char opt = '1';
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons("8080");

    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
                 = accept(server_fd, (struct sockaddr*)&address,
                          &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer,
                   1024 - 1); // subtract 1 for the null
    // terminator at the end
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);

    thread t1(hello);
    t1.join();

    return 0;

}
