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

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Veuillez donner l'adresse IP");
    return EXIT_FAILURE; 
  }

  int fdsocket =  socket(PF_INET /* pourquoi pas AF_INET? */, SOCK_DGRAM /*Pour UDP, mode datagram*/, 0);
  if (fdsocket == -1) {
    perror("échec de création de la socket (Serveur udp)");
  };

 // -------------------- définit l'adresse IP ----------------------------  
  struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT)};
  socklen_t addlen = sizeof(addy);

  // convertit de la notation décimale pointée à l'adresse binaire
  int k = inet_pton(PF_INET, argv[1], &(addy.sin_addr)); 
  if (k <= 0) {
    perror("échec de création de la socket (Serveur udp)");
  }
  // ------------------- bind/configuration de la connexion -------------------------
  // le (const struct sockaddr *) est un cast de sockaddr_in -> sockaddr pour faire taire le compilateur qui demande
  // explicitement un sockaddr (sockaddr_in est un type de sockaddr)
  int b = bind(fdsocket, (const struct sockaddr *)&addy, addlen);
  if (b < 0) {
    perror("échec de l'attachement");
  }

  char* buf = (char*)malloc(100*sizeof(char));

  int timeout = 0;
  while (timeout < 10) {
    // récéption des paquets udp

    // écriture sur la sortie standard
    //printf("Message recu : ");
    int t = recvfrom(fdsocket, buf, 100*sizeof(char), 0, (struct sockaddr *)&addy, &addlen);
    write(STDOUT_FILENO, buf, sizeof(buf) - 1);
    printf("\n");
    timeout++;
  }
  close(fdsocket);
}