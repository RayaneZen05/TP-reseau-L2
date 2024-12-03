#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

/* Port local du serveur */
#define PORT 9600
   
int main(int argc, char *argv[]) {
    /**
      * variables du serveur
      *
      * à déclarer : 
        - sockfd descripteur de socket
        - structure d'adresse locale du serveur
        - structure d'adresse du client
        - taille de l'adresse du client
     **/
     
     /**
      * code du serveur
      *
      * - Ouvrir le socket du serveur
      * - Remplir la structure d'adresse locale du serveur :
          - la famille d'adresse
          - l'adresse IP
          - le ort
      * - Spécifier l'adresse locale du socket du serveur
     **/

     while (1) {
        break;
        // code de l'intérieur de la boucle
     }
     
}