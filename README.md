# TP Réseau L2

## Membres
* Jean-Baptiste Verbrugghe
* Rayane Zenaidi

## Groupe de TD
* LDDIM-01

## Note importante pour WSL
Si vous utilisez WSL, notez que l'hébergement de serveurs peut être problématique car WSL fonctionne dans un environnement virtualisé.

## Fichiers

### Client UDP (`udp_client.c`)
Ce client UDP envoie des messages à un serveur UDP. Il prend en argument l'adresse IP du serveur et permet d'envoyer des messages saisis au clavier.

Pour l'exécuter :
```bash
gcc udp_client.c -o udp_client
./udp_client <adresse_ip_serveur>
```
Exemple : `./udp_client 192.168.1.100`

### Serveur UDP (`udp_server.c`)
Ce serveur UDP attend une connexion d'un client et se lie (bind) à l'adresse IP spécifiée en argument lors du lancement.  L'adresse IP utilisée pour le bind et le serveur doit être une adresse valide appartenant au réseau local, on va la choisir dans la plage d'adresses définie par l'interface réseau `eth0` et son masque de sous-réseau dans le cadre de ce projet.  On obtient tout ca par un `ifconfig`.

### Serveur TCP Simple (`tcp_server.c`)
Ce serveur TCP basique écoute sur le port 9600 et accepte les connexions entrantes. Il peut gérer plusieurs clients séquentiellement (un à la fois). Il affiche les messages reçus des clients.

Pour l'exécuter :
```bash
gcc tcp_server.c -o tcp_server
./tcp_server <adresse_ip_hote>
```
L'adresse IP doit être une adresse valide de votre machine. Utilisez `ifconfig` ou `ip addr` pour la trouver.

### Jeu (`tcp_serv_ths.c`)
Version évoluée du serveur TCP qui implémente un jeu de devinettes. L'hôte choisit un nombre secret et les clients doivent le deviner. Le serveur répond avec des indices "plus grand" ou "plus petit".

Pour l'exécuter :
```bash
gcc tcp_serv_ths.c -o tcp_serv_ths
./tcp_serv_ths
```
Le serveur utilisera INADDR_ANY, donc il écoutera sur toutes les interfaces réseau disponibles.

### Client TCP (Jeu) (`tcp_client_ths.c`)
Client correspondant au serveur de jeu. Il permet aux joueurs de se connecter et d'essayer de deviner le nombre secret.

Pour l'exécuter :
```bash
gcc tcp_client_ths.c -o tcp_client_ths
./tcp_client_ths <adresse_ip_serveur>
```

## Configuration réseau
1. Vérifiez votre configuration réseau avec `ifconfig` ou `ip addr`
2. Assurez-vous que le pare-feu autorise les connexions sur le port 9600 (`sudo ufw allow 9600/tcp` sinon/si accés root)
3. Pour tester localement, c'est plus simple d'utiliser directement 127.0.0.1 avant de passer à autre chose

## Dépannage
- Si "Address already in use" : attendez quelques secondes ou utilisez un autre port
- Si "Connection refused" : vérifiez que le serveur est bien lancé
- Sur WSL : vérifiez que Windows Defender n'empêche pas les connexions ?