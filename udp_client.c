#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include<arpa/inet.h> // uniquement pour inet_pton, à voir

#define SIZE 100
#define PORT 9600
 
int main(int argc, char *argv[]){
    if (argc < 2) {
        perror("Veuillez donner l'adresse IP du serveur");
        return EXIT_FAILURE; 
    }
    /*
    * Variables du client
    *
    * Déclarer ici les variables suivantes : 
    *    - sockfd le descripteur de socket
    *    - structure d'adresse du serveur 
    *    - pointeur vers la structure descriptive de machine (hostent)
    *    - zone de mémoire destinée à acceuillir la chaîne 
    *      entrée au clavier
    *    - taille de la chaîne à envoyer
    */
    
 
    /*
    * Code du client
    * - Ouvrir le socket du client
    * - Récupérer l'adresse IP du serveur à partir de son nom
    *   donné en ligne de commande 
    * - Remplir la structure d'adresse du serveur
    * - Lire une ligne de l'entrée standard
    * - Envoyer la chaîne lue au serveur
    */
    // ouverture du socket :
    int fdsocket =  socket(PF_INET , SOCK_DGRAM /*Pour UDP, mode datagram*/, 0);
    if (fdsocket == -1) {
        perror("échec de création de la socket (client udp)");
    };
    // récupération de l'adresse :
    struct sockaddr_in serv_addr = {.sin_family=PF_INET, .sin_port=htons(PORT)};
    socklen_t addlen = sizeof(serv_addr);

    // convertion de notation décimale pointée à int
    int k = inet_pton(PF_INET, argv[1], &(serv_addr.sin_addr)); 
    if (k <= 0) {
        perror("échec de création de la socket (client udp)");
    }
    
    int _bind = bind(fdsocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (bind <= 0) perror("Echec de l'attachement (client)");

    // lecture
    // création du buffer
    char buf[SIZE];
    memset(buf, 0, SIZE);
    printf("Entrez un message à envoyer : ");
    fgets(buf, SIZE, stdin);
    printf("Message à envoyer : %s\n", buf);
    // envoi du message
    int w = sendto(fdsocket, buf, strlen(buf) /* prendre seulement la partie "écrite" du buffer (jusqu'au \0)*/, 0, (struct sockaddr *) &serv_addr, addlen);

    if (w < 0) {
        perror("Echec de l'envoi du message");
    }
    close(fdsocket);
    return 0;
}