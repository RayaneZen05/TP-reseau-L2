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
int connected = 0;

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
    printf("Connexion réussie\n");
    char* buf = (char*)malloc(100*sizeof(char));
    connected = 1;
    while(connected){
    // ------------------- read/write --------------------------------
    // lecture du message à envoyer
        printf("Message à envoyer : \n");
        int t = read(STDIN_FILENO, buf, 100*sizeof(char));
        if(t < 0){    
            perror("Erreur lors de l'écriture");
            return EXIT_FAILURE;
        }
        if(buf[0] == '\n'){
            connected = 0;
        }
        int w = write(fdsocket, buf, 100*sizeof(char));
        if(w < 0){
            perror("Erreur lors de l'écriture");
            return EXIT_FAILURE;
        }
        printf("\n");
    }
    printf("------ Fermeture de la connexion avec le serveur ------\n");
    close(fdsocket);
    return 0;
}