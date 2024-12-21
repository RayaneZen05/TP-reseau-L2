# TP Réseau L2

## Membres

* Jean-Baptiste Verbrugghe
* Rayane Zenaidi

## Groupe de TD

* LDDIM-01

## Fichiers

### Serveur UDP (`udp_server.c`)

Ce serveur UDP attend une connexion d'un client et se lie (bind) à l'adresse IP spécifiée en argument lors du lancement.  L'adresse IP utilisée pour le bind et le serveur doit être une adresse valide appartenant au réseau local, on va la choisir dans la plage d'adresses définie par l'interface réseau `eth0` et son masque de sous-réseau dans le cadre de ce projet.  On obtient tout ca par un `ifconfig`.

#### À faire:

- [X] Tester le code
- [ ] Organiser la documentation

### Client UDP

TODO...

### Serveur + Client TCP

TODO...