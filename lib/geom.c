#include <stdio.h>
#include <math.h>

#include "geom.h"

/************************************************************/
int 	GeomERROR=0;
int 	GeomInt=0;
float   GeomFloat=0;

/*==========================================================*/
/*============================================================
			sPoint fonctions                    */

void SetPtCo (sPoint *P,float i,float j,float k)
	{
	 P->x=i;
	 P->y=j;
	 P->z=k;
	}

void SetPt (sPoint *P,sPoint *Q)
	{
	 P->x=Q->x;
	 P->y=Q->y;
	 P->z=Q->z;
	 if (fabs(P->x)<MINI)  P->x=0;
	 if (fabs(P->y)<MINI)  P->y=0;
	 if (fabs(P->z)<MINI)  P->z=0;
	}

sPoint * Pt (float a,float b,float c)
	{
	 static sPoint result;
	 SetPtCo(&result,a,b,c);
	 return (&result);
	}

sPoint * PtVec (sPoint *P,sVecteur *u)
	{
	 static sPoint Res;
	 Res.x=P->x+u->u;
	 Res.y=P->y+u->v;
	 Res.z=P->z+u->w;
	 return (&Res);
	}

sPoint * PtPro (sPoint *m,sPlan *p)
	{
	 static sPoint result;
	 sVecteur pm;
	 SetVec(&pm,VecAB(&p->P,m));
	 SetPt(&result,PtVec(&p->P,VecPro(&pm,&p->u)));
	 return (&result);
	}

sPoint * PtRepNew (sPoint *p,sRepere *new)
	{
	 static sPoint result;
	 sVecteur om;
	 float a,b,c;
	 a=Norme(&new->i);
	 b=Norme(&new->j);
	 c=Norme(&new->k);
	 SetVec (&om,VecAB(&new->O,p));
	 result.x=Scal(&om,&new->i)/(a*a);
	 result.y=Scal(&om,&new->j)/(b*b);
	 result.z=Scal(&om,&new->k)/(c*c);
	 return (&result);
	}

sPoint * PtRepOld (sPoint *p,sRepere *old)
	{
	 static sPoint result;
	 sVecteur u;
	 SetVec(&u,VecK(p->x,&old->i));
	 SetVec(&u,VecPlus(&u,VecK(p->y,&old->j)));
	 SetVec(&u,VecPlus(&u,VecK(p->z,&old->k)));
	 SetPt (&result,PtVec(&old->O,&u));
	 return (&result);
	}

sPoint * PtCerAngle (sCercle *c,float angle)
	{
	 static sPoint result;
	 sVecteur om;
	 float ro,a,b;

	 a=c->a;
	 b=c->b;
	 ro=1/sqrt((cos(angle)*cos(angle)/a/a)+(sin(angle)*sin(angle)/b/b));

	 SetVec(&om,VecK(ro*cos(angle),&c->i));
	 SetVec(&om,VecPlus(&om,VecK(ro*sin(angle),&c->j)));
	 SetPt(&result,PtVec(&c->C,&om));
	 return (&result);
	}


sPoint * PtCer	(sCercle *c,float n)
	{
	 static sPoint result;
	 sVecteur om;
	 float angle,ro,a,b;

	 a=c->a;
	 b=c->b;
	 if (c->beta<=c->alpha)
	   {
	    angle=(2*PI-c->alpha+c->beta)*n+c->alpha;
	   }
	 else
	   {
	    angle=(c->beta-c->alpha)*n+c->alpha;
	   }
	 if (angle>(2*PI))
	   {
	    angle=angle-2*PI;
	   }
	 ro=1/sqrt((cos(angle)*cos(angle)/a/a)+(sin(angle)*sin(angle)/b/b));
	 SetVec(&om,VecK(ro*cos(angle),&c->i));
	 SetVec(&om,VecPlus(&om,VecK(ro*sin(angle),&c->j)));
	 SetPt(&result,PtVec(&c->C,&om));
	 return (&result);
	}

sPoint * PtDt	(sDroite *d,float n)
	{
	 static sPoint result;
	 sVecteur ab;
	 if (d->segment>0)
	    SetVec(&ab,VecAB(&d->A,&d->B));
	 else
	    SetVec(&ab,&d->u);
	 SetPt(&result,PtVec(&d->A,VecK(n,&ab)));
	 return (&result);
	}



sPoint * PtTanCer (sPoint *p,sCercle *c,int pos)
	{
	 static sPoint result[2];
	 float a,l,d,x,y;
	 sVecteur i,j,r;

	 if (Norme(VecAB(p,&c->C))<c->a)
	    {
	     GeomERROR=DISTANT;
	     return (&result[0]);
	    }
	 GeomERROR=OK;

	 SetVec (&i,VecNorme(VecAB(p,&c->C)));
	 SetVec (&j,VecVec(&c->k,&i));
	 d=Norme(VecAB(p,&c->C));
	 l=sqrt(d*d-c->a*c->a);
	 a=acos(l/d);
	 x=l*l/d;
	 y=l*sin(a);

	 SetVec (&r,VecK(x,&i));
	 if (pos==GAUCHE)
	    {
	     SetPt(&result[0],PtVec(p,VecPlus(&r,VecK(y,&j))));
	     SetPt(&result[1],PtVec(p,VecMoins(&r,VecK(y,&j))));
	    }
	 else
	    {
	     SetPt(&result[1],PtVec(p,VecPlus(&r,VecK(y,&j))));
	     SetPt(&result[0],PtVec(p,VecMoins(&r,VecK(y,&j))));
	    }
	 return (&result[0]);
	}



/*==========================================================*/
/*============================================================
			sVecteur fonctions                        */


void SetVecCo (sVecteur *V,float i,float j,float k)
	{
	 V->u=i;
	 V->v=j;
	 V->w=k;
	}

void SetVec (sVecteur *u,sVecteur *v)
	{
	 u->u=v->u;
	 u->v=v->v;
	 u->w=v->w;
	 if (fabs(u->u)<MINI)  u->u=0;
	 if (fabs(u->v)<MINI)  u->v=0;
	 if (fabs(u->w)<MINI)  u->w=0;
	}

sVecteur * Vec (float a,float b,float c)
	{
	 static sVecteur result;
	 SetVecCo (&result,a,b,c);
	 return (&result);
	}

sVecteur * VecAB (sPoint *A,sPoint *B)
	{
	 static sVecteur bVec;
	 bVec.u=B->x-A->x;
	 bVec.v=B->y-A->y;
	 bVec.w=B->z-A->z;
	 return (&bVec);
	}

sVecteur * VecK (float k,sVecteur *u)
	{
	 static sVecteur bVec;
	 bVec.u=u->u*k;
	 bVec.v=u->v*k;
	 bVec.w=u->w*k;
	 return (&bVec);
	}

sVecteur * VecdK (float k,sVecteur *u)
	{
	 static sVecteur bVec;
	 float l;
	 l=1/k;
	 SetVec(&bVec,VecK(l,u));
	 return (&bVec);
	}

sVecteur * VecVec (sVecteur *u,sVecteur *v)
	{
	 static sVecteur bVec;
	 bVec.u=(u->v)*(v->w)-(u->w)*(v->v);
	 bVec.v=(v->u)*(u->w)-(u->u)*(v->w);
	 bVec.w=(u->u)*(v->v)-(u->v)*(v->u);
	 return (&bVec);
	}

sVecteur * VecPlus (sVecteur *u,sVecteur *v)
	{
	 static sVecteur bVec;
	 bVec.u=(u->u)+(v->u);
	 bVec.v=(u->v)+(v->v);
	 bVec.w=(u->w)+(v->w);
	 return (&bVec);
	}

sVecteur * VecMoins (sVecteur *u,sVecteur *v)
	{
	 sVecteur l;
	 static sVecteur k;
	 SetVec(&l,VecK(-1,v));
	 SetVec(&k,VecPlus(u,&l));
	 return (&k);
	}

float Norme (sVecteur *u)
	{
	 float k;
	 k=sqrt((u->u)*(u->u)+(u->v)*(u->v)+(u->w)*(u->w));
	 return k;
	}

sVecteur * VecNorme (sVecteur *u)
	{
	 static sVecteur bVec;
	 float k;
	 k=Norme(u);
	 if (k>MINI)
	    {bVec.u=u->u/k;
	     bVec.v=u->v/k;
	     bVec.w=u->w/k;
	    }
	 else
	   {
	    bVec.u=0;
	    bVec.v=0;
	    bVec.w=0;
	   }
	 return (&bVec);
	}

sVecteur * VecRot    (sVecteur *u,sVecteur *v,float a)
	{
	 static sVecteur result;
	 sVecteur i,j,k;
	 float x;
	 SetVec(&k,VecNorme(v));
	 SetVec(&j,VecNorme(VecVec(u,&k)));
	 SetVec(&i,VecNorme(VecVec(&k,&j)));
	 x=Scal(u,&i);
	 SetVec(&result,VecK(Scal(u,&k),&k));
	 SetVec(&result,VecPlus(&result,VecK(x*cos(a),&i)));
	 SetVec(&result,VecPlus(&result,VecK(x*sin(a),&j)));
	 return (&result);
	}


float Scal (sVecteur *u,sVecteur *v)
	{
	 float fScalaire;
	 fScalaire=u->u*v->u+u->v*v->v+u->w*v->w;
	 return (fScalaire);
	}

float Angle (sVecteur *a,sVecteur *b,sVecteur *c)
	{
	 float k,l;
	 sVecteur u,v,w;
	 SetVec(&u,VecNorme(a));
	 SetVec(&v,VecNorme(b));
	 SetVec(&w,VecNorme(c));
	 k=Scal(&u,&v);
	 if (k>1) k=1;
	 if (k<-1) k=-1;
	 k=acos(k);
	 if (Scal(&u,&w)<0) k=2*PI-k;
	 return (k);
	}

sVecteur * VecPro (sVecteur *u,sVecteur *v)
	{
	 static sVecteur result;
	 sVecteur w;
	 SetVec(&w,VecNorme(v));
	 SetVec(&result,VecMoins(u,VecK(Scal(u,&w),&w)));
	 return (&result);
	}

sVecteur * VecRepNew (sVecteur *u,sRepere *new)
	{
	 static sVecteur result;
	 result.u=Scal(u,&new->i);
	 result.v=Scal(u,&new->j);
	 result.w=Scal(u,&new->k);
	 return (&result);
	}

sVecteur * VecRepOld (sVecteur *u,sRepere *old)
	{
	 static sVecteur result;
	 SetVec(&result,VecK(u->u,&old->i));
	 SetVec(&result,VecPlus(&result,VecK(u->v,&old->j)));
	 SetVec(&result,VecPlus(&result,VecK(u->w,&old->k)));
	 return (&result);
	}

/*============================================================
==============================================================
		fonctions associees a sPlan                     */

void SetPlCo(sPlan *pl,sPoint *P,sVecteur *u)
	{
	 SetPt(&pl->P,P);
	 SetVec(&pl->u,VecNorme(u));
	}

void SetPl (sPlan *P,sPlan *Q)
	{
	 SetPt (&P->P,&Q->P);
	 SetVec(&P->u,&Q->u);
	}

sPlan* Pl (sPoint* p,sVecteur *u)
	{
	 static sPlan result;
	 SetPlCo (&result,p,u);
	 return (&result);
	}

sPlan * PlPt3 (sPoint *A,sPoint *B,sPoint *C)
	{
	 static sPlan pl;
	 sVecteur ba;
	 SetPt (&pl.P,B);
	 SetVec(&ba,VecAB(B,A));
	 SetVec(&pl.u,VecNorme(VecVec(VecAB(B,C),&ba)));
	 return &pl;
	}

sPlan *PlPara (sPlan *pl,sPoint *M)
	{
	 static sPlan Q;
	 SetPl(&Q,pl);
	 SetPt(&Q.P,M);
	 return &Q;
	}

sPlan *PlParaDi (sPlan *pl,float d)
	{
	 static sPlan Q;
	 SetPl(&Q,pl);
	 SetPt(&Q.P,PtVec(&pl->P,VecK(d,&pl->u)));
	 return &Q;
	}

/*============================================================
==============================================================
		  definition fonctions  sSphere             */

void SetSphCo (sSphere *Sp,sPoint *P,float r)
	{
	 SetPt(&Sp->C,P);
	 if (r<MINI)  r=0;
	 Sp->R=r;
	}

void SetSph (sSphere *S,sSphere *T)
	{
	 S->R=T->R;
	 SetPt(&S->C,&T->C);
	}

sSphere* SphPt4 (sPoint* A, sPoint* B, sPoint* C, sPoint *D)
	{
	 static sSphere result;
	 sPlan P,Q,R;
	 sDroite dt;
	 sPoint M;
	 sVecteur k;

	 SetPt   (&M,PtVec(A,VecK(0.5,VecAB(A,B))));
	 SetVec  (&k,VecAB(A,B));
	 SetPlCo (&P,&M,&k);

	 SetPt   (&M,PtVec(A,VecK(0.5,VecAB(A,C))));
	 SetVec  (&k,VecAB(A,C));
	 SetPlCo (&Q,&M,&k);

	 SetPt   (&M,PtVec(A,VecK(0.5,VecAB(A,D))));
	 SetVec  (&k,VecAB(A,D));
	 SetPlCo (&R,&M,&k);

	 SetDt (&dt,Plan_Plan(&P,&Q));
	 SetPt (&result.C,Plan_Droite(&R,&dt));

	 result.R=Norme(VecAB(&result.C,A));
	 return (&result);
	}

/*============================================================
==============================================================
			Definition fonctions sCone          */

void SetCoCo ( sCone *co,sPoint *pt,sVecteur *v,float a)
	{
	 sVecteur nul;
	 SetPt (&co->S,pt);
	 SetVec(&co->u,VecNorme(v));
	 if (a<MINI)  a=0;
	 co->Agl=a;
	 SetVecCo(&nul,0,0,0);
	 SetVec(&co->a,&nul);
	 SetVec(&co->b,&nul);
	}

void SetCo (sCone *c, sCone *d)
	{
	 SetPt(&c->S,&d->S);
	 SetVec(&c->u,&d->u);
	 SetVec(&c->a,&d->a);
	 SetVec(&c->b,&d->b);
	 c->a=d->a;
	 c->Agl=d->Agl;
	}

sCone * CoCutPt (sCone *co, sPoint *A,sPoint *B)
	{
	 static sCone result;
	 sVecteur v,w;
	 SetCo( &result,co);

	 SetVec(&v,VecAB(&co->S,A));
	 SetVec(&w,VecNorme(VecPro(&v,&co->u)));
	 SetVec(&result.a,&w);

	 SetVec(&v,VecAB(&co->S,B));
	 SetVec(&w,VecNorme(VecPro(&v,&co->u)));
	 SetVec(&result.b,&w);
	 return (&result);
	}

/*============================================================
==============================================================
			 fonctions  sCercle                 */

void SetCerCoCer (sCercle *cer,sPoint *p,sVecteur *v,float r)
	{
	 sVecteur i;

	 SetPt(&cer->C,p);
	 SetVec(&cer->k,VecNorme(v));
	 SetVecCo(&i,1,0,0);
	 if (fabs(Scal(&i,&cer->k))>0.7)
	    SetVecCo(&i,0,1,0);
	 SetVec (&cer->i,VecNorme(VecPro(&i,&cer->k)));
	 SetVec (&cer->j,VecNorme(VecVec(&cer->k,&cer->i)));
	 cer->a=r;
	 cer->b=r;
	 cer->alpha=0;
	 cer->beta=0;
	 cer->cercle=1;
	}

void SetCer ( sCercle *Cer1, sCercle *Cer2)
	{
	 SetPt (&Cer1->C,&Cer2->C);
	 SetVec(&Cer1->i,&Cer2->i);
	 SetVec(&Cer1->j,&Cer2->j);
	 SetVec(&Cer1->k,&Cer2->k);
	 Cer1->a=Cer2->a;
	 Cer1->b=Cer2->b;
	 Cer1->alpha=Cer2->alpha;
	 Cer1->beta=Cer2->beta;
	 Cer1->cercle=Cer2->cercle;
	 if (fabs(Cer1->a)<MINI) Cer1->a=0;
	 if (fabs(Cer1->b)<MINI) Cer1->b=0;
	 if (fabs(Cer1->alpha)<MINI) Cer1->alpha=0;
	 if (fabs(Cer1->beta)<MINI) Cer1->beta=0;
	}

sCercle * CerPt3 (sPoint *a,sPoint*b, sPoint *c)
	{
	 static sCercle result;
	 sPlan p;
	 sDroite d,e;
	 sPoint centre,m1,m2;
	 sVecteur i,j,k;

	 SetVec(&i,VecAB(b,c));
	 SetVec(&j,VecAB(b,a));
	 SetVec(&k,VecNorme(VecVec(&i,&j)));
	 SetPt(&m1,PtVec(b,VecK(0.5,&i)));
	 SetPt(&m2,PtVec(b,VecK(0.5,&j)));
	 SetVec(&i,VecVec(&k,&i));
	 SetVec(&j,VecVec(&j,&k));

	 SetDtCo(&d,&m1,&i);
	 SetDtCo(&e,&m2,&j);
	 SetPlCo(&p,b,&k);

	 SetPt(&centre,Droite_Droite(&d,&e,&p));

	 SetPt(&result.C,&centre);
	 SetVec(&result.i,VecNorme(&i));
	 SetVec(&result.k,&k);
	 SetVec(&result.j,VecVec(&result.k,&result.i));
	 result.a=Norme(VecAB(b,&centre));
	 result.b=result.a;
	 result.alpha=0;
	 result.beta=0;
	 result.cercle=1;

	 return (&result);
	}

sCercle * CerCutPt (sCercle *ce, sPoint *A,sPoint *B)
	{
	 static sCercle result;
	 sVecteur v,w;
	 SetCer( &result,ce);

	 SetVec(&v,VecAB(&ce->C,A));
	 SetVec(&w,VecNorme(VecPro(&v,&result.k)));
	 if (result.cercle)
	    {
	     SetVec(&result.i,&w);
	     SetVec(&result.j,VecVec(&result.k,&result.i));
	     result.alpha=0;
	    }
	 else
	    result.alpha=Angle(&w,&result.i,&result.j);

	 SetVec(&v,VecAB(&ce->C,B));
	 SetVec(&w,VecNorme(VecPro(&v,&result.k)));
	 result.beta=Angle(&w,&result.i,&result.j);
	 return (&result);
	}

void InitDebutCer (sCercle *ce,sPoint *A)
	{
	 sVecteur v,w;

	 SetVec (&v,VecAB(&ce->C,A));
	 SetVec(&w,VecNorme(VecPro(&v,&ce->k)));
	 if (ce->cercle)
	    {
	     SetVec(&ce->i,&w);
	     SetVec(&ce->j,VecVec(&ce->k,&ce->i));
	     ce->alpha=0;
	     ce->beta=0;
	    }
	 else
	    {
	     ce->alpha=Angle(&w,&ce->i,&ce->j);
	     ce->beta=ce->alpha;
	    }
	}


float CerLong (sCercle* c)
	{
	 float angle;
	 if (c->beta<=c->alpha)
	    angle=2*PI-c->alpha+c->beta;
	 else
	    angle=c->beta-c->alpha;
	 if (angle>(2*PI))
	    angle=angle-PI;
	 return (angle*fabs(c->a+c->b)/2);
	}


/*============================================================
==============================================================
			fonctions sDroite                   */

void SetDtCo (sDroite *dt,sPoint *p,sVecteur *v)
	{
	 SetVec(&dt->u,VecNorme(v));
	 SetPt(&dt->A,p);
	 SetPtCo(&dt->B,0,0,0);
	 dt->segment=0;
	}

void SetDt (sDroite *d,sDroite *e)
	{
	 SetVec(&d->u,&e->u);
	 SetPt(&d->A,&e->A);
	 SetPt(&d->B,&e->B);
	 d->segment=e->segment;
	}

sDroite * Dt(sPoint *p,sVecteur *v)
	{
	 static sDroite result;
	 SetDtCo(&result,p,v);
	 return (&result);
	}

sDroite * DtPt (sPoint* a,sPoint* b)
	{
	 static sDroite result;
	 SetDtCo (&result,a,VecAB(a,b));
	 SetDt(&result,DtCutPt (&result,a,b));
	 return (&result);
	}

sDroite * DtPro (sDroite * d,sPlan *p)
	{
	 static sDroite result;
	 SetVec(&result.u,VecNorme(VecPro(&d->u,&p->u)));
	 SetPt (&result.A,PtPro (&d->A,p));
	 if (d->segment>0)
	    SetPt(&result.B,PtPro(&d->B,p));
	 else
	    SetPtCo(&result.B,0,0,0);
	 return (&result);
	}

sDroite * DtCutPt (sDroite *d,sPoint *a,sPoint *b)
	{
	 static sDroite result;
	 sVecteur v;
	 SetDt (&result,d);
	 SetVec(&v,VecAB(&result.A,a));
	 SetPt(&result.A,PtVec(&result.A,VecK(Scal(&v,&result.u),&result.u)));
	 SetVec(&v,VecAB(&result.A,b));
	 SetPt(&result.B,PtVec(&result.A,VecK(Scal(&v,&result.u),&result.u)));
	 result.segment=1;
	 return (&result);
	}

float DtLong (sDroite* d)
	{
	 if (d->segment==0) return(0);
	 return Norme(VecAB(&d->A,&d->B));
	}

/*============================================================
==============================================================
			fonctions sCylindre                  */

void SetCyCo (sCylindre *cy,sPoint *pt,sVecteur *v,float a)
	{
	 SetPt(&cy->p,pt);
	 SetVec(&cy->u,VecNorme(v));
	 cy->r=a;
	}

void SetCy (sCylindre *a,sCylindre *b)
	{
	 SetPt  (&a->p,&b->p);
	 SetVec (&a->u,&b->u);
	 a->r=b->r;
	}


/*============================================================
==============================================================
			fonctions sRepere                   */

void SetRepCo (sRepere *r,sPoint *p,sVecteur *i,sVecteur *j,sVecteur *k)
	{
	 SetPt (&r->O,p);
	 SetVec(&r->i,i);
	 SetVec(&r->j,j);
	 SetVec(&r->k,k);
	}

void SetRep   (sRepere *r,sRepere *s)
	{
	 SetPt (&r->O,&s->O);
	 SetVec(&r->i,&s->i);
	 SetVec(&r->j,&s->j);
	 SetVec(&r->k,&s->k);
	}



/*========================================================================
==========================================================================
			fonctions sNoeud                    		*/

void SetNdCo (sNoeud *n,sPoint *p,int nb)
	{
	 SetPt (&n->P,p);
	 n->num=nb;
	}

void SetNd   (sNoeud *n, sNoeud *nn)
	{
	 SetPt (&n->P,&nn->P);
	 n->num=nn->num;
	}



/*========================================================================
==========================================================================
			fonctions sMaille                   		*/

void SetMaCo (sMaille *m,int a,int b,int c, int d,int e,int f,int g,int h,
			 int mat)
	{
	 m->P[0]=a;
	 m->P[1]=b;
	 m->P[2]=c;
	 m->P[3]=d;
	 m->P[4]=e;
	 m->P[5]=f;
	 m->P[6]=g;
	 m->P[7]=h;
	 m->materiau=mat;
	}

void SetMa   (sMaille *m,sMaille *n)
	{
	 int i;
	 for (i=0;i<8;i++)
	     m->P[i]=n->P[i];
	 m->materiau=n->materiau;
	}





/*=======================================================================
=========================================================================
	fonctions de positionnment, de situation et de distance
========================================================================*/
int  PosPtCo (sPoint *p,sCone *c)
	{
	 int pos;
	 float condition;
	 sVecteur v;
	 SetVec (&v,VecAB(&c->S,p));
	 condition=fabs(Scal(&v,&c->u))-(Norme(&v)*cos(c->Agl));
	 if (fabs(condition)<MINI)
	    pos=SUR;
	 else if (condition>0)
	    pos=EXTERNE;
	 else
	    pos=INTERNE;
	 return (pos);
	}

int   PosPtCer(sPoint *p,sCercle *c)
	{
	 int pos;
	 float a,condition;
	 sPoint pr,pc;
	 sVecteur cp;

	 SetPt (&pr,PtPro(p,Pl(&c->C,&c->k)));
	 SetVec(&cp,VecAB(&c->C,&pr));
	 if (c->cercle==1)
	    condition=Norme(&cp)-c->a;
	 else
	    {
	     a=Angle(&c->i,&cp,&c->k);
	     SetPt (&pc,PtCerAngle(c,a));
	     a=Norme (&cp);
	     condition=a-Norme(VecAB(&c->C,&pc));
	    }

	 if (fabs(condition)<MINI)
	     pos=SUR;
	 else if (condition>0)
	     pos=EXTERNE;
	 else
	     pos=INTERNE;
	 return(pos);
	}

int   PosPtDt (sPoint *p,sDroite *d,sPlan*pl)
	{
	 float pos;
	 sVecteur u,v;
	 SetVec (&u,VecVec(&pl->u,&d->u));
	 SetVec (&v,VecPro(VecAB(&d->A,p),&pl->u));
	 pos=Scal(&u,&v);
	 if (fabs(pos)<MINI)
	    pos=SUR;
	 else if (pos>0)
	    pos=GAUCHE;
	 else
	    pos=DROITE;
	 return (pos);
	}


int   PosPtPl (sPoint *p,sPlan *pl)
	{
	 int pos;
	 float condition;
	 condition=Scal(&pl->u,VecAB(&pl->P,p));
	 if (fabs(condition)<MINI)
	   {
	    pos=SUR;
	    condition=0;
	   }
	 else if (condition>0)
	    pos=EXTERNE;
	 else
	    pos=INTERNE;
	 GeomFloat=condition;
	 return (pos);
	}

int   PosDtCy (sDroite *d,sCylindre *c)
	{
	 int pos;
	 GeomFloat=DistDtDt(d,Dt(&c->p,&c->u));

	 if (GeomInt==PARALLELE)
	    {
	     pos=PARALLELE;
	     return pos;
	    }

	 if (fabs(GeomFloat-c->r)/c->r<MINIP)
	    pos=SUR;
	 else if (GeomFloat>c->r)
	    pos=EXTERNE;
	 else
	    pos=INTERNE;
	 return pos;
	}


float DistPtDt (sPoint *p,sDroite *d)
	{
	 float pos;
	 pos=Norme(VecPro(VecAB(&d->A,p),&d->u));
	 return pos;
	}


float DistDtDt (sDroite *a,sDroite *b)
	{
	 float pos;
	 sVecteur u,v;
	 sPoint *pt;
	 sPlan  p;

	 if (fabs(Scal(&a->u,&b->u))>1-MINIP)
	    {
	     SetVec (&u,VecAB(&a->A,&b->B));
	     pos=Norme(VecPro(&u,&a->u));
	     GeomInt=PARALLELE;
	     return pos;
	    }

	 SetVec (&u,VecVec(&a->u,&b->u));
	 SetVec (&v,VecVec(&b->u,&u));

	 SetPlCo (&p,&b->A,&v);

	 pt=Plan_Droite(&p,a);
	 pos=DistPtDt (pt,b);
	 GeomInt=OK;
	 return pos;
	}



float SitePtDt (sPoint *m,sDroite *d)
{
 float pos;
 sVecteur u;
 SetVec (&u,VecAB(&d->A,m));
 pos=Norme(VecMoins(&u,VecPro(&u,&d->u)));
 if (fabs(pos)<MINI) pos=0;
 if (d->segment==1) pos=pos/Norme(VecAB(&d->A,&d->B));
 return (pos);
}


/*============================================================
==============================================================
	fonctions de symŠtries
============================================================*/


/*=================   SymŠtrique d'un point/un plan ========*/
sPoint * PtSymPlan (sPoint *m,sPlan *p)
	{
	 static sPoint result;
 SetPt(&result,PtVec(m,VecK(2,VecAB(m,PtPro(m,p)))));
 return (&result);
}




/*============================================================
==============================================================
	fonctions d'intersections de surface
============================================================*/

/*=====================   Plan Sphere   ====================*/
sCercle * Plan_Sphere (sPlan *pl,sSphere *sp)
	{
	 static sCercle resultnet;
	 sCercle result;
	 sVecteur v;
	 sPoint   C;
	 float    r;

	 SetVec(&v,VecK(Scal(VecAB(&sp->C,&pl->P),&pl->u),&pl->u));
	 SetPt (&C,PtVec(&sp->C,&v));
	 r=sqrt(sp->R*sp->R-Norme(VecAB(&sp->C,&C))
		*Norme(VecAB(&sp->C,&C)));
	 SetVec(&v,&pl->u);
	 SetCerCoCer (&result,&C,&v,r);
	 SetCer(&resultnet,&result);
	 return (&resultnet);
	}


/*===================   Plan Plan =========================*/
sDroite *Plan_Plan (sPlan *p,sPlan *q)
	{
	 static sDroite result;
	 sPoint P;
	 sVecteur i,j,n,u;
	 SetVec(&i,&p->u);
	 SetVec(&j,&q->u);
	 SetVec(&n,VecVec(&i,&j));
	 SetPt(&P,Plan_Droite(p,Dt(&q->P,VecVec(&n,&j))));
	 SetDtCo(&result,&P,&n);
	 return (&result);
	}


/*=====================   Plan Cone    ====================*/
sCercle * Plan_Cone (sPlan *pl, sCone *co)
	{
	 static sCercle resultnet;
	 sCercle result;
	 sPoint o;
	 float k,l,b;
	 k= Scal(VecAB(&co->S,&pl->P),&pl->u)
	   /Scal(&pl->u,&co->u);
	 SetPt (&o,PtVec(&co->S,VecK(k,&co->u)));

	 SetVec(&result.k,&pl->u);
	 SetVec(&result.i,VecNorme(VecPro(&co->u,&pl->u)));
	 if (Norme(&result.i)<MINI)
	    {
	     if ((1-fabs(Scal(Vec(1,0,0),&pl->u)))<MINI)
		SetVec(&result.i,VecNorme(VecPro(Vec(0,1,0),&pl->u)));
	     else
		SetVec(&result.i,VecNorme(VecPro(Vec(1,0,0),&pl->u)));
	    }
	 SetVec(&result.j,VecVec(&result.k,&result.i));

	 b=acos(Scal(&co->u,&pl->u));
	 k=Norme(VecAB(&co->S,&o))*sin(b)*tan(co->Agl)*tan(co->Agl);
	 k=k/(cos(b)*cos(b)-sin(b)*sin(b)*tan(co->Agl)*tan(co->Agl));
	 SetPt(&result.C,PtVec(&o,VecK(k,&result.i)));

	 result.a=Norme(VecAB(&co->S,&o))*tan(co->Agl)*cos(b)
		  /(cos(b)*cos(b)-sin(b)*sin(b)*tan(co->Agl)*tan(co->Agl));

	 l=k*sin(b)+Norme(VecAB(&co->S,&o));
	 l=l*l*tan(co->Agl)*tan(co->Agl)-k*cos(b)*cos(b);
	 result.b=sqrt(l);
	 if (fabs((result.a-result.b)/result.a)<MINI)
	    result.cercle=1;
	 else
	    result.cercle=0;

	 result.alpha=0;
	 result.beta=0;
	 if (Norme(&co->a)>1E-5)
	    result.alpha=Angle(&co->a,&result.i,&result.j);
	 if (Norme(&co->b)>1E-5)
	    result.beta=Angle(&co->b,&result.i,&result.j);
	 SetCer (&resultnet,&result);
	 return(&resultnet);
	}

/*===================   Plan Droite =========================*/
sPoint *Plan_Droite (sPlan *p,sDroite *d)
	{
	 static sPoint result;
	 float k;
	 if (fabs(Scal(&p->u,&d->u))<MINIP)
	  {
	   GeomERROR=PARALLELE;
	   return (&result);
	  }
	 else GeomERROR=OK;

	 k=Scal(VecAB(&d->A,&p->P),&p->u);
	 k=k/Scal(&d->u,&p->u);
	 SetPt(&result,PtVec(&d->A,VecK(k,&d->u)));
	 return (&result);
	}


/*================= Droite Droite sur un plan  ==============*/
sPoint * Droite_Droite (sDroite *d,sDroite *e,sPlan *p)
	{
	 static sPoint result;
	 sDroite a,b;
	 sVecteur i,j;
	 float xa,ya,xb,yb,k;

	 SetDt(&a,DtPro(d,p));
	 SetDt(&b,DtPro(e,p));

	 SetVec(&i,VecNorme(VecAB(&a.A,&b.A)));
	 SetVec(&j,VecVec(&p->u,&i));
	 xa=Scal(&a.u,&i);
	 ya=Scal(&a.u,&j);
	 xb=Scal(&b.u,&i);
	 yb=Scal(&b.u,&j);
	 k=Norme(VecAB(&a.A,&b.A))/(xa-ya/yb*xb);
	 SetPt(&result,PtVec(&a.A,VecK(k,&a.u)));
	 return (&result);
	}


/*=========================  Droite Cylindre ==================*/
sPoint * Droite_Cylindre (sDroite* d,sCylindre* c)
	{
	 static sPoint result[2];
	 sPoint *pt;
	 int pos;
	 sVecteur v;
	 sPlan p;
	 sCercle ce;

	 if (d==NULL) return result;

	 pos=PosDtCy(d,c);
	 if (pos==EXTERNE || pos==PARALLELE)
	    {
	     GeomERROR=pos;
	     return result;
	    }

	 SetVec (&v,VecVec(&d->u,&c->u));
	 SetVec (&v,VecNorme(VecVec(&v,&d->u)));
	 SetPlCo (&p,&d->A,&v);

	 SetCer (&ce,Plan_Cylindre(&p,c));

	 pt=Cercle_Droite(&ce,d);

	 if (GeomERROR!=OK) return result;

	 SetPt(&result[0],pt);
	 SetPt(&result[1],pt+1);

	 return result;
	}


/*=========================  Cone Droite  ==================*/
sPoint  * Cone_Droite (sCone *c,sDroite *d)
{
 static sPoint tabPoint [2];
 pPoint pPt;
 sCercle cer;
 sVecteur i,j,k,w;
 sPlan pl;

 if ((1-fabs(Scal(&c->u,&d->u)))<MINI)
  {
   SetVec(&k,&c->u);
   SetVec(&i,VecNorme(VecVec(VecAB(&c->S,&d->A),&k)));
   SetVec(&j,VecVec(&k,&i));

   SetVec(&w,VecK(cos(c->Agl),&i));
   SetVec(&w,VecPlus(&w,VecK(sin(c->Agl),&j)));

   SetPt(&tabPoint[0],
	 PtVec(&c->S,VecK(SitePtDt(&c->S,d)*tan(PI/2-c->Agl),&w)));
   GeomERROR=PARALLELE;
  }
 else
  {
   SetVec(&k,VecNorme(VecVec(VecAB(&c->S,&d->A),&d->u)));
   SetVec(&i,VecVec(&d->u,&k));
   SetPlCo (&pl,&d->A,&i);

   SetCer (&cer,Plan_Cone(&pl,c));
   pPt=Cercle_Droite(&cer,d);
   SetPt (&tabPoint[0],pPt);
   SetPt (&tabPoint[1],(pPt+1));
  }
  return((sPoint*)tabPoint);
}


/*=========================  Cone Cone  ====================*/
sDroite * Cone_Cone (sCone * ca,sCone * cb)
{
 static sDroite tabDroite [2];
 sDroite net;
 sVecteur i,j,k,u,v;
 float a,b,x,y,z;
 SetVec (&i,VecNorme(&ca->u));
 SetVec (&j,VecNorme(&cb->u));
 SetVec (&k,VecNorme(VecVec(&i,&j)));
 a=ca->Agl;
 b=cb->Agl;

 if (a+b<acos(Scal(&i,&j))+MINIP)
    {
     GeomERROR=DISTANT;
     if (fabs(a+b-acos(Scal(&i,&j)))<=MINIP) GeomERROR=TANGENT;

     SetVec(&u,VecVec(&k,&i));
     SetVec(&v,VecK(cos(a),&i));
     SetVec(&v,VecPlus(&v,VecK(sin(a),&u)));
     SetDtCo (&tabDroite[1],&ca->S,&v);

     SetVec(&u,VecVec(&k,&j));
     SetVec(&v,VecK(cos(a),&j));
     SetVec(&v,VecPlus(&v,VecK(-sin(a),&u)));
     SetDtCo (&tabDroite[2],&cb->S,&v);

     return ((sDroite*)tabDroite);
    }

 GeomERROR=OK;

 x=(cos(a)-cos(b)*Scal(&i,&j))/(1-Scal(&i,&j)*Scal(&i,&j));
 y=(cos(b)-cos(a)*Scal(&i,&j))/(1-Scal(&i,&j)*Scal(&i,&j));
 z=sqrt(1-x*x-y*y-2*x*y*Scal(&i,&j));
 SetVec (&u,VecK(x,&i));
 SetVec (&u,VecPlus(&u,VecK(y,&j)));
 SetVec (&v,VecMoins(&u,VecK(z,&k)));
 SetDtCo(&net,&ca->S,&v);
 SetDt  (&tabDroite[0],&net);

 SetVec (&v,VecPlus(&u,VecK(z,&k)));
 SetDtCo(&net,&ca->S,&v);
 SetDt  (&tabDroite[1],&net);
 return ((sDroite*)tabDroite);
}

/*=========================  Cercle Droite==================*/
sPoint  * Cercle_Droite (sCercle *c,sDroite *d)
{
 static sPoint tabPoint[2];
 float  al,be,m,n,u,v,w,x1,x2,y1,y2;
 float a,b;
 float temp;
 sPoint tempPt;
 sVecteur cm;

 al=Scal(&d->u,&c->i);
 be=Scal(&d->u,&c->j);
 m=Scal(VecAB(&c->C,&d->A),&c->i);
 n=Scal(VecAB(&c->C,&d->A),&c->j);
 a=c->a;
 b=c->b;


 if (fabs(al)<MINI)
    {
     if (fabs(m)>a)
	GeomERROR=DISTANT;
     else
       {
	x1=m;
	x2=m;
	y1=b*sqrt(1-m*m/a/a);
	y2=-y1;
	GeomERROR=OK;
       }
    }
 else if (fabs(be)<MINI)
    {
     if (fabs(n)>b)
	GeomERROR=DISTANT;
     else
       {
	y1=n;
	y2=n;
	x1=a*sqrt(1-n*n/b/b);
	x2=-x1;
	GeomERROR=OK;
       }
    }
 else
    {
     u=1+(a*a*be*be)/(b*b*al*al);
     v=2*(a*a*be)/(b*b*al*al)*(al*n-be*m);
     w=(al*n-be*m);
     w=w*w*a*a/(b*b*al*al)-a*a;
     Eq2dDegre (u,v,w,&x1,&x2);
     if (GeomERROR==DETNEG)
	 GeomERROR=DISTANT;
     else
	{
	 u=1+(b*b*al*al)/(a*a*be*be);
	 v=2*(b*b*al)/(a*a*be*be)*(be*m-al*n);
	 w=(be*m-al*n);
	 w=w*w*b*b/(a*a*be*be)-b*b;
	 Eq2dDegre (u,v,w,&y1,&y2);
	 if (GeomERROR==DETNEG)
	    GeomERROR=DISTANT;
	 else
	    GeomERROR=OK;
	}
     if (GeomERROR==OK)
	{
	 temp=be*x1-al*y1-be*m+al*n;
	 if (fabs(temp)>MINI)
	    {
	     temp=x1;
	     x1=x2;
	     x2=temp;
	    }
	}
    }
 if (GeomERROR==OK)
  {
   SetVec(&cm,VecK(x1,&c->i));
   SetVec(&cm,VecPlus(&cm,VecK(y1,&c->j)));
   SetPt (&tabPoint[0],PtVec(&c->C,&cm));

   SetVec(&cm,VecK(x2,&c->i));
   SetVec(&cm,VecPlus(&cm,VecK(y2,&c->j)));
   SetPt (&tabPoint[1],PtVec(&c->C,&cm));

   a=Scal(&d->u,VecAB(&d->A,&tabPoint[0]));
   b=Scal(&d->u,VecAB(&d->A,&tabPoint[1]));
   if ( (((a*b)>0)&&(a>b)) || (((a*b)<0)&&(b>a)) )
    {
     SetPt(&tempPt,&tabPoint[0]);
     SetPt(&tabPoint[0],&tabPoint[1]);
     SetPt(&tabPoint[1],&tempPt);
    }
  }
 return ((sPoint*)tabPoint);
}


/*=========================  Plan_Cylindre ==================*/
sCercle * Plan_Cylindre (sPlan * pl,sCylindre * cy)
	{
	 static sCercle result;
	 sDroite dt;
	 float d;

	 SetDtCo (&dt,&cy->p,&cy->u);
	 SetPt (&result.C,Plan_Droite(pl,&dt));

	 if (GeomERROR!=OK) return(&result);

	 d=Scal(&pl->u,&cy->u)/(Norme(&pl->u)*Norme(&cy->u));
	 if (fabs(d)>1-MINIP)
	    {
	     if (d>0)
		SetCerCoCer (&result,&result.C,&cy->u,cy->r);
	     else
		SetCerCoCer (&result,&result.C,VecK(-1,&cy->u),cy->r);

	     return (&result);
	    }

	 d=fabs(d);

	 SetVec(&result.i,VecNorme(VecK(-1,VecPro(&cy->u,&pl->u))));
	 SetVec(&result.j,VecNorme(VecVec(&pl->u,&result.i)));
	 SetVec(&result.k,VecNorme(&pl->u));

	 result.a=(cy->r)/(d);
	 result.b=cy->r;
	 result.cercle=0;

	 return(&result);
}


/*============================================================
==============================================================
	fonctions diverses
============================================================*/

/* resolution d'une equation du second degre ax^2+bx+c=0		*/
void Eq2dDegre(float a,float b,float c,float* r1,float* r2)
{
 float d;
 d=b*b-4*(a*c);
 if (d<0)
    {
     GeomERROR=DETNEG;
     *r1=-b/2/a;
     *r2=sqrt(-d);
    }
 else
    {
     GeomERROR=OK;
     *r1=(-b-sqrt(d))/2/a;
     *r2=(-b+sqrt(d))/2/a;
    }
 if(fabs(*r1)<MINI) *r1=0;
 if(fabs(*r2)<MINI) *r2=0;
}


/* fonctions d'arrondissement d'un reel en entier			*/
int rnd (float x)
	{
	 double i,j;
	 modf ((double)x,&i);
	 if (i<=0.5) return (floor((double)x));
	 else return (ceil((double)x));
	}
