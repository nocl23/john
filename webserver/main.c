#include <stdio.h>
#include <string.h>
#include "socket.h"

int main (void)
{
	while(1)
	{
		initialiser_signaux();
		creer_serveur(8080);
	}
	return 0;
}

