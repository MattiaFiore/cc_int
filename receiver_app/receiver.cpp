// UDP Receiver and Echo
#include <iostream>
#include <cstring>       // For memset()
#include <sys/socket.h>
#include <arpa/inet.h>   // For inet_addr() and inet_ntoa()
#include <unistd.h>      // For close()
#include <stdlib.h>      // For exit()

int main() {
    using std::cout;
    
    // Create a UDP socket.
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to local IP 10.0.0.2 and port 55556.
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(55556);
    localAddr.sin_addr.s_addr = inet_addr("10.0.0.2");
    
    if (bind(sockfd, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    cout << "Receiver socket bound to 10.0.0.2:55556\n";
    
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    
    // Loop indefinitely to receive and echo packets.
    for (;;) {
        struct sockaddr_in senderAddr;
        socklen_t senderAddrLen = sizeof(senderAddr);
        ssize_t recvBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                     (struct sockaddr *)&senderAddr, &senderAddrLen);
        if (recvBytes < 0) {
            perror("recvfrom");
            continue; // Optionally, break or handle the error.
        }
        
        // Null-terminate the received data (if treating as a string)
        if (recvBytes < BUFFER_SIZE) {
            buffer[recvBytes] = '\0';
        }
        
        cout << "Received " << recvBytes << " bytes from "
             << inet_ntoa(senderAddr.sin_addr) << ":"
             << ntohs(senderAddr.sin_port) << "\n";
        cout << "Data: " << buffer << "\n";
        
        // Echo the packet back to the sender.
        ssize_t sentBytes = sendto(sockfd, buffer, recvBytes, 0,
                                   (struct sockaddr *)&senderAddr, senderAddrLen);
        if (sentBytes < 0) {
            perror("sendto");
        } else {
            cout << "Echoed " << sentBytes << " bytes back to sender.\n";
        }
    }
    
    close(sockfd);
    return 0;
}
