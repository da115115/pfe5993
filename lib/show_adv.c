//#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <math.h>

#include "geom.h"
#include "geom_adv.h"

#pragma  hdrstop
#include "show.h"
#include "show_adv.h"



/*======================================================================*/
/*			 affichage des courbes				*/

void SetDataGCoCourbe   (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sCourbe),coul,Fen3DCourbeAffiche);
	}

void Fen3DAddCourbe (sFenetre3D* fen, sCourbe*tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoCourbe (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void far Fen3DCourbeAffiche (sFenetre3D* fen, sCourbe* c, int coul)
	{
	 float a;
	 sDroite d;
	 sPoint p;
	 int    i,n;

	 if (c==NULL)
	    {
	     Fen3DDtAffiche(fen,NULL,coul);
	     Fen3DCerAffiche(fen,NULL,coul);
	     return;
	    }

	 for (i=0; i<c->nbElem; i++)
	     {
	      switch (c->qui[i])
		{
		 case 1 : Fen3DDtAffiche (fen,c->ou[i],coul);
			  break;
		 case 2 : Fen3DCerAffiche(fen,c->ou[i],coul);
			  break;
		}
	     }
	}

void SuiteAddCourbe     (sFenASuite* suite, sCourbe*s, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddCourbe(&(suite->fenetre[i])->fenD,s,nb,coul);
	}

void EcranAddCourbe     (sCourbe* s, int nb, int coul)
	{
	 SuiteAddCourbe (&Show_SuiteEcran,s,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}



/*======================================================================*/
/*			 affichage des quadrillages			*/

void SetDataGCoQuadri   (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sQuadri),coul,Fen3DQuadriAffiche);
	}

void Fen3DAddQuadri (sFenetre3D* fen, sQuadri*tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoQuadri (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void far Fen3DQuadriAffiche (sFenetre3D* fen, sQuadri* c, int coul)
	{
	 sDroite d;
	 sPoint p;
	 int    i,j;

	 if (c==NULL)
	    {
	     Fen3DDtAffiche(fen,NULL,coul);
	     return;
	    }

	 for (j=0; j<c->ly; j++)
	     {
	      for (i=0; i<c->lx; i++)
		 {
		  SetDt (&d,DtPt(c->tab+i*(c->ly+1)+j,c->tab+(i+1)*(c->ly+1)+j));
		  Fen3DDtAffiche (fen,&d,coul);

		  SetDt (&d,DtPt(c->tab+i*(c->ly+1)+j,c->tab+i*(c->ly+1)+j+1));
		  Fen3DDtAffiche (fen,&d,coul);
		 }
	      i=c->lx;
	      SetDt (&d,DtPt(c->tab+i*(c->ly+1)+j,c->tab+i*(c->ly+1)+j+1));
	      Fen3DDtAffiche (fen,&d,coul);
	     }

	 j=c->ly;
	 for (i=0; i<c->lx;i++)
	     {
	      SetDt (&d,DtPt(c->tab+i*(c->ly+1)+j,c->tab+(i+1)*(c->ly+1)+j));
	      Fen3DDtAffiche (fen,&d,coul);
	     }
	}


void SuiteAddQuadri     (sFenASuite* suite, sQuadri*s, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddQuadri(&(suite->fenetre[i])->fenD,s,nb,coul);
	}

void EcranAddQuadri     (sQuadri* s, int nb, int coul)
	{
	 SuiteAddQuadri (&Show_SuiteEcran,s,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}

