Veuillez suivre les instructions suivantes pour tester l'application sur linux :

  D'abord lancer l'application serveur dans un terminal :
      
     - Compiler le fichier Serveur.c (exemple : gcc Serveur.c -o serveur)
     - Exécuter de la manière suivante : ./nom_fichier_généré numéro_port hôte (exemple : ./serveur 5555 localhost)
  
  Puis lancer l'application client dans une autre machine/un autre terminal :
  
     - Compiler le fichier Client.c (exemple : gcc Client.c -o client)
     - Exécuter de la manière suivante : ./nom_fichier_généré numéro_port hôte (exemple : ./client 5555 localhost)
