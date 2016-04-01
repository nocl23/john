#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
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
	
	int optval = 1;
	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
		perror("Can not set SO_REUSEADDR option");
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d’écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

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
	
	
	return socket_serveur;
}

void fgets_or_exit(char *buffer, int size, FILE * stream)
{
	http_request request;
	fgets(buffer, size, stream);

	char *request_line = malloc(size);
	strcpy(request_line, buffer);
	avoid_line(buffer, size, stream);

	int req = parse_http_request(request_line, &request);

	if(req)
	{
		send_response(stream, 400, "Bad Request", "Bad request\r\n");
	}else if(request.method == HTTP_UNSUPPORTED)
	{
		send_response(stream, 405, "Method Not Allowed", "Method Not Allowed\r\n");
	}else if (strcmp(request.url, "/") == 0)
	{
		send_response(stream, 200, "OK ", "wesh\n");
	}else
	{
		send_response(stream, 404, "Not Found", "Not Found\r\n");
	}
}


void traitement_signal(int sig)
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


int parse_http_request(char *request_line, http_request *request)
{
	char **mots = malloc(strlen(request_line));
	char *part;
	int cpt = 0;
	const char sep[2] = " ";

	part = strtok(request_line, sep);

	while(part != NULL)
	{
		mots[cpt] = part;
		cpt++;
		part = strtok(NULL, sep);
	}

	if(strcmp(mots[0], "GET") == 0)
	{
		request->method = HTTP_GET;
	}else
	{
		request->method = HTTP_UNSUPPORTED;
	}

	request->url = mots[1];
	request->major_version = mots[2][5] - '0';
	request->minor_version = mots[2][7] - '0';

	if(request->major_version == 1 && (request->minor_version == 0 || request->minor_version == 1))
	{
		return 0;
	}
	return 1;	
}

void send_status ( FILE * client , int code , const char * reason_phrase ) {
	char * phrase = malloc(50);
	sprintf(phrase, "HTTP/1.1 %d %s\r\n", code , reason_phrase);
	write ( 1 , phrase , strlen (phrase));
	fputs(phrase,client);
}

void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body ) {
	send_status(client, code, reason_phrase);
	char * phrase = malloc(50);
	sprintf(phrase, "Connection: close\r\nContent-Length: %zu \r\n\r\n%s",strlen(message_body),message_body);	
	write ( 1 , phrase , strlen (phrase));
	fputs(phrase,client);
}

