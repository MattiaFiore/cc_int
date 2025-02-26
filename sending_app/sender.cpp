// Sender udp 
#include <iostream>
#include <sys/errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstring> 
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>     // For close()
#include <stdlib.h>     // For exit()
#include <chrono>
#include <thread>

int main(){

    using std::cout; 
    // Porta 
    int sockfd; 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));   // Zero out the structure
    addr.sin_family = AF_INET;          // IPv4
    addr.sin_port = htons(55555);       // Port 55555 in network byte order
    addr.sin_addr.s_addr = inet_addr("10.0.0.1"); // Bind to IP address 10.0.0.1

    // Bind the socket to the specified IP and port
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

     std::cout << "Socket bound to 10.0.0.1:55555\n";

    // Prepare the destination address
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(55556); // destination port
    destAddr.sin_addr.s_addr = inet_addr("10.0.0.2"); // destination IP

    const char* message = "Hello, world!";
    for (;;){
        ssize_t sentBytes = sendto(sockfd, message, strlen(message), 0,
                               (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (sentBytes < 0) {
            perror("sendto");
        } else {
            std::cout << "Sent " << sentBytes << " bytes\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0; 
}