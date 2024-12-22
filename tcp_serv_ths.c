#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9600
#define BACKLOG 1

int main() {
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // création de la socket tcp
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // création de l'adresse
    server_addr.sin_family = AF_INET;
    // INADDR_ANY pour ne pas avoir besoin de connaitre l'adresse de l'hôte
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // Listen
    if (listen(sockfd, BACKLOG) < 0) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    // Accept
    printf("En attente d'une connexion sur le port %d...\n", PORT);
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("accept");
        close(sockfd);
        return 1;
    }
    printf("Connexion réussie.\n");

    // jeu
    srand(time(NULL));
    int secret = rand() % 100 + 1;
    int guess;
    char buffer[64];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_sock, buffer, sizeof(buffer), 0) <= 0) break;
        guess = atoi(buffer);

        if (guess < secret)
            strcpy(buffer, "Plus grand\n");
        else if (guess > secret)
            strcpy(buffer, "Plus petit\n");
        else {
            char reponsef[64] = "Félicitations, la réponse était bien %d\n"; // régler le formatting
            sprintf(buffer, reponsef, secret);
            strcpy(buffer, reponsef);
            send(client_sock, buffer, strlen(buffer), 0);
            break;
        }
        send(client_sock, buffer, strlen(buffer), 0);
    }

    close(client_sock);
    close(sockfd);
    return 0;
}