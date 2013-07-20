#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "littext.h"



int LITTEXT_Error;	/* variable d'erreure */
int LITTEXT_ErrorMode=MODEDIRECT;
			/* indique si une erreure stop le programme :
			   mode MODEDIRECT ou non : MODEINDIRECT     	*/


char LigneReelle[TAILLELIGNE];
char LigneTravailReelle[TAILLELIGNE];
char MotReelle  [TAILLELIGNE];

char* Ligne=NULL;
char* Mot=NULL;
char* LigneTravail=LigneTravailReelle;

FILE* Fichier;


char* LitLigne ()
	{
	 if (!feof(Fichier))
	  {
	   Ligne=fgets(LigneReelle,TAILLELIGNE,Fichier);
	   strcpy(LigneTravail,Ligne);
	  }
	 else
	   Ligne=NULL;
	 Mot=NULL;
	 return (Ligne);
	}

char* PremierMot (char* sep)
	{
	 if (Ligne==NULL) return (NULL);
	 Mot=strtok(LigneTravail,sep);
	 return (Mot);
	}

char* MotLigneSuivant (char* sep)
	{
	 if (Ligne==NULL) LitLigne();
	 if (Ligne==NULL) return (NULL);
	 if (Mot==NULL)
	    PremierMot (sep);
	 else
	    Mot=strtok(NULL,sep);
	 return (Mot);
	}

char* MotSuivant (char* sep)
	{
	 if (Ligne!=NULL)
	    if (Mot==NULL)
		Mot=strtok(LigneTravail,sep);
	    else
		Mot=strtok(NULL,sep);

	 if (Mot==NULL)
	    do
	    {
	     LitLigne();
	     if (Ligne==NULL)
		continue;
	     PremierMot(sep);
	    } while (Mot==NULL && Ligne!=NULL);

	 return (Mot);
	}

int Change (char* chaine,char ancien,char nouveau)
	{
	 char* ptr;
	 char  tmp[2];
	 int result=0;
	 tmp[0]=ancien;
	 tmp[1]='\0';
	 while((ptr=strpbrk(chaine,tmp))!=NULL)
	       {
		*ptr=nouveau;
		result=1;
	       }
	 return (result);
	}


char* Cherche (char* texte,char* sep)
	{
	 while (MotSuivant(sep)!=NULL)
	       if (strcmp(Mot,texte)==0) return (Mot);
	 return (NULL);
	}



int Ouvre (char* nom)
	{
	 Fichier=fopen(nom,"rt");
	 if (Fichier==NULL)
	  if (LITTEXT_Error==MODEDIRECT)
	   {
	    fprintf (stderr,"\nFichier %s inexistant !\n",nom);
	    exit(EXIT_FAILURE);
	   }
	  else return (INEXISTANT);
	 return 0;
	}


void Ferme (void)
	{
	 fclose (Fichier);
	}
