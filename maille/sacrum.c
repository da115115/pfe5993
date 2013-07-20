#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>

#include "geom.h"
#include "geom_adv.h"
#include "filedata.h"
#include "show.h"


#include "maille.h"

#define NN	50


/*-----------------------  Tableau du maillage  ---------------------------*/
#define MAXNOEUD 	1000


int J,SACRUM;
float EPAISLAR;
float EPAISHAUT;
float LH;
float LV;
float lh;
float lv;
float EPSL;

int nLH,nLV,nlh,nlv;

int i,j;


int sacrum (int COTE)
{
 sPoint pTemp,pTemp1,pTemp2,pTemp3;
 sVecteur vTemp,vTemp1,vTemp2;
 static sDroite  dTemp,dTemp1,dTemp2;
 static sCourbe  cTemp;

 sSection data;


 /* initialisation des parametres		*/

 if (InitSection (&data,cDefaut,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration %s introuvable !\n",cDefaut);
      exit (-1);
     }
 OuvreSection (&data,"sacrum");
 if (GeomERROR!=OK) return 1;
 nLH=atoi(ChercheInfo(&data,"nLH"));
 nLV=atoi(ChercheInfo(&data,"nLV"));
 nlh=atoi(ChercheInfo(&data,"nlh"));
 nlv=atoi(ChercheInfo(&data,"nlv"));
 FermeSection (&data);


 if (InitSection (&data,cFormes,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration %s introuvable !\n",cFormes);
      exit (-1);
     }

 if (COTE==1) OuvreSection (&data,"Sacrum_Droit");
 else 	      OuvreSection (&data,"Sacrum_Gauche");
 if (GeomERROR!=OK) return 1;

/*=========================================================================
======			MAILLAGE DE L'ARRETE UNIQUEMENT		     ======
=========================================================================*/

 LH=atof(ChercheInfo(&data,"LH"));
 LV=atof(ChercheInfo(&data,"LV"));
 lh=atof(ChercheInfo(&data,"lh"));
 lv=atof(ChercheInfo(&data,"lv"));

 SetPt (&pTemp1,PtSection(&data,"Pt_SacreAntInf"));
 SetPt (&pTemp2,PtSection(&data,"Pt_SacreAntSup"));
 SetPt (&pTemp3,PtSection(&data,"Pt_SacrePostInf"));

 SetVec(&vTemp ,VecNorme(VecAB(&pTemp1,&pTemp2)));
 SetVec(&vTemp1,VecNorme(VecAB(&pTemp1,&pTemp3)));

 for (i=0; i<=nLH; i++)
    for (j=0; j<=nlh; j++)
       {
	SetVec (&vTemp2,VecK((float)i/nLH*LH,&vTemp));
	SetVec (&vTemp2,VecPlus(&vTemp2,VecK((float)j/nlh*lh,&vTemp1)));
	SetNdCo(&(Sacro_Iliaque[i][j]),PtVec(&pTemp1,&vTemp2),NbNoeud+2000);
	SetNd  (NdTab(&Noeud,NbNoeud),&Sacro_Iliaque[i][j]);
	NbNoeud++;
       }

 for (i=0; i<=nlv; i++)
    for (j=1; j<=nLV-nlh; j++)
       {
	SetVec (&vTemp2,VecK((float)i/nlv*lv,&vTemp));
	SetVec (&vTemp2,VecPlus(&vTemp2,
				VecK((float)j/(nLV-nlh)*(LV-lh)+lh,&vTemp1)));
	SetNdCo(&Sacro_Iliaque[i][j+nlh], PtVec(&pTemp1,&vTemp2),NbNoeud+2000);
	SetNd  (NdTab(&Noeud,NbNoeud),&Sacro_Iliaque[i][j+nlh]);
	NbNoeud++;
       }


SetDt (&dTemp,DtPt(&pTemp2,&pTemp1));
SetDt (&dTemp1,DtPt(&pTemp1,&pTemp3));
SetDt (&dTemp2,DtPt(&Sacro_Iliaque[1][5].P,&(Sacro_Iliaque[1][2].P)));
CourbeInit(&cTemp);
CourbeAddDt(&cTemp,&dTemp);
CourbeAddDt(&cTemp,&dTemp1);
CourbeAddDt(&cTemp,&dTemp2);

EcranAddCourbe(&cTemp,1,YELLOW);

 FermeSection (&data);

 return 0;

}

int SacrumNbNoeud (void)
    {
     return 530;
    }


int SacrumNbMaille (void)
    {
     return 230;
    }