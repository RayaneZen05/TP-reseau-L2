#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

/* Port local du serveur */
#define PORT 9600

int main(int argc, char** argv) {
    
    int fdsocket =  socket(PF_INET , SOCK_STREAM /*Pour TCP, mode connecté*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (Serveur TCP)");
        return EXIT_FAILURE;
    };
    
    // -------------------- définit l'adresse IP ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT), .sin_addr.s_addr=INADDR_ANY /* Toutes les interfaces */};
    socklen_t addlen = sizeof(addy);
    
    // ------------------- bind -------------------------
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
    // création du socket fils pour traiter la demande du client, on se remet en écoute sur l'ancien socket ensuite
    while (1) {
        // accepte la connexion
        struct sockaddr clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        int fdsocket2 = accept(fdsocket, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (fdsocket2 < 0) {
            perror("échec de l'acceptation");
            continue;
        }
        // lecture du message recu sur le socket
        printf("Message recu: ");
        char buf[100];
        int t = read(fdsocket2, buf, 100*sizeof(char));
        fflush(stdout);
        write(STDOUT_FILENO, buf, t);
        printf("\n");
        close(fdsocket2);
    }
    close(fdsocket);
    return 0;
}