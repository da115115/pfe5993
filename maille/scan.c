#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <string.h>
#include <math.h>

#include <geom.h>
#include <filedata.h>
#include <show.h>


#define COTE_DROIT 	1
#define COTE_GAUCHE    -1

void erreur (char * comment);
void general(int COTE);
void cotyle(int COTE);
void sacrum(int COTE);
void iliaque(int COTE);

sSphere* sphere_cotyle( sPoint* Q, sPoint* R,sPoint* P);



boolean bDroit=TRUE, bGauche=TRUE, bSacrum=TRUE;
boolean bPelvien=FALSE;
sRepere rPelvien;      /* repere lie au plan Pelvien */
char	cPoints[80]="points.cfg"; /* nom du fichier contenant les points */
char	cFinal [80]="maille.cfg"; /* nom du fichier a generer */
char    cDefaut[80]="defaut.cfg"; /* nom du fichier contenant les
				     donnees par defauts */
sSection sPoints, sFinal, sDefaut;

sPoint Ha1,Ha2,A,B,ESA,E,F,G,H,I,J,K,L,D,M,Ep,Fp,Gp,Hp,Ip,Jp,Kp,Lp,O,N;
sPoint T,U,V,W,P,Pp,Q,R,S,Sp,Tp,Up,Vp,Wp,Rp,Pub,Z,Is;



sPoint PtTemp;
sPoint C0,E1,E1p;

sVecteur U0,U1,VecTemp,Vec_Co;

sPlan Pl_Pub,Pl_Inf,Pl_Mil;

sCercle Ce_Cotyle,Ce_Cotyle2;

float R0,Epais;





sPoint Pt_IlEpAntSup;		/* epine iliaque antero sup et interne	*/
sPoint Pt_IlEpPostSup;		/* epine iliaque postero superieure	*/
sPoint Pt_IlEpPostInf;		/* epine iliaque postero inferieure	*/
sPoint Pt_IlMedianeAntSup;	/* point sup de la ligne mediane  	*/

sPoint Pt_SacreAntInf;         /* point antero-inf. de l'articula-
				    -tion sacro-iliaque          	*/
sPoint Pt_SacreAntSup;
sPoint Pt_SacrePostInf;

sPoint Pt_IlExtPostInf;
sPoint Pt_IlExtPostSup;
sPoint Pt_EpExtPostInf;
sPoint Pt_EpExtPostSup;


sPoint Pt_IlioPelvien;
sPoint Pt_CoCentre;            /* centre du cotyle               	*/

sPoint Pt_CoSup;		/* Point superieur du bord du cotyle	*/

sPoint Pt_CorneAnt;            /* point definissant la corne anterieure*/
sPoint Pt_CornePost;		/* point definissant la corne posterieure*/

sCercle Ce_Pelvien;	/* Cercle pelvien contenu dans le plan pelvien	*/
sCercle Ce_Cotyle,Ce_Cotyle2;
sCercle Ce_IlSupInt;	/* crete iliaque interieure */
sCercle Ce_IlSupExt;	/* crete iliaque exterieure */

sPlan Pl_Pelvien;      /* plan pelvien                         */
sPlan Pl_Scan;		/* plan des coupes scannaires		*/
sPlan Pl_CoBord;       /* plan determinant le bord du cotyle   */
sPlan Pl_CoFond;       /* plan de fond de cotyle               */


sCone Co_IlAnt;        /* Cone anterieur                       */
sCone Co_IlPost;       /* cone posterieur                      */


sSphere Sp_IlFosse;    /* sphere de la fosse iliaque           */
sSphere Sp_Cotyle;     /* sphere du cotyle                     */
sSphere Sp_CoFond;	/* sphere du fond du cotyle		*/

sCylindre Cy_Pelvien;  /* Cylindre Plevien			*/


/* cotyle */
sPoint E1,E1p,E2,E2p,N1,N1p,N1s,N2,N2p,P1,P1p,P2,P2p,T1,T1p,V1,V1p,V1s;

/* sacrum */
float LH,LV,lh,lv;


/* variables temporaires */
int		iTemp,iTemp1;
float		fTemp,fTemp1;
char		ccTemp[256],ccTemp1[256];
sPoint		pTemp,pTemp1,pTemp2,pTemp3,pTemp4,pTemp5;
sVecteur	vTemp,vTemp1,vTemp2,vTemp3;
sPlan		plTemp,plTemp1;
sCercle		cerTemp;
sCylindre	cyTemp;
sSphere		sTemp;
sDroite		dTemp,dTemp1;





/*==========================================================================
=====================      PROGRAMME PRINCIPAL      ========================
==========================================================================*/

int main (int argc,char *argv[])

{
  char    cTemp[128];
  boolean bTemp;
  int i;
/* lecture des arguments	*/
  bTemp=TRUE;
  for (i=2;i<=argc;i++)
    {
     if (*argv[i-1]!='/' && i==2)
	strcpy (cPoints,argv[i-1]);

     else if (*argv[i-1]!='/' && i==3)
	strcpy (cFinal,argv[i-1]);

     else if (*(argv[i-1]+1)=='f' || *(argv[i-1]+1)=='F')
	strcpy (cDefaut,argv[i-1]+2);


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

     else if (*(argv[i-1]+1)=='r' || *(argv[i-1]+1)=='R') bPelvien=TRUE;

     else {
	   erreur(NULL);
	   return (-1);
	  }
    }

 /* ouverture des sections */
 if (InitSection(&sPoints,cPoints,LECTURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible d'ouvrir le fichier de points : %s !",cPoints);
     erreur (cTemp);
    }

 if (InitSection(&sFinal,cFinal,ECRITURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible de creer le fichier de points : %s !",cFinal);
     FermeSection(&sPoints);
     erreur (cTemp);
    }

 if (InitSection(&sDefaut,cDefaut,LECTURE,NULL)==NONOUVRABLE)
    {
     sprintf (cTemp,"Impossible d'ouvrir le fichier par defaut : %s !",
		    cDefaut);
     FermeSection(&sPoints);
     FermeSection(&sFinal);
     erreur (cTemp);
    }


 /* verification que les sections correspondant aux elements du bassin
    existent */

 if (bDroit  && (OuvreSection(&sPoints,"droit" )!=OK)) bDroit =FALSE;
 if (bGauche && (OuvreSection(&sPoints,"gauche")!=OK)) bGauche=FALSE;
 if (bSacrum && (OuvreSection(&sPoints,"sacrum")!=OK)) bSacrum=FALSE;



 /*==================  PLAN ET REPERE PELVIEN ===========================*/
 CreeSection(&sFinal,"general",NULL);
 general (0);


 /*========================= ILION DROIT  ===============================*/

 /* lecture des points */
 if (bDroit)
 {
 OuvreSection(&sPoints,"droit");

 SetPt (&A,PtRepNew(PtSection(&sPoints,"A"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point A inexistant\n");

 SetPt (&B,PtRepNew(PtSection(&sPoints,"B"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point B inexistant\n");

 SetPt (&E,PtRepNew(PtSection(&sPoints,"E"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point E inexistant\n");

 SetPt (&Ep,PtRepNew(PtSection(&sPoints,"E'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point E' inexistant\n");

 SetPt (&ESA,PtRepNew(PtSection(&sPoints,"ESA"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point ESA inexistant\n");

 SetPt (&O,PtRepNew(PtSection(&sPoints,"O"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point O inexistant\n");

 SetPt (&W,PtRepNew(PtSection(&sPoints,"W"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point W inexistant\n");

 SetPt (&Up,PtRepNew(PtSection(&sPoints,"U'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point U' inexistant\n");

 SetPt (&Vp,PtRepNew(PtSection(&sPoints,"V'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point V' inexistant\n");

 SetPt (&Wp,PtRepNew(PtSection(&sPoints,"W'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point W' inexistant\n");

 SetPt (&Ha1,PtRepNew(PtSection(&sPoints,"Ha1"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Ha1 inexistant\n");

 SetPt (&Ha2,PtRepNew(PtSection(&sPoints,"Ha2"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Ha2 inexistant\n");

 SetPt (&N,PtRepNew(PtSection(&sPoints,"N"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point N inexistant\n");

 SetPt (&M,PtRepNew(PtSection(&sPoints,"M"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point M inexistant\n");

 SetPt (&T,PtRepNew(PtSection(&sPoints,"T"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point T inexistant\n");

 SetPt (&Tp,PtRepNew(PtSection(&sPoints,"T'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point T' inexistant\n");

 SetPt (&Pp,PtRepNew(PtSection(&sPoints,"P'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point P' inexistant\n");

 SetPt (&Sp,PtRepNew(PtSection(&sPoints,"S'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point S' inexistant\n");

 SetPt (&Rp,PtRepNew(PtSection(&sPoints,"R'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point R' inexistant\n");

 SetPt (&P,PtRepNew(PtSection(&sPoints,"P"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point P inexistant\n");

 SetPt (&R,PtRepNew(PtSection(&sPoints,"R"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point R inexistant\n");

 SetPt (&Q,PtRepNew(PtSection(&sPoints,"Q"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Q inexistant\n");

 SetPt (&U,PtRepNew(PtSection(&sPoints,"U"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point U inexistant\n");

 SetPt (&V,PtRepNew(PtSection(&sPoints,"V"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point V inexistant\n");

 SetPt (&H,PtRepNew(PtSection(&sPoints,"H"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point H inexistant\n");

 SetPt (&G,PtRepNew(PtSection(&sPoints,"G"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point G inexistant\n");

 SetPt (&Hp,PtRepNew(PtSection(&sPoints,"H'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point H' inexistant\n");

 SetPt (&Gp,PtRepNew(PtSection(&sPoints,"G'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point G' inexistant\n");

 SetPt (&J,PtRepNew(PtSection(&sPoints,"J"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point J inexistant\n");

 SetPt (&L,PtRepNew(PtSection(&sPoints,"L"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point L inexistant\n");

 SetPt (&Jp,PtRepNew(PtSection(&sPoints,"J'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point J' inexistant\n");

 SetPt (&Lp,PtRepNew(PtSection(&sPoints,"L'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point L' inexistant\n");

 SetPt (&Pub,PtRepNew(PtSection(&sPoints,"Pub"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Pub inexistant\n");

 SetPt (&Is,PtRepNew(PtSection(&sPoints,"Is"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Is inexistant\n");

 SetPt (&Z,PtRepNew(PtSection(&sPoints,"Z"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'droit' : point Z inexistant\n");


 CreeComment (&sFinal,NULL);
 CreeComment (&sFinal,NULL);
 CreeComment (&sFinal,NULL);

 CreeSection(&sFinal,"General_Droit",NULL);
 general(COTE_DROIT);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Cotyle_Droit",NULL);
 cotyle(COTE_DROIT);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Sacrum_Droit",NULL);
 sacrum(COTE_DROIT);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Iliaque_Droit",NULL);
 iliaque(COTE_DROIT);

 }


 /*========================= ILION GAUCHE  ==============================*/

 /* lecture des points */
 if (bGauche)
 {
 OuvreSection(&sPoints,"gauche");

 SetPt (&A,PtRepNew(PtSection(&sPoints,"A"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point A inexistant\n");

 SetPt (&B,PtRepNew(PtSection(&sPoints,"B"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point B inexistant\n");

 SetPt (&E,PtRepNew(PtSection(&sPoints,"E"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point E inexistant\n");

 SetPt (&Ep,PtRepNew(PtSection(&sPoints,"E'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point E' inexistant\n");

 SetPt (&ESA,PtRepNew(PtSection(&sPoints,"ESA"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point ESA inexistant\n");

 SetPt (&O,PtRepNew(PtSection(&sPoints,"O"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point O inexistant\n");

 SetPt (&W,PtRepNew(PtSection(&sPoints,"W"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point W inexistant\n");

 SetPt (&Up,PtRepNew(PtSection(&sPoints,"U'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point U' inexistant\n");

 SetPt (&Vp,PtRepNew(PtSection(&sPoints,"V'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point V' inexistant\n");

 SetPt (&Wp,PtRepNew(PtSection(&sPoints,"W'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point W' inexistant\n");

 SetPt (&Ha1,PtRepNew(PtSection(&sPoints,"Ha1"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Ha1 inexistant\n");

 SetPt (&Ha2,PtRepNew(PtSection(&sPoints,"Ha2"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Ha2 inexistant\n");

 SetPt (&N,PtRepNew(PtSection(&sPoints,"N"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point N inexistant\n");

 SetPt (&M,PtRepNew(PtSection(&sPoints,"M"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point M inexistant\n");

 SetPt (&T,PtRepNew(PtSection(&sPoints,"T"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point T inexistant\n");

 SetPt (&Tp,PtRepNew(PtSection(&sPoints,"T'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point T' inexistant\n");

 SetPt (&Pp,PtRepNew(PtSection(&sPoints,"P'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point P' inexistant\n");

 SetPt (&Sp,PtRepNew(PtSection(&sPoints,"S'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point S' inexistant\n");

 SetPt (&Rp,PtRepNew(PtSection(&sPoints,"R'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point R' inexistant\n");

 SetPt (&P,PtRepNew(PtSection(&sPoints,"P"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point P inexistant\n");

 SetPt (&R,PtRepNew(PtSection(&sPoints,"R"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point R inexistant\n");

 SetPt (&Q,PtRepNew(PtSection(&sPoints,"Q"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Q inexistant\n");

 SetPt (&U,PtRepNew(PtSection(&sPoints,"U"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point U inexistant\n");

 SetPt (&V,PtRepNew(PtSection(&sPoints,"V"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point V inexistant\n");

 SetPt (&H,PtRepNew(PtSection(&sPoints,"H"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point H inexistant\n");

 SetPt (&G,PtRepNew(PtSection(&sPoints,"G"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point G inexistant\n");

 SetPt (&Hp,PtRepNew(PtSection(&sPoints,"H'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point H' inexistant\n");

 SetPt (&Gp,PtRepNew(PtSection(&sPoints,"G'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point G' inexistant\n");

 SetPt (&J,PtRepNew(PtSection(&sPoints,"J"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point J inexistant\n");

 SetPt (&L,PtRepNew(PtSection(&sPoints,"L"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point L inexistant\n");

 SetPt (&Jp,PtRepNew(PtSection(&sPoints,"J'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point J' inexistant\n");

 SetPt (&Lp,PtRepNew(PtSection(&sPoints,"L'"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point L' inexistant\n");

 SetPt (&Pub,PtRepNew(PtSection(&sPoints,"Pub"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Pub inexistant\n");

 SetPt (&Is,PtRepNew(PtSection(&sPoints,"Is"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Is inexistant\n");

 SetPt (&Z,PtRepNew(PtSection(&sPoints,"Z"),&rPelvien));
 if (GeomERROR!=OK) erreur ("Section 'gauche' : point Z inexistant\n");


 CreeComment (&sFinal,NULL);
 CreeComment (&sFinal,NULL);
 CreeComment (&sFinal,NULL);

 CreeSection(&sFinal,"General_Gauche",NULL);
 general(COTE_GAUCHE);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Cotyle_Gauche",NULL);
 cotyle(COTE_GAUCHE);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Sacrum_Gauche",NULL);
 sacrum(COTE_GAUCHE);

 CreeComment(&sFinal,NULL);
 CreeComment(&sFinal,NULL);

 CreeSection(&sFinal,"Iliaque_Gauche",NULL);
 iliaque(COTE_GAUCHE);

 }



 FermeSection(&sPoints);
 FermeSection(&sFinal);
 FermeSection(&sDefaut);
 return (OK);
}



/*========================================================================*/
void general(int COTE)
{
 if (COTE==0)
 /* section generale independante des cotes */
 {

   /* initialisation du repere lie ou non au plan pelvien */
  SetVecCo (&vTemp,1,0,0);
  SetVecCo (&vTemp1,0,1,0);
  SetRepCo (&rPelvien,Pt(0,0,0),&vTemp,&vTemp1,Vec(0,0,1));

  /* plan pelvien */
  if ((iTemp=OuvreSection(&sPoints,"droit"))==OK)
     {
      SetPt (&pTemp,PtSection(&sPoints,"T"));
      if (GeomERROR!=OK) erreur ("Section 'droit' : point T inexistant\n");
      SetPt (&pTemp1,PtSection(&sPoints,"N"));
      if (GeomERROR!=OK) erreur ("Section 'droit' : point N inexistant\n");

      /* recherche du centre de la sphere pour determiner le repere lie au
	 plan pelvien */
      if (bPelvien)
      {
       SetPt (&P,PtSection(&sPoints,"P"));
       if (GeomERROR!=OK) erreur ("Section 'droit' : point P inexistant\n");
       SetPt (&R,PtSection(&sPoints,"R"));
       if (GeomERROR!=OK) erreur ("Section 'droit' : point R inexistant\n");
       SetPt (&Q,PtSection(&sPoints,"Q"));
       if (GeomERROR!=OK) erreur ("Section 'droit' : point Q inexistant\n");
       SetSph (&sTemp,sphere_cotyle(&Q,&R,&P));
       SetPt (&pTemp4,&sTemp.C);
      }
     }

  if ((iTemp1=OuvreSection(&sPoints,"gauche"))==OK)
     {
      SetPt (&pTemp2,PtSection(&sPoints,"T"));
      if (GeomERROR!=OK) erreur ("Section 'gauche' : point T inexistant\n");
      SetPt (&pTemp3,PtSection(&sPoints,"N"));
      if (GeomERROR!=OK) erreur ("Section 'gauche' : point N inexistant\n");

      /* recherche du centre de la sphere pour determiner le repere lie au
	 plan pelvien */
      if (bPelvien && iTemp==OK)
      {
       SetPt (&P,PtSection(&sPoints,"P"));
       if (GeomERROR!=OK) erreur ("Section 'gauche' : point P inexistant\n");
       SetPt (&R,PtSection(&sPoints,"R"));
       if (GeomERROR!=OK) erreur ("Section 'gauche' : point R inexistant\n");
       SetPt (&Q,PtSection(&sPoints,"Q"));
       if (GeomERROR!=OK) erreur ("Section 'gauche' : point Q inexistant\n");
       SetSph (&sTemp,sphere_cotyle(&Q,&R,&P));
       SetPt (&pTemp5,&sTemp.C);
      }
     }

  if (iTemp!=OK || iTemp1!=OK) return;

  /* plan passant par les points N, droit et gauche, et par le point
     milieu des T, droit et gauche. */
  SetVec(&vTemp,VecK(0.5,VecAB(&pTemp,&pTemp2)));
  SetPt (&pTemp,PtVec(&pTemp,&vTemp));
  SetPl (&plTemp,PlPt3(&pTemp1,&pTemp3,&pTemp));

  /* calcul du repere lie au plan pelvien */
  if (bPelvien)
    {
     SetPt (&pTemp4,PtPro(&pTemp4,&plTemp));
     SetPt (&pTemp5,PtPro(&pTemp5,&plTemp));
     SetVec(&vTemp,VecK(0.5,VecAB(&pTemp4,&pTemp5)));
     SetPt (&plTemp.P,PtVec(&pTemp4,&vTemp));
     /* vecteur j : de la gauche vers la droite */
     SetVec(&vTemp,VecNorme(VecAB(&pTemp5,&pTemp4)));
     /* vecteur i= j^k */
     SetVec(&vTemp1,VecVec(&vTemp,&plTemp.u));
     SetRepCo(&rPelvien,&plTemp.P,&vTemp1,&vTemp,&plTemp.u);
    }

  CreePl(&sFinal,&plTemp,"Plan_ Pelvien",NULL);
  if (bPelvien) CreeRep (&sFinal,&rPelvien,"Rep_Pelvien",NULL);
  return;
 }


 /* SphŠre cotyloidienne */

 SetCer(&Ce_Cotyle,CerPt3(&Q,&R,&P));
 SetSphCo(&Sp_Cotyle,&Ce_Cotyle.C,Ce_Cotyle.a);
 CreeSph (&sFinal,&Sp_Cotyle,"Sp_Cotyle",NULL);

 /* Diff‚rentes variables caractŠristiques */

 SetVecCo(&U0,0,0,1);       /* Vecteur directeur du cylindre pelvien */
 C0=Sp_Cotyle.C;            /* Centre de la sphŠre cotyloidienne */
 R0=Sp_Cotyle.R;            /* Rayon de cette sphŠre */
 Epais=R0*11/9;             /* Rayon du cercle Ce_Cotyle2 */

 /* Plan du bord du cotyle */

 SetVec(&VecTemp,VecAB(&Pp,&Rp));
 SetVec(&U1,VecVec(VecAB(&Pp,&Sp),&VecTemp));
 SetVec(&U1,VecNorme(VecPro(&U1,&U0)));
 SetPlCo(&Pl_CoBord,&Sp_Cotyle.C,&U1);
 CreePl (&sFinal,&Pl_CoBord,"Pl_CoBord",NULL);

 /* Cylindre pelvien : axe perpendiculaire aux scans,
	     + cercle passant par les projetes de U, V, T' sur le scan */

 SetPlCo(&Pl_Inf,&U,&U0);
 SetPt(&PtTemp,PtPro(&Tp,&Pl_Inf));
 SetCer(&Ce_Pelvien,CerPt3(&U,&V,&PtTemp));
 SetCyCo(&Cy_Pelvien,&Ce_Pelvien.C,&U0,Ce_Pelvien.a);
 CreeCy (&sFinal,&Cy_Pelvien,"Cy_Pelvien",NULL);

 /* Plan Pelvien de r‚f‚rence : U0=vecteur directeur du cylindre
				u=U0^NT; n=u^NT ;
				le plan est defini par (N, n) */
 SetVec(&VecTemp,VecAB(&N,&T));
 SetVec(&U1,VecVec(&U0,&VecTemp));
 SetVec(&U1,VecNorme(VecVec(&VecTemp,&U1)));
 SetPlCo(&Pl_Pelvien,&T,&U1);
 CreePl (&sFinal,&Pl_Pelvien,"Pl_Pelvien",NULL);

 /* Pl_Pub : Plan pelvien donnant la ligne arqu‚e (passant par E1) */

 /* E1 et E1' */
 SetVec(&Vec_Co,VecPro(VecAB(&C0,&ESA),&Pl_CoBord.u));
 SetVec(&Vec_Co,VecK(Epais,VecNorme(&Vec_Co)));
 SetPt(&E1,PtVec(&C0,&Vec_Co));
 SetPlCo(&Pl_Mil,&C0,&U0);
 SetPt(&E1p,PtSymPlan(&E1,&Pl_Mil));

 SetPlCo(&Pl_Pub,&E1,&U1);
 CreePl (&sFinal,&Pl_Pub,"Pl_Pub",NULL);

 /* cecle Pelvien : intersection plan pelvien et cylindre pelvien */
 SetCer (&Ce_Pelvien,Plan_Cylindre(&Pl_Pub,&Cy_Pelvien));
 CreeCer (&sFinal,&Ce_Pelvien,"Ce_Pelvien",NULL);

 /* Ce_Cotyle */
 /* Cercle cotylo‹dien */
 /* inter. (Pl. et Sp. cotylo‹dien) */
 SetCer(&Ce_Cotyle,Plan_Sphere(&Pl_CoBord,&Sp_Cotyle));

 /* Ce_Cotyle2=Cercle concentrique … Ce_Cotyle, de rayon Epais */
 SetCerCoCer(&Ce_Cotyle2,&Ce_Cotyle.C,&Ce_Cotyle.k,Epais);
 CreeCer (&sFinal,&Ce_Cotyle2,"Ce_Cotyle2",NULL);


}



/*========================================================================*/
/* Routine de calcul de la sphere cotyloidienne */
/*========================================================================*/

/* routine de calcul des spheres cotyloidiennes : independante de cotyle()
   car elle est aussi utilisee par general() pour determiner le repere
   lie au plan pelvien */
sSphere* sphere_cotyle(sPoint* P, sPoint* Q, sPoint* R)
{
 /* sphere cotyloidienne : centre = centre cercle (Q,R,P)
			   rayon = rayon cercle (Q,R,P) */
 static sSphere result;
 SetCer(&cerTemp,CerPt3(P,Q,R));
 SetSphCo (&result,&cerTemp.C,cerTemp.a);
 return (&result);
}


void cotyle(int COTE)
{
 float		pi,RC,Ep_Il;
/* sPoint 	E2,E2p,N1,N1p,N1s,N2,N2p,P1,P1p,P2,P2p,T1,T1p,V1,V1p,V1s;*/
 sPlan 		Pl_Inf2,Pl_Ant;
 sDroite	Dt_Il,Dt_Sup,Dt_Pub,Dt_Inf;


pi=3.14159265359;                     /* D‚finition de pi */
Ep_Il=Norme(VecAB(&A,&E));          /* Epaisseur de l'aile iliaque */


/* Plan de symŠtrie */
/* SetPlCo(&Pl_Mil2,&Pub,VecVec(&U0,VecAB(&Cy_Pelvien.p,&Tp)));*/

/* E2 et E2' */
InitDebutCer(&Ce_Cotyle2,&E1);
SetPt(&E2,PtCer(&Ce_Cotyle2,1*Ep_Il/(2*pi*Epais)));
InitDebutCer(&Ce_Cotyle2,&E1p);
SetPt(&E2p,PtCer(&Ce_Cotyle2,-1*Ep_Il/(2*pi*Epais)));

/* N1 et N1' (N1'') */
SetDtCo(&Dt_Il,&E1,VecAB(&Ep,&N));
SetPt(&N1s,Droite_Cylindre(&Dt_Il,&Cy_Pelvien));
SetPt(&N1,Plan_Droite(&Pl_Pub,Dt(&N1s,&U0)));
SetPlCo(&Pl_Inf,&E1p,&U0);
SetPt(&N1p,PtPro(&N1s,&Pl_Inf));

/* N2 et N2' */
SetPt(&N2,PtVec(&N1,VecAB(&E1,&E2)));
SetPlCo(&Pl_Inf2,&E2p,&U0);
SetPt(&N2p,PtPro(&N2,&Pl_Inf2));

/* V1 et V1' (V1'') */
SetDtCo(&Dt_Sup,&E1,VecK(-COTE,&Ce_Cotyle.k));
SetPt(&V1s,Droite_Cylindre(&Dt_Sup,&Cy_Pelvien));
SetPt(&V1,Plan_Droite(&Pl_Pub,Dt(&V1s,&U0)));
SetPt(&V1p,PtPro(&V1s,&Pl_Inf));

/* T1 et T1' (P2) */
SetVec(&Vec_Co,VecNorme(VecVec(VecK(1,&Ce_Cotyle.k),&U0)));
SetVec(&Vec_Co,VecK(-1*Epais,&Vec_Co));
SetPt(&P2,PtVec(&C0,&Vec_Co));
SetPlCo(&Pl_Ant,&P2,VecAB(&C0,&P2));
SetVec(&Pl_Pub.u,VecK(COTE,&Pl_Pub.u));
SetDt(&Dt_Pub,Plan_Plan(&Pl_Pub,&Pl_Ant));
SetPt(&T1,Droite_Cylindre(&Dt_Pub,&Cy_Pelvien));
SetPt(&T1p,PtSymPlan(&T1,&Pl_Mil));

/* P1 et P1' */
SetPt(&P1,PtPro(&T1,&Pl_CoBord));
SetVec(&Vec_Co,VecAB(&P1,&C0));
SetDtCo(&Dt_Sup,&P1,&Vec_Co);
SetPt(&P1,Cercle_Droite(&Ce_Cotyle2,&Dt_Sup));
SetPt(&P1p,PtPro(&T1p,&Pl_CoBord));
SetVec(&Vec_Co,VecAB(&P1p,&C0));
SetDtCo(&Dt_Inf,&P1p,&Vec_Co);
SetPt(&P1p,Cercle_Droite(&Ce_Cotyle2,&Dt_Inf));


 /* ecriture des points */
 CreeComment (&sFinal,NULL);

 CreePt (&sFinal,&Tp,"T'",NULL);
 CreePt (&sFinal,&Pub,"Pub",NULL);
 CreePt (&sFinal,&Is,"Is",NULL);
 CreePt (&sFinal,&Z,"Z",NULL);

 CreePt (&sFinal,&E1,"E1",NULL);
 CreePt (&sFinal,&E1p,"E1'",NULL);
 CreePt (&sFinal,&E2,"E2",NULL);
 CreePt (&sFinal,&E2p,"E2'",NULL);

 CreePt (&sFinal,&N1,"N1",NULL);
 CreePt (&sFinal,&N2,"N2",NULL);
 CreePt (&sFinal,&N1p,"N1'",NULL);
 CreePt (&sFinal,&N2p,"N2'",NULL);

 CreePt (&sFinal,&V1,"V1",NULL);
 CreePt (&sFinal,&T1,"T1",NULL);
 CreePt (&sFinal,&V1p,"V1'",NULL);
 CreePt (&sFinal,&T1p,"T1'",NULL);

 CreePt (&sFinal,&P1,"P1",NULL);
 CreePt (&sFinal,&P1p,"P1'",NULL);

 sprintf(ccTemp,"%f",Ep_Il);
 CreeComment(&sFinal,NULL);
 CreeInfo(&sFinal,"Ep_Il",ccTemp,NULL);

}






/*========================================================================*/
/*==========================           ===================================*/
/*==========================  SACRUM   ===================================*/
/*==========================           ===================================*/
/*========================================================================*/


void sacrum(int COTE)
{
 float LVlh,LHlv,LVLH;

 OuvreSection (&sDefaut,"sacrum");
 LVlh=atof(ChercheInfo(&sDefaut,"LV/lh"));
 LHlv=atof(ChercheInfo(&sDefaut,"LH/lv"));
 LVLH=atof(ChercheInfo(&sDefaut,"LV/LH"));

 /* point Pt_SacreAntInf */
 SetDtCo (&dTemp,&N,&Cy_Pelvien.u);
 SetPt   (&pTemp,Plan_Droite(&Pl_Pelvien,&dTemp));
 InitDebutCer (&Ce_Pelvien,&pTemp);
 SetPt   (&Pt_SacreAntInf,PtCer(&Ce_Pelvien,0));

 /* point Pt_SacrePostInf */
 SetVec (&vTemp,VecNorme(VecAB(&N,&M)));
 SetVec (&vTemp1,VecNorme(VecAB(&N,&O)));
 SetVec (&vTemp1,VecVec(&vTemp1,&vTemp));
 SetVec (&vTemp1,VecVec(&vTemp,&vTemp1));

 LV=Scal(VecAB(&N,&O),&vTemp1);
 lh=LV/LVlh;
 LH=LV/LVLH;
 lv=LH/LHlv;

 SetPt (&Pt_SacrePostInf,PtVec(&Pt_SacreAntInf,VecK(LV,&vTemp1)));

 /* point Pt_SacreAntSup */
 SetPt  (&Pt_SacreAntSup,PtVec(&Pt_SacreAntInf,VecK(LH,&vTemp)));


 CreePt  (&sFinal,&Pt_SacreAntInf,"Pt_SacreAntInf",NULL);
 CreePt  (&sFinal,&Pt_SacreAntSup,"Pt_SacreAntSup",NULL);
 CreePt  (&sFinal,&Pt_SacrePostInf,"Pt_SacrePostInf",NULL);

 sprintf(ccTemp,"%f",LH);
 CreeComment(&sFinal,NULL);
 CreeInfo(&sFinal,"LH",ccTemp,NULL);
 sprintf(ccTemp,"%f",LV);
 CreeInfo(&sFinal,"LV",ccTemp,NULL);
 sprintf(ccTemp,"%f",lh);
 CreeInfo(&sFinal,"lh",ccTemp,NULL);
 sprintf(ccTemp,"%f",lv);
 CreeInfo(&sFinal,"lv",ccTemp,NULL);

}




/*========================================================================*/
/*==========================           ===================================*/
/*==========================  ILIAQUE  ===================================*/
/*==========================           ===================================*/
/*========================================================================*/

void iliaque(int COTE)
{
 float Ep_Il,Ep_Mediane;
 char points[80],commentaires[80],resultat[80],sommet[80];
 char *pt;
 FILE* filetemp;

 /* crete iliaque : cercle passant par Ha2, Ha1, ESA */
 SetCer (&Ce_IlSupInt, CerPt3(&Ha2,&Ha1,&ESA));
 SetDtCo (&dTemp,&Pt_SacreAntInf,VecAB(&Pt_SacreAntInf,&Pt_SacreAntSup));
 SetPlCo (&plTemp,&Ce_IlSupInt.C,&Ce_IlSupInt.k);
 SetPt   (&pTemp,Plan_Droite(&plTemp,&dTemp));
 SetCer (&Ce_IlSupInt,CerCutPt(&Ce_IlSupInt,&pTemp,&ESA));

 /* Epine Iliaque antero Sup, face interne : Pt_EpIlAntSup */
 SetPt (&Pt_IlEpAntSup,&ESA);

 /* point Pt_IlMedianeAntSup */
 SetPt (&Pt_IlMedianeAntSup,PtCer(&Ce_IlSupInt,0));

 /* sphere iliaque */
 SetCer(&cerTemp,CerPt3(&A,&B,&M));
 SetSphCo (&Sp_IlFosse,&cerTemp.C,cerTemp.a);

 /* Epaisseurs de l'iliaque */
 Ep_Il=Norme(VecAB(&A,&ESA));
 Ep_Mediane=Norme(VecAB(&N,&Lp));


 /* Pt_IlEpPostSup : epine iliaque postero superieure */
 fTemp=LH+.5*Norme(VecAB(&Pt_SacreAntSup,&Pt_IlMedianeAntSup));
 SetVec(&vTemp,VecNorme(VecAB(&Pt_SacreAntInf,&Pt_SacreAntSup)));
 SetPt (&Pt_IlEpPostSup,PtVec(&Pt_SacrePostInf,VecK(fTemp,&vTemp)));


 /* crete iliaque externe : deduite de la crete interieure par
    translation */
 SetPt  (&pTemp,PtCer(&Ce_IlSupInt,0.05));
 SetVec (&vTemp,VecNorme(VecAB(&Pt_IlMedianeAntSup,&pTemp)));
 SetVec (&vTemp,VecVec(&vTemp,&Pl_Pelvien.u));
/* SetVec (&vTemp1,VecNorme (VecAB(&Pt_IlMedianeAntSup,&Pt_IlEpPostSup)));
 SetVec (&vTemp2,VecVec (&vTemp1,&vTemp));
 SetVec (&vTemp3,VecVec (&vTemp2,&vTemp1));
 fTemp=Angle(&vTemp,&vTemp1,&vTemp3);
 SetVec (&vTemp,VecRot(&vTemp1,&vTemp2,fTemp/2));*/
 SetPt  (&pTemp,PtVec(&Pt_IlMedianeAntSup,VecK(Ep_Il,&vTemp)));


 SetPt  (&pTemp1,PtCer(&Ce_IlSupInt,0.5));
 SetPt  (&pTemp2,PtCer(&Ce_IlSupInt,0.55));
 SetVec (&vTemp,VecNorme(VecAB(&pTemp1,&pTemp2)));
 SetVec (&vTemp,VecVec(&vTemp,&Pl_Pelvien.u));
 SetPt  (&pTemp1,PtVec(&pTemp1,VecK(Ep_Il,&vTemp)));

 SetPt  (&pTemp2,PtCer(&Ce_IlSupInt,0.95));
 SetVec (&vTemp,VecNorme(VecAB(&pTemp2,&Pt_IlEpAntSup)));
 SetVec (&vTemp,VecVec(&vTemp,&Pl_Pelvien.u));
 SetPt  (&pTemp2,PtVec(&Pt_IlEpAntSup,VecK(Ep_Il,&vTemp)));

 SetCer (&Ce_IlSupExt,CerPt3(&pTemp,&pTemp1,&pTemp2));
 SetCer (&Ce_IlSupExt,CerCutPt(&Ce_IlSupExt,&pTemp,&pTemp2));


 /* Pt_IlExtPostInf */
 SetVec (&vTemp,VecNorme(VecAB(&N1,&N2)));
 SetVec (&vTemp,VecK(Ep_Mediane,&vTemp));
 SetPt  (&Pt_IlExtPostInf,PtVec(&Pt_SacreAntInf,&vTemp));


 /* Pt_IlExtPostSup */
 SetVec (&vTemp,VecAB(&Pt_IlMedianeAntSup,PtCer(&Ce_IlSupExt,0)));
 SetPt (&Pt_IlExtPostSup,PtVec(&Pt_IlMedianeAntSup,VecK(2,&vTemp)));

 /* Pt_EpExtPostSup */
 SetVec (&vTemp,VecNorme(VecAB(&Pt_SacrePostInf,&Pt_IlEpPostSup)));
 SetVec (&vTemp1,VecNorme(VecAB(&Pt_IlEpPostSup,&Pt_IlMedianeAntSup)));
 SetVec (&vTemp,VecVec(&vTemp1,&vTemp));
 SetPt  (&Pt_EpExtPostSup,PtVec(&Pt_IlEpPostSup,VecK(Ep_Il,&vTemp)));


 /* Pt_EpExtPostInf */
 SetVec (&vTemp,VecNorme(VecAB(&Pt_SacrePostInf,&Pt_IlEpPostSup)));
 SetVec (&vTemp1,VecNorme(VecAB(&Pt_SacrePostInf,&Pt_SacreAntInf)));
 SetVec (&vTemp,VecVec(&vTemp1,&vTemp));
 SetPt  (&Pt_EpExtPostInf,PtVec(&Pt_SacrePostInf,VecK(Ep_Il/5,&vTemp)));


 /* cone anterieur */
 tmpnam(points);
 tmpnam(sommet);
 tmpnam(commentaires);
 tmpnam(resultat);

 /* ecriture fichier de points */
 if ((filetemp=fopen(points,"wt"))==NULL)
    {
     sprintf (ccTemp,"Impossible de creer le fichier temporaire %s.",points);
     erreur (ccTemp);
    }
 fprintf(filetemp,"%f\t%f\t%f\n",ESA.x,ESA.y,ESA.z);
 fprintf(filetemp,"%f\t%f\t%f\n",E.x,E.y,E.z);
 fprintf(filetemp,"%f\t%f\t%f\n",G.x,G.y,G.z);
 fprintf(filetemp,"%f\t%f\t%f\n",H.x,H.y,H.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Ep.x,Ep.y,Ep.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Gp.x,Gp.y,Gp.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Hp.x,Hp.y,Hp.z);
 fclose(filetemp);

 /* ecriture fichier sommet */
 if ((filetemp=fopen(sommet,"wt"))==NULL)
    {
     sprintf (ccTemp,"Impossible de creer le fichier temporaire %s.",sommet);
     erreur (ccTemp);
    }
 fprintf(filetemp,"%f\t%f\t%f\n",Sp_Cotyle.C.x,Sp_Cotyle.C.y,Sp_Cotyle.C.z);
 fclose(filetemp);

 /* calcul du cone */
 sprintf (ccTemp,"cone %s %s /C%s /S%s /D",points,resultat,commentaires,sommet);
 system (ccTemp);

 /* recuperation du resultat et nettoyage du disque */
 filetemp=fopen(resultat,"rt");
 strcpy (ccTemp,"");
 strcpy (ccTemp1,"");
 while (!feof(filetemp)) strcat(ccTemp,fgets(ccTemp1,80,filetemp));
 pt=ccTemp;
 SetCo (&Co_IlAnt,CoLit(&pt));


 /* cone posterieur */
 /* ecriture fichier de points */
 if ((filetemp=fopen(points,"wt"))==NULL)
    {
     sprintf (ccTemp,"Impossible de creer le fichier temporaire %s.",points);
     erreur (ccTemp);
    }
 fprintf(filetemp,"%f\t%f\t%f\n",H.x,H.y,H.z);
 fprintf(filetemp,"%f\t%f\t%f\n",J.x,J.y,J.z);
 fprintf(filetemp,"%f\t%f\t%f\n",L.x,L.y,L.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Hp.x,Hp.y,Hp.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Jp.x,Jp.y,Jp.z);
 fprintf(filetemp,"%f\t%f\t%f\n",Lp.x,Lp.y,Lp.z);
 fclose(filetemp);

 /* calcul du cone */
 sprintf (ccTemp,"cone %s %s /C%s /S%s /D",points,resultat,commentaires,sommet);
 system (ccTemp);

 /* recuperation du resultat et nettoyage du disque */
 filetemp=fopen(resultat,"rt");
 strcpy (ccTemp,"");
 strcpy (ccTemp1,"");
 while (!feof(filetemp)) strcat(ccTemp,fgets(ccTemp1,80,filetemp));
 pt=ccTemp;
 SetCo (&Co_IlPost,CoLit(&pt));

 remove(commentaires);
 remove(resultat);
 remove(sommet);
 remove(points);



 /*!!!!!!!!!!!!!!!!!!!!  BRUTUS POUR LES CONES  !!!!!!!!!!!!!!!!!!!!!!!!!*/
/* SetPt (&pTemp ,PtCer(&Ce_IlSupExt,1));
 SetVec(&vTemp ,VecNorme(VecAB(&Co_IlAnt.S,&pTemp)));
 fTemp=acos(Scal(&vTemp,&Co_IlAnt.u));
 SetVec(&vTemp1,VecVec(&Co_IlAnt.u,&vTemp));
 SetVec (&Co_IlAnt.u,VecRot(&Co_IlAnt.u,&vTemp1,-fTemp+Co_IlAnt.Agl));
 */
 SetPt (&pTemp ,PtCer(&Ce_IlSupExt,0.5));
 SetVec(&vTemp ,VecNorme(VecAB(&Co_IlAnt.S,&pTemp)));
 fTemp=acos(Scal(&vTemp,&Co_IlAnt.u));
 SetVec(&vTemp1,VecVec(&Co_IlAnt.u,&vTemp));
 SetVec (&Co_IlAnt.u,VecRot(&Co_IlAnt.u,&vTemp1,-fTemp+Co_IlAnt.Agl));


 SetPt (&pTemp ,PtCer(&Ce_IlSupExt,0.0));
 SetVec(&vTemp ,VecNorme(VecAB(&Co_IlPost.S,&pTemp)));
 fTemp=acos(Scal(&vTemp,&Co_IlPost.u));
 SetVec(&vTemp1,VecVec(&Co_IlPost.u,&vTemp));
 SetVec (&Co_IlPost.u,VecRot(&Co_IlPost.u,&vTemp1,-fTemp+Co_IlPost.Agl));




 /* ecriture des informations */
 CreeCer (&sFinal,&Ce_IlSupInt,"Ce_IlSupInt",NULL);
 CreePt  (&sFinal,&Pt_IlEpAntSup,"Pt_IlEpAntSup",NULL);
 CreePt  (&sFinal,&Pt_IlEpPostSup,"Pt_IlEpPostSup",NULL);
 CreePt  (&sFinal,&Pt_IlMedianeAntSup,"Pt_IlMedianeAntSup",NULL);
 CreeSph (&sFinal,&Sp_IlFosse,"Sp_IlFosse",NULL);

 CreeComment (&sFinal,NULL);
 CreeCer (&sFinal,&Ce_IlSupExt,"Ce_IlSupExt",NULL);
 CreeCo  (&sFinal,&Co_IlAnt,"Co_IlAnt",NULL);
 CreeCo  (&sFinal,&Co_IlPost,"Co_IlPost",NULL);
 CreePt  (&sFinal,&Pt_IlExtPostInf,"Pt_IlExtPostInf",NULL);
 CreePt  (&sFinal,&Pt_IlExtPostSup,"Pt_IlExtPostSup",NULL);
 CreePt  (&sFinal,&Pt_EpExtPostInf,"Pt_EpExtPostInf",NULL);
 CreePt  (&sFinal,&Pt_EpExtPostSup,"Pt_EpExtPostSup",NULL);

 sprintf(ccTemp,"%f",Ep_Il);
 CreeComment(&sFinal,NULL);
 CreeInfo(&sFinal,"Ep_Il",ccTemp,NULL);

 sprintf(ccTemp,"%f",Ep_Mediane);
 CreeInfo(&sFinal,"Ep_Mediane",ccTemp,NULL);

 CreeComment(&sFinal,NULL);

}




/*========================================================================*/
void erreur (char * comment)
{
 if (comment!=NULL)
    fprintf (stderr,"\n%s",comment);

 fprintf (stderr,"\nscan [fic. de points] [fic. resultats] [/Ffic. defaut] [/D] [/G] [/S] [/R]\n");
 fprintf (stderr,"   fic. de points : nom du fichier de points (par defaut : POINTS.CFG)\n");
 fprintf (stderr,"   fic. resultats : nom du fichier des resultats (par defaut : GEOM.CFG)\n");
 fprintf (stderr,"   fic. defaut    : nom du fichier contenant les info. par defauts (DEFAUT.CFG).\n");
 fprintf (stderr,"   /D             : creation des donnees pour l'ilion droit\n");
 fprintf (stderr,"   /G             : creation des donnees pour l'ilion gauche\n");
 fprintf (stderr,"   /S             : creation des donnees pour le sacrum\n");
 fprintf (stderr,"   /R             : place le repere dans le plan pelvien\n");
 fprintf (stderr,"   Par defaut tout le bassin est cr‚e, sans ˆtre forc‚ dans le plan pelvien.\n");
 exit (-1);
}


