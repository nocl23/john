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



/*int parse_request (char* mess_client,FILE * flux_client,char * url){
	int nbMots=0;
	char * mots;



	fgets_or_exit(mess_client ,256,flux_client);

	printf("%s\n",mess_client );

	mots = strtok(mess_client," ");
	while ( mots != NULL ){

                        if( nbMots==0 && strcmp(mots,"GET") != 0){ //verification du premier mot
                        	printf("GET\n");
                        	return 0;
                        }
                        
                        if(nbMots == 1 && strcmp(mots, "/") != 0)
                        {
                        	return 0;
                        }else if((nbMots == 1 && strcmp(mots, "/") == 0))
                        {
                        	strcpy(url, mots);
                        	printf("url %s\n", url);
                        }

                        if (nbMots == 2 && (strcmp(mots, "HTTP/1.0") == 0 || strcmp(mots, "HTTP/1.1") == 0)) {
                        	printf("%s",mots);
                        	printf("nb mots %d\n",nbMots );
                        	return 0;
                        }

                           // printf("%s\n", mots);

                        mots = strtok(NULL," ");
                        nbMots++;

                    }
                       // printf("fin while\n");

                    if (nbMots!=3) {
                    	printf("erreur nb\n");
                    	return 0;
                    }

                       // printf("ici\n");



                       // printf("et ici\n");
                    

                   // printf("je retourne 1\n");
                    return 1;
                }*/

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


/*FILE *flux_socket_client = fdopen(socket_client,"w+");
                            //parse_request(msg_client,flux_socket_client);
                			char msg_client[256];
                			char* response;
                			int c_length;
                			char* url= malloc(sizeof(char));

                			int rq = parse_request(msg_client,flux_socket_client,url);
                			printf("requeteeee%d\n",rq);
                			avoid_line(msg_client, 256, flux_socket_client);
                          // printf("retour avoid %d\n", a);
                			if(rq && (strcmp(url,"/")==0)){
                				response = "Bienvenue les amis !";
                				c_length = strlen(response);
                				printf("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: %d\r\n",c_length);
                			}else if (!rq && (strcmp(url,"/")==0)){

                				response = "400 Bad Request";
                				c_length=strlen(response);
                				printf("HTTP/1.1 %s\r\nConnection: close\r\nContent-Length: %d\r\n",response,c_length);

                			}else{
                				printf("MDR\n");
                				response = "404 not found";
                				c_length=strlen(response);
                				printf("HTTP/1.1 %s\r\nConnection: close\r\nContent-Length: %d\r\n",response,c_length);

                			}

                			close (socket_client);
                			exit(1);
                		}else{

                			close(socket_client);
                           // printf("socket_client fermeeeee\n");

                		}

                	}

                	close(socket_serveur);
                	return 0;
                }*/


