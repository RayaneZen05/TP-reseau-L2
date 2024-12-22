#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

#include<arpa/inet.h> // uniquement pour inet_pton, à voir

/* Port local du serveur */
#define PORT 9600

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Veuillez donner l'adresse IP");
        return EXIT_FAILURE; 
    }
    
    int fdsocket =  socket(PF_INET /* pourquoi pas AF_INET? */, SOCK_STREAM /*Pour TCP, mode connecté*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (client tcp)");
        return EXIT_FAILURE;
    };
    
    // -------------------- "buffers" pour l'adresse IP du serveur ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(addy);
    
    // convertit de la notation décimale pointée à l'adresse binaire
    int k = inet_pton(PF_INET, argv[1], &(addy.sin_addr)); 
    if (k <= 0) {
        perror("échec de conversion de l'adresse IP");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    // ------------------- connect -------------------------
    int c = connect(fdsocket, (const struct sockaddr *)&addy, addlen);
    if (c < 0) {
        perror("échec de la connexion");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    char* buf = (char*)malloc(100*sizeof(char));
    // ------------------- read/write --------------------------------
    // première lecture du serveur
    int t = read(fdsocket, buf, 100*sizeof(char));
    write(STDOUT_FILENO, buf, t);
    printf("\n");
    // ------------------- write --------------------------------
    // écriture sur la socket
    char* message = "Hello from client";
    write(fdsocket, message, strlen(message));
    // ------------------- read --------------------------------
    // lecture de la réponse du serveur
    t = read(fdsocket, buf, 100*sizeof(char));
    write(STDOUT_FILENO, buf, t);
    printf("\n");
    close(fdsocket);
    return 0;
}