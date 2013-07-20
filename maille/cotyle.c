/*************************************************************************/
/*************************************************************************/
/*************************                        ************************/
/*************************       Bassin Bas       ************************/
/*************************                        ************************/
/*************************     Bojhu & Clintos    ************************/
/*************************                        ************************/
/*************************        29/04/93        ************************/
/*************************                        ************************/
/*************************************************************************/
/*************************************************************************/





#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "geom.h"
#include "geom_adv.h"
#include "maille.h"
#include "filedata.h"
#include "show.h"



int cotyle (void)
{
static sPoint C0,Tp,Pub,Z,Is;
static sPoint E1,E1p,E2,E2p,N1,N1p,N2,N2p,P1,P1p,P2,P2p,T1,T1p,V1,V1p;
static sPoint Pubint,Pubext,Pubint2,Pubext2,Pubde,Pubdi,Pubdep,Pubdip;
static sPoint I,Ip,Miext,Pubintp,Pubextp,Ipp,Miextp;
static sPoint tAi00,tAich0,tAij0,tAijcp,tAijk;
static sPoint tB0j0,tBcej0,tBij0,tBijcp,tBijk;
static sPoint tCi00,tCi0ce,tCich0,tCichce,tCij0,tCijce,tCijk;
static sPoint tDi00,tDiil0,tDiilce,tDi0ce,tDij0,tDijce,tDijk;
static sPoint tEi00,tEil0,tEilce,tEi0ce,tEij0,tEijce,tEijk;
static sPoint Zb,Qb,Q,R,S,Mii,Mie,Mieb,Sp,Rp,Pubdis,Pubints,Pubexts,Pubdes,PtTemp;
static sPoint Pubdit,Pubintt,Pubextt,Pubdet;
static sPlan  Pl_Inf,Bof,PlTemp;
static sVecteur U0,U1,U2,U3,U4,Ray,VecTemp1,VecTemp2,Trans,Trans1,Trans2,Trans3,VecTemp;
static sDroite DtTemp,DtTp,Dt1,Dt2,Dt3,Dt4;
static sDroite N1N2,N1pN2p;
static sDroite PubdePubext,PubdepPubextp,Pubintext,N1pZ,N2pS,E2pR,E1pQ,E2pRp,N2pSp;
static sCercle CBof;
static sCercle P1E1,E1E2,E2E2p,E2pE1p,E1pP1p,P1pP1,T1V1,V1N1,T1pV1p,V1pN1p;
static sCercle T1Pubint,P1Pubde,P2pPubintp,P2Pubdep;
static sCercle T1Pubdi,PubdiPubint,P2pPubdip,PubdipPubintp;
static sCercle ZPubdis,QPubdes,ZbPubdit,QbPubdet;
static sCourbe cT1N1,cT1pN1p;
static sDroite Tempj,Tempk,Tempi,Tempjext,Tempjint,DtTpe,DtTpi;
sTabDynamique tA,tB,tC,tD,tE;
int i,j,k;
int ia,ja,ib,jb,ic,jc,id,jd;
int Deb,NbNoeudA,NbNoeudAB,NbNoeudABC,NbNoeudABCD;
int a,b,c,d,e,f,g,h,mat;
int COTYLE,CO_LARGEUR,IL_EPAISSEUR,CO_HAUTEUR;
int CO_PROFONDEUR,CO_EPAISSEUR;
int PUB_LONG1,PUB_LONG2,IS_LONG,IS_LARG;
int IS_HAUTEUR;
float R0,pos,lambda,EpaisPub,Epaisisch,Hautisch,hi,hb;
float posi,posj,posk;


/****** initialisation *****/

sSection donnees;

if (InitSection (&donnees,cFormes,LECTURE,NULL)!=OK)
   {
    printf ("Fichier de formes %s introuvable !\n",cFormes);
    exit (-1);
   }
/*
if(COTE==-1)
OuvreSection (&donnees,"General_Gauche");

if(COTE==1)
OuvreSection (&donnees,"General_Droit");
*/

if(COTE==-1)
OuvreSection (&donnees,"Cotyle_Gauche");

if(COTE==1)
OuvreSection (&donnees,"Cotyle_Droit");

SetPt(&E1,PtSection(&donnees,"E1"));
SetPt(&E1p,PtSection(&donnees,"E1'"));
SetPt(&E2,PtSection(&donnees,"E2"));
SetPt(&E2p,PtSection(&donnees,"E2'"));
SetPt(&N1,PtSection(&donnees,"N1"));
SetPt(&N2,PtSection(&donnees,"N2"));
SetPt(&N1p,PtSection(&donnees,"N1'"));
SetPt(&N2p,PtSection(&donnees,"N2'"));
SetPt(&V1,PtSection(&donnees,"V1"));
SetPt(&T1,PtSection(&donnees,"T1"));
SetPt(&V1p,PtSection(&donnees,"V1'"));
SetPt(&T1p,PtSection(&donnees,"T1'"));
SetPt(&P1,PtSection(&donnees,"P1"));
SetPt(&P1p,PtSection(&donnees,"P1'"));

SetPt(&Tp,PtSection(&donnees,"T'"));
SetPt(&Pub,PtSection(&donnees,"Pub"));
SetPt(&Is,PtSection(&donnees,"Is"));
SetPt(&Z,PtSection(&donnees,"Z"));

FermeSection(&donnees);


if (InitSection (&donnees,cDefaut,LECTURE,NULL)!=OK)
   {
    printf ("Fichier de configuration DEFAUT.CFG introuvable !\n");
    exit (-1);
   }

OuvreSection (&donnees,"cotyle");
COTYLE=atoi(ChercheInfo(&donnees,"COTYLE"));
CO_LARGEUR=atoi(ChercheInfo(&donnees,"CO_LARGEUR"));
CO_HAUTEUR=atoi(ChercheInfo(&donnees,"CO_HAUTEUR"));
CO_PROFONDEUR=atoi(ChercheInfo(&donnees,"CO_PROFONDEUR"));
CO_EPAISSEUR=atoi(ChercheInfo(&donnees,"CO_EPAISSEUR"));
PUB_LONG1=atoi(ChercheInfo(&donnees,"PUB_LONG1"));
PUB_LONG2=atoi(ChercheInfo(&donnees,"PUB_LONG2"));
IS_HAUTEUR=atoi(ChercheInfo(&donnees,"IS_HAUTEUR"));
IS_LONG=atoi(ChercheInfo(&donnees,"IS_LONG"));
IS_LARG=atoi(ChercheInfo(&donnees,"IS_LARG"));

OuvreSection (&donnees,"iliaque");
IL_EPAISSEUR=atoi(ChercheInfo(&donnees,"IL_EPAISSEUR"));

FermeSection (&donnees);

/* D‚finition des constantes */
C0=Sp_Cotyle->C;      /* Centre de la sphŠre cotylo‹dienne */
R0=Sp_Cotyle->R;      /* Rayon de cette sphŠre */
U1=Pl_Pelvien->u;     /* Vecteur normal au plan Pl_Pub */
SetVec(&U0,VecNorme(&Cy_Pelvien->u)); /* Vecteur directeur du cylindre pelvien */

/***********  initialisation des tableaux d'echanges  ************/
TabNdInit (&Iliaque_Cotyle,2,CO_EPAISSEUR+1,IL_EPAISSEUR+1);

/****** Pr‚liminaires *******/



/***************************************************/
/********** D‚finition des segments de droite ******/
/***************************************************/

/* D‚coupage du contour la face interne */
SetDt(&N1N2,DtPt(&N1,&N2));
SetDt(&N1pN2p,DtPt(&N1p,&N2p));

if (COTE==-1)
{
/* Portions d'ellipses (intersections plans-cylindre cotylo‹dien) Gauche */
SetCer(&T1V1,CerCutPt(Plan_Cylindre(PlPt3(&V1,&T1,&E1),Cy_Pelvien),&T1,&V1));
SetCer(&V1N1,CerCutPt(Plan_Cylindre(PlPt3(&N1,&V1,&E1),Cy_Pelvien),&V1,&N1));
SetCer(&V1pN1p,CerCutPt(Plan_Cylindre(PlPt3(&N1p,&V1p,&E1p),Cy_Pelvien),&V1p,&N1p));
SetCer(&T1pV1p,CerCutPt(Plan_Cylindre(PlPt3(&V1p,&T1p,&E1p),Cy_Pelvien),&T1p,&V1p));
}
if (COTE==1)
{
/* Portions d'ellipses (intersections plans-cylindre cotylo‹dien) */
SetPl(&PlTemp,PlPt3(&T1,&V1,&E1));
SetVec(&PlTemp.u,VecK(-COTE,&PlTemp.u));
SetCer(&T1V1,CerCutPt((Plan_Cylindre(&PlTemp,Cy_Pelvien)),&T1,&V1));
SetPl(&PlTemp,PlPt3(&V1,&N1,&E1));
SetVec(&PlTemp.u,VecK(-COTE,&PlTemp.u));
SetCer(&V1N1,CerCutPt(Plan_Cylindre(&PlTemp,Cy_Pelvien),&V1,&N1));
SetPl(&PlTemp,PlPt3(&N1p,&V1p,&E1p));
SetCer(&V1pN1p,CerCutPt(Plan_Cylindre(&PlTemp,Cy_Pelvien),&V1p,&N1p));
SetPl(&PlTemp,PlPt3(&V1p,&T1p,&E1p));
SetCer(&T1pV1p,CerCutPt(Plan_Cylindre(&PlTemp,Cy_Pelvien),&T1p,&V1p));
}

/* D‚coupage du contour la face externe (portions de cercle) */
SetCer(&P1E1,CerCutPt(Ce_Cotyle2,&P1,&E1));
SetCer(&E1E2,CerCutPt(Ce_Cotyle2,&E1,&E2));
SetCer(&E2E2p,CerCutPt(Ce_Cotyle2,&E2,&E2p));
SetCer(&E2pE1p,CerCutPt(Ce_Cotyle2,&E2p,&E1p));
SetCer(&E1pP1p,CerCutPt(Ce_Cotyle2,&E1p,&P1p));
SetCer(&P1pP1,CerCutPt(Ce_Cotyle2,&P1p,&P1));

/* Assemblage des portions d'ellipse */
CourbeInit(&cT1N1);
CourbeInit(&cT1pN1p);

CourbeAddCer(&cT1N1,&T1V1);
CourbeAddCer(&cT1N1,&V1N1);

CourbeAddCer(&cT1pN1p,&T1pV1p);
CourbeAddCer(&cT1pN1p,&V1pN1p);

/*
 InitEcran();
 EcranAddCer(Ce_Cotyle2,1,2);
 EcranAddDt(&N1N2,1,3);
 EcranAddDt(&N1pN2p,1,4);
 EcranAddDt(DtPt(&N2,&N2p),1,3);
 EcranAddCer(&T1V1,1,3);
 EcranAddCer(&V1N1,1,5);
 EcranAddCer(&V1pN1p,1,3);
 EcranAddCer(&T1pV1p,1,3);
 EcranAddCer(&P1E1,1,3);
 EcranAddCer(&E1E2,1,3);
 EcranAddCer(&E2E2p,1,3);
 EcranAddCer(&E2pE1p,1,3);
 EcranAddDt(DtPt(&T1p,&T1),1,3);
 FermeEcran();
*/

/*      El‚ment A       */
TabPtInit(&tA,3,CO_LARGEUR+IL_EPAISSEUR+1,CO_HAUTEUR+1,CO_PROFONDEUR+1);

for(i=0;i<=CO_LARGEUR+IL_EPAISSEUR;i++)
   {
   if (i<=CO_LARGEUR)
     {
     posi=1/(float)CO_LARGEUR*i;

     SetPt(&tAi00,PtCourbe(&cT1pN1p,posi));
     SetPt(&tAich0,PtCourbe(&cT1N1,posi));
     }
   else
     {
     posi=(float)(i-CO_LARGEUR)/(float)IL_EPAISSEUR;
     SetPt(&tAi00,PtDt(&N1pN2p,posi));
     SetPt(&tAich0,PtDt(&N1N2,posi));
     }
   SetDt(&Tempj,DtPt(&tAi00,&tAich0));

   for(j=0;j<=CO_HAUTEUR;j++)
      {
      posj=1/(float)CO_HAUTEUR*j;
      SetPt(&tAij0,PtDt(&Tempj,posj));
      if(posj==0.5 && i==0 )
	 {
	 SetPt(&P2p,&tAij0);
	 }

      SetVec(&Ray,VecNorme(VecAB(&C0,&tAij0)));
      SetPt(&tAijcp,PtVec(&C0,VecK(R0,&Ray)));
      SetDt(&Tempk,DtPt(&tAij0,&tAijcp));

      for(k=0;k<=CO_PROFONDEUR;k++)
	 {
	 posk=1/(float)CO_PROFONDEUR*k;
	 SetPt(&tAijk,PtDt(&Tempk,posk));
	 SetPt(tA.e(i,j,k),&tAijk);
	 }
      }
   }


/* Num‚rotation des noeuds de l'‚l‚ment A */

/* Premier noeud */
if (COTE==-1) Deb=1000*COTYLE;
	      else Deb=NbNoeud;

TabNdFocus(&Noeud);
NbNoeudA=NbNoeud;
for(i=0;i<=CO_LARGEUR+IL_EPAISSEUR;i++)
   for(j=0;j<=CO_HAUTEUR;j++)
      for(k=0;k<=CO_PROFONDEUR;k++)
	 {
	 a=Deb+Num_Pos(i,j,k,CO_HAUTEUR,CO_PROFONDEUR);
	 SetNdCo(Noeud.e(NbNoeud),tA.e(i,j,k),a);
	 NbNoeud++;
	 }
NbNoeudA=NbNoeud-NbNoeudA;

/* Remplissage du tableau d'‚change Iliaque_Cotyle */
for(j=0;j<=CO_EPAISSEUR;j++)
   {
   ia=CO_LARGEUR+j;
   ja=CO_HAUTEUR;
   a=Deb+Num_Pos(ia,ja,0,CO_HAUTEUR,CO_PROFONDEUR);
   SetNdCo(NdTab(&Iliaque_Cotyle,0,j),tA.p(ia,ja,0),a);
   }

/* Num‚rotation des mailles de l'‚l‚ment A */

TabMaFocus(&Maille);
for(i=0;i<=CO_LARGEUR+IL_EPAISSEUR-1;i++)
   for(j=0;j<=CO_HAUTEUR-1;j++)
      for(k=0;k<=CO_PROFONDEUR-2;k++)
	 {
	 a=Deb+Num_Pos(i,j,k,CO_HAUTEUR,CO_PROFONDEUR);
	 b=Deb+Num_Pos(i+1,j,k,CO_HAUTEUR,CO_PROFONDEUR);
	 c=Deb+Num_Pos(i+1,j+1,k,CO_HAUTEUR,CO_PROFONDEUR);
	 d=Deb+Num_Pos(i,j+1,k,CO_HAUTEUR,CO_PROFONDEUR);
	 e=Deb+Num_Pos(i,j,k+1,CO_HAUTEUR,CO_PROFONDEUR);
	 f=Deb+Num_Pos(i+1,j,k+1,CO_HAUTEUR,CO_PROFONDEUR);
	 g=Deb+Num_Pos(i+1,j+1,k+1,CO_HAUTEUR,CO_PROFONDEUR);
	 h=Deb+Num_Pos(i,j+1,k+1,CO_HAUTEUR,CO_PROFONDEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* D‚finition du tableau B */

/* D‚finition … ses limites … partir de A d‚j… d‚fini */
TabPtInit(&tB,3,CO_EPAISSEUR+1,2*(CO_LARGEUR+IL_EPAISSEUR+CO_HAUTEUR),CO_PROFONDEUR+1);

for(j=0;j<=CO_HAUTEUR;j++)
   {
   TabPtFocus(&tA);
   SetPt(&tB0j0,tA.e(0,j,0));
   TabPtFocus(&tB);
   SetPt(tB.e(0,j,0),&tB0j0);
   }

for(j=CO_HAUTEUR+1;j<=CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR;j++)
   {
   TabPtFocus(&tA);
   SetPt(&tB0j0,tA.e(j-CO_HAUTEUR,CO_HAUTEUR,0));
   TabPtFocus(&tB);
   SetPt(tB.e(0,j,0),&tB0j0);
   }

for(j=CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR+1;j<=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR;j++)
   {
   TabPtFocus(&tA);
   SetPt(&tB0j0,tA.e(CO_LARGEUR+IL_EPAISSEUR,(2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-j,0));
   TabPtFocus(&tB);
   SetPt(tB.e(0,j,0),&tB0j0);
   }

for(j=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR+1;j<=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1;j++)
   {
   TabPtFocus(&tA);
   SetPt(&tB0j0,tA.e(2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-j,0,0));
   TabPtFocus(&tB);
   SetPt(tB.e(0,j,0),&tB0j0);
   }

TabDel(&tA);

/*      El‚ment B       */
TabPtFocus(&tB);
for(j=0;j<=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1;j++)
   {
     if(j<=CO_HAUTEUR)
	{
	posj=1/(float)CO_HAUTEUR*j;
	SetPt(&tBcej0,PtCer(&P1pP1,posj));
	if (posj==0.5) SetPt(&P2,&tBcej0);
	}
     if((CO_HAUTEUR<j)&&(j<=CO_HAUTEUR+CO_LARGEUR))
	{
	posj=1/(float)CO_LARGEUR*(j-CO_HAUTEUR);
	SetPt(&tBcej0,PtCer(&P1E1,posj));
	}
     if((CO_HAUTEUR+CO_LARGEUR<j)&&(j<=CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR))
	{
	posj=1/(float)IL_EPAISSEUR*(j-(CO_HAUTEUR+CO_LARGEUR));
	SetPt(&tBcej0,PtCer(&E1E2,posj));
	}
     if((CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR<j)&&(j<=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR))
	{
	posj=1/(float)CO_HAUTEUR*(j-(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR));
	SetPt(&tBcej0,PtCer(&E2E2p,posj));
	}
     if((2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR<j)&&(j<=2*CO_HAUTEUR+CO_LARGEUR+2*IL_EPAISSEUR))
	{
	posj=1/(float)IL_EPAISSEUR*(j-(2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR));
	SetPt(&tBcej0,PtCer(&E2pE1p,posj));
	}

     if((2*CO_HAUTEUR+2*IL_EPAISSEUR+CO_LARGEUR<j)&&(j<2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)))
	{
	posj=1/(float)CO_LARGEUR*(j-(2*CO_HAUTEUR+CO_LARGEUR+2*IL_EPAISSEUR));
	SetPt(&tBcej0,PtCer(&E1pP1p,posj));
	}
     SetPt(&tB0j0,tB.e(0,j,0));
     SetDt(&Tempi,DtPt(&tB0j0,&tBcej0));

   for(i=1;i<=CO_EPAISSEUR;i++)
      {
      posi=1/(float)CO_EPAISSEUR*i;
      SetPt(&tBij0,PtDt(&Tempi,posi));
      SetVec(&Ray,VecNorme(VecAB(&C0,&tBij0)));
      SetPt(&tBijcp,PtVec(&C0,VecK(R0,&Ray)));

      SetDt(&Tempk,DtPt(&tBij0,&tBijcp));

      for(k=0;k<=CO_PROFONDEUR;k++)
	 {
	 posk=1/(float)CO_PROFONDEUR*k;
	 SetPt(&tBijk,PtDt(&Tempk,posk));
	 SetPt(tB.e(i,j,k),&tBijk);
	 }
      }
   }

/* Num‚rotation des noeuds de l'‚l‚ment B */

TabNdFocus(&Noeud);
NbNoeudAB=NbNoeud;
for(i=1;i<=CO_EPAISSEUR;i++)
   for(j=0;j<=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1;j++)
      for(k=0;k<=CO_PROFONDEUR;k++)
	 {
	 a=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 SetNdCo(Noeud.e(NbNoeud),tB.e(i,j,k),a);
	 NbNoeud++;
	 }
NbNoeudAB=NbNoeudA+(NbNoeud-NbNoeudAB);

/* Remplissage du tableau d'‚change Iliaque_Cotyle */
for(i=1;i<=CO_EPAISSEUR;i++)
   for(j=0;j<=IL_EPAISSEUR;j++)
      {
      ib=i;
      jb=CO_HAUTEUR+CO_LARGEUR+j;
      a=Deb+NbNoeudA+Num_Pos(ib-1,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      SetNdCo(NdTab(&Iliaque_Cotyle,i,j),tB.p(ib,jb,0),a);
      }

TabDel(&tB);

/* Num‚rotation des mailles de l'‚l‚ment B */

/* Par rapport … A, i->i-1 car le 1er noeud de B correspond … i=1 */
/* D‚part de i=1 car num‚ros de noeuds de la frontiŠre sont ceux de A<>B */

TabMaFocus(&Maille);
for(i=1;i<=CO_EPAISSEUR-1;i++)
   for(j=0;j<=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-2;j++)
      for(k=0;k<=CO_PROFONDEUR-1;k++)
	 if (((i!=CO_EPAISSEUR-1)&&(k!=CO_PROFONDEUR-1))||((j>=CO_HAUTEUR/2)&&(j<2*(CO_HAUTEUR+IL_EPAISSEUR)+CO_LARGEUR)))
	 {
	 a=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 b=Deb+NbNoeudA+Num_Pos(i,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 c=Deb+NbNoeudA+Num_Pos(i,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 d=Deb+NbNoeudA+Num_Pos(i-1,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 e=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 f=Deb+NbNoeudA+Num_Pos(i,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 g=Deb+NbNoeudA+Num_Pos(i,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 h=Deb+NbNoeudA+Num_Pos(i-1,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* j=cste=> Fermeture du cylindre en j de B */
for(i=1;i<=CO_EPAISSEUR-1;i++)
      for(k=0;k<=CO_PROFONDEUR-1;k++)
	 if ((i!=CO_EPAISSEUR-1)&&(k!=CO_PROFONDEUR-1))
	 {
/* j=cste=> 4 avant-derniers points du "cylindre en j de B" */
	 j=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1;
	 a=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 b=Deb+NbNoeudA+Num_Pos(i,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 e=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 f=Deb+NbNoeudA+Num_Pos(i,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);

/* j=cste=> 4 premiers points du "cylindre en j de B" */
	 j=0;
	 c=Deb+NbNoeudA+Num_Pos(i,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 d=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 g=Deb+NbNoeudA+Num_Pos(i,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 h=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* ia=i=cste=> premier plan de noeuds (communs … A et … B) */

/* Face 1 : face ant‚rieure */
for(j=0;j<=CO_HAUTEUR-1;j++)
   for(k=0;k<=CO_PROFONDEUR-1;k++)
      if ((k!=CO_PROFONDEUR-1)||(j>=CO_HAUTEUR/2))
      {
/* ia=i=cste=> Sur plan (j,k) de A (ant‚rieur) */
      i=0;
      ia=i;
      ja=j;
      a=Deb+Num_Pos(ia,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      b=Deb+Num_Pos(ia,ja+1,k,CO_HAUTEUR,CO_PROFONDEUR);
      c=Deb+Num_Pos(ia,ja+1,k+1,CO_HAUTEUR,CO_PROFONDEUR);
      d=Deb+Num_Pos(ia,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);

/* ia=i=cste=> Sur deuxiŠme plan (j,k) de B ant‚rieur */
      i=1;
      e=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      f=Deb+NbNoeudA+Num_Pos(i-1,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      g=Deb+NbNoeudA+Num_Pos(i-1,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      h=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* Face 2 : face sup‚rieure */
for(j=CO_HAUTEUR;j<=CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR-1;j++)
     for(k=0;k<=CO_PROFONDEUR-1;k++)
      {
/* i=1=cste=> Sur plan (ia,k) de A (sup‚rieur) */
      ia=j-CO_HAUTEUR;
      ja=CO_HAUTEUR;
      a=Deb+Num_Pos(ia,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      b=Deb+Num_Pos(ia+1,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      c=Deb+Num_Pos(ia+1,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);
      d=Deb+Num_Pos(ia,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);

/* i=1=cste=> Sur deuxiŠme plan (j,k) sup‚rieur de B */
      e=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      f=Deb+NbNoeudA+Num_Pos(i-1,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      g=Deb+NbNoeudA+Num_Pos(i-1,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      h=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* Face 3 : face post‚rieure */
for(j=CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR;j<=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR-1;j++)
     for(k=0;k<=CO_PROFONDEUR-1;k++)
      {
/* i=1=cste=> Sur plan (j,k) de A (post‚rieur) */
      ia=CO_LARGEUR+IL_EPAISSEUR;
      ja=(2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-j;
      a=Deb+Num_Pos(ia,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      b=Deb+Num_Pos(ia,ja-1,k,CO_HAUTEUR,CO_PROFONDEUR);
      c=Deb+Num_Pos(ia,ja-1,k+1,CO_HAUTEUR,CO_PROFONDEUR);
      d=Deb+Num_Pos(ia,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);

/* i=1=cste=> Sur deuxiŠme plan (j,k) post‚rieur de B */
      e=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      f=Deb+NbNoeudA+Num_Pos(i-1,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      g=Deb+NbNoeudA+Num_Pos(i-1,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      h=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* Face 4 : face inf‚rieure */
for(j=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR;j<=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-2;j++)
     for(k=0;k<=CO_PROFONDEUR-1;k++)
      if ((k!=CO_PROFONDEUR-1)||(j<2*(CO_HAUTEUR+IL_EPAISSEUR)+CO_LARGEUR))
      {
/* i=1=cste=> Sur plan (i,k) de A (inf‚rieur) */
      ia=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-j;
      ja=0;
      a=Deb+Num_Pos(ia,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      b=Deb+Num_Pos(ia-1,ja,k,CO_HAUTEUR,CO_PROFONDEUR);
      c=Deb+Num_Pos(ia-1,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);
      d=Deb+Num_Pos(ia,ja,k+1,CO_HAUTEUR,CO_PROFONDEUR);

/* i=1=cste=> Sur deuxiŠme plan (j,k) inf‚rieur de B */
      e=Deb+NbNoeudA+Num_Pos(i-1,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      f=Deb+NbNoeudA+Num_Pos(i-1,j+1,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      g=Deb+NbNoeudA+Num_Pos(i-1,j+1,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      h=Deb+NbNoeudA+Num_Pos(i-1,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* DerniŠres mailles : coin ant‚ro-inf‚rieur */
for(k=0;k<=CO_PROFONDEUR-2;k++)
   {
   a=Deb+Num_Pos(0,0,k,CO_HAUTEUR,CO_PROFONDEUR);
   b=Deb+Num_Pos(1,0,k,CO_HAUTEUR,CO_PROFONDEUR);
   c=Deb+Num_Pos(1,0,k+1,CO_HAUTEUR,CO_PROFONDEUR);
   d=Deb+Num_Pos(0,0,k+1,CO_HAUTEUR,CO_PROFONDEUR);

   i=1;
   j=2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1;

   e=Deb+NbNoeudA+Num_Pos(0,0,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   f=Deb+NbNoeudA+Num_Pos(0,j,k,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   g=Deb+NbNoeudA+Num_Pos(0,j,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   h=Deb+NbNoeudA+Num_Pos(0,0,k+1,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   mat=CORTICAL;
   SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
   NbMaille++;
   }



/*       ELEMENT  C      */


 SetVec(&Pl_Pub->u,&U1);

/* Points Pubint Pubext Pubde */
 SetPt(&Pubint2,PtPro(&Tp,Pl_Pub));
 SetPt(&Pubext2,PtPro(&Pub,Pl_Pub));
 EpaisPub=DtLong(DtPt(&Pubint2,&Pubext2));
 SetPt(&Pubint,Droite_Cylindre(DtPt(&Pubext2,&Pubint2),Cy_Pelvien));
 SetPt(&Pubext,PtVec(&Pubint,VecK(EpaisPub,VecNorme(VecAB(&Pubint2,&Pubext2)))));
 SetDt(&Pubintext,DtPt(&Pubint,&Pubext));

/* Cercle T1Pubint */
 SetPl(&Bof,PlPt3(&T1,&V1,&E1));
 SetCer(&CBof,Plan_Cylindre(&Bof,Cy_Pelvien));
 SetCer(&T1Pubint,CerCutPt(&CBof,&T1,&Pubint));
 SetPt(&Pubint,PtCer(&T1Pubint,1));

/* Point Pubdi, Pubde */
SetPt(&Pubdi,PtCer(&T1Pubint,1-1*EpaisPub/CerLong(&T1Pubint)));
SetPt(&Pubde,PtVec(&Pubdi,VecAB(&Pubint,&Pubext)));

/* Cercles T1Pubdi et PubdiPubint */
 SetCer(&T1Pubdi,CerCutPt(&CBof,&T1,&Pubdi));
 SetCer(&PubdiPubint,CerCutPt(&CBof,&Pubdi,&Pubint));
/*
 InitEcran();
 EcranAddCer(&T1Pubdi,1,4);
 EcranAddCer(&PubdiPubint,1,4);
 EcranAddCer(&T1Pubint,1,2);
 EcranAddCer(&CBof,1,3);
 FermeEcran();
*/

/* Cercle P1Pubde   */
 SetPt(&I,PtDt(DtPt(&Pubde,&P1),0.5));
 SetPt(&Ip,PtCer(&T1Pubint,0.5));
 lambda=0.5;
 SetPt(&Miext,PtVec(&Ip,VecK(lambda,VecAB(&Ip,&I))));
 SetCer(&P1Pubde,CerPt3(&P1,&Miext,&Pubde));
 SetCer(&P1Pubde,CerCutPt(&P1Pubde,&P1,&Pubde));
/*      */
 SetVec(&Trans,VecK(-1*EpaisPub,&U0));
 SetPt(&Pubextp,PtVec(&Pubext,&Trans));
 SetPt(&Pubintp,PtVec(&Pubint,&Trans));
 SetPt(&Pubdep,PtVec(&Pubde,&Trans));
 SetPt(&Miextp,PtVec(&Miext,&Trans));
 SetPt(&Ipp,PtVec(&Ip,&Trans));
 SetPt(&Pubdip,PtVec(&Pubdi,&Trans));
 SetDt(&DtTp,DtPt(&Pubdip,&Pubdep));
 SetPt(&Pubdep,PtDt(&DtTp,0.5*EpaisPub/DtLong(&DtTp)));
 SetDt(&DtTp,DtPt(&Pubintp,&Pubextp));
 SetPt(&Pubextp,PtDt(&DtTp,0.5*EpaisPub/DtLong(&DtTp)));
 /*      */
 SetPl(&Bof, PlPt3(&P2p,&Ipp,&Pubintp));
 SetCer(&P2pPubintp,Plan_Cylindre(&Bof,Cy_Pelvien));
 SetCer(&P2Pubdep,CerPt3(&P2,&Miextp,&Pubdep));
 SetCer(&P2Pubdep,CerCutPt(&P2Pubdep,&P2,&Pubdep));
 SetCer(&P2pPubdip,CerCutPt(&P2pPubintp,&P2p,&Pubdip));
 SetCer(&PubdipPubintp,CerCutPt(&P2pPubintp,&Pubdip,&Pubintp));


/*      */
SetDt(&PubdePubext,DtPt(&Pubde,&Pubext));
SetDt(&PubdepPubextp,DtPt(&Pubdep,&Pubextp));

/*
 InitEcran();
 EcranAddCer(&T1Pubdi,1,7);
 EcranAddCer(&PubdiPubint,1,4);
 EcranAddCer(&P1Pubde,1,3);
 EcranAddDt(&PubdePubext,1,3);
 EcranAddCer(&P2pPubdip,1,1);
 EcranAddCer(&PubdipPubintp,1,1);
 EcranAddCer(&P2Pubdep,1,5);
 EcranAddDt(&PubdepPubextp,1,5);
 FermeEcran();
*/

TabPtInit(&tC,3,PUB_LONG1+PUB_LONG2+1,CO_HAUTEUR/2+1,CO_EPAISSEUR+1);


for(i=0;i<=PUB_LONG1+PUB_LONG2;i++)
   {
   if(i<=PUB_LONG1)
     {
     posi=1/(float)PUB_LONG1*i;
     SetPt(&tCi00,PtCer(&T1Pubdi,posi));
     SetPt(&tCi0ce,PtCer(&P1Pubde,posi));
     SetPt(&tCich0,PtCer(&P2pPubdip,posi));
     SetPt(&tCichce,PtCer(&P2Pubdep,posi));
     }
   else
     {
     posi=(i-PUB_LONG1)/(float)PUB_LONG2;
     SetPt(&tCi00,PtCer(&PubdiPubint,posi));
     SetPt(&tCi0ce,PtDt(&PubdePubext,posi));
     SetPt(&tCich0,PtCer(&PubdipPubintp,posi));
     SetPt(&tCichce,PtDt(&PubdepPubextp,posi));
     }

   SetDt(&Tempjint,DtPt(&tCi00,&tCich0));
   SetDt(&Tempjext,DtPt(&tCi0ce,&tCichce));
   for(j=0;j<=CO_HAUTEUR/2;j++)
      {
      posj=1/(float)CO_HAUTEUR*j*2;
      SetPt(&tCij0,PtDt(&Tempjint,posj));
      SetPt(&tCijce,PtDt(&Tempjext,posj));
      SetDt(&Tempk,DtPt(&tCij0,&tCijce));
      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 posk=1/(float)CO_EPAISSEUR*k;
	 SetPt(&tCijk,PtDt(&Tempk,posk));
	 SetPt(tC.e(i,j,k),&tCijk);
	 }
      }
   }

/* Num‚rotation des noeuds de l'‚l‚ment C */

TabNdFocus(&Noeud);
NbNoeudABC=NbNoeud;
for(i=1;i<=PUB_LONG1+PUB_LONG2;i++)
   for(j=0;j<=CO_HAUTEUR/2;j++)
      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 a=Deb+NbNoeudAB+Num_Pos(i-1,j,k,CO_HAUTEUR/2,CO_EPAISSEUR);
	 SetNdCo(Noeud.e(NbNoeud),tC.e(i,j,k),a);
	 NbNoeud++;
	 }
NbNoeudABC=NbNoeudAB+(NbNoeud-NbNoeudABC);

TabDel(&tC);


/* Num‚rotation des mailles de l'‚l‚ment C */
TabMaFocus(&Maille);
for(i=1;i<=PUB_LONG1+PUB_LONG2-1;i++)
   for(j=0;j<=CO_HAUTEUR/2-1;j++)
      for(k=0;k<=CO_EPAISSEUR-1;k++)
	 {
	 a=Deb+NbNoeudAB+Num_Pos(i-1,j,k,CO_HAUTEUR/2,CO_EPAISSEUR);
	 b=Deb+NbNoeudAB+Num_Pos(i,j,k,CO_HAUTEUR/2,CO_EPAISSEUR);
	 c=Deb+NbNoeudAB+Num_Pos(i,j+1,k,CO_HAUTEUR/2,CO_EPAISSEUR);
	 d=Deb+NbNoeudAB+Num_Pos(i-1,j+1,k,CO_HAUTEUR/2,CO_EPAISSEUR);
	 e=Deb+NbNoeudAB+Num_Pos(i-1,j,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
	 f=Deb+NbNoeudAB+Num_Pos(i,j,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
	 g=Deb+NbNoeudAB+Num_Pos(i,j+1,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
	 h=Deb+NbNoeudAB+Num_Pos(i-1,j+1,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* Face de raccord entre B et C exclusivement (n'inclue pas le bord de B (=A)) */
for(j=0;j<=CO_HAUTEUR/2-1;j++)
   for(k=1;k<=CO_EPAISSEUR-1;k++)
      {
      ib=k;
      jb=CO_HAUTEUR-j;
      a=Deb+NbNoeudA+Num_Pos(ib-1,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      b=Deb+NbNoeudA+Num_Pos(ib,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      c=Deb+NbNoeudA+Num_Pos(ib,jb-1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      d=Deb+NbNoeudA+Num_Pos(ib-1,jb-1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);

      e=Deb+NbNoeudAB+Num_Pos(0,j,k,CO_HAUTEUR/2,CO_EPAISSEUR);
      f=Deb+NbNoeudAB+Num_Pos(0,j,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
      g=Deb+NbNoeudAB+Num_Pos(0,j+1,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
      h=Deb+NbNoeudAB+Num_Pos(0,j+1,k,CO_HAUTEUR/2,CO_EPAISSEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* Segment de raccord entre A,B et C */
for(j=0;j<=CO_HAUTEUR/2-1;j++)
   {
   ia=0;
   ja=CO_HAUTEUR-j;
   ib=1;
   jb=CO_HAUTEUR-j;
   a=Deb+Num_Pos(ia,ja,0,CO_HAUTEUR,CO_PROFONDEUR);
   b=Deb+NbNoeudA+Num_Pos(ib-1,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   c=Deb+NbNoeudA+Num_Pos(ib-1,jb-1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   d=Deb+Num_Pos(ia,ja-1,0,CO_HAUTEUR,CO_PROFONDEUR);

   e=Deb+NbNoeudAB+Num_Pos(0,j,0,CO_HAUTEUR/2,CO_EPAISSEUR);
   f=Deb+NbNoeudAB+Num_Pos(0,j,1,CO_HAUTEUR/2,CO_EPAISSEUR);
   g=Deb+NbNoeudAB+Num_Pos(0,j+1,1,CO_HAUTEUR/2,CO_EPAISSEUR);
   h=Deb+NbNoeudAB+Num_Pos(0,j+1,0,CO_HAUTEUR/2,CO_EPAISSEUR);
   mat=CORTICAL;
   SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
   NbMaille++;
   }

/* El‚ment D, E, F */

Hautisch=0.8;
Epaisisch=EpaisPub;

/* Z,Q,R,S  */

SetPlCo(&Pl_Inf,&Z,&U0);

SetPt(&PtTemp,Plan_Droite(&Pl_Inf,Dt(&Cy_Pelvien->p,&Cy_Pelvien->u)));
SetVec(&U2,VecAB(&PtTemp,&Z));
SetVec(&U2,VecNorme(&U2));
SetPt(&Z,PtVec(&PtTemp,VecK(Cy_Pelvien->r,&U2)));

SetPt(&Z,PtPro(&T1p,&Pl_Inf));

SetPt(&S,PtVec(&N2p,VecK(Hautisch,VecAB(&N2p,&Is))));
SetPt(&R,PtVec(&E2p,VecK(Hautisch,VecAB(&N2p,&Is))));
SetPt(&Q,PtVec(&Z,VecK(Epaisisch,&U2)));

SetVec(&Trans1,VecK(-0.5*EpaisPub,&U0));
SetPt(&Pubints,PtVec(&Pubintp,&Trans1));
SetPt(&Pubdis,PtVec(&Pubdip,&Trans1));
SetVec(&Trans2,VecK(0.5*EpaisPub,VecNorme(VecAB(&Pubint2,&Pubext2))));
SetPt(&Pubdes,PtVec(&Pubdis,&Trans2));
SetPt(&Pubexts,PtVec(&Pubints,&Trans2));
SetVec(&Trans3,VecK(EpaisPub,VecNorme(&Trans1)));
SetPt(&Pubintt,PtVec(&Pubints,&Trans3));
SetPt(&Pubdit,PtVec(&Pubdis,&Trans3));
SetPlCo(&Pl_Inf,&Pubintt,&Trans3);
SetPt(&Pubdet,PtPro(&Pubdep,&Pl_Inf));
SetPt(&Pubextt,PtPro(&Pubextp,&Pl_Inf));

SetCer(&ZPubdis,Plan_Cylindre(PlPt3(&Z,&Q,&Pubdis),Cy_Pelvien));
SetCer(&ZPubdis,CerCutPt(&ZPubdis,&Z,&Pubdis));
SetPt(&Mii,PtCer(&ZPubdis,0.5));
SetVec(&U3,VecNorme(VecAB(&Pubdis,&Pubdes)));
SetVec(&VecTemp,VecK(0.5*Epaisisch,VecNorme(VecPlus(&U2,&U3))));
SetPt(&Mie,PtVec(&Mii,&VecTemp));
SetCer(&QPubdes,CerPt3(&Q,&Pubdes,&Mie));
SetVec(&QPubdes.k,VecK(-1,&QPubdes.k));
SetCer(&QPubdes,CerCutPt(&QPubdes,&Q,&Pubdes));

/*  Points bas  */
hi=DtLong(DtPt(&Pubextt,&Pubexts));
SetVec(&VecTemp1,VecK(-1,&U0)); /*VecNorme(VecAB(&T1p,&Z)));*/
SetVec(&VecTemp2,VecK(0.33,VecNorme(VecAB(&S,&Z))));
SetVec(&U4,VecK(hi,VecNorme(VecPlus(&VecTemp1,&VecTemp2))));

SetPt(&Zb,PtVec(&Z,&U4));
SetPt(&Qb,PtVec(&Q,&U4));
SetCer(&ZbPubdit,Plan_Cylindre(PlPt3(&Zb,&Qb,&Pubdit),Cy_Pelvien));
SetCer(&ZbPubdit,CerCutPt(&ZbPubdit,&Zb,&Pubdit));
SetPt(&PtTemp,PtCer(&ZbPubdit,0.5));
SetPt(&Mieb,PtVec(&PtTemp,VecAB(&Mii,&Mie)));
SetCer(&QbPubdet,CerPt3(&Mieb,&Qb,&Pubdet));
SetVec(&QbPubdet.k,VecK(-1,&QbPubdet.k));
SetCer(&QbPubdet,CerCutPt(&QbPubdet,&Qb,&Pubdet));

SetDt(&E1pQ,DtPt(&P1p,&Q));
SetDt(&E2pR,DtPt(&E2p,&R));
SetDt(&N1pZ,DtPt(&T1p,&Z));
SetDt(&N2pS,DtPt(&N2p,&S));
hb=1-hi/DtLong(&N2pS);
SetPt(&Sp,PtDt(&N2pS,hb));
SetPt(&Rp,PtDt(&E2pR,hb));
SetDt(&E2pRp,DtPt(&E2p,&Rp));
SetDt(&N2pSp,DtPt(&N2p,&Sp));
SetPt(&Pubdit,PtCer(&ZbPubdit,1));
SetPt(&Pubdes,PtCer(&QPubdes,1));
SetPt(&Pubdis,PtCer(&ZPubdis,1));
SetPt(&Pubdet,PtCer(&QbPubdet,1));
SetDt(&Dt1,DtPt(&Pubdis,&Pubints));
SetDt(&Dt2,DtPt(&Pubdes,&Pubexts));
SetDt(&Dt3,DtPt(&Pubdit,&Pubintt));
SetDt(&Dt4,DtPt(&Pubdet,&Pubextt));



EcranAddDt(&E1pQ,1,2);
EcranAddDt(&E2pR,1,3);
EcranAddDt(&N1pZ,1,2);
EcranAddDt(&N2pS,1,3);
EcranAddCy(Cy_Pelvien,1,6);
EcranAddCer(&QPubdes,1,4);
EcranAddCer(&ZPubdis,1,4);
EcranAddCer(&ZbPubdit,1,2);
EcranAddCer(&QbPubdet,1,2);
/*
EcranAddDt(&Dt1,1,3);
EcranAddDt(&Dt2,1,3);
EcranAddDt(&Dt3,1,3);
EcranAddDt(&Dt4,1,5);
*/

/*  Noeuds de l'El‚ment D  */

TabPtInit(&tD,3,IS_HAUTEUR+IS_LARG+1,IL_EPAISSEUR+CO_LARGEUR+1,CO_EPAISSEUR+1);

 for(i=0;i<=(IS_HAUTEUR+IS_LARG);i++)
    {
    if(i<=IS_HAUTEUR)
      {
      posi=1/(float)IS_HAUTEUR*i;
      SetPt(&tDi00,PtDt(DtPt(&N2p,&Sp),posi));
      SetPt(&tDiil0,PtDt(DtPt(&T1p,&Z),posi));
      SetPt(&tDiilce,PtDt(DtPt(&P1p,&Q),posi));
      SetPt(&tDi0ce,PtDt(DtPt(&E2p,&Rp),posi));
      }
      else
      {
      posi=(i-IS_HAUTEUR)/(float)IS_LARG;
      SetPt(&tDi00,PtDt(DtPt(&Sp,&S),posi));
      SetPt(&tDi0ce,PtDt(DtPt(&Rp,&R),posi));
      SetPt(&tDiil0,PtDt(DtPt(&Z,&Zb),posi));
      SetPt(&tDiilce,PtDt(DtPt(&Q,&Qb),posi));
      }
      SetDt(&DtTpi,DtPt(&tDi00,&tDiil0));
      SetDt(&DtTpe,DtPt(&tDi0ce,&tDiilce));

    for(j=0;j<=IL_EPAISSEUR+CO_LARGEUR;j++)
       {
      posj=1/(float)(IL_EPAISSEUR+CO_LARGEUR)*j;
      SetPt(&tDij0,PtDt(&DtTpi,posj));
      SetPt(&tDijce,PtDt(&DtTpe,posj));
      SetDt(&Tempk,DtPt(&tDij0,&tDijce));


      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 posk=1/(float)CO_EPAISSEUR*k;
	 SetPt(&tDijk,PtDt(&Tempk,posk));
	 SetPt(tD.e(i,j,k),&tDijk);
	 }

      }

     }

/* Num‚rotation des noeuds de l'‚l‚ment D */

TabNdFocus(&Noeud);
NbNoeudABCD=NbNoeud;
for(i=1;i<=IS_HAUTEUR+IS_LARG;i++)
   for(j=0;j<=IL_EPAISSEUR+CO_LARGEUR;j++)
      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 a=Deb+NbNoeudABC+Num_Pos(i-1,j,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 SetNdCo(Noeud.e(NbNoeud),tD.e(i,j,k),a);
	 NbNoeud++;
	 }
NbNoeudABCD=NbNoeudABC+(NbNoeud-NbNoeudABCD);

TabDel(&tD);


/* Num‚rotation des mailles de l'‚l‚ment D */
TabMaFocus(&Maille);
for(i=1;i<=IS_HAUTEUR+IS_LARG-1;i++)
   for(j=0;j<=IL_EPAISSEUR+CO_LARGEUR-1;j++)
      for(k=0;k<=CO_EPAISSEUR-1;k++)
	 {
	 a=Deb+NbNoeudABC+Num_Pos(i-1,j,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 b=Deb+NbNoeudABC+Num_Pos(i,j,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 c=Deb+NbNoeudABC+Num_Pos(i,j+1,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 d=Deb+NbNoeudABC+Num_Pos(i-1,j+1,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 e=Deb+NbNoeudABC+Num_Pos(i-1,j,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 f=Deb+NbNoeudABC+Num_Pos(i,j,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 g=Deb+NbNoeudABC+Num_Pos(i,j+1,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 h=Deb+NbNoeudABC+Num_Pos(i-1,j+1,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* Face de raccord entre B et D exclusivement (n'inclue pas le bord de B (=A)) */
for(j=0;j<=IL_EPAISSEUR+CO_LARGEUR-1;j++)
   for(k=1;k<=CO_EPAISSEUR-1;k++)
      {
      ib=k;
      jb=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR+j;
      a=Deb+NbNoeudA+Num_Pos(ib-1,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      b=Deb+NbNoeudA+Num_Pos(ib,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      c=Deb+NbNoeudA+Num_Pos(ib,jb+1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
      d=Deb+NbNoeudA+Num_Pos(ib-1,jb+1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);

      e=Deb+NbNoeudABC+Num_Pos(0,j,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      f=Deb+NbNoeudABC+Num_Pos(0,j,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      g=Deb+NbNoeudABC+Num_Pos(0,j+1,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      h=Deb+NbNoeudABC+Num_Pos(0,j+1,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }

/* Segment de raccord entre A,B et D */
for(j=0;j<=IL_EPAISSEUR+CO_LARGEUR-1;j++)
   {
   ia=CO_LARGEUR+IL_EPAISSEUR-j;
   ja=0;
   ib=1;
   jb=2*CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR+j;
   a=Deb+Num_Pos(ia,ja,0,CO_HAUTEUR,CO_PROFONDEUR);
   b=Deb+NbNoeudA+Num_Pos(ib-1,jb,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   c=Deb+NbNoeudA+Num_Pos(ib-1,jb+1,0,2*(CO_HAUTEUR+CO_LARGEUR+IL_EPAISSEUR)-1,CO_PROFONDEUR);
   d=Deb+Num_Pos(ia-1,ja,0,CO_HAUTEUR,CO_PROFONDEUR);

   e=Deb+NbNoeudABC+Num_Pos(0,j,0,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
   f=Deb+NbNoeudABC+Num_Pos(0,j,1,IL_EPAISSEUR+CO_LARGEUR+CO_LARGEUR,CO_EPAISSEUR);
   g=Deb+NbNoeudABC+Num_Pos(0,j+1,1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
   h=Deb+NbNoeudABC+Num_Pos(0,j+1,0,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
   mat=CORTICAL;
   SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
   NbMaille++;
   }
/*    Element  E  */

TabPtInit(&tE,3,IS_LONG+PUB_LONG2+1,IS_LARG+1,CO_EPAISSEUR+1);

 for(i=0;i<=(IS_LONG+PUB_LONG2);i++)
    {
    if(i<=IS_LONG)
      {
      posi=1/(float)IS_LONG*i;
      SetPt(&tEi00,PtCer(&ZPubdis,posi));
      SetPt(&tEil0,PtCer(&ZbPubdit,posi));
      SetPt(&tEilce,PtCer(&QbPubdet,posi));
      SetPt(&tEi0ce,PtCer(&QPubdes,posi));
      }
      else
      {
      posi=(i-IS_LONG)/(float)(PUB_LONG2);
      SetPt(&tEi00,PtDt(&Dt1,posi));
      SetPt(&tEi0ce,PtDt(&Dt2,posi));
      SetPt(&tEil0,PtDt(&Dt3,posi));
      SetPt(&tEilce,PtDt(&Dt4,posi));
      }
      SetDt(&DtTpi,DtPt(&tEi00,&tEil0));
      SetDt(&DtTpe,DtPt(&tEi0ce,&tEilce));

    for(j=0;j<=IS_LARG;j++)
       {
      posj=1/(float)IS_LARG*j;
      SetPt(&tEij0,PtDt(&DtTpi,posj));
      SetPt(&tEijce,PtDt(&DtTpe,posj));
      SetDt(&Tempk,DtPt(&tEij0,&tEijce));


      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 posk=1/(float)CO_EPAISSEUR*k;
	 SetPt(&tEijk,PtDt(&Tempk,posk));
	 SetPt(tE.e(i,j,k),&tEijk);
	 }
       }
    }

/* Num‚rotation des noeuds de l'‚l‚ment E */

TabNdFocus(&Noeud);
for(i=1;i<=IS_LONG+PUB_LONG2;i++)
   for(j=0;j<=IS_LARG;j++)
      for(k=0;k<=CO_EPAISSEUR;k++)
	 {
	 a=Deb+NbNoeudABCD+Num_Pos(i-1,j,k,IS_LARG,CO_EPAISSEUR);
	 SetNdCo(Noeud.e(NbNoeud),tE.e(i,j,k),a);
	 NbNoeud++;
	 }

TabDel(&tE);


/* Num‚rotation des mailles de l'‚l‚ment E */
TabMaFocus(&Maille);
for(i=1;i<=IS_LONG+PUB_LONG2-1;i++)
   for(j=0;j<=IS_LARG-1;j++)
      for(k=0;k<=CO_EPAISSEUR-1;k++)
	 {
	 a=Deb+NbNoeudABCD+Num_Pos(i-1,j,k,IS_LARG,CO_EPAISSEUR);
	 b=Deb+NbNoeudABCD+Num_Pos(i,j,k,IS_LARG,CO_EPAISSEUR);
	 c=Deb+NbNoeudABCD+Num_Pos(i,j+1,k,IS_LARG,CO_EPAISSEUR);
	 d=Deb+NbNoeudABCD+Num_Pos(i-1,j+1,k,IS_LARG,CO_EPAISSEUR);
	 e=Deb+NbNoeudABCD+Num_Pos(i-1,j,k+1,IS_LARG,CO_EPAISSEUR);
	 f=Deb+NbNoeudABCD+Num_Pos(i,j,k+1,IS_LARG,CO_EPAISSEUR);
	 g=Deb+NbNoeudABCD+Num_Pos(i,j+1,k+1,IS_LARG,CO_EPAISSEUR);
	 h=Deb+NbNoeudABCD+Num_Pos(i-1,j+1,k+1,IS_LARG,CO_EPAISSEUR);
	 mat=CORTICAL;
	 SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
	 NbMaille++;
	 }

/* Face de raccord entre E et D */
for(j=0;j<=IS_LARG-1;j++)
   for(k=0;k<=CO_EPAISSEUR-1;k++)
      {
      id=IS_HAUTEUR+j;
      jd=IL_EPAISSEUR+CO_LARGEUR;
      a=Deb+NbNoeudABC+Num_Pos(id-1,jd,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      b=Deb+NbNoeudABC+Num_Pos(id-1,jd,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      c=Deb+NbNoeudABC+Num_Pos(id,jd,k+1,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);
      d=Deb+NbNoeudABC+Num_Pos(id,jd,k,IL_EPAISSEUR+CO_LARGEUR,CO_EPAISSEUR);

      e=Deb+NbNoeudABCD+Num_Pos(0,j,k,IS_LARG,CO_EPAISSEUR);
      f=Deb+NbNoeudABCD+Num_Pos(0,j,k+1,IS_LARG,CO_EPAISSEUR);
      g=Deb+NbNoeudABCD+Num_Pos(0,j+1,k+1,IS_LARG,CO_EPAISSEUR);
      h=Deb+NbNoeudABCD+Num_Pos(0,j+1,k,IS_LARG,CO_EPAISSEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }


/*     Element entre E et C   */

for(i=IS_LONG;i<=IS_LONG+PUB_LONG2-1;i++)
   for(k=0;k<=CO_EPAISSEUR-1;k++)
      {
      j=0;
      ic=PUB_LONG1-IS_LONG+i;
      jc=CO_HAUTEUR/2;
      a=Deb+NbNoeudABCD+Num_Pos(i-1,j,k,IS_LARG,CO_EPAISSEUR);
      b=Deb+NbNoeudABCD+Num_Pos(i-1,j,k+1,IS_LARG,CO_EPAISSEUR);
      c=Deb+NbNoeudABCD+Num_Pos(i,j,k+1,IS_LARG,CO_EPAISSEUR);
      d=Deb+NbNoeudABCD+Num_Pos(i,j,k,IS_LARG,CO_EPAISSEUR);

      e=Deb+NbNoeudAB+Num_Pos(ic-1,jc,k,CO_HAUTEUR/2,CO_EPAISSEUR);
      f=Deb+NbNoeudAB+Num_Pos(ic-1,jc,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
      g=Deb+NbNoeudAB+Num_Pos(ic,jc,k+1,CO_HAUTEUR/2,CO_EPAISSEUR);
      h=Deb+NbNoeudAB+Num_Pos(ic,jc,k,CO_HAUTEUR/2,CO_EPAISSEUR);
      mat=CORTICAL;
      SetMaCo(Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
      NbMaille++;
      }


mat=SPONGIEUX;
return 0;
}
int CotyleNbNoeud (void)
	{
	 return 2000;
	}

int CotyleNbMaille (void)
	{
	 return 2000;
	}
