#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

#include "geom.h"

#pragma  hdrstop
#include "show.h"

/*======================================================================*/
/*			Variables globales				*/

#define CLAIRE	LIGHTGRAY
#define SOMBRE	DARKGRAY

#define ROTATION	0	/* definition associees aux types de 	*/
#define TRANSLATION	1	/* deplacement dans les fenetres d'anim.*/

sFenetre*   Show_pFenetre=NULL;	 /* pointeur sur la fenetre courante	*/
sFenetre3D* Show_pFenetre3D=NULL;/* pointeur sur la fenetre 3D courante	*/

sFenASuite  Show_SuiteEcran;     /* suite de l'affichage par defaut 	*/

/*======================================================================*/
/*			fonctions statiques				*/

static char*    VitesseText (sFenetreA* fena,int type,float vitesse);
				/* renvoie les commentaires sur la vitesse
				   dans une fenetre d'animation		*/
static char FenControleAnime (sFenetreA* fen);
				/* gestion des fenetres de controles	*/
static char FenControleResetEsclave (sFenetreA* fena);
				/* reaffiche une fenetre esclave	*/
static void FenAAddMaitre (sFenetreA* fena,sFenetreA* maitre);
				/* rajoute un maitre a une fenetre anim.*/



/*======================================================================*/
/*			fonctions DataG					*/

void SetDataGCo (sDataG* d, void* t, int n, int tail,int coul,void far *fonc)
	{
	 d->Tab=t;
	 d->Taille=tail;
	 d->NbObjets=n;
	 d->Couleur=coul;
	 d->AfficheObjet=(tGFonc) fonc;
	}


void SetDataGCoPt (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sPoint),coul,Fen3DPtAffiche);
	}


void SetDataGCoNd (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sNoeud),coul,Fen3DNdAffiche);
	}


void SetDataGCoDt (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sDroite),coul,Fen3DDtAffiche);
	}

void SetDataGCoCer (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sCercle),coul,Fen3DCerAffiche);
	}

void SetDataGCoSph (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sSphere),coul,Fen3DSphAffiche);
	}

void SetDataGCoCy (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sCylindre),coul,Fen3DCyAffiche);
	}

void SetDataGCoCo (sDataG* d, void* t, int n, int coul)
	{
	 SetDataGCo (d,t,n,sizeof(sCone),coul,Fen3DCoAffiche);
	}


void SetDataG (sDataG* A, sDataG* B)
	{
	 A->Tab=B->Tab;
	 A->Taille=B->Taille;
	 A->NbObjets=B->NbObjets;
	 A->Couleur=B->Couleur;
	 A->AfficheObjet=B->AfficheObjet;
	}

void DataGAffiche (sDataG* d, sFenetre3D* Fen3D)
	{
	 int i;
	 char* p;

	 p=d->Tab;
	 d->AfficheObjet  (Fen3D,NULL,d->Couleur);
	 for (i=0; i<d->NbObjets; i++)
	     {
	      d->AfficheObjet (Fen3D,(void*)p,d->Couleur);
	      p=p+d->Taille;
	     }
	}


/*======================================================================*/
/*			fonctions fenetre				*/

void SetFenCo (sFenetre* fenetre,int x,int y,int lx, int ly)
	{
	 fenetre->x=x;
	 fenetre->y=y;
	 fenetre->lx=lx;
	 fenetre->ly=ly;
	 fenetre->style=SIMPLE;
	 FenFocus(fenetre);
	}

void FenFocus (sFenetre* fen)
	{
	 if (Show_pFenetre!=NULL)
	    FenLostFocus(Show_pFenetre);

	 switch (fen->style)
	  {
	   case BORDURE:
		   setcolor(CLAIRE);
		   rectangle (fen->x,fen->y,fen->lx+fen->x,fen->ly+fen->y);
		   break;
	   case ANIMATION:
	   case CONTROLE:
		   setcolor(CLAIRE);
		   rectangle (fen->x,fen->y-15,fen->lx+fen->x,fen->ly+fen->y);
		   break;
	  }

	 setviewport (fen->x+1,fen->y+1,fen->x+fen->lx-2,fen->y+fen->ly-2,1);

	 Show_pFenetre=fen;
	}

void FenLostFocus (sFenetre *fen)
	{
	 setviewport(0,0,getmaxx(),getmaxy(),0);
	 switch (fen->style)
	  {
	   case BORDURE:
		   setcolor (SOMBRE);
		   rectangle (fen->x,fen->y,fen->x+fen->lx,fen->y+fen->ly);
		   break;
	   case ANIMATION:
	   case CONTROLE:
		   setcolor(SOMBRE);
		   rectangle (fen->x,fen->y-15,fen->lx+fen->x,fen->ly+fen->y);
		   break;
	  }
	}

void SetFen (sFenetre* fenA,sFenetre *fenB)
	{
	 SetFenCo (fenA,fenB->x,fenB->y,fenB->lx,fenB->ly);
	 FenStyle (fenA,fenB->style);
	}

void FenStyle (sFenetre *fen,int style)
	{
	 fen->style=style;
	 switch (style)
	  {
	   case ANIMATION:
		   fen->y+=15;
		   fen->ly-=15;
		   break;
	  }
	 FenFocus(fen);
	}


/*======================================================================*/
/*			fonctions fenetre texte				*/

void SetFenTCo (sFenetreT* fenT,sFenetre* fen)
	{
	 SetFen (&fenT->fenetre,fen);
	 fenT->texte=LIGHTBLUE;
	 fenT->question=LIGHTGREEN;
	}

void FenTFocus (sFenetreT* fenT)
	{
	 FenFocus (&fenT->fenetre);
	}

void FenTSet   (sFenetreT* fenT,char* texte)
	{
	 int oldcolor;
	 struct viewporttype viewinfo;
	 getviewsettings(&viewinfo);
	 setviewport (fenT->fenetre.x+1,fenT->fenetre.y+1,
		      fenT->fenetre.x+fenT->fenetre.lx-2,
		      fenT->fenetre.y+fenT->fenetre.ly-2,1);
	 clearviewport();
	 oldcolor=getcolor();
	 setcolor (fenT->texte);
	 outtextxy(2,fenT->fenetre.ly/2-textheight(texte)/2,texte);
	 setcolor (oldcolor);
	 setviewport (viewinfo.left,viewinfo.top,
		      viewinfo.right,viewinfo.bottom,viewinfo.clip);
	}


/*======================================================================*/
/*			fonctions fenetre 3D				*/

void SetFen3DCo (sFenetre3D* fen3D,sFenetre* fenetre,sPoint* O,
				   sVecteur* u,float angle)
	{
	 sVecteur i;
	 SetFen   (&fen3D->fenetre,fenetre);
	 SetPt    (&fen3D->O,O);
	 SetPtCo (&fen3D->focus,0,0,0);
	 SetVec   (&fen3D->k,VecNorme(u));
	 SetVecCo (&i,1,0,0);
	 if (fabs(Scal(u,&i))>0.7)
	    SetVecCo(&i,0,1,0);
	 SetVec (&i,VecVec(&i,u));
	 SetVec (&fen3D->i,VecNorme(VecVec(u,&i)));
	 fen3D->angle=angle;
	 fen3D->nbdata=0;
	 fen3D->GetXY=Fen3DGetXYAxono;
	 Fen3DFocus(fen3D);
	}


void SetFen3DNdCo (sFenetre3D* fen3D,sFenetre* fenetre,
		   sPoint* O,sVecteur* u,float angle,sNoeud* tab,int nb)
	{
	 SetFen3DCo (fen3D,fenetre,O,u,angle);
	 Fen3DAddNd (fen3D,tab,nb,WHITE);
	}

void SetFen3DPtCo (sFenetre3D *fen3D,sFenetre* fenetre,
		   sPoint* O,sVecteur* u,float angle,sPoint* tab,int nb)
	{
	 SetFen3DCo (fen3D,fenetre,O,u,angle);
	 Fen3DAddPt (fen3D,tab,nb,WHITE);
	}



void SetFen3D	  (sFenetre3D* fena,sFenetre3D* fenb)
	{
	 int i;
	 SetFen (&fena->fenetre,&fenb->fenetre);
	 SetPt  (&fena->O,&fenb->O);
	 SetPt  (&fena->focus,&fenb->focus);
	 SetVec (&fena->i,&fenb->i);
	 SetVec (&fena->k,&fenb->k);
	 fena->angle=fenb->angle;
	 for (i=0;i<fenb->nbdata;i++)
	      SetDataG(&fena->data[i],&fenb->data[i]);
	 fena->nbdata=fenb->nbdata;
	 fena->GetXY=fenb->GetXY;
	 Fen3DFocus (fena);
	}


void Fen3DAddPt (sFenetre3D* fen, sPoint* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoPt (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddNd (sFenetre3D* fen, sNoeud* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoNd (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddDt (sFenetre3D* fen, sDroite* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoDt (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddCer (sFenetre3D* fen, sCercle* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoCer (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddSph (sFenetre3D* fen, sSphere* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoSph (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddCy (sFenetre3D* fen, sCylindre* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoCy (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}

void Fen3DAddCo (sFenetre3D* fen, sCone* tab, int n, int coul)
	{
	 if (fen->nbdata==MAXDATAG) return;
	 SetDataGCoCo (&fen->data[fen->nbdata],tab,n,coul);
	 Fen3DFocus (fen);
	 DataGAffiche (&fen->data[fen->nbdata],fen);
	 fen->nbdata++;
	}


void Fen3DDataChange (sFenetre3D *fen,void *quoi,void *par,int nb,int coul)
	{
	 int i;
	 for (i=0; i<fen->nbdata;i++)
	     if (fen->data[i].Tab==quoi)
		 {
		  fen->data[i].Tab=par;
		  fen->data[i].NbObjets=nb;
		  fen->data[i].Couleur=coul;
		  Fen3DFocus (fen);
		  DataGAffiche (&fen->data[i],fen);
		 }
	}



void Fen3DFocus (sFenetre3D* fenetre)
	{
	 if (Show_pFenetre3D!=NULL)
	    Fen3DLostFocus(fenetre);

	 FenFocus (&fenetre->fenetre);
	 Show_pFenetre3D=fenetre;
	}

void Fen3DLostFocus (sFenetre3D* fen)
	{
	 FenLostFocus (&fen->fenetre);
	}

void Fen3DStyle (sFenetre3D *fen,int style)
	{
	 FenStyle (&fen->fenetre,style);
	 Fen3DFocus(fen);
	}

void Fen3DShow (sFenetre3D* fen)
	{
	 int i;

	 Fen3DFocus(fen);
	 clearviewport();

	 for (i=0;i<fen->nbdata;i++)
	     DataGAffiche(&(fen->data[i]),fen);

	 return;
	}


void far Fen3DPtAffiche (sFenetre3D* fen, sPoint* p,int coul)
	{
	 static float X,Y;
	 if (fen->GetXY(fen,p,&X,&Y)==OK)
	    putpixel(X,Y,coul);
	}


void far Fen3DNdAffiche (sFenetre3D* fen, sNoeud* N,int coul)
	{
	 static float X,Y;
	 if (N==NULL)
	    {
	     fen->GetXY(fen,NULL,0,0);
	     return;
	    }

	 if (fen->GetXY(fen,&N->P,&X,&Y)==OK)
	    putpixel(X,Y,coul);
	}


void far Fen3DDtAffiche (sFenetre3D* fen, sDroite* d, int coul)
	{
	 static float X,Y,X1,Y1;
	 int    eA, eB;
	 if (d==NULL)
	    {
	     fen->GetXY(fen,NULL,0,0);
	     setcolor (coul);
	     return;
	    }

	 if (d->segment==1)
	    {
	     eA=fen->GetXY(fen,&d->A,&X,&Y);
	     eB=fen->GetXY(fen,&d->B,&X1,&Y1);

	     if (eA==INTERNE || eB==INTERNE) return;

	     line(X,Y,X1,Y1);
	    }
	}


void far Fen3DCerAffiche (sFenetre3D* fen, sCercle* c, int coul)
	{
	 float a;
	 sDroite d;
	 sPoint p;
	 int    i,n;

	 if (c==NULL)
	    {
	     Fen3DDtAffiche(fen,NULL,coul);
	     return;
	    }

	 a=fabs(c->alpha-c->beta);

	 if (a==PI || a==0) n=12;
	 else
	   {
	    n=rnd(a/PI*6)+1;
	    if (n<4) n=4;
	   }

	 for (i=0;i<n;i++)
	     {
	      SetPt (&p,PtCer(c,(float)i/(float)(n)));
	      SetDt (&d,DtPt(&p,PtCer(c,(float)(i+1)/(float)(n))));
	      Fen3DDtAffiche (fen,&d,coul);
	     }
	}


void far Fen3DSphAffiche (sFenetre3D* fen, sSphere* s, int coul)
	{
	 sCercle c;
	 sVecteur z,u;
	 int i;

	 if (s==NULL)
	    {
	     Fen3DCerAffiche(fen,NULL,coul);
	     return;
	    }

	 SetVecCo (&z,0,0,1);
	 SetVecCo (&u,1,0,0);

	 SetCerCoCer (&c,&s->C,&z,s->R);
	 Fen3DCerAffiche (fen,&c,coul);

	 for (i=0;i<6;i++)
	     {
	      SetCerCoCer (&c,&s->C,&u,s->R);
	      Fen3DCerAffiche (fen,&c,coul);
	      SetVec (&u,VecRot(&u,&z,PI/6));
	     }
	}

void far Fen3DCyAffiche (sFenetre3D* fen, sCylindre* c, int coul)
	{
	 sCercle ce;
	 sDroite d;
	 sPoint A,B;
	 float haut=3,bas=-10;
	 int i;

	 if (c==NULL)
	    {
	     Fen3DCerAffiche(fen,NULL,coul);
	     return;
	    }

	 SetPt (&A,PtVec (&c->p,VecK(haut,&c->u)));
	 SetPt (&B,PtVec (&c->p,VecK(bas,&c->u)));
	 SetDt (&d,DtPt(&B,&A));

	 for (i=0;i<6;i++)
	     {
	      SetPt (&A,PtDt(&d,(float)i/5.));
	      SetCerCoCer (&ce,&A,&c->u,c->r);
	      Fen3DCerAffiche (fen,&ce,coul);
	     }

	 SetCerCoCer (&ce,&B,&c->u,c->r);
	 for (i=0;i<6;i++)
	     {
	      SetPt (&B,PtCer(&ce,(float)i/6));
	      SetPt (&A,PtVec(&B,VecK(haut-bas,&c->u)));
	      SetDt (&d,DtPt(&B,&A));
	      Fen3DDtAffiche (fen,&d,coul);
	     }
	}

void far Fen3DCoAffiche (sFenetre3D* fen, sCone* c, int coul)
	{
	 sCercle ce;
	 sDroite d;
	 sPoint B;
	 float haut=10,angle,distance;
	 int i;

	 if (c==NULL)
	    {
	     Fen3DCerAffiche(fen,NULL,coul);
	     return;
	    }

	 SetPt (&B,PtVec (&c->S,VecK(haut,&c->u)));
	 SetDt (&d,DtPt(&c->S,&B));
	 angle=tan(c->Agl);
	 for (i=1;i<7;i++)
	     {
	      SetPt (&B,PtDt(&d,(float)i/6.));
	      distance=Norme(VecAB(&c->S,&B));
	      SetCerCoCer (&ce,&B,&c->u,distance*angle);
	      Fen3DCerAffiche (fen,&ce,coul);
	     }

	 for (i=0;i<6;i++)
	     {
	      SetPt (&B,PtCer(&ce,(float)i/6));
	      SetDt (&d,DtPt(&c->S,&B));
	      Fen3DDtAffiche (fen,&d,coul);
	     }
	}


int far Fen3DGetXYAxono (sFenetre3D* fen, sPoint* P, float* X, float* Y)
	{
	 static float 	 ech,rapport,z;
	 static int   	 sup;
	 static sVecteur j,u;

	 if (P==NULL)
	    {
	     if (fen->fenetre.lx>fen->fenetre.ly)
		sup=fen->fenetre.lx/2;
	     else
		sup=fen->fenetre.ly/2;

	     rapport=fen->fenetre.lx/fen->fenetre.ly;
	     SetVec (&j,VecVec(&fen->i,&fen->k));
	     return (ERREURE);
	    }


	 SetVec(&u,VecAB(&fen->O,P));
	 *X=Scal(&fen->i,&u);
	 *Y=Scal(&j,&u);
	 z=Scal(&fen->k,&u);

	 if (z-1/tan(fen->angle)<0) return (INTERNE);


	 ech=z*tan(fen->angle);
	 if (fabs(*X)>ech || fabs(*Y)/rapport>ech)
	    {
	     *X=fen->fenetre.lx/2+sup*(*X)/ech;
	     *Y=fen->fenetre.ly/2-sup*(*Y)/ech;
	     return (DISTANT);
	    }

	 *X=fen->fenetre.lx/2+sup*(*X)/ech;
	 *Y=fen->fenetre.ly/2-sup*(*Y)/ech;

	 return(OK);

	}


/*======================================================================*/
/*			fonctions fenetre d'animation			*/

void SetFenACo (sFenetreA* fenA,sFenetre3D* fenD,char* titre)
	{
	 sFenetre fen;
	 int i;
	 SetFen3D  (&fenA->fenD,fenD);

	 SetFenCo (&fen,fenD->fenetre.x,fenD->fenetre.y-15,
		   fenD->fenetre.lx,15);

	 if (fenA->fenD.fenetre.style!=ANIMATION)
	    {
	     Fen3DStyle (&fenA->fenD,ANIMATION);
	     fen.y+=15;
	    }

	 FenStyle (&fen,BORDURE);
	 SetFenTCo(&fenA->fenT,&fen);

	 strcpy(fenA->titre,titre);
	 FenTSet (&fenA->fenT,titre);

	 fenA->VitesseTrans[0]=.5;
	 fenA->VitesseTrans[1]=1;
	 fenA->VitesseRot  [0]=15;
	 fenA->VitesseRot  [1]=5;
	 fenA->Vitesse=0;

	 fenA->fenEsclave=NULL;
	 fenA->nbmaitre=0;
	 for (i=0;i<MAXMAITRE;i++)
	     fenA->fenMaitre[i]=NULL;

	 FenAFocus (fenA);
	}

void FenAFocus (sFenetreA* fen)
	{
	 Fen3DFocus(&fen->fenD);
	}


void FenAShow  (sFenetreA* fen)
	{
	 Fen3DShow (&fen->fenD);
	}

char FenAnime (sFenetreA* fen)
	{
	 char cmd=' ';
	 FenAFocus(fen);

	 if (fen->fenD.fenetre.style==CONTROLE)
	    {
	     cmd=FenControleAnime (fen);
	     if (cmd!=27) return(cmd);
	     fen=(sFenetreA*)(fen->fenEsclave);
	     FenAFocus (fen);
	    }

	 cmd=getch();
	 if(cmd==0) cmd=-getch();

	 while (cmd!=27 && cmd!=9 && cmd!=-15)
	 {

	  switch (cmd)
	   {
	    case '+':   cmd=FenAZoom (fen,1);
			break;
	    case '-':	cmd=FenAZoom (fen,-1);
			break;
	    case -77:
	    case -75:
	    case -72:
	    case -80:   cmd=FenAMoveFocus(fen,cmd);
			break;
	    case '8':
	    case '2':
	    case '4':
	    case '6':	cmd=FenARotate (fen,cmd);
			break;

	    case '7':
	    case '9':	cmd=FenATourne(fen,cmd);
			break;
	    case 'v':
	    case 'V':	fen->Vitesse=-(fen->Vitesse-1);
			cmd=getch();
			if (cmd==0) cmd=-getch();
			break;
	    case 'i':   cmd=FenAAligne(fen,Vec(1,0,0));
			break;
	    case 'j':   cmd=FenAAligne(fen,Vec(0,1,0));
			break;
	    case 'k':   cmd=FenAAligne(fen,Vec(0,0,1));
			break;
	    case 'p':   cmd=FenAAligne(fen,Vec(1,1,1));
			break;



	    default:	cmd=getch();
			if (cmd==0) cmd=-getch();
	   }

	 };
	 return cmd;
	}


char FenAAligne(sFenetreA* fena,sVecteur *u)
	{
	 sVecteur j;
	 sFenetre3D* fen=&fena->fenD;
	 float dis;
	 char cmd;
	 char comment[20];

	 if (fabs(Scal(&fen->k,VecNorme(u)))>0.9)
	    {
	     SetVec(&j,VecNorme(VecVec(u,&fen->i)));
	     SetVec(&fen->k,VecNorme(u));
	     SetVec(&fen->i,VecVec(&j,&fen->k));
	    }
	 else
	    {
	     SetVec(&j,VecNorme(VecVec(&fen->k,u)));
	     SetVec(&fen->k,VecNorme(u));
	     SetVec(&fen->i,VecVec(&fen->k,&j));
	    }

	 dis=Norme(VecAB(&fen->O,&fen->focus));
	 SetPt(&fen->O,PtVec(&fen->focus,VecK(-dis,&fen->k)));

	 sprintf (comment,"Selon: (%.1f,%.1f,%.1f)",fen->k.u,fen->k.v,fen->k.w);
	 FenTSet (&fena->fenT,comment);

	 Fen3DShow (fen);

	 cmd=getch();
	 if (cmd==0) cmd=-getch();

	 FenTSet (&fena->fenT,fena->titre);

	 return (cmd);
	}


char FenATourne(sFenetreA* fena,char sens)
	{
	 sFenetre3D* fen;
	 sVecteur j,dep;
	 float inc,x,total;
	 char cmd,fin;
	 char comment[30];

	 fen=&fena->fenD;

	 FenTSet (&fena->fenT,(char*)"Rotation de l'image");

	 fin=sens;
	 cmd=sens;

	 inc=fena->VitesseRot[fena->Vitesse];
	 total=inc;

	 do
	 {
	  switch (cmd)
	  {
	   case '7':	x=1; break;
	   case '9':	x=-1; break;
	   default: 	x=0;
	  }

	  SetVec (&fen->i,VecRot(&fen->i,&fen->k,x*inc/180*PI));

	  sprintf(comment,"Rotation image:%+4.2f  %s",total*x,
			  VitesseText(fena,ROTATION,inc));
	  FenTSet(&fena->fenT,comment);

	  Fen3DShow (fen);

	  if (fena->Vitesse==1)
	     inc+=fena->VitesseRot[1];
	  total+=inc;

	  cmd=getch();
	  if (cmd==0) cmd=-getch();
	 } while (cmd==fin);

	 FenTSet (&fena->fenT,fena->titre);

	 if (cmd==27) return ('\\');
	 return (cmd);
	}



char FenARotate(sFenetreA* fena,char sens)
	{
	 sVecteur j,u;
	 float inc,x,y,total=0;
	 char cmd,fin;
	 char comment[80];
	 sFenetre3D *fen=&fena->fenD;
	 fin=sens;
	 cmd=sens;

	 inc=fena->VitesseRot[fena->Vitesse];

	 do
	 {
	  switch (cmd)
	  {
	   case '4':	x=-1; y=0; break;
	   case '6':	x=1;y=0; break;
	   case '2':	x=0; y=-1; break;
	   case '8':	x=0; y=1;break;
	   default: 	x=0; y=0;
	  }

	  SetVec (&u,VecAB(&fen->focus,&fen->O));

	  if (y!=0)
	     {
	      SetVec(&u,VecRot(&u,&fen->i,y*inc*PI/180));
	      SetPt(&fen->O,PtVec(&fen->focus,&u));
	      SetVec(&fen->k,VecNorme(VecAB(&fen->O,&fen->focus)));
	      total+=y*inc;
	     }
	  if (x!=0)
	     {
	      SetVec (&j,VecVec(&fen->k,&fen->i));
	      SetVec (&u,VecRot(&u,&j,x*inc*PI/180));
	      SetPt  (&fen->O,PtVec(&fen->focus,&u));
	      SetVec (&fen->k,VecNorme(VecAB(&fen->O,&fen->focus)));
	      SetVec (&fen->i,VecVec(&j,&fen->k));
	      total+=x*inc;
	     }

	  sprintf(comment,"Rotation:%4.2f  %s",total,
			  VitesseText(fena,ROTATION,inc));
	  FenTSet(&fena->fenT,comment);

	  Fen3DShow (fen);

	  if (fena->Vitesse==1)
	     inc+=fena->VitesseRot[1];

	  cmd=getch();
	  if (cmd==0) cmd=-getch();
	 } while (cmd==fin);

	 FenTSet (&fena->fenT,fena->titre);

	 if (cmd==27) return ('\\');
	 return (cmd);
	}


char FenASetFocus (sFenetreA* fena,sPoint* p)
	{
	 char cmd;
	 sVecteur j,u;
	 sFenetre3D* fen=&fena->fenD;
	 char comment[80];

	 SetVec(&u,VecAB(&fen->O,p));

	 if (fabs(Scal(&fen->k,VecNorme(&u)))>0.9)
	    {
	     SetVec(&j,VecNorme(VecVec(&u,&fen->i)));
	     SetVec(&fen->k,VecNorme(&u));
	     SetVec(&fen->i,VecVec(&j,&fen->k));
	    }
	 else
	    {
	     SetVec(&j,VecNorme(VecVec(&fen->k,&u)));
	     SetVec(&fen->k,VecNorme(&u));
	     SetVec(&fen->i,VecVec(&fen->k,&j));
	    }

	 SetPt(&fen->focus,p);

	 sprintf(comment,"Focus : (%.1f,%.1f,%.1f)",p->x,p->y,p->z);
	 FenTSet(&fena->fenT,comment);

	 Fen3DShow (fen);

	 cmd=getch();
	 if (cmd==0) cmd=-getch();

	 FenTSet (&fena->fenT,fena->titre);
	 return (cmd);
	}


char FenAMoveFocus(sFenetreA* fena,char sens)
	{
	 sVecteur j,dep;
	 float inc,x,y,cote;
	 char comment[80];
	 char cmd,fin;
	 sFenetre3D* fen=&fena->fenD;
	 fin=sens;
	 cmd=sens;

	 SetVec (&j,VecVec(&fen->i,&fen->k));

	 inc=fena->VitesseTrans[fena->Vitesse];

	 do
	 {
	  switch (cmd)
	  {
	   case -77:	x=-1; y=0; break;
	   case -75:	x=1;y=0; break;
	   case -72:	x=0; y=-1; break;
	   case -80:	x=0; y=1;break;
	   default: 	x=0; y=0;
	  }

	  SetVec(&dep,VecK(x*inc,&fen->i));
	  SetVec(&dep,VecPlus(&dep,VecK(inc*y,&j)));

	  SetPt (&fen->focus,PtVec(&fen->focus,&dep));
	  SetPt (&fen->O,PtVec(&fen->O,&dep));

	  sprintf(comment,"Translation  %s",VitesseText(fena,TRANSLATION,inc));
	  FenTSet(&fena->fenT,comment);

	  Fen3DShow (fen);

	  if (fena->Vitesse==1)
	     inc+=fena->VitesseTrans[1];

	  cmd=getch();
	  if (cmd==0) cmd=-getch();
	 } while (cmd==fin);

	 FenTSet (&fena->fenT,fena->titre);

	 if (cmd==27) return ('\\');
	 return (cmd);
	}


char FenAZoom  (sFenetreA* fena,float sens)
	{
	 char cmd,fin;
	 float inc;
	 sFenetre3D* fen;
	 sVecteur u;
	 float dis;
	 char comment[80];

	 fen=&fena->fenD;

	 inc=fena->VitesseTrans[fena->Vitesse]*sens;

	 if (sens>0)
	    fin='+';
	 else
	    fin='-';
	 do
	 {
	  SetPt(&fen->O,PtVec(&fen->O,VecK(inc,&fen->k)));

	  SetVec (&u,VecAB(&fen->O,&fen->focus));
	  dis=Norme (&u);
	  if (Scal(&u,&fen->k)<0)
	     dis=-dis;
	  sprintf(comment,"Zoom:%4.2f  %s",dis,VitesseText(fena,TRANSLATION,inc));
	  FenTSet(&fena->fenT,comment);

	  Fen3DShow(fen);

	  if (fena->Vitesse==1)
	     inc+=fena->VitesseTrans[1]*sens;

	  cmd=getch();
	  if (cmd==0) cmd=-getch();
	 } while (cmd==fin);

	 FenTSet(&fena->fenT,fena->titre);
	 if (cmd==27) return ('\\');
	 return (cmd);
	}


/*======================================================================*/
/*	 fonctions specifiques aux controles des fenetres d'animmation	*/

void SetFenControleCo (sFenetreA* controle,sFenetreA* esclave)
	{
	 controle->fenEsclave=esclave;
	 Fen3DStyle (&controle->fenD,CONTROLE);
	 FenAAddMaitre (esclave,controle);
	}




/*======================================================================*/
/*			fonctions suite de fenetres			*/

void SuiteInit (sFenASuite* suite)
	{
	 int i;
	 for (i=0;i<MAXANIME;i++)
	     suite->fenetre[i]=NULL;
	 suite->nbfenetre=0;
	 suite->courante=0;
	}

void SuiteAnime (sFenASuite* suite)
	{
	 char cmd;
	 do
	 {
	  cmd=FenAnime(suite->fenetre[suite->courante]);
	  if (cmd==-15) suite->courante--;
	  if (cmd==9)  suite->courante++;
	  if (suite->courante<0) suite->courante=suite->nbfenetre-1;
	  if (suite->courante==suite->nbfenetre) suite->courante=0;
	 } while (cmd!=27);

	}

void SuiteAdd   (sFenASuite* suite,sFenetreA* fena)
	{
	 if (suite->nbfenetre==MAXANIME) return;
	 suite->fenetre[suite->nbfenetre]=fena;
	 suite->nbfenetre++;
	 FenAShow(fena);
	}


void SuiteAddPt (sFenASuite* suite, sPoint* P, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddPt(&(suite->fenetre[i])->fenD,P,nb,coul);

	}


void SuiteAddNd (sFenASuite* suite, sNoeud* N, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddNd(&(suite->fenetre[i])->fenD,N,nb,coul);
	}

void SuiteAddDt (sFenASuite* suite, sDroite* d, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddDt(&(suite->fenetre[i])->fenD,d,nb,coul);
	}


void SuiteAddCer (sFenASuite* suite, sCercle* c, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddCer(&(suite->fenetre[i])->fenD,c,nb,coul);
	}

void SuiteAddSph (sFenASuite* suite, sSphere* s, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddSph(&(suite->fenetre[i])->fenD,s,nb,coul);
	}

void SuiteAddCy (sFenASuite* suite, sCylindre* s, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddCy(&(suite->fenetre[i])->fenD,s,nb,coul);
	}

void SuiteAddCo (sFenASuite* suite, sCone* s, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre; i++)
	     Fen3DAddCo(&(suite->fenetre[i])->fenD,s,nb,coul);
	}


void SuiteChange (sFenASuite* suite, void* quoi, void* par, int nb,int coul)
	{
	 int i;
	 for (i=0; i<suite->nbfenetre;i++)
	     Fen3DDataChange (&(suite->fenetre[i])->fenD,quoi,par,nb,coul);
	}


/*======================================================================*/
/*			fonctions d'affichage global			*/

void InitEcran (void)
{
 int gdriver=DETECT, gmode, errorcode;
 sFenetre fen;
 sFenetre3D fend;
 static sFenetreA fi,fj,fk,fijk,fb;


 initgraph(&gdriver, &gmode,"");
 errorcode = graphresult();
 if (errorcode != grOk)
    {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     printf("Press any key to halt:");
     getch();
     exit(1);
    }

 cleardevice();


 SetFenCo (&fen,0,0,150,150);
 FenStyle (&fen,BORDURE);
 SetFen3DCo (&fend,&fen,Pt(50,0,0),Vec(-1,0,0),PI*15/180);
 SetFenACo(&fi,&fend,(char*)"selon i");

 SetFenCo (&fen,300,0,150,150);
 FenStyle (&fen,BORDURE);
 SetFen3DCo (&fend,&fen,Pt(0,0,50),Vec(0,0,-1),PI*15/180);
 SetFenACo(&fk,&fend,(char*)"selon k");


 SetFenCo (&fen,150,0,150,150);
 FenStyle (&fen,BORDURE);
 SetFen3DCo (&fend,&fen,Pt(0,50,0),Vec(0,-1,0),PI*15/180);
 SetFenACo(&fj,&fend,(char*)"selon j");

 SetFenCo (&fen,450,0,150,150);
 FenStyle (&fen,BORDURE);
 SetFen3DCo (&fend,&fen,Pt(50,50,50),Vec(-1,-1,-1),PI*15/180);
 SetFenACo(&fijk,&fend,(char*)"perspec.");

 SetFenCo (&fen,0,150,getmaxx(),getmaxy()-150);
 FenStyle (&fen,BORDURE);
 SetFen3DCo (&fend,&fen,Pt(50,50,50),Vec(-1,-1,-1),PI*15/180);
 SetFenACo(&fb,&fend,(char*)"cellule de zoom");


 SetFenControleCo (&fi,&fb);
 SetFenControleCo (&fj,&fb);
 SetFenControleCo (&fk,&fb);

 SuiteInit(&Show_SuiteEcran);
 SuiteAdd (&Show_SuiteEcran,&fb);
 SuiteAdd (&Show_SuiteEcran,&fi);
 SuiteAdd (&Show_SuiteEcran,&fj);
 SuiteAdd (&Show_SuiteEcran,&fk);
 SuiteAdd (&Show_SuiteEcran,&fijk);

 return;
}


void EcranAddPt (sPoint* p, int nb,int coul)
	{
	 SuiteAddPt (&Show_SuiteEcran,p,nb,coul);
	 SuiteAnime (&Show_SuiteEcran);
	}


void EcranAddNd (sNoeud* n, int nb, int coul)
	{
	 SuiteAddNd (&Show_SuiteEcran,n,nb,coul);
	 SuiteAnime (&Show_SuiteEcran);
	}


void EcranAddDt (sDroite* d, int nb, int coul)
	{
	 SuiteAddDt (&Show_SuiteEcran,d,nb,coul);
	 SuiteAnime (&Show_SuiteEcran);
	}


void EcranAddCer (sCercle* c, int nb, int coul)
	{
	 SuiteAddCer (&Show_SuiteEcran,c,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}

void EcranAddSph (sSphere* s, int nb, int coul)
	{
	 SuiteAddSph (&Show_SuiteEcran,s,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}

void EcranAddCy (sCylindre* s, int nb, int coul)
	{
	 SuiteAddCy (&Show_SuiteEcran,s,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}

void EcranAddCo (sCone* s, int nb, int coul)
	{
	 SuiteAddCo (&Show_SuiteEcran,s,nb,coul);
	 SuiteAnime  (&Show_SuiteEcran);
	}


void EcranChange (void* quoi, void* par, int nb, int coul)
	{
	 SuiteChange(&Show_SuiteEcran,quoi,par,nb,coul);
	 SuiteAnime (&Show_SuiteEcran);
	}

void FermeEcran (void)
	{
	 closegraph();
	}




void NdAffiche (sNoeud* Noeud,int NbNoeud)
{
 InitEcran ();

 SuiteAddNd (&Show_SuiteEcran,Noeud,NbNoeud,WHITE);

 SuiteAnime (&Show_SuiteEcran);

 closegraph();
}


void PtAffiche (sPoint* p,int NbPoint)
{
 InitEcran ();

 SuiteAddPt (&Show_SuiteEcran,p,NbPoint,WHITE);

 SuiteAnime (&Show_SuiteEcran);

 closegraph();

 return;
}



/*======================================================================*/
/*======================================================================*/
/*			definitions des fonctions statiques		*/


static char*    VitesseText (sFenetreA* fena,int type,float vitesse)
	{
	 static char text[30];

	 if (type==ROTATION)
	    {
	     if (fena->Vitesse==0)
		sprintf (text,"R=%.1f",fena->VitesseRot[0]);
	     else
		sprintf (text,"R=%.1f(%.1f)",vitesse,fena->VitesseRot[1]);
	    }
	 else
	    {
	     if (fena->Vitesse==0)
		sprintf (text,"V=%.1f",fena->VitesseTrans[0]);
	     else
		sprintf (text,"V=%.1f(%.1f)",vitesse,fena->VitesseTrans[1]);
	    }

	 return text;
	}


char FenControleAnime (sFenetreA* fen)
	{
	 char cmd=' ';
	 FenAFocus(fen);

	 cmd=getch();
	 if(cmd==0) cmd=-getch();

	 while (cmd!=27 && cmd!=9 && cmd!=-15)
	 {

	  switch (cmd)
	   {
	    case '+':   cmd=FenAZoom (fen,1);
			break;
	    case '-':	cmd=FenAZoom (fen,-1);
			break;
	    case -77:
	    case -75:
	    case -72:
	    case -80:   cmd=FenAMoveFocus(fen,cmd);
			break;
	    case '7':
	    case '9':	cmd=FenATourne(fen,cmd);
			break;
	    case 'v':
	    case 'V':	fen->Vitesse=-(fen->Vitesse-1);
			cmd=getch();
			if (cmd==0) cmd=-getch();
			break;

	    case  13:   cmd=FenControleResetEsclave (fen);
			break;

	    default:	cmd=getch();
			if (cmd==0) cmd=-getch();
	   }

	 };

	 return cmd;
	}


char FenControleResetEsclave (sFenetreA* fena)
	{
	 sFenetre3D* fen=&fena->fenD;
	 sFenetre3D* fend;
	 sFenetreA* fenesclave=(sFenetreA*)(fena->fenEsclave);
	 sPoint new;
	 char cmd;
	 int i;

	 for (i=0;i<fenesclave->nbmaitre;i++)
	     {
	      if (fenesclave->fenMaitre[i]==fena) continue;
	      fend=&(((sFenetreA*)(fenesclave->fenMaitre[i]))->fenD);
	      SetPt(&new,PtVec(&fen->O,VecK(
			 Scal(&fen->k,VecAB(&fen->O,&fend->focus)),&fen->k)));
	      ungetch(27);
	      ungetch('~');
	      ungetch('~');
	      cmd=FenASetFocus ((sFenetreA*)(fenesclave->fenMaitre[i]),&new);
	     }

	 fend=&fenesclave->fenD;
	 SetPt(&new,PtVec(&fen->O,VecK(
			 Scal(&fen->k,VecAB(&fen->O,&fend->focus)),&fen->k)));
	 cmd=FenASetFocus ((sFenetreA*)(fena->fenEsclave),&new);

	 FenAFocus(fena);

	 return (cmd);
	}

void FenAAddMaitre (sFenetreA* fena,sFenetreA* maitre)
	{
	 if (fena->nbmaitre==MAXMAITRE) return;
	 fena->fenMaitre[fena->nbmaitre++]=maitre;
	}

