#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "geom.h"
#include "show.h"
#include "transf.h"

int NbPoint;
sPoint Point[500];
sRepere Repere;
char  *sep=" \t\n,;";

void changerep ();


main (int argc,char *argv[])
	{
	 FILE 	*destination;
	 int 	i,j,k,cont;

	 char   nom[128];
	 char	temp[128];
	 char  *pt;
	 float	x,y,z;


	 if (argc<2)
	    {
	     printf ("pas de fichier a traduire !");
	     return;
	    }

	 strcpy (nom,argv[1]);

	 Fichier=fopen(nom,"rt");
	 if(Fichier==NULL)
	   {
	    printf ("Fichier introuvable.\n");
	    return;
	   }

	 strcpy (temp,nom);
	 strtok(temp,(char*)".");
	 strcat(temp,(char*)".pt");

	 pt=strtok(nom,".");
	 pt=strtok(NULL,".\t\n ,;");
	 if (strlen(pt)<3)
	    strcat (temp,"1");
	 else if ((*pt=='p' && *(pt+1)=='t') || (*pt=='P' && *(pt+1)=='T'))
	   {
	    *nom=*(pt+2)+1;
	    *(nom+1)='\0';
	    strcat (temp,nom);
	   }
	 else
	   {
	    strcat (temp,"0");
	   }


	 destination=fopen(temp,"wt");

	 SetRepCo (&Repere,Pt(0,0,0),Vec(1,0,0),Vec(1,0,0),Vec(1,0,0));
	 SetVec   (&Repere.j,Vec(0,1,0));
	 SetVec	  (&Repere.k,Vec(0,0,1));

	 LitLigne ();
	 PremierMot (sep);

	 while (strcmp(Mot,"repere")==0)
	       {
		changerep ();
	       }
	 MotSuivant(sep);

	 while (Ligne!=NULL)
	       {
		if (strcmp(Mot,"repere")==0)
		   {
		    changerep();
		   }

		x=atof (Mot);
		MotSuivant (sep);
		y=atof (Mot);
		MotSuivant (sep);
		z=atof (Mot);

		SetPt (&Point[NbPoint],PtRepNew(Pt(x,y,z),&Repere));
		NbPoint++;
		MotSuivant(sep);
	       }


	 fprintf (destination,"%5d\n",NbPoint);

	 for (i=0;i<NbPoint;i++)
	     {
	      fprintf (destination,"%9.3f  %9.3f  %9.3f\n",
		       Point[i].x,Point[i].y,Point[i].z);
	     }

	 PtAffiche (Point,NbPoint);

	}




void changerep()
	{
	 float x,y,z;
	 int 	sortie=0;

	 do{
	 MotSuivant(sep);
	 switch (*Mot)
	 {
	  case 'O':
	  case 'o': x=atof (MotSuivant(sep));
		    y=atof (MotSuivant(sep));
		    z=atof (MotSuivant(sep));
		    SetPtCo (&Repere.O,x,y,z);
		    break;

	  case 'i':
	  case 'I': x=atof (MotSuivant(sep));
		    y=atof (MotSuivant(sep));
		    z=atof (MotSuivant(sep));
		    SetVecCo (&Repere.i,x,y,z);
		    break;
	  case 'j':
	  case 'J': x=atof (MotSuivant(sep));
		    y=atof (MotSuivant(sep));
		    z=atof (MotSuivant(sep));
		    SetVecCo (&Repere.j,x,y,z);
		    break;

	  case 'k':
	  case 'K': x=atof (MotSuivant(sep));
		    y=atof (MotSuivant(sep));
		    z=atof (MotSuivant(sep));
		    SetVecCo (&Repere.k,x,y,z);
		    break;

	  default:  sortie=1;
	 }
	 } while (sortie==0);

	 return;
	}
