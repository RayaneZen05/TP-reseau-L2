#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9600

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[64];
    char guess[10];
    char SERVER_IP[16];
    sprintf(SERVER_IP, "%s", argv[1]);
    printf("IP du serveur : %s\n", SERVER_IP);
    // socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }

    // Connect
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    while (1) {
        printf("Devinez un nombre entre 1 et 100 : ");
        fflush(stdout); //! flush la sortie standard pour éviter d'avoir des entrées indésirables lors de buffer overflow d'autres programmes etc
        if (!fgets(guess, sizeof(guess), stdin)) break; // on utilise fgets au lieu de read pour lire jusqu'au \n, rajoute ensuite un 0 (fin de chaine)

        // on envoie le guess
        if (send(sockfd, guess, strlen(guess), 0) < 0) {
            perror("send");
            break;
        }

        // on reçoit la réponse
        if (recv(sockfd, buffer, sizeof(buffer), 0) <= 0) {
            perror("recv");
            break;
        }
        printf("%s", buffer);

        // si c'est juste, on s'arrête -> c'est juste lorsqu'on recoit "Félicitations"
        if (strstr(buffer, "Félicitations") != NULL) { // cherche le substring "Félicitations" dans buffer
            break;
        }
    }

    close(sockfd);
    return 0;
}