#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "socket.h"

void traitement_signal ( int sig )
	{
		
		int pid;
		int status;
		if((pid=waitpid(-1,&status,WNOHANG))>0){
			printf ( " Signal % d reçu \n " , sig );
		}



	}

	void initialiser_signaux(void)
	{
		struct sigaction sa ;
		sa.sa_handler = traitement_signal ;
		sigemptyset (&sa.sa_mask);
		sa.sa_flags = SA_RESTART ;
		if ( sigaction ( SIGCHLD , & sa , NULL ) == -1)
		{
			perror ("sigaction(SIGCHLD)");
		}
	}



int main (void)
{

	int socket_serveur = creer_serveur(8080);
	int socket_client ;
	
	 char msg_client[20];
	initialiser_signaux();

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
			FILE *flux_socket_client = fdopen(socket_client,"w+");

		/* On peut maintenant dialoguer avec le client */
			const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\nLa température exterieure est de 14 °C, il est actuellement 8h46 du matin\nPour ceux qui prennent l'avion direction le serveur de Quentin Porion, il est parti en avance,\nNous vous invitons à vous rediriger vers le serveur de Corwin Nolimittometal qui va être en retard.\nNous espérons que vous avez fait bon voyage à bord de notre avion avec Air NoCl23,\n et nous espérons vous revoir bientôt.\nÀ une prochaine fois !\n";
			fwrite(message_bienvenue,1,strlen(message_bienvenue),flux_socket_client);

			while(1){
				
				fgets( msg_client , sizeof(msg_client),flux_socket_client);
				fprintf(flux_socket_client,"<john> %s",msg_client);
				

			}

			fclose(flux_socket_client);
			close(socket_client);

			exit(0);
		}else{

			close(socket_client);
			printf("socket_client fermee\n");

		}



	}
	close(socket_serveur);
	return 0;
}

