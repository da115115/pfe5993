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



sCercle *calculEllipse(sPoint A,sPoint B,sPoint C,sPoint D,sRepere *R);
void 	Ell1(sPoint A,sPoint B,sPoint C,sPoint D,sRepere *R,sPoint *A1,sPoint *A2,sPoint *A3,sPoint *A4);
void 	Ell2(sPoint *C1,sPoint *C2,sPoint *C3,sPoint *C4,float *cx,float *cy,float *kc);sCercle *calculEllipse(sPoint A,sPoint B,sPoint C,sPoint D,sRepere *R);
sPoint *sacro_ill(float k,float l,float pair);
void 	fichier1(sPoint Min);
void 	fichier2(sPoint Min);
void 	fichier3(sPoint Min);
void 	parametreellipse1(sCercle E6,sPoint F,sPoint H,float pas);
void 	creat_tg(void);
sCercle *creat_arc(sPoint Q);
void 	creat_pt(sCercle C);
sPoint *parametrecercle(float r,float a,float b,float c);
void 	parametre(void);
void 	parametreseg(sPoint A,sPoint B,float p);
void 	maillage(void);
sPoint	Ptepais(float un,float vb);
void 	sacro_lomber(void);

sCercle E1,E2,E3,E4,E5;
sRepere R1,R2,R3,R4,R5,RC;
sRepere RBIO;
sPoint A[13],PA1,PA2;
int i,j;
int N;
float a1,b1;
sVecteur b,n,t;
sPoint *Nd;
sVecteur AUHD,AUVD,AUHG,AUVG;

sPoint pD,pN,pO;  /*  <<<<<---------- */
sDroite DtTemp;/*  <<<<<---------- */


int sacrum (void)
{

 sPoint PP;
 float d1,d2;
 sVecteur u;
 sPlan P2;
 sSection data;


 /* initialisation des parametres		*/

 if (InitSection (&data,"defaut.cfg",LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration DEFAUT.CFG introuvable !\n");
      exit (-1);
     }

 OuvreSection (&data,"sacrum");
 SACRUM=atoi(ChercheInfo(&data,"SACRUM"));

 J=atoi(ChercheInfo(&data,"NBCOUPE"));
 EPAISLAR=atof(ChercheInfo(&data,"EPAISLAR"));
 EPAISHAUT=atof(ChercheInfo(&data,"EPAISHAUT"));
 LH=atof(ChercheInfo(&data,"LH"));
 LV=atof(ChercheInfo(&data,"LV"));
 lh=atof(ChercheInfo(&data,"lh"));
 lv=atof(ChercheInfo(&data,"lv"));
 EPSL=atof(ChercheInfo(&data,"H_LOMBO_SACREE"));

 FermeSection(&data);

 if (InitSection (&data,"points.cfg",LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration POINTS.CFG introuvable !\n");
      exit (-1);
     }
 OuvreSection (&data,"sacrum");

 SetPt(&A[1],PtSection(&data,"Pt_PSAnt"));
 SetPt(&A[2],PtSection(&data,"Pt_PSPost"));
 SetPt(&A[3],PtSection(&data,"Pt_PSLatG"));
 SetPt(&A[4],PtSection(&data,"Pt_SIDroit"));
 SetPt(&A[5],PtSection(&data,"Pt_SIGauche"));
 SetPt(&A[6],PtSection(&data,"Pt_LSGauche"));
 SetPt(&A[7],PtSection(&data,"Pt_LSDroit"));
 SetPt(&A[8],PtSection(&data,"Pt_AntMedian"));
 SetPt(&A[9],PtSection(&data,"Pt_AntInf"));
 SetPt(&A[10],PtSection(&data,"Pt_SIInfD"));
 SetPt(&A[11],PtSection(&data,"Pt_SIInfG"));

/*
 OuvreSection (&data,"droit");
 SetPt (&pD,PtSection(&data,"D"));
 SetPt (&pN,PtSection(&data,"N"));
 SetPt (&pO,PtSection(&data,"O"));
*/

 FermeSection(&data);



/*=========================================================================
======			MAILLAGE DE L'ARRETE UNIQUEMENT		     ======
=========================================================================*/

/*
 SetVec(&n,VecNorme(VecAB(&pN,&pD)));
 SetPt (&PP,PtVec(Pt_SacreAntInf,VecK(LH,&n)));
 SetDt (&DtTemp,DtPt(Pt_SacreAntInf,&PP));
*/

/*EcranAddDt (&DtTemp,1,GREEN);*/

/*
 for (i=0;i<=2;i++)
     {
      SetPt(&PP,PtDt(&DtTemp,(float)i/2.));
      SetNdCo (&Sacro_Iliaque[i][0],&PP,i*2+3+SACRUM*1000);
     }

 return 0;
*/

/*========================================================================
=========================================================================*/



 j=0;
 i=0;
 N=0;
 Nd=calloc(MAXNOEUD,sizeof(sPoint));
	if(Nd==NULL)
	{
	 printf("pas assez de m‚moire pour l'execution\n");
	 exit(-1);
	}

 SetVec(&AUVD,VecNorme(VecAB(&A[4],&A[10])));
 SetVec(&AUVG,VecNorme(VecAB(&A[5],&A[11])));

 SetVecCo(&RBIO.i,1,0,0);
 SetVecCo(&RBIO.j,0,1,0);
 SetVecCo(&RBIO.k,0,0,1);
 SetPtCo(&RBIO.O,0,0,0);


 creat_tg();


	/**********************************************************/
	/**                 ellipse E1                           **/
	/**********************************************************/


 SetCer(&E1,calculEllipse(A[7],A[4],A[5],A[6],&R1));
InitEcran();
EcranAddCer (&E1,1,BLUE);
FermeEcran();
	/**********************************************************/
	/**                 ellipse E2                           **/
	/**********************************************************/


 SetPt(&PP,PtVec(&A[1],VecdK(2,VecAB(&A[1],&A[2]))));
 b1=Norme(VecdK(2,VecAB(&A[1],&A[2])));
 a1=Norme(VecAB(&A[3],&PP));
 SetPl(&P2,PlPt3(&A[3],&A[2],&A[1]));
 SetVec(&u,VecNorme(VecAB(&A[1],&PP)));
 SetRepCo(&R2,&PP,&u,VecNorme(VecVec(&u,&P2.u)),&P2.u);
 SetPt(&E2.C,&PP);
 E2.a=a1;
 E2.b=b1;
 E2.cercle=0;
 SetVec(&E2.k,&P2.u);
 SetVec(&E2.j,&u);
 SetVec(&E2.i,VecNorme(VecVec(&u,&P2.u)));

/**/

	/**********************************************************/
	/**                 ellipse E3                           **/
	/**********************************************************/

 E2.alpha=0;
 E2.beta=2*PI;
 SetCer(&E2,CerCutPt(&E2,&A[1],&A[1]));
 SetPt(&PA1,PtCer(&E2,7./8.));
 SetPt(&PA2,PtCer(&E2,1./8.));
 d1=Scal(&R1.k,VecAB(&PA1,&R1.O));
 d2=Scal(&R1.k,VecAB(&PA2,&R1.O));
 SetPt(&PA1,PtVec(&PA1,VecK(d1,&R1.k)));
 SetPt(&PA2,PtVec(&PA2,VecK(d2,&R1.k)));
 SetCer(&E3,calculEllipse(A[4],PA1,PA2,A[5],&R3));

InitEcran();
EcranAddCer (&E1,1,BLUE);
EcranAddCer (&E2,1,GREEN);
EcranAddCer (&E3,1,RED);
EcranAddPt  (&A[1],1,WHITE);
EcranAddPt  (&A[2],1,WHITE);
EcranAddPt  (&A[3],1,WHITE);
EcranAddPt  (&A[4],1,WHITE);
EcranAddPt  (&A[5],1,WHITE);
EcranAddPt  (&A[6],1,WHITE);
EcranAddPt  (&A[7],1,WHITE);
EcranAddPt  (&A[8],1,WHITE);
EcranAddPt  (&A[9],1,WHITE);
EcranAddPt  (&A[10],1,WHITE);
EcranAddPt  (&A[11],1,WHITE);
FermeEcran();

	/**********************************************************/
	/**             parametrisation des ellipses             **/
	/**********************************************************/

 NbNoeud--;
 parametre();




	/**********************************************************/
	/**                    mise en volume                    **/
	/**********************************************************/


 N=0;

	for(i=1;i<37;i++)

		{

		N=i;
		creat_pt(*creat_arc(Nd[i]));
		N=N-1200;

		}
 sacro_lomber();
 maillage();

 NbNoeud++;


 /* initialisation tableau d'echange Sacro_Iliaque		*/

 for (i=0;i<2;i++)
     for (j=0;j<6;j++)
	 SetNdCo (&Sacro_Iliaque[i][j],&Nd[i*2+3+j*NN],i*2+3+j*NN+SACRUM*1000);


 free(Nd);
InitEcran();
EcranAddNd (Noeud.tab,NbNoeud,YELLOW);
FermeEcran();

 return 0;
}
	/**********************************************************/
	/**                calcul les ellipses                   **/
	/**********************************************************/


sCercle *calculEllipse(sPoint A,sPoint B,sPoint C,sPoint D,sRepere *R)
{

 sPoint B1,B2,B3,B4,OC;
 float xc,yc,kt,ae,be;
 static sCercle E;

 Ell1(A,B,C,D,R,&B1,&B2,&B3,&B4);
 Ell2(&B1,&B2,&B3,&B4,&xc,&yc,&kt);
	if(kt<0)
	{
	 Ell1(B,A,D,C,R,&B1,&B2,&B3,&B4);
	 Ell2(&B1,&B2,&B3,&B4,&xc,&yc,&kt);
	}

	if(kt<0)
	{
	 printf("erreur de donn‚es");
	 exit(-1);
	}

 ae=sqrt((xc)*(xc)+(kt)*(yc)*(yc));
 be=ae/sqrt(kt);

 SetPtCo(&OC,xc,yc,0);
 SetPt(&E.C,PtRepOld(&OC,R));
 E.a=ae;
 E.b=be;
 E.cercle=0;
 SetVec(&E.k,&R->k);
 SetVec(&E.i,&R->i);
 SetVec(&E.j,&R->j);
 return &E;

}

void Ell1(sPoint Ae,sPoint Be,sPoint Ce,sPoint De,sRepere *Re,sPoint *A1,sPoint *A2,sPoint *A3,sPoint *A4)
{
 sPlan P;
 sPoint G;
 sVecteur V;
 float d;

 SetPl(&P,PlPt3(&Ae,&Be,&De));
 SetPt(&G,PtPro(&Ce,&P));
 SetVec(&V,VecNorme(VecAB(&Ae,&De)));
 SetRepCo(Re,&Ae,&V,VecNorme(VecVec(&P.u,&V)),&P.u);
 SetPt(A1,PtRepNew(&Ae,Re));
 SetPt(A2,PtRepNew(&Be,Re));
 SetPt(A3,PtRepNew(&De,Re));
 SetPt(A4,PtRepNew(&G,Re));
}
void Ell2(sPoint *C1,sPoint *C2,sPoint *C3,sPoint *C4,float *cx,float *cy,float *kc)
{

 float x[4],y[4];
 float a;
 sPoint M[4];

 SetPt(&M[0],C1);
 SetPt(&M[1],C2);
 SetPt(&M[2],C4);
 SetPt(&M[3],C3);

	for(i=0;i<4;i++)
	{
	 x[i]=M[i].x;
	 y[i]=M[i].y;
	}
 a=(x[2]*x[2]-x[2]*x[3])/(x[1]*x[1]-x[1]*x[3]);
 *cx=x[3]/2;
 *cy=(a*y[1]*y[1]-y[2]*y[2])/(a*y[1]-y[2])/2;
 *kc=-((x[1]*x[1]-x[1]*x[3])/(y[1]*y[1]-2*y[1]*(*cy)));

}



	/**********************************************************/
	/**                   les noeuds                         **/
	/**********************************************************/

void fichier1(sPoint Min)
{

 N=(N+1);

 NbNoeud=NbNoeud+1;
 SetPt(&Nd[SACRUM*1000+N],&Min);
 switch(N)
 {
  case 3:SetVec(&AUHD,VecNorme(VecAB(&A[4],&Nd[SACRUM*1000+3])));
	 SetPt(&Min,sacro_ill(LH,0,1));
	 break;
  case 4:SetVec(&AUHG,VecNorme(VecAB(&A[5],&Nd[SACRUM*1000+4])));
	 SetPt(&Min,sacro_ill(LH,0,0));
	 break;
  case 5:SetPt(&Min,sacro_ill(LH/2,0,1));
	 break;
  case 6:SetPt(&Min,sacro_ill(LH/2,0,0));
	 break;
  case 7:SetPt(&Min,sacro_ill(0,0,1));
	 break;
  case 8:SetPt(&Min,sacro_ill(0,0,0));
	 break;
 default:break;
  }

 TabNdFocus(&Noeud);
 SetNdCo(Noeud.e(NbNoeud),&Min,SACRUM*1000+N);
 SetPt(&Nd[SACRUM*1000+N],&Min);
}


void fichier2(sPoint Min)
{

 N=N+NN;
 NbNoeud=NbNoeud+1;

  switch(N)
 {
  case 3+NN:SetPt(&Min,sacro_ill(LH,lh/2,1));
	 break;
  case 4+NN:SetPt(&Min,sacro_ill(LH,lh/2,0));
	 break;
  case 5+NN:SetPt(&Min,sacro_ill(LH/2,lh/2,1));
	 break;
  case 6+NN:SetPt(&Min,sacro_ill(LH/2,lh/2,0));
	 break;
  case 7+NN:SetPt(&Min,sacro_ill(0,lh/2,1));
	 break;
  case 8+NN:SetPt(&Min,sacro_ill(0,lh/2,0));
	 break;
  case 3+2*NN:SetPt(&Min,sacro_ill(LH,lh,1));
	 break;
  case 4+2*NN:SetPt(&Min,sacro_ill(LH,lh,0));
	 break;
  case 5+2*NN:SetPt(&Min,sacro_ill(LH/2,lh,1));
	 break;
  case 6+2*NN:SetPt(&Min,sacro_ill(LH/2,lh,0));
	 break;
  case 7+2*NN:SetPt(&Min,sacro_ill(0,lh,1));
	 break;
  case 8+2*NN:SetPt(&Min,sacro_ill(0,lh,0));
	 break;
  case 5+3*NN:SetPt(&Min,sacro_ill(lv,3*LV/5,1));
	 break;
  case 6+3*NN:SetPt(&Min,sacro_ill(lv,3*LV/5,0));
	 break;
  case 7+3*NN:SetPt(&Min,sacro_ill(0,3*LV/5,1));
	 break;
  case 8+3*NN:SetPt(&Min,sacro_ill(0,LV*3/5,0));
	 break;
  case 5+4*NN:SetPt(&Min,sacro_ill(lv,4*LV/5,1));
	 break;
  case 6+4*NN:SetPt(&Min,sacro_ill(lv,4*LV/5,0));
	 break;
  case 7+4*NN:SetPt(&Min,sacro_ill(0,4*LV/5,1));
	 break;
  case 8+4*NN:SetPt(&Min,sacro_ill(0,4*LV/5,0));
	 break;
  case 5+5*NN:SetPt(&Min,sacro_ill(lv,LV,1));
	 break;
  case 6+5*NN:SetPt(&Min,sacro_ill(lv,LV,0));
	 break;
  case 7+5*NN:SetPt(&Min,sacro_ill(0,LV,1));
	 break;
  case 8+5*NN:SetPt(&Min,sacro_ill(0,LV,0));
	 break;

  default:
	 break;
 }
  TabNdFocus(&Noeud);

  SetPt(&Nd[SACRUM*1000+N],&Min);
  SetNdCo(Noeud.e(NbNoeud),&Min,SACRUM*1000+N);

}

void fichier3(sPoint Min)
{

 NbNoeud=NbNoeud+1;

 TabNdFocus(&Noeud);
 SetPt(&Nd[SACRUM*1000+N+J*NN],&Min);
 SetNdCo(Noeud.e(NbNoeud),&Min,SACRUM*1000+N+J*NN);

}


	/**********************************************************/
	/**                 parametre ellipse                    **/
	/**********************************************************/


void parametreellipse1(sCercle E6,sPoint F,sPoint H,float pas)
{

 SetCer(&E6,CerCutPt(&E6,&F,&H));

 fichier1(*PtCer(&E6,pas));

}


	/**********************************************************/
	/**                 parametre segment                    **/
	/**********************************************************/


void parametreseg(sPoint A,sPoint B,float p)
{
 sVecteur AB;

 SetVec(&AB,VecAB(&A,&B));
 fichier1(*PtVec(&A,VecdK(p,&AB)));

}


	/**********************************************************/
	/**                      parametre                       **/
	/**********************************************************/


void parametre(void)
{


	for(i=1;i<5;i++)
	{
	 parametreellipse1(E1,A[7],A[4],(float)i/4.);
	 fichier3(Ptepais(-1.,((float)i-1.)/3));
	 parametreellipse1(E1,A[5],A[6],1.-(float)i/4.);
	 fichier3(Ptepais(1.,((float)i-1.)/3.));

	 }

	for(i=0;i<8;i++)
	{
	 parametreellipse1(E2,A[1],A[1],(float)i/8.);

		switch(i)
		{
		case 0:
		 fichier3(Ptepais(0.,1.));
		break;
		case 1:
		 fichier3(Ptepais(0.25,1.));
		break;
		case 2:
		 fichier3(Ptepais(0.25,2./3.));
		break;
		case 3:
		 fichier3(Ptepais(0.25,1./3.));
		break;
		case 4:
		 fichier3(Ptepais(0.,1./3.));
		break;
		case 5:
		 fichier3(Ptepais(-0.25,1./3.));
		break;
		case 6:
		 fichier3(Ptepais(-0.25,2./3.));
		break;
		case 7:
		 fichier3(Ptepais(-0.25,1.));
		break;
		default:
		printf("erreur programme");
		exit(-1);
		break;
		}

	}

 fichier1(E2.C);

 fichier3(Ptepais(0,2./3.));
	for(i=1;i<3;i++)
	{
	 parametreellipse1(E3,A[5],PA2,(float)i/4.);
	 fichier3(Ptepais(1.-(float)i/4.,1.));
	 parametreellipse1(E3,PA1,A[4],1.-(float)i/4.);
	 fichier3(Ptepais(-1.+(float)i/4.,1.));
	}

	for(i=1;i<4;i++)
	{
	 parametreseg(A[6],Nd[20],4./(float)i);
	 fichier3(Ptepais(0.5,((float)i-1.)/3.));
	 parametreseg(A[7],Nd[21],4./(float)i);
	 fichier3(Ptepais(-0.5,((float)i-1.)/3.));
	}

	for(i=0;i<3;i++)
	{
	 parametreseg(Nd[2+i*2],Nd[22+i*2],2.);
	 fichier3(Ptepais(0.75,(float)i/3.));
	 parametreseg(Nd[1+i*2],Nd[23+i*2],2.);
	 fichier3(Ptepais(-0.75,(float)i/3.));
	}

	for(i=1;i<4;i++)
	{
	 parametreseg(Nd[22],Nd[23],4./(float)i);
	 fichier3(Ptepais(0.25*((float)i-2.),0));
	}

}


	/**********************************************************/
	/**                 cree vecteur tangent                 **/
	/**********************************************************/

void creat_tg(void)
{

 sCercle C0;
 sPlan Pe;

 SetCer(&C0,CerPt3(&A[8],&A[1],&A[9]));
 SetVec(&t,VecNorme(VecVec(&C0.k,VecAB(&C0.C,&A[9]))));
 SetPl(&Pe,PlPt3(&A[8],&A[1],&A[9]));
 SetVec(&n,&Pe.u);
 SetVec(&b,VecVec(&n,&t));


}


	/**********************************************************/
	/**                 cree arc de cercle                   **/
	/**********************************************************/

sCercle *creat_arc(sPoint Q)
{
 static sCercle result;

 sPlan P;

 sCercle C1;
 sPoint IM,OC;
 sDroite D1,D2;

 SetPlCo(&P,&Q,VecVec(&t,VecAB(&Q,&Nd[N+(J*NN)])));
 SetPt(&IM,PtVec(&Q,VecdK(2,VecAB(&Q,&Nd[N+(J*NN)]))));
 SetDtCo(&D1,&IM,VecVec(&P.u,VecAB(&IM,&Nd[N+(J*NN)])));
 SetDtCo(&D2,&Nd[N+(J*NN)],VecVec(&P.u,&t));
 SetPt(&OC,Droite_Droite(&D1,&D2,&P));
 SetCerCoCer(&C1,&OC,&P.u,Norme(VecAB(&OC,&Q)));


 SetCer(&result,CerCutPt(&C1,&Q,&Nd[N+(J*NN)]));
 return (&result);

}
	/**********************************************************/
	/**                 cree un point du cercle              **/
	/**********************************************************/

void creat_pt(sCercle C)
{

 float pas,m;

 pas=0;
 m=0;


	for(m=1;m<J;m++)
	{
	   if(m<6)
	   {

	    pas=m/13.;
	   }
	   else
	   {

	    pas=(m)/13.;
	   }
	   fichier2(*PtCer(&C,pas));
	 }

}


	/**********************************************************/
	/**               parametrisation d'un cercle            **/
	/**********************************************************/

sPoint *parametrecercle(float r,float a,float b,float c)
{

 float x,y;
 static sPoint M;

 x=r*cos((b-a)/c);
 y=r*sin((b-a)/c);
 SetPtCo(&M,x,y,0);
 return &M;
}



	/**********************************************************/
	/**                       maillage                       **/
	/**********************************************************/



void maillage(void)
{
 int k;

 TabMaFocus (&Maille);

 for(k=SACRUM*1000;k<5*NN+SACRUM*1000;k=k+2*NN)
 {
 j=29;
	for(i=1;i<5;i=i+2)
	 {

	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),i+k,i+2+k,j+2+k,j+k,i+k+NN,i+2+k+NN,j+2+k+NN,j+k+NN,1);
	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),j-1+k,j+1+k,i+3+k,i+1+k,j-1+k+NN,j+1+k+NN,i+3+k+NN,i+1+k+NN,1);
	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),j+k,j+2+k,j-4+k,j-6+k,j+k+NN,j+2+k+NN,j-4+k+NN,j-6+k+NN,1);
	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),j-7+k,j-5+k,j+1+k,j-1+k,j-7+k+NN,j-5+k+NN,j+1+k+NN,j-1+k+NN,1);

	  j=j+2;
	 }
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),5+k,7+k,19+k,33+k,5+k+NN,7+k+NN,19+k+NN,33+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),32+k,18+k,8+k,6+k,32+k+NN,18+k+NN,8+k+NN,6+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),33+k,19+k,21+k,27+k,33+k+NN,19+k+NN,21+k+NN,27+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),26+k,20+k,18+k,32+k,26+k+NN,20+k+NN,18+k+NN,32+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),27+k,21+k,16+k,15+k,27+k+NN,21+k+NN,16+k+NN,15+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),11+k,10+k,20+k,26+k,11+k+NN,10+k+NN,20+k+NN,26+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),23+k,25+k,14+k,36+k,23+k+NN,25+k+NN,14+k+NN,36+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),34+k,12+k,24+k,22+k,34+k+NN,12+k+NN,24+k+NN,22+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),25+k,27+k,15+k,14+k,25+k+NN,27+k+NN,15+k+NN,14+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),12+k,11+k,26+k,24+k,12+k+NN,11+k+NN,26+k+NN,24+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),14+k,15+k,17+k,13+k,14+k+NN,15+k+NN,17+k+NN,13+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),13+k,17+k,11+k,12+k,13+k+NN,17+k+NN,11+k+NN,12+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),15+k,16+k,9+k,17+k,15+k+NN,16+k+NN,9+k+NN,17+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),17+k,9+k,10+k,11+k,17+k+NN,9+k+NN,10+k+NN,11+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),36+k,14+k,13+k,35+k,36+k+NN,14+k+NN,13+k+NN,35+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),35+k,13+k,12+k,34+k,35+k+NN,13+k+NN,12+k+NN,34+k+NN,1);
 }
 for(k=SACRUM*1000+NN;k<J*NN+SACRUM*1000;k=k+2*NN)
 {
 j=29;
	for(i=1;i<5;i=i+2)
	 {

	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),i+k,i+2+k,j+2+k,j+k,i+k+NN,i+2+k+NN,j+2+k+NN,j+k+NN,1);
	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),j-1+k,j+1+k,i+3+k,i+1+k,j-1+k+NN,j+1+k+NN,i+3+k+NN,i+1+k+NN,1);

	  j=j+2;

	 }
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),5+k,7+k,19+k,33+k,5+k+NN,7+k+NN,19+k+NN,33+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),32+k,18+k,8+k,6+k,32+k+NN,18+k+NN,8+k+NN,6+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),14+k,15+k,17+k,13+k,14+k+NN,15+k+NN,17+k+NN,13+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),13+k,17+k,11+k,12+k,13+k+NN,17+k+NN,11+k+NN,12+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),15+k,16+k,9+k,17+k,15+k+NN,16+k+NN,9+k+NN,17+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),17+k,9+k,10+k,11+k,17+k+NN,9+k+NN,10+k+NN,11+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),36+k,14+k,13+k,35+k,36+k+NN,14+k+NN,13+k+NN,35+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),35+k,13+k,12+k,34+k,35+k+NN,13+k+NN,12+k+NN,34+k+NN,1);

 }
 for(k=SACRUM*1000+6*NN;k<J*NN+SACRUM*1000;k=k+2*NN)
 {
 j=29;
	for(i=1;i<5;i=i+2)
	 {

	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),i+k,i+2+k,j+2+k,j+k,i+k+NN,i+2+k+NN,j+2+k+NN,j+k+NN,1);
	  NbMaille=NbMaille+1;
	  SetMaCo(Maille.e(NbMaille),j-1+k,j+1+k,i+3+k,i+1+k,j-1+k+NN,j+1+k+NN,i+3+k+NN,i+1+k+NN,1);

	  j=j+2;
	 }
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),5+k,7+k,19+k,33+k,5+k+NN,7+k+NN,19+k+NN,33+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),32+k,18+k,8+k,6+k,32+k+NN,18+k+NN,8+k+NN,6+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),14+k,15+k,17+k,13+k,14+k+NN,15+k+NN,17+k+NN,13+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),13+k,17+k,11+k,12+k,13+k+NN,17+k+NN,11+k+NN,12+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),15+k,16+k,9+k,17+k,15+k+NN,16+k+NN,9+k+NN,17+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),17+k,9+k,10+k,11+k,17+k+NN,9+k+NN,10+k+NN,11+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),36+k,14+k,13+k,35+k,36+k+NN,14+k+NN,13+k+NN,35+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),35+k,13+k,12+k,34+k,35+k+NN,13+k+NN,12+k+NN,34+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),29+k,31+k,14+k,36+k,29+k+NN,31+k+NN,14+k+NN,36+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),31+k,33+k,15+k,14+k,31+k+NN,33+k+NN,15+k+NN,14+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),33+k,19+k,16+k,15+k,33+k+NN,19+k+NN,16+k+NN,15+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),34+k,12+k,30+k,28+k,34+k+NN,12+k+NN,30+k+NN,28+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),12+k,11+k,32+k,30+k,12+k+NN,11+k+NN,32+k+NN,30+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),11+k,10+k,18+k,32+k,11+k+NN,10+k+NN,18+k+NN,32+k+NN,1);

 }
 for(k=SACRUM*1000;k<2*NN+SACRUM*1000;k=k+NN)
 {
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),37+k,29+k,23+k,39+k,37+k+NN,29+k+NN,23+k+NN,39+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),39+k,23+k,36+k,41+k,39+k+NN,23+k+NN,36+k+NN,41+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),38+k,22+k,28+k,42+k,38+k+NN,22+k+NN,28+k+NN,42+k+NN,1);
 NbMaille=NbMaille+1;
 SetMaCo(Maille.e(NbMaille),40+k,34+k,22+k,38+k,40+k+NN,34+k+NN,22+k+NN,38+k+NN,1);
 }

}

	/**********************************************************/
	/**                    partie coccyx                     **/
	/**********************************************************/

sPoint Ptepais(float un,float vb)
{

 sVecteur u,v;
 sPlan Pe;


 SetVec(&u,VecK(un*EPAISLAR,&n));
 SetVec(&v,VecK(vb*EPAISHAUT,&b));
 return *PtVec(&A[9],VecPlus(&u,&v));


}

	/**********************************************************/
	/**                   sacro illiaque                     **/
	/**********************************************************/

sPoint *sacro_ill(float k,float l,float pair)
{
 sVecteur a,b;
 static sPoint result;

	if(pair==1)
	{
	 SetVec(&a,VecK(k,&AUHD));
	 SetVec(&b,VecK(l,&AUVD));
	 SetPt(&result,PtVec(&A[4],VecPlus(&a,&b)));
	 }
		else
		 if(pair==0)
		{
		 SetVec(&a,VecK(k,&AUHG));
		 SetVec(&b,VecK(l,&AUVG));
		 SetPt(&result,PtVec(&A[5],VecPlus(&a,&b)));
		}
			else
			{
			 printf("erreur de donn‚es sur le sacro-illiaque");
			 printf("appuyer sur une touche");
			 getch();
			 exit(-1);
			}


 return &result;
}

	/**********************************************************/
	/**                     sacro lomber                     **/
	/**********************************************************/

void sacro_lomber(void)
{

 sPoint I;

 SetCer(&E4,&E2);
 E4.a=Norme(VecAB(&A[6],&A[7]))/2;
 E4.b=E4.b*2/3;
 SetPt(&E4.C,PtVec(&A[6],VecdK(2,VecAB(&A[6],&A[7]))));
 CerCutPt(&E4,&A[6],&A[6]);

	for(i=0;i<3;i++)
	{
	 NbNoeud=NbNoeud+1;
	 SetNdCo(Noeud.e(NbNoeud),PtVec(&A[7],VecK(-i*EPSL/2,&E4.k)),SACRUM*1000+i*NN+NN-5);
	 NbNoeud=NbNoeud+1;
	 SetNdCo(Noeud.e(NbNoeud),PtVec(&A[6],VecK(-i*EPSL/2,&E4.k)),SACRUM*1000+i*NN+NN);

		for(j=1;j<3;j++)
		{
		 SetPt(&I,PtCer(&E4,(15.+(float)j)/20.));
		 NbNoeud=NbNoeud+1;
		 SetNdCo(Noeud.e(NbNoeud),PtVec(&I,VecK(-i*EPSL/2,&E4.k)),SACRUM*1000+i*NN+NN-5+2*j);
		 SetPt(&I,PtCer(&E4,(5.-((float)j))/20.));
		 NbNoeud=NbNoeud+1;
		 SetNdCo(Noeud.e(NbNoeud),PtVec(&I,VecK(-i*EPSL/2,&E4.k)),SACRUM*1000+i*NN+NN-6+2*j);
		}
	}

}



int SacrumNbNoeud (void)
    {
     return 500;
    }


int SacrumNbMaille (void)
    {
     return 500;
    }