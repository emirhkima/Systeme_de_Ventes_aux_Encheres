/* emetteur portReceveur machineReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int sockDialogue;
struct sockaddr_in adresseVente;
int offreCourante;
int offreLaPlusEleveeRecue;

int main(int argc, char **argv)
{
int envoye, recu;
struct sockaddr_in adresseReceveur;
int lgadresseReceveur;
int lgadresseVente;
struct hostent *hote;
char conditions[100];
int phase1=0;
/* cr'eation de la socket */
if ((sockDialogue = socket( AF_INET , SOCK_DGRAM , 0 )) == -1) 
  {
    perror("socket"); 
    exit(1);
    }

/* recherche de l'@ IP de la machine distante */
if ((hote = gethostbyname(argv[2])) == NULL)
  {
    perror("gethostbyname"); 
    close(sockDialogue); 
    exit(2);
    }

/* preparation de l'adresse distante : port + la premier @ IP */
adresseVente.sin_family = AF_INET;
adresseVente.sin_port = htons(atoi(argv[1]));
bcopy(hote->h_addr, &adresseVente.sin_addr, hote->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseVente.sin_addr));

/* envoi d'une demande de participation au serveur */
printf("Veuillez patienter pendant que l'hôte examine votre requête...\n");
lgadresseVente = sizeof(adresseVente);
char message[10]="Demande ?";
    if ((envoye = sendto( sockDialogue, &message, strlen(message)+1, 0, (struct sockaddr *) &adresseVente, lgadresseVente))!= strlen(message)+1) 
    {
        perror("sendto demande"); 
        close(sockDialogue); 
        exit(1);
    }
  /* reception des conditions et de l'offre initiale*/

   if ((recu = recvfrom( sockDialogue, &conditions, 100, 0, (struct sockaddr *)&adresseVente, &lgadresseVente)) == -1) 
  {
      perror("recvfrom conditions"); 
      close(sockDialogue); 
      exit(1);
  }
  printf("%s\n", conditions);

    if ((recu = recvfrom( sockDialogue, &offreLaPlusEleveeRecue, sizeof(int), 0, (struct sockaddr *)&adresseVente, &lgadresseVente)) == -1) 
  {
      perror("recvfrom offreLaPlusEleveeRecue"); 
      close(sockDialogue); 
      exit(1);
  }

  /* parties 3/4 */
  while(offreLaPlusEleveeRecue != -1){
    printf("L'offre la plus elevee : %d\n", offreLaPlusEleveeRecue);
    printf("Souhaitez-vous faire une offre ?\ntapez 1 pour oui  /  tapez 2 pour non\n");
    scanf("%d",&phase1);

    /* le client veut faire une offre*/
    if(phase1 == 1){
      printf("Entrez votre offre :\n");
      scanf("%d", &offreCourante);
      if ((envoye = sendto( sockDialogue, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresseVente, lgadresseVente))!= sizeof(int)) 
      {
        perror("sendto offreCourante"); 
        close(sockDialogue); 
        exit(1);
      }
    }
    if ((recu = recvfrom( sockDialogue, &offreLaPlusEleveeRecue, sizeof(int), 0, (struct sockaddr *)&adresseVente, &lgadresseVente)) == -1) 
    {
      perror("recvfrom offreLaPlusEleveeRecue"); 
      close(sockDialogue); 
      exit(1);
    }
  }

  /* Vente terminée */
  if ((recu = recvfrom( sockDialogue, &offreLaPlusEleveeRecue, sizeof(int), 0, (struct sockaddr *)&adresseVente, &lgadresseVente)) == -1) 
  {
      perror("recvfrom offreLaPlusEleveeRecue"); 
      close(sockDialogue); 
      exit(1);
  }
  printf("La dernière offre recue est : %d\n", offreLaPlusEleveeRecue);
  if(offreLaPlusEleveeRecue == offreCourante)
    printf("Félicitation! Vous avez remporté cette vente !\n");
  else
    printf("Un autre client a remporté la vente\n");

close(sockDialogue);
return 1;
}
