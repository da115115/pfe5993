#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>

#include "geom.h"
#include "geom_adv.h"
#include "filedata.h"
#include "show.h"
#include "maille.h"


/* declarations de fonction */
int sacrum (int COTE);
int cotyle (void);
int iliaque(int COTE);

int IliaqueNbNoeud(void);
int CotyleNbNoeud (void);
int SacrumNbNoeud (void);

int IliaqueNbMaille(void);
int CotyleNbMaille (void);
int SacrumNbMaille (void);



/*-----------------------------------------------------------------------*/
/*---------------------  definitions des variables externes -------------*/
/*-----------------------------------------------------------------------*/

int CORTICAL,SPONGIEUX;
int COTYLE;
int COTE;


sTabDynamique  Noeud;
sTabDynamique  Maille;

int NbNoeud=0;
int NbMaille=0;

char    cFormes[80]="maille.cfg"; /* nom du fichier conetenant les formes*/
char	cPoints[80]="points.cfg"; /* nom du fichier contenant les points */
char	cFinal [80]="maille.pr";  /* nom du fichier a generer */
char    cDefaut[80]="defaut.cfg"; /* nom du fichier contenant les
				     donnees par defauts */
void erreur (char * comment);

boolean bDroit=TRUE, bGauche=TRUE, bSacrum=TRUE;

char cTemp[128];

/*--------------------  Tableaux d'echanges  ----------------------------*/

sTabDynamique Iliaque_Cotyle;
sNoeud Sacro_Iliaque[3][6];

/*-----------------  definitions des variables externes  ----------------*/

sPoint* Pt_IlEpineAnteroSup;	/* epine iliaque antero posterieure	*/
sPoint* Pt_IlEpinePostSup;	/* epine iliaque postero superieure	*/
sPoint* Pt_IlEpinePostInf;	/* epine iliaque postero inferieure	*/
sPoint* Pt_IlMedianeAnteroSup;	/* point sup de la ligne mediane  	*/
sPoint* Pt_IlMedioAnt;          /* point le plus elev‚ de la face
				    anterieure                   	*/
sPoint* Pt_SacreAntInf;         /* point antero-inf. de l'articula-
				    -tion sacro-iliaque          	*/
sPoint* Pt_CoCentre;            /* centre du cotyle               	*/

sPoint* Pt_CoSup;		/* Point superieur du bord du cotyle	*/

sPoint* Pt_CorneAnt;            /* point definissant la corne anterieure*/
sPoint* Pt_CornePost;		/* point definissant la corne posterieure*/

sCercle* Ce_Pelvien;	/* Cercle pelvien contenu dans le plan pelvien	*/
sCercle* Ce_Cotyle2;	/* Cercle cotylo‹dien */

sPlan* Pl_Pelvien;      /* plan pelvien de r‚f‚rence            */
sPlan* Pl_Pub;		/* plan pelvien donnant la ligne arqu‚e */
sPlan* Pl_Scan;		/* plan des coupes scannaires		*/
sPlan* Pl_CoBord;       /* plan determinant le bord du cotyle   */
sPlan* Pl_CoFond;       /* plan de fond de cotyle               */


sCone* Co_IlAnt;        /* Cone anterieur                       */
sCone* Co_IlPost;       /* cone posterieur                      */


sSphere *Sp_IlFosse;    /* sphere de la fosse iliaque           */
sSphere *Sp_Cotyle;     /* sphere du cotyle                     */
sSphere *Sp_CoFond;	/* sphere du fond du cotyle		*/

sCylindre* Cy_Pelvien;  /* Cylindre Pelvien			*/



/*-------------------------------------------------------------------------*/
/*----------------------------  INITIALISATION  ---------------------------*/
/*-------------------------------------------------------------------------*/
#pragma argsused
void initialisation (void)
 {
  /* declarations des variables statiques	*/
  static sPoint  pt1,pt2,pt3,pt4,pt5,pt6,pt7,pt8,pt9,pt10;
  static sCercle ce1,ce2;
  static sPlan   pl1,pl2,pl3,pl4,pl5;
  static sCone   co1,co2;
  static sSphere sp1,sp2,sp3;
  static sCylindre cy1;

  sSection donnees;

  /* declaration et initialisation des pointeurs externes		*/
  Pt_IlEpineAnteroSup=&pt1;   /* epine iliaque antero posterieur	*/
  Pt_IlEpinePostSup=&pt2;     /* epine iliaque postero superieur	*/
  Pt_IlEpinePostInf=&pt3;     /* epine iliaque postero inferieure	*/
  Pt_IlMedianeAnteroSup=&pt4; /* point sup de la ligne mediane  	*/
  Pt_IlMedioAnt=&pt5;         /* point le plus elev‚ de la face
					   anterieure			*/
  Pt_SacreAntInf=&pt6;        /* point antero-inf. de l'articulation
					   sacro-iliaque            	*/
  Pt_CoCentre=&pt7;           /* centre du cotyle               	*/
  Pt_CoSup=&pt8;	      /* Point superieur du bord du cotyle	*/

  Pt_CorneAnt=&pt9;           /* corne anterieure			*/
  Pt_CornePost=&pt10;	      /* corne posterieure			*/

  Ce_Pelvien=&ce1;	      /* cercle pelvien du plan pelvien		*/
  Ce_Cotyle2=&ce2;	      /* cercle cotylo‹dien			*/

  Pl_Pelvien=&pl1;       /* plan pelvien de r‚f‚rence            	*/
  Pl_Pub=&pl5;		 /* plan pelvien donnant la ligne arqu‚e	*/
  Pl_Scan   =&pl2;	 /* plan des coupes scannaires			*/
  Pl_CoBord=&pl3;        /* plan determinant le bord du cotyle   	*/
  Pl_CoFond=&pl4;        /* plan de fond de cotyle               	*/


  Co_IlAnt=&co1;         /* Cone anterieur                       	*/
  Co_IlPost=&co2;        /* cone posterieur                      	*/


  Sp_IlFosse=&sp1;     	/* sphere de la fosse iliaque           	*/
  Sp_Cotyle=&sp2;      	/* sphere du cotyle                     	*/
  Sp_CoFond=&sp3;	/* sphere du fond du cotyle			*/

  Cy_Pelvien=&cy1;	/* cylindre pelvien				*/

  /*---------------------------  Initialisation  -----------------------*/

 if (InitSection (&donnees,cFormes,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration MAILLE.CFG introuvable !\n");
      exit (-1);
     }

 if (COTE==1)
  OuvreSection (&donnees,"General_Droit");

 if (COTE==-1)
  OuvreSection (&donnees,"General_Gauche");

  SetCy(Cy_Pelvien,CySection(&donnees,"Cy_Pelvien"));
  SetPl(Pl_Pelvien,PlSection(&donnees,"Pl_Pelvien"));
  SetPl(Pl_Pub,PlSection(&donnees,"Pl_Pub"));
  SetPl(Pl_CoBord,PlSection(&donnees,"Pl_CoBord"));
  SetCer(Ce_Pelvien,CerSection(&donnees,"Ce_Pelvien"));
  SetSph(Sp_Cotyle,SphSection(&donnees,"Sp_Cotyle"));
  SetCer(Ce_Cotyle2,CerSection(&donnees,"Ce_Cotyle2"));

/*
  SetSph    	(Sp_IlFosse,SphSection(&donnees,"Sp_IlFosse"));
  SetPl     	(Pl_Scan   ,PlSection(&donnees,"Pl_Scan"));
  SetPt  	(Pt_CoCentre,PtSection(&donnees,"Pt_CoCentre"));
  SetSphCo 	(Sp_Cotyle,Pt_CoCentre,
			   atof(ChercheInfo(&donnees,"rayon_Cotyle")));
  SetSphCo	(Sp_CoFond,Pt_CoCentre,
			   atof(ChercheInfo(&donnees,"rayon_CoFond")));
  SetPl     	(Pl_CoFond,PlSection(&donnees,"Pl_CoFond"));
  SetPt		(Pt_IlMedianeAnteroSup,
		 PtSection(&donnees,"Pt_IlMedianeAnteroSup"));
  SetPt		(Pt_IlEpineAnteroSup,PtSection(&donnees,"Pt_IlEpineAnteroSup"));
  SetPt		(Pt_IlEpinePostSup,PtSection(&donnees,"Pt_IlEpinePostSup"));
  SetPt		(Pt_IlEpinePostInf,PtSection(&donnees,"Pt_IlEpinePostInf"));
  SetPt		(Pt_IlMedioAnt,PtSection(&donnees,"Pt_IlMedioAnt"));
  SetPt		(Pt_SacreAntInf,PtSection(&donnees,"Pt_SacreAntInf"));
  SetPt		(Pt_CoSup,PtSection(&donnees,"Pt_CoSup"));
  SetPt		(Pt_CorneAnt,PtSection(&donnees,"Pt_CorneAnt"));
  SetPt		(Pt_CornePost,PtSection(&donnees,"Pt_CornePost"));

  SetCo  	(Co_IlAnt,CoSection(&donnees,"Co_IlAnt"));
  SetCo		(Co_IlPost,CoSection(&donnees,"Co_IlPost"));
*/

  FermeSection (&donnees);

  return;
 }



void fin (void)
 {
  TabDel (&Iliaque_Cotyle);

  TabDel (&Noeud);
  TabDel (&Maille);
 }




/*----------------------------------------------------------------------*/
/*------------------  ecriture d'un fichier au format ANSYS  -----------*/
/*----------------------------------------------------------------------*/

void ansys (void)
{
 int i,j,k;
 FILE *f;
 sSection donnees;
 sMaille* pMaille;

 /*---------------------  Numero de materiaux  --------------------------*/

 f=fopen(cFinal,"wt");

 fprintf(f,"/nopr\n\n");
 fprintf(f,"ET,1,45\n\n");

 for (i=0;i<NbNoeud;i++)
     fprintf(f,"N,%d,\%f,\%f,\%f,0.0,0.0,0.0,\n",NdTab(&Noeud,i)->num,
	     NdTab(&Noeud,i)->P.x,NdTab(&Noeud,i)->P.y,
	     NdTab(&Noeud,i)->P.z);
 fprintf(f,"\n\n");


 for (i=0;i<NbMaille;i++)
     {
      fprintf(f,"E");
      pMaille=MaTab (&Maille,i);
      for (j=0;j<8;j++)
	  fprintf(f,",%d",pMaille->P[j]);
      fprintf(f,"\n");
     }

 fprintf(f,"\n/type,1,4\n");
 fprintf(f,"\n/view,1,0,-1,0\n");
 fprintf(f,"\neplot\n");

/* FermeSection (&donnees);*/

 fclose (f);
}



/*----------------------------------------------------------------------*/
/*------------------  ecriture d'un fichier au format MARK   -----------*/
/*----------------------------------------------------------------------*/

void mentat (void)
{
 int i,j,k;
 FILE *f;
 sSection donnees;
 sMaille* pMaille;

 /*---------------------  Numero de materiaux  --------------------------*/

 f=fopen(cFinal,"wt");

 fprintf(f,"TITLE Os Iliaque\n");
 fprintf(f,"SISING 100000 %d %d\n",NbMaille,NbNoeud);
 fprintf(f,"ELEMENTS 7\nEND\n\n");

 fprintf(f,"CONNECTIVITY\n");
 for (i=0;i<NbMaille;i++)
     {
      fprintf(f,"%d 7",i+1);
      pMaille=MaTab(&Maille,i);
      for (j=0;j<8;j++)
	  fprintf(f," %d",pMaille->P[j]);
      fprintf(f,"\n");
     }

 fprintf (f,"COORDINATES\n");
 for (i=0;i<NbNoeud;i++)
     fprintf(f,"%d %f %f %f\n",NdTab(&Noeud,i)->num,NdTab(&Noeud,i)->P.x,
	     NdTab(&Noeud,i)->P.y,NdTab(&Noeud,i)->P.z);

 FermeSection (&donnees);

 fclose (f);
}


/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
/*-----------------------    MAIN     ----------------------*/
/*----------------------------------------------------------*/
int main (int argc,char *argv[])

{
  sSection donnees;
  int cont;
  int i;
  boolean bTemp=TRUE;

/* lecture des arguments	*/
  for (i=2;i<=argc;i++)
    {
     if (*argv[i-1]!='/' && i==2)
	strcpy (cFormes,argv[i-1]);

     else if (*(argv[i-1]+1)=='f' || *(argv[i-1]+1)=='F')
	strcpy (cDefaut,argv[i-1]+2);

     else if (*(argv[i-1]+1)=='p' || *(argv[i-1]+1)=='P')
	strcpy (cPoints,argv[i-1]+2);

     else if (*(argv[i-1]+1)=='r' || *(argv[i-1]+1)=='R')
	strcpy (cFinal,argv[i-1]+2);


     else if (*(argv[i-1]+1)=='d' || *(argv[i-1]+1)=='D')
	   {
	    if (bTemp)
	      {
	       bGauche=FALSE;
	       bSacrum=FALSE;
	       bTemp=TRUE;
	      }
	    bDroit=TRUE;
	   }

     else if (*(argv[i-1]+1)=='s' || *(argv[i-1]+1)=='S')
	   {
	    if (bTemp)
	      {
	       bGauche=FALSE;
	       bDroit=FALSE;
	       bTemp=TRUE;
	      }
	    bSacrum=TRUE;
	   }

     else if (*(argv[i-1]+1)=='g' || *(argv[i-1]+1)=='G')
	   {
	    if (bTemp)
	      {
	       bDroit=FALSE;
	       bSacrum=FALSE;
	       bTemp=TRUE;
	      }
	    bGauche=TRUE;
	   }
     else {
	   erreur(NULL);
	   return (-1);
	  }
    }



 /* ouverture des sections */
 if (InitSection(&donnees,cFormes,LECTURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible d'ouvrir le fichier de formes : %s !",cFormes);
     erreur (cTemp);
    }
 FermeSection (&donnees);

 if (InitSection(&donnees,cPoints,LECTURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible d'ouvrir le fichier de points : %s !",cPoints);
     erreur (cTemp);
    }
 FermeSection(&donnees);

 if (InitSection(&donnees,cDefaut,LECTURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible d'ouvrir le fichier par defaut : %s !",
		    cDefaut);
     erreur (cTemp);
    }
 FermeSection (&donnees);



/*-----------------  Initialisation des tableaux du maillage  -----------*/
  i=0;
  if (bDroit) i=1;
  if (bGauche) i++;

  cont=(IliaqueNbNoeud()+CotyleNbNoeud())*i;
  if (bSacrum) cont=cont+SacrumNbNoeud();
  TabNdInit (&Noeud,1,cont);

  cont=(IliaqueNbMaille()+CotyleNbMaille())*i;
  if (bSacrum) cont=cont+SacrumNbMaille();
  TabMaInit (&Maille,1,cont);


  NbMaille=0;
  NbNoeud=0;


 if (bGauche)
    {
     COTE=-1;
     initialisation ();

InitEcran ();
     sacrum (COTE);
     cotyle ();
     /*iliaque (COTE);*/
FermeEcran ();
    }

 if (bDroit)
    {
     COTE=1;
     initialisation ();

InitEcran ();
     sacrum (COTE);
     cotyle ();
     /*iliaque (COTE);*/
FermeEcran ();
    }







 InitEcran();
 EcranAddNd (Noeud.tab,NbNoeud,YELLOW);
 EcranAddNd (Iliaque_Cotyle.tab,16,RED);
 FermeEcran();



 ansys();

 fin ();

 return 0;
}


/*========================================================================*/
void erreur (char * comment)
{
 if (comment!=NULL)
    fprintf (stderr,"\n%s",comment);

 fprintf (stderr,"\nmaille [fic. de formes] [/Pfic. de points] [/Ffic. defaut] [/Rfic.resultat]");
 fprintf (stderr,"\n       [/D] [/G] [/S]\n");
 fprintf (stderr,"   fic. de formes : nom du fichier de formes (par defaut : MAILLE.CFG)\n");
 fprintf (stderr,"   fic. de points : nom du fichier de points (par defaut : POINTS.CFG)\n");
 fprintf (stderr,"   fic. defaut    : nom du fichier contenant les info. par defaut (DEFAUT.CFG).\n");
 fprintf (stderr,"   fic. resultat  : nom du fichier de resultat (par defaut : MAILLE.PR)\n");
 fprintf (stderr,"   /D             : creation de l'ilion droit\n");
 fprintf (stderr,"   /G             : creation de l'ilion gauche\n");
 fprintf (stderr,"   /S             : creation du sacrum\n");
 fprintf (stderr,"   Par defaut tout le bassin est cr‚e.\n");
 exit (-1);
}


