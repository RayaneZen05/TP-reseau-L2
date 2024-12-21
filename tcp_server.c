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
    };
    
    // -------------------- définit l'adresse IP ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(addy);
    
    // convertit de la notation décimale pointée à l'adresse binaire
    int k = inet_pton(PF_INET, argv[1], &(addy.sin_addr)); 
    if (k <= 0) {
        perror("échec de création de la socket (Serveur tcp)");
    }
    // ------------------- bind/configuration de la connexion -------------------------
    int b = bind(fdsocket, (const struct sockaddr *)&addy, addlen);
    if (b < 0) {
        perror("échec de l'attachement");
    }
    
    // ------------------- listen -------------------------
    int l = listen(fdsocket, 0); // On écoute sur le port, on ne met aucun client en attente pour l'instant
    if (l < 0) {
        perror("échec de l'écoute");
    }
    // ------------------- accept --------------------------------
    // création du socket fils pour traiter la demande du client, on se remet en écoute sur l'ancien socket si besoin
    struct sockaddr clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    int fdsocket2 = accept(fdsocket, (struct sockaddr *)&clientaddr, &clientaddrlen); // pour l'instant on accepte qu'une seule socket (pas de boucle)
    if (fdsocket2 < 0) {
        perror("échec de l'acceptation");
    }
    int timeout = 0;
    char* buf = (char*)malloc(100*sizeof(char));
    // lecture depuis la socket client
    int t = read(fdsocket2, buf, 100*sizeof(char));
    // écriture sur la sortie standard
    write(STDOUT_FILENO, buf, sizeof(buf) - 1);
    printf("\n");
    timeout++;
    /*
    while (timeout < 10) {
        // lecture depuis la socket client
        int t = read(fdsocket2, buf, 100*sizeof(char));
        // écriture sur la sortie standard
        write(STDOUT_FILENO, buf, sizeof(buf) - 1);
        printf("\n");
        timeout++;
    }*/
    close(fdsocket);
}