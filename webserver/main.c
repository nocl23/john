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

int avoid_line(char* buffer,int size, FILE* fd) {
	while( (strcmp(buffer,"\n") != 0) && (strcmp(buffer,"\r\n") != 0)) {
      //printf("boucle : %s\n", buffer);
		if (fgets(buffer,size,fd) == NULL) {
           // printf("c null \n");
			exit(1);
		}
	}

	return 1;
}

int parse_request (char* mess_client,FILE * flux_client,char * url){
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
                        	printf("BATARD%s\n", url);
                        }

                        if (nbMots == 2 && (strcmp(mots, "HTTP/1.0") == 0 || strcmp(mots, "HTTP/1.1") == 0)) {
                        	printf("%sPUTAIN DE MOTS",mots);
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
                }

                int main (void)
                {

                	initialiser_signaux();
                	int socket_serveur = creer_serveur(8080);
                	int socket_client ;



                	pid_t newP; 


                	while(1)
                	{

                		socket_client = accept (socket_serveur, NULL, NULL);

                		if(socket_client == -1)
                		{
                			perror("accept");
                /* traitement d ’ erreur */
                		}



                		if ( (newP = fork()) == 0){
                			FILE *flux_socket_client = fdopen(socket_client,"w+");
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
                }