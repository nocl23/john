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



int main (void)
{

    int socket_serveur = creer_serveur(8080);
    int socket_client ;
    //char * first_line;
    char msg_client[20];
    initialiser_signaux();
    pid_t newP; 
    
    
    while(1)
    {

        socket_client = accept (socket_serveur, NULL, NULL);
        
        if(socket_client == -1)
        {
            perror("accept");
        /* traitement d ’ erreur */
        }
        
        FILE *flux_socket_client = fdopen(socket_client,"w+");

        if((newP=fork()) == 0){


        /* On peut maintenant dialoguer avec le client */

            int nbMots=0;
            char * mots;

            fgets( msg_client , sizeof(msg_client),flux_socket_client) ;
            mots = strtok(msg_client," ");
            
            while ( mots != NULL ){
                
                if( nbMots==0 && strcmp(mots,"GET") == 0){ /*verification du premier mot*/
                    printf("%s ", mots);
                    nbMots++;
                }

                mots = strtok(NULL," ");   /*deuxieme mots*/

                if( nbMots ==1 || strcmp(mots,"/")==0){
                    printf("%s ", mots);
                    nbMots++;
                }
                mots = strtok (NULL,"\r\n");  /* verification du 3e mot*/
                if( nbMots ==2 && (strcmp(mots,"HTTP/1.1") == 0 || strcmp(mots,"HTTP/1.0"))){
                    printf("%s\n",mots);
                    nbMots++;
                }
                
            }



            fclose(flux_socket_client);
            close(socket_client);
            exit(0);

        }else{

            close(socket_client);
            //printf("socket_client fermeeeee\n");

        }

    }

    close(socket_serveur);
    return 0;
}
