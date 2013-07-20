#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include "geom.h"
#include "geom_adv.h"





/*=======================  variables globales    =======================*/
sTabDynamique* Geom_advPTabPtDyn=NULL;
sTabDynamique* Geom_advPTabNdDyn=NULL;
sTabDynamique* Geom_advPTabMaDyn=NULL;


/*=======================  fonctions statiques  ========================*/
static void* TabGetPt (int x, ...);
static void* TabGetNd (int x, ...);
static void* TabGetMa (int x, ...);


/*========================================================================
==========================================================================
			fonctions sCourbe                   		*/

void CourbeInit	 (sCourbe* cour)
	{
	 int i;
	 cour->nbElem=0;
	 cour->totale=0;
	 for (i=0;i<COURBE_ELEM_MAX;i++)
	     {
	      cour->ou[i]=NULL;
	      cour->qui[i]=NULL;
	      cour->taille[i]=0;
	     }
	}

void CourbeAddDt (sCourbe* cour,sDroite* d)
	{
	 if (cour->nbElem==COURBE_ELEM_MAX) return;
	 cour->ou[cour->nbElem]=(int*)d;
	 if (DtLong(d)<MINI) return;
	 cour->qui[cour->nbElem]=1;
	 cour->taille[cour->nbElem]=DtLong(d);
	 cour->totale+=cour->taille[cour->nbElem];
	 cour->nbElem++;
	}

void CourbeAddCer(sCourbe* cour,sCercle* c)
	{
	 if (cour->nbElem==COURBE_ELEM_MAX) return;
	 if (CerLong(c)<MINI) return;
	 cour->ou[cour->nbElem]=(int*)c;
	 cour->qui[cour->nbElem]=2;
	 cour->taille[cour->nbElem]=CerLong(c);
	 cour->totale+=cour->taille[cour->nbElem];
	 cour->nbElem++;
	}

sPoint* PtCourbe (sCourbe* cour,float n)
	{
	 static sPoint result;
	 int i=0;
	 float nc;
	 float localetaille=0;
	 float tempsous=0;
	 while (i<cour->nbElem)
	       {
		localetaille+=cour->taille[i];
		if(localetaille>=(cour->totale*n)) break;
		tempsous=localetaille;
		i++;
	       }
	 if (i==cour->nbElem)
	    {
	     localetaille=cour->totale;
	     i--;
	     tempsous=localetaille-cour->taille[i];
	    }
	 nc=(n*cour->totale-tempsous)/cour->taille[i];

	 switch(cour->qui[i])
		{
		 case 1: SetPt(&result,PtDt((sDroite*)(cour->ou[i]),nc));
			 break;
		 case 2: SetPt(&result,PtCer((sCercle*)(cour->ou[i]),nc));
			 break;
		}
	 return &result;
	}



/*========================================================================
==========================================================================
			fonctions sSpline                   		*/

int SplineInit (sSpline *s,sPoint *A,sVecteur *ua,sPoint *B,sVecteur *ub)
	{
	 sVecteur i,j,k;
	 float a,b,ab,ftemp,ftemp1;

	 ab=Norme(VecAB(A,B));
	 if (ab<MINI) return (SUR);

	 SetVec (&i,VecNorme(VecAB(A,B)));
	 SetVec (&j,VecVec(&i,Vec(1,0,0)));

	 if (Norme(&j)<0.7)
	    SetVec (&j,VecNorme(VecVec(&i,Vec(0,1,0))));
	 else
	    SetVec (&j,VecNorme(&j));

	 SetVec (&k,VecVec(&i,&j));

	 SetRepCo (&s->R,A,&i,&j,&k);
	 s->longeur=ab;

	 if (fabs(Scal(&i,ua))<MINIP || fabs(Scal(&i,ub))<MINIP)
	    return (PARALLELE);

	 /* dans le plan Oij	*/
	 ftemp=Scal(&i,ua);
	 ftemp1=Scal(&j,ua);
	 if (ftemp<0)
	    {
	     ftemp=-ftemp;
	     ftemp1=ftemp1;
	    }
	 a=ftemp1/ftemp;

	 ftemp=Scal(&i,ub);
	 ftemp1=Scal(&j,ub);
	 if (ftemp<0)
	    {
	     ftemp=-ftemp;
	     ftemp1=ftemp1;
	    }
	 b=ftemp1/ftemp;

	 s->coeff[0][0]=(a+b)/ab/ab;
	 s->coeff[0][1]=(-b-2*a)/ab;
	 s->coeff[0][2]=a;

	 /* dans le plan Oik */
	 ftemp=Scal(&i,ua);
	 ftemp1=Scal(&k,ua);
	 if (ftemp<0)
	    {
	     ftemp=-ftemp;
	     ftemp1=ftemp1;
	    }
	 a=ftemp/ftemp1;

	 ftemp=Scal(&i,ub);
	 ftemp1=Scal(&k,ub);
	 if (ftemp<0)
	    {
	     ftemp=-ftemp;
	     ftemp1=ftemp1;
	    }
	 b=ftemp/ftemp1;

	 s->coeff[1][0]=(a+b)/ab/ab;
	 s->coeff[1][1]=(-b-2*a)/ab;
	 s->coeff[1][2]=a;

	 return OK;
	}


sPoint* PtSpline (sSpline *s,float n)
	{
	 static sPoint result;
	 float y,z;

	 n=n*s->longeur;
	 y=s->coeff[0][0]*n*n*n+s->coeff[0][1]*n*n+s->coeff[0][2]*n;
	 z=s->coeff[1][0]*n*n*n+s->coeff[1][1]*n*n+s->coeff[1][2]*n;
	 SetPt (&result,PtRepOld(Pt(n,y,z),&s->R));
	 return &result;
	}



/*========================================================================
/*========================================================================
			fonctions d'echelle		    		*/

void   EchelleInit(sEchelle* ec,int nb,float p)
	{
	 float a,b,x;

	 ec->nb=nb;
	 ec->p0=p;

	 x=1/pow(p*(float)(nb),1/(float)(nb-1));
	 if(p>(1/(float)nb)) x-=0.2;
	 else x+=0.2;

	 a=(x*(1/p-pow(x,(float)(nb-1)))-1/p+1);
	 while (fabs(a)>MINI)
	 {
	  b=(1/p-(float)(nb)*pow(x,(float)(nb-1)));
	  x=x-a/b;
	  a=(x*(1/p-pow(x,(float)(nb-1)))-1/p+1);
	 }

	 ec->q=x;
	}

float  EcPos      (sEchelle* ec,int pos)
	{
	 int i;
	 float result=0;
	 for (i=1;i<=pos;i++)
	   result=result+ec->p0*pow(ec->q,(float)i-1);
	 return result;
	}



/*========================================================================
/*========================================================================
			fonctions de quadrillage	    		*/

void    QuadriInit (sQuadri* q,sPoint* p,int x,int y)
	{
	 q->tab=p;
	 q->lx=x-1;
	 q->ly=y-1;
	}



sPoint * PtQuadri (sQuadri* q,float x,float y)
	{
	 static sPoint result;
	 int xx,yy;
	 double temp;
	 sPoint * a,*b,*c,*d,i,j;
	 sVecteur am;
	 float fxr,fyr;

	 fxr=1;
	 fyr=1;
	 fxr=modf(x*q->lx,&temp);
	 fyr=modf(y*q->ly,&temp);

	 if (fabs(fxr)<MINI && fabs(fyr)<MINI)
	    {
	     SetPt (&result,q->tab+(rnd(x*q->lx)*(q->ly+1)+rnd(y*q->ly )));
	     return &result;
	    }

	 xx=floor(x*q->lx);
	 yy=floor(y*q->ly);
	 if (xx==q->lx) { xx--; fxr=1;}
	 if (yy==q->ly) { yy--; fyr=1;}


	 a=q->tab+(xx*(q->ly+1)+yy);
	 b=q->tab+((xx+1)*(q->ly+1)+yy);
	 c=q->tab+(xx*(q->ly+1)+yy+1);
	 d=q->tab+((xx+1)*(q->ly+1)+yy+1);

	 SetPt (&i,PtVec(a,VecK(fxr,VecAB(a,b))));
	 SetPt (&j,PtVec(c,VecK(fxr,VecAB(c,d))));

	 SetPt (&result,PtVec(&i,VecK(fyr,VecAB(&i,&j))));

	 return &result;
	}



sPoint* SitePtQuadri (sPoint *p,sQuadri *q)
	{
	 static sPoint result;
	 float d=0,dmini,fab,fbc;
	 float xa,ya,xc,yc,xr,yr;
	 int   i,j,sx,sy,x,y;
	 sPoint a,b,c;

	 dmini=3.4E38;
	 for (i=0;i<=q->lx;i=i+q->lx)
	    {
	     for (j=0;j<=q->ly;j++)
		 {
		  d=Norme(VecAB(p,q->tab+i*(q->ly+1)+j));
		  if (d<dmini)
		     {
		      dmini=d;
		      x=i;
		      y=j;
		     }
		 }
	     if (q->lx==0) i++;
	    }


	 for (i=0;i<=q->lx;i++)
	     for (j=0;j<=q->ly;j=j+q->ly)
		 {
		  d=Norme(VecAB(p,q->tab+i*(q->ly+1)+j));
		  if (d<dmini)
		     {
		      dmini=d;
		      x=i;
		      y=j;
		     }
		  if (q->ly==0) j++;
		 }

	 if (x>0 && x<q->lx)
	    { i=x+1; j=y; sx=x-1; sy=y; }
	 else if (y>0 && y<q->ly)
	    { i=x; j=y+1; sx=x; sy=y-1; }
	 else
	 {
	 if      (x==0)      { i=1; sx=0;   }
	 else if (x==q->lx)  { i=x-1; sx=x; }
	 if      (y==0)      { j=0; sy=1;   }
	 else if (y==q->ly)  { j=y; sy=y-1; }
	 }

	 SetPt (&c,q->tab+i*(q->ly+1)+j);
	 SetPt (&b,q->tab+x*(q->ly+1)+y);
	 SetPt (&a,q->tab+sx*(q->ly+1)+sy);

	 fab=SitePtDt (p,DtPt(&a,&b));
	 fbc=SitePtDt (p,DtPt(&b,&c));



	 if (fab<1 && fab>0)
	    {
	     SetPt (&a,PtDt(DtPt(&a,&b),fab));
	     d=Norme (VecAB(p,&a));
	     xa=sx+fab*(x-sx);
	     ya=sy+fab*(y-sy);
	     sx=1;
	    }
	 else sx=0;

	 if (fbc<1 && fbc>0)
	    {
	     SetPt (&c,PtDt(DtPt(&b,&c),fbc));
	     dmini=Norme (VecAB(p,&c));
	     xc=x+fbc*(i-x);
	     yc=y+fbc*(j-y);
	     i=1;
	    }
	 else i=0;

	 if (i==0 && sx==0) {xr=x; yr=y;  }
	 if (i==1 && sx==1)
	    if (d<dmini) i=0;
	       else      sx=0;
	 if (i==1 && sx==0) {xr=xc;yr=yc; }
	 if (i==0 && sx==1) {xr=xa;yr=ya; }

	 if (q->lx==0) xr=0;
	 else          xr=xr/q->lx;

	 if (q->ly==0) yr=0;
	 else          yr=yr/q->ly;

	 SetPtCo (&result,xr,yr,0);

	 return &result;
	}






/*========================================================================
/*========================================================================
			fonctions du tableau dynamique	    		*/


void TabDel (sTabDynamique* t)
	{
	 if (t->tab!=NULL) free (t->tab);
	}



/*-------------------------   POINTS   ---------------------------------*/

void TabPtInit (sTabDynamique* t,int nbdimension, ...)
	{
	 va_list ap;
	 int arg,i,cont=1;

	 va_start(ap,nbdimension);

	 t->nbdimension=nbdimension;

	 for (i=0;i<nbdimension;i++)
	     {
	      arg=va_arg(ap,int);
	      cont=cont*arg;
	      t->dimension[i]=arg;
	     }

	 t->tab=calloc (cont,sizeof(sPoint));
	 if (t->tab==NULL)
	    {
	     printf("Pas assez de memoire dans le tas pour ");
	     printf("creer un tableau de %d Points\n",cont);
	     exit (-1);
	    }

	 Geom_advPTabPtDyn=t;
	 t->e=TabGetPt;
	 t->p=TabGetPt;
	 t->m=TabGetPt;
	 t->n=TabGetPt;

	 va_end (ap);
	}


sPoint* PtTab (sTabDynamique* t, ...)
	{
	 sPoint * p;
	 va_list ap;
	 int i,j,arg,cont;

	 p=(sPoint*)(t->tab);

	 va_start(ap,t);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      arg=va_arg(ap,int);
	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}


void* TabGetPt (int x, ...)
	{
	 sPoint * p;
	 sTabDynamique* t;
	 va_list ap;
	 int i,j,arg,cont;

	 t=Geom_advPTabPtDyn;
	 p=(sPoint*)(t->tab);

	 va_start(ap,x);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      if (i==0)
		 arg=x;
	      else
		 arg=va_arg(ap,int);

	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}



void TabPtFocus (sTabDynamique* t)
	{
	 Geom_advPTabPtDyn=t;
	}



/*-------------------------   NOEUDS   ---------------------------------*/

void TabNdInit (sTabDynamique* t,int nbdimension, ...)
	{
	 va_list ap;
	 int arg,i,cont=1;

	 va_start(ap,nbdimension);

	 t->nbdimension=nbdimension;

	 for (i=0;i<nbdimension;i++)
	     {
	      arg=va_arg(ap,int);
	      cont=cont*arg;
	      t->dimension[i]=arg;
	     }

	 t->tab=calloc (cont,sizeof(sNoeud));
	 if (t->tab==NULL)
	    {
	     printf("Pas assez de memoire dans le tas pour ");
	     printf("creer un tableau de %d Noeuds\n",cont);
	     exit (-1);
	    }

	 Geom_advPTabNdDyn=t;
	 t->e=TabGetNd;
	 t->p=TabGetNd;
	 t->m=TabGetNd;
	 t->n=TabGetNd;

	 va_end (ap);
	}


sNoeud* NdTab (sTabDynamique* t, ...)
	{
	 sNoeud * p;
	 va_list ap;
	 int i,j,arg,cont;

	 p=(sNoeud*)(t->tab);

	 va_start(ap,t);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      arg=va_arg(ap,int);
	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}


void* TabGetNd (int x, ...)
	{
	 sNoeud * p;
	 sTabDynamique* t;
	 va_list ap;
	 int i,j,arg,cont;

	 t=Geom_advPTabNdDyn;
	 p=(sNoeud*)(t->tab);

	 va_start(ap,x);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      if (i==0)
		 arg=x;
	      else
		 arg=va_arg(ap,int);

	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}


void TabNdFocus (sTabDynamique* t)
	{
	 Geom_advPTabNdDyn=t;
	}


/*-------------------------  MAILLES   ---------------------------------*/

void TabMaInit (sTabDynamique* t,int nbdimension, ...)
	{
	 va_list ap;
	 int arg,i,cont=1;

	 va_start(ap,nbdimension);

	 t->nbdimension=nbdimension;

	 for (i=0;i<nbdimension;i++)
	     {
	      arg=va_arg(ap,int);
	      cont=cont*arg;
	      t->dimension[i]=arg;
	     }

	 t->tab=calloc (cont,sizeof(sMaille));
	 if (t->tab==NULL)
	    {
	     printf("Pas assez de memoire dans le tas pour ");
	     printf("creer un tableau de %d Noeuds\n",cont);
	     exit (-1);
	    }

	 Geom_advPTabMaDyn=t;
	 t->e=TabGetMa;
	 t->p=TabGetMa;
	 t->m=TabGetMa;
	 t->n=TabGetMa;

	 va_end (ap);
	}


sMaille* MaTab (sTabDynamique* t, ...)
	{
	 sMaille * p;
	 va_list ap;
	 int i,j,arg,cont;

	 p=(sMaille*)(t->tab);

	 va_start(ap,t);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      arg=va_arg(ap,int);
	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}


void* TabGetMa (int x, ...)
	{
	 sMaille * p;
	 sTabDynamique* t;
	 va_list ap;
	 int i,j,arg,cont;

	 t=Geom_advPTabMaDyn;
	 p=(sMaille*)(t->tab);

	 va_start(ap,x);

	 for (i=0;i<t->nbdimension;i++)
	     {
	      cont=1;
	      for (j=i+1;j<t->nbdimension;j++)
		   cont=cont*t->dimension[j];

	      if (i==0)
		 arg=x;
	      else
		 arg=va_arg(ap,int);

	      if (arg>=t->dimension[i]) arg=t->dimension[i]-1;
	      p=p+cont*arg;
	     }

	 va_end(ap);
	 return p;
	}


void TabMaFocus (sTabDynamique* t)
	{
	 Geom_advPTabMaDyn=t;
	}




/*========================================================================
/*========================================================================
			autre fonctions			    		*/

float Fondu	(float milieu,float pos)
	{
	 if (milieu<0 || milieu>1) milieu=0.5;
	 if (pos>milieu)
		return (0.5+(pos-milieu)*0.5/(1-milieu));
	 else
		return (pos/milieu*0.5);
	}


int Num_Pos(int i,int j,int k,int J,int K)
    {
    static int result;
    result=k+j*(K+1)+i*(K+1)*(J+1);
    return result;
    }

