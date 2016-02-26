#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "socket.h"

int main (void)
{

	int socket_serveur = creer_serveur(8080);
	int socket_client ;

	void initialiser_signaux(void)
	{
		if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		{
			perror("signal");
		}
	}

	while(1)
	{
	
	socket_client = accept (socket_serveur, NULL, NULL);
	if(socket_client == -1)
	{
		perror("accept");
		/* traitement d ’ erreur */
	}
	int newP; 
	
	if((newP=fork()) == 0){

		/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\nLa température exterieure est de 14 °C, il est actuellement 8h46 du matin\nPour ceux qui prennent l'avion direction le serveur de Quentin Porion, il est parti en avance,\nNous vous invitons à vous rediriger vers le serveur de Corwin Nolimittometal qui va être en retard.\nNous espérons que vous avez fait bon voyage à bord de notre avion avec Air NoCl23,\n et nous espérons vous revoir bientôt.\nÀ une prochaine fois !\n";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));
		exit(0);
	}else{

	close(socket_client);
	printf("socket_client fermee\n");

	}
	
	

	}
	return 0;
}

