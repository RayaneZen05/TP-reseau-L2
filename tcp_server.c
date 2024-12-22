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
    
    int fdsocket =  socket(PF_INET , SOCK_STREAM /*Pour TCP, mode connecté*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (Serveur TCP)");
        return EXIT_FAILURE;
    };
    
    // -------------------- définit l'adresse IP ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(addy);
    
    // convertit de la notation décimale pointée à l'adresse binaire
    int k = inet_pton(PF_INET, argv[1], &(addy.sin_addr)); 
    if (k <= 0) {
        perror("échec de conversion de l'adresse IP");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    // ------------------- bind/configuration de la connexion -------------------------
    int b = bind(fdsocket, (const struct sockaddr *)&addy, addlen);
    if (b < 0) {
        perror("échec de l'attachement");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    
    // ------------------- listen -------------------------
    int l = listen(fdsocket, 5); // On écoute sur le port, on permet jusqu'à 5 clients en attente
    if (l < 0) {
        perror("échec de l'écoute");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    // ------------------- accept --------------------------------
    // création du socket fils pour traiter la demande du client, on se remet en écoute sur l'ancien socket si besoin
    while (1) {
        struct sockaddr clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        int fdsocket2 = accept(fdsocket, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (fdsocket2 < 0) {
            perror("échec de l'acceptation");
            continue;
        }
        if (fork() == 0) { // processus fils
            close(fdsocket);
            char* buf = (char*)malloc(100*sizeof(char));
            int t = read(fdsocket2, buf, 100*sizeof(char));
            write(STDOUT_FILENO, buf, t);
            printf("\n");
            close(fdsocket2);
            exit(0);
        } else {
            close(fdsocket2); // processus pére qui ferme la socket
        }
    }
    close(fdsocket);
    return 0;
}