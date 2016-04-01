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
        #include <ctype.h>
        #include "socket.h"

int main (void)
{

  	initialiser_signaux();
   	int socket_serveur = creer_serveur(8080);
   	int status;
   	int size = 256;

   	while(1)
   	{
   		int socket_client = accept (socket_serveur, NULL, NULL);
   		if(socket_client == -1)
            {
            	perror("accept");
            	/* traitement d ’ erreur */
             }
	
	    pid_t newP; 

   		if((newP = fork()) == 0)
   		{
  			char *buffer = malloc(size);
   			FILE *flux_client = fdopen(socket_client, "w+");
   			fgets_or_exit(buffer, size, flux_client);
   			free(buffer);
   			fclose(flux_client);
   			return 1; 
        }
    	printf("connexion acceptée\n") ;
			if (waitpid(newP, &status, WNOHANG) == -1)
			{
				perror("erreur waitpid");
            	close(socket_client);
			}
    }
    return 0;
}

