#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

#include<arpa/inet.h>

/* Port local du serveur */
#define PORT 9600

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Veuillez donner l'adresse IP");
        return EXIT_FAILURE; 
    }
    
    int fdsocket =  socket(PF_INET, SOCK_STREAM /*Pour TCP, mode connecté*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (client tcp)");
        return EXIT_FAILURE;
    };
    
    // -------------------- l'adresse IP du serveur ----------------------------  
    struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(addy);
    
    // convertit de la notation décimale pointée à l'adresse en décimal
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
    // lecture du message à envoyer
    printf("Message à envoyer: ");
    int t = read(STDIN_FILENO, buf, 100*sizeof(char));
    if (t < 0) {
        perror("échec de la lecture");
        close(fdsocket);
        return EXIT_FAILURE;
    }
    printf("Message envoyé: %s\n", buf);
    write(fdsocket, buf, t);
    printf("\n");
    close(fdsocket);
    return 0;
}