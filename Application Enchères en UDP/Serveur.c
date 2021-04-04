/* receveur portReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#define NBMAXCLIENT 100

int offreLaPlusElevee = 20;
int nbClients = 0;
struct sockaddr_in tabAdressesClients[NBMAXCLIENT];

int main (int argc, char **argv)
{
int sock,socketVente,recu,envoye;
struct sockaddr_in adresseLocale;
int lgadresseLocale;
struct sockaddr_in adresseClient;
int lgadresseClient;
struct hostent *hote;
struct sigaction action;
char conditions[100];
int phase1 = 1, phase2 = 1;
int offre;
char demande[10];
int valeur;

  /* création de la socket */
  if ((sock = socket( AF_INET , SOCK_DGRAM , 0 )) == -1) 
    {
      perror("socket"); 
      exit(1);
    }
  /* préparation de l'adresse locale : port + toutes les adresses IP */
  adresseLocale.sin_family = AF_INET;
  adresseLocale.sin_port = htons(atoi(argv[1]));
  adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

/* attachement de la socket à l'adresse locale */
  lgadresseLocale = sizeof(adresseLocale);
if ((bind( sock, (struct sockaddr *)&adresseLocale, lgadresseLocale)) == -1) 
  {
    perror("bind"); 
    exit(1);
  }


/* accueil des clients */
  lgadresseClient = sizeof(struct sockaddr_in);
while(phase1 == 1)
{
  if ((recu = recvfrom( sock, demande, 10, 0, (struct sockaddr *)&adresseClient, &lgadresseClient)) == -1) 
  {
      perror("recvfrom unEntier"); 
      close(sock); 
      exit(1);
  }
  tabAdressesClients[nbClients] = adresseClient;
  nbClients++;
  printf("Voulez-vous continuer l'accueil ? (nombre de clients : %d)\ntapez 1 pour oui  /  tapez 2 pour non\n", nbClients);
  scanf("%d",&phase1);
}

/*envoi des conditions à tous les clients */
printf("Nombre de clients : %d\n", nbClients);
strcpy(conditions,"Les acheteurs (clients) reçoivent les conditions initiales de la vente aux enchères.");
 for(int i=0 ; i<nbClients ; i++){
    lgadresseClient = sizeof(tabAdressesClients[i]);
    if ((envoye = sendto( sock, &conditions, strlen(conditions)+1, 0, (struct sockaddr *) &tabAdressesClients[i], lgadresseClient))!= strlen(conditions)+1)  
    {
      perror("sendto conditions"); 
      close(sock); 
      exit(1);
    }
    if ((envoye = sendto( sock, &offreLaPlusElevee, sizeof(offreLaPlusElevee), 0, (struct sockaddr *) &tabAdressesClients[i], lgadresseClient))!= sizeof(offreLaPlusElevee)) 
    {
      perror("sendto offreLaPlus"); 
      close(sock); 
      exit(1);
    }
 }

/* boucle de réception des offres */
while(phase2 == 1){

  printf("Attente de la réception d'une nouvelle offre\n");
  /* réception d'une offre */
    if ((recu = recvfrom( sock, &offre, sizeof(offre), 0, (struct sockaddr *)&adresseClient, &lgadresseClient)) == -1) 
  {
      perror("recvfrom offre"); 
      close(sock); 
      exit(1);
  }
  printf("Offre précédente : %d  |  Offre Actuelle : %d\n", offreLaPlusElevee, offre);

  if(offre > offreLaPlusElevee)
    offreLaPlusElevee = offre;
    valeur = offreLaPlusElevee; /* "valeur" est la valeur qu'on va envoyé à l'aide de sendto*/
    printf("Voulez-vous continuer les enchères ?\ntapez 1 pour oui  /  tapez 2 pour non(2)\n");
    scanf("%d",&phase2);

  /* envoi de l'offre actuelle à tous les clients */
    if(phase2 == 2)
      valeur = -1;
   for(int i=0 ; i<nbClients ; i++){
      lgadresseClient = sizeof(tabAdressesClients[i]);
      if ((envoye = sendto( sock, &valeur, sizeof(valeur), 0, (struct sockaddr *) &tabAdressesClients[i], lgadresseClient))!= sizeof(valeur)) 
      {
        perror("sendto offreLaPlusElevee"); 
        close(sock); 
        exit(1);
      }
    }
  }


  /*envoi du message de longueur 0 et de la dernière offre à tous les clients*/
  for(int i=0 ; i<nbClients ; i++){
    lgadresseClient = sizeof(tabAdressesClients[i]);
    if ((envoye = sendto( sock, &offreLaPlusElevee, sizeof(offreLaPlusElevee), 0, (struct sockaddr *) &tabAdressesClients[i], lgadresseClient))!= sizeof(offreLaPlusElevee)) 
    {
      perror("sendto offreLaPlusElevee"); 
      close(sock); 
      exit(1);
    }
  }

close(sock);
return 1;
}
