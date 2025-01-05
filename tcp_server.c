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
    printf("Adresse IP : OK\n");
    
    int fdsocket =  socket(PF_INET , SOCK_STREAM /*Pour TCP, mode connecté*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (Serveur TCP)");
        return EXIT_FAILURE;
    };
    printf("Création socket : OK\n");

    // -------------------- définit l'adresse IP ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(addy);
    
    // convertit de la notation décimale pointée à l'adresse binaire
    int k = inet_pton(PF_INET, argv[1], &(addy.sin_addr)); //TODO: set à INADDR_ANY pour ne pas avoir à préciser sur quelle interface réseau écouter sur l'invite de commande...
    if (k <= 0) {
        perror("échec de conversion de l'adresse IP");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    printf("Conversion décimale -> binaire : OK\n");
    // ------------------- bind/configuration de la connexion -------------------------
    int b = bind(fdsocket, (const struct sockaddr *)&addy, addlen);
    if (b < 0) {
        perror("échec de l'attachement");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    printf("Configuration de la connexion : OK\n");
    // ------------------- listen -------------------------
    int l = listen(fdsocket, 5); // On écoute sur le port, on permet jusqu'à 5 clients en attente
    if (l < 0) {
        perror("échec de l'écoute");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    // ------------------- accept --------------------------------
    // création du socket fils pour traiter la demande du client, on se remet en écoute sur l'ancien socket ensuite
    while (1) {
        printf("En attente d'un client : OK\n");
        struct sockaddr clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        int fdsocket2 = accept(fdsocket, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (fdsocket2 < 0) {
            perror("échec de l'acceptation");
            continue;
        }
        char* buf = (char*)malloc(100*sizeof(char));
        printf("Connexion réussie\n");
        do{
            buf[0] = '\0';
            int t = read(fdsocket2, buf, 100*sizeof(char));
            if(t < 0){
                perror("Erreur lors de la lecture");
                return EXIT_FAILURE;
            }
            if(buf[0] == '\n'){
                break;
            }
            printf("%s", buf);
        }while(1);
       
        printf("\n----- Fermeture de connexion avec le client courant ------\n");
        close(fdsocket2);
    }
    close(fdsocket);
    return 0;
}