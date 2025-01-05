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

  int fdsocket =  socket(PF_INET, SOCK_DGRAM /*Pour UDP, mode datagram*/, 0);
  if (fdsocket == -1) {
    perror("échec de création de la socket (Serveur udp)");
  };

 // -------------------- définit l'adresse IP ----------------------------  
  struct sockaddr_in addy = {.sin_family=PF_INET, .sin_port=htons(PORT), .sin_addr.s_addr=INADDR_ANY /* "écoute"/récéption sur toutes les interfaces réseau du serveur */};
  socklen_t addlen = sizeof(addy);


  // ------------------- bind/configuration de la connexion -------------------------
  // le (const struct sockaddr *) est un cast de sockaddr_in -> sockaddr pour faire taire le compilateur qui demande
  // explicitement un sockaddr (sockaddr_in est un type de sockaddr) ...
  int b = bind(fdsocket, (const struct sockaddr *)&addy, addlen);
  if (b < 0) {
    perror("échec de l'attachement");
  }

  char buf[100];
  memset(buf, 0, sizeof(buf));

  while (1) {
    // récéption des paquets udp
    int t = recvfrom(fdsocket, buf, sizeof(buf), 0, (struct sockaddr *)&addy, &addlen);

    // écriture sur la sortie standard
    printf("Message recu : ");
    fflush(stdout); // "forcer" le print en flushant la sortie standard
    write(STDOUT_FILENO, buf, t); // on écrit seulement le nombre d'octets recu
    printf("\n");

  }
  close(fdsocket);
}