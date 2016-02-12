#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "socket.h"

int creer_serveur(int port)
{
	int socket_serveur;
	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
		if(socket_serveur == -1)
	{
		perror ("socket_serveur");
		return -1;
		/* traitement de l’erreur */
	}
	/* Utilisation de la socket serveur */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d’écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
	int optval = 1;
	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
		perror("Can not set SO_REUSEADDR option");
	}
	if(bind(socket_serveur, (struct sockaddr *)& saddr, sizeof(saddr)) == -1)
	{
		perror("bind socker_serveur");
		/* traitement de l’erreur */
	}
	if(listen(socket_serveur, 10) == -1)
	{
		perror("listen socket_serveur");
		/* traitement d’erreur */
	}
	int socket_client ;
	socket_client = accept (socket_serveur, NULL, NULL);
	if(socket_client == -1)
	{
		perror("accept");
		/* traitement d ’ erreur */
	}
	/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\nLa température exterieure est de 14 °C, il est actuellement 8h46 du matin\nPour ceux qui prennent l'avion direction le serveur de Quentin Porion, il est parti en avance,\nNous vous invitons à vous rediriger vers le serveur de Corwin Nolimittometal qui va être en retard.\nNous espérons que vous avez fait bon voyage à bord de notre avion avec Air NoCl23,\n et nous espérons vous revoir bientôt.\nÀ une prochaine fois !";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));
	return socket_serveur;
}

	void initialiser_signaux(void)
	{
		if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		{
			perror("signal");
		}
	}