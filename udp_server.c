#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

/* Port local du serveur */
#define PORT 9600

// truc pour locker adresse ip
/*
struct sockaddr_in {
    short            sin_family;   // famille d'adresse : AF_INET
    unsigned short   sin_port;     // numéro de port
    struct in_addr   sin_addr;     // adresse IP
    char             sin_zero[8];  //? inutilisé
};
struct in_addr {
    unsigned long s_addr;  // initialiser avec inet_aton()
};
*/

int main(int argc, char *argv[]) {
    /**
      * variables du serveur
      *
      * à déclarer : 
        - sockfd descripteur de socket OK
        - structure d'adresse locale du serveur OK
        - structure d'adresse du client OK
        - taille de l'adresse du client OK
     **/

    /** 0 = entête/id du protocole ip
      * création du descripteur de socket
      */
    int fdsocket =  socket(PF_INET /* pourquoi pas AF_INET? */, SOCK_DGRAM /*Pour UDP, mode datagram*/, 0);
    if (fdsocket == -1) {
      perror("échec de création de la socket (Serveur udp)");
    };


   // faire le bind 
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