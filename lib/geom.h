#if !defined(_GEOM_H)
#define _GEOM_H


#define PI   	3.141562654
#define MINI 	1e-5    	/* difference mini entre 2 reels pour qu'ils
				   ne soient pas consideres egaux	*/
#define MINIP   .01		/* Pourcentage mini			*/


#define INTERNE -1
#define SUR     0
#define EXTERNE 1
#define GAUCHE  2
#define DROITE  3

#define OK      0
#define ERREURE 1
#define COUPE   2
#define DISTANT 3	/* l'intersection n'existe pas		*/
#define PARALLELE 4	/* les objets sont paralleles		*/
#define TANGENT   5	/* les objets sont tangents		*/
#define DETNEG    6	/* determinant de l'equation du 2d degre negatif */


enum tagboolean {FALSE, TRUE};
typedef enum tagboolean boolean;

/*============================================================
			Structure sPoint                    	*/
typedef struct {
		float x;
		float y;
		float z;
		 } sPoint;
typedef sPoint * pPoint;

/*============================================================
			Structure sVecteur                  	*/
typedef struct {
		float u,v,w;
		 } sVecteur;
typedef sVecteur * pVecteur;

/*============================================================
			Structure sPlan                     	*/
typedef struct {
		sPoint P;
		sVecteur u;
		 } sPlan;
typedef sPlan * pPlan;

/*============================================================
			Structure sSphere                   	*/
typedef struct {
		sPoint C;
		float R;
		 } sSphere;

/*============================================================
			Structure sCone                     	*/
typedef struct {
		sPoint S;
		sVecteur u;
		float Agl;
		sVecteur a,b;
		 }sCone;

/*============================================================
			Structure sCercle                   	*/
typedef struct {
		sPoint   C;
		sVecteur i,j,k;
		float    a,b;
		float    alpha,beta;
		int      cercle;
		 }sCercle;
typedef sCercle* pCercle;

/*========================================================================
			Structure sDroite                   		*/
typedef struct {
		sPoint A,B;
		sVecteur u;
		int segment;
		 } sDroite;

/*========================================================================
			Structure repere		    	     	*/
typedef struct {
		sVecteur i,j,k;
		sPoint   O;
		 } sRepere;

/*========================================================================
			Structure Cylindre                              */

typedef struct {
		sPoint p;
		sVecteur u;
		float r;
	       } sCylindre;


/*========================================================================
			Structure Noeud				    	*/
typedef struct {
		sPoint P;
		int  num;
		 } sNoeud;

/*========================================================================
			Structure Maille		    		*/
typedef struct {
		int P[8];
		int materiau;
		 } sMaille;



/************************************************************************/
/*========================================================================
			fonctions sPoint          	          	*/

void SetPtCo (sPoint *P,float i,float j,float k);
void SetPt (sPoint *P,sPoint *Q);
sPoint * Pt (float a,float b,float c);
sPoint * PtVec (sPoint *P,sVecteur *u);
sPoint * PtPro (sPoint *m,sPlan *p);
sPoint * PtRepNew (sPoint *p,sRepere *new);
sPoint * PtRepOld (sPoint *p,sRepere *old);
sPoint * PtCerAngle (sCercle *c,float angle);
sPoint * PtCer	  (sCercle *c,float n);
sPoint * PtDt	  (sDroite *d,float n);
sPoint * PtTanCer (sPoint *p,sCercle *c,int pos);
sPoint * PtSymPlan (sPoint *m, sPlan *p);


/*========================================================================
			fonctions sVecteur                  		*/

void SetVecCo (sVecteur *V,float i,float j,float k);
void SetVec (sVecteur *u,sVecteur *v);
sVecteur * Vec (float a,float b,float c);
sVecteur * VecAB (sPoint *A,sPoint *B);
sVecteur * VecK (float k,sVecteur *u);
sVecteur * VecdK (float k,sVecteur *u);
sVecteur * VecVec (sVecteur *u,sVecteur *v);
sVecteur * VecPlus (sVecteur *u,sVecteur *v);
sVecteur * VecMoins (sVecteur *u,sVecteur *v);
sVecteur * VecNorme (sVecteur *u);
sVecteur * VecPro (sVecteur *u,sVecteur *v);
sVecteur * VecRepNew (sVecteur *u,sRepere *new);
sVecteur * VecRepOld (sVecteur *u,sRepere *old);
sVecteur * VecRot    (sVecteur *u,sVecteur *v,float a);

float Scal (sVecteur *u,sVecteur *v);
float Angle (sVecteur *a,sVecteur *b,sVecteur *c);
float Norme (sVecteur *u);

/*============================================================
			fonctions sPlan                     	*/

void SetPlCo(sPlan *pl,sPoint *P,sVecteur *u);
void SetPl (sPlan *P,sPlan *Q);
sPlan *Pl    (sPoint* p, sVecteur *u);
sPlan *PlPt3 (sPoint *A,sPoint *B,sPoint *C);
sPlan *PlPara (sPlan *pl,sPoint *M);
sPlan *PlParaDi (sPlan *pl,float d);

/*============================================================
			fonctions sSphere                   	*/

void SetSphCo (sSphere *Sp,sPoint *P,float r);
void SetSph (sSphere *S,sSphere *T);
sSphere* SphPt4 (sPoint* A, sPoint* B, sPoint* C, sPoint *D);

/*============================================================
			fonctions sCone                     	*/

void SetCoCo ( sCone *co,sPoint *pt,sVecteur *v,float a);
void SetCo (sCone *c, sCone *d);
sCone * CoCutPt (sCone *co, sPoint *A,sPoint *B);

/*============================================================
			fonctions sCercle                   	*/

void SetCerCoCer (sCercle *cer,sPoint *p,sVecteur *v,float r);
void SetCer ( sCercle *Cer1, sCercle *Cer2);
sCercle * CerPt3 (sPoint *a,sPoint*b, sPoint *c);
void 	  InitDebutCer (sCercle *ce,sPoint *A);
sCercle * CerCutPt (sCercle *ce, sPoint *A,sPoint *B);
float	  CerLong  (sCercle*);

/*============================================================
			fonctions sDroite                   	*/

void SetDtCo (sDroite *dt,sPoint *p,sVecteur *v);
void SetDt (sDroite *d,sDroite *e);
sDroite * Dt (sPoint *p,sVecteur *v);
sDroite * DtPt (sPoint*,sPoint*);
sDroite * DtPro (sDroite * d,sPlan *p);
sDroite * DtCutPt (sDroite *d,sPoint *a,sPoint *b);
float 	  DtLong  (sDroite*);

/*============================================================
			fonctions sCylindre                     */

void SetCyCo (sCylindre *cy,sPoint *p,sVecteur *u,float r);
void SetCy (sCylindre *a,sCylindre *b);


/*============================================================
			fonctions de sRepere		    		*/

void SetRepCo (sRepere *r,sPoint *p,sVecteur *i,sVecteur *j,sVecteur *k);
void SetRep   (sRepere *r,sRepere *s);


/*============================================================
			fonctions de sNoeud		    		*/

void SetNdCo (sNoeud *n,sPoint *p,int nb);
void SetNd   (sNoeud *n, sNoeud *nn);


/*============================================================
			fonctions de sMaille		    		*/

void SetMaCo (sMaille *m,int a,int b,int c, int d,int e,int f,int g,int h,
			 int mat);
void SetMa   (sMaille *m,sMaille *n);



/************************************************************/
/*============================================================
			fonctions de position               	*/
int   PosPtCo (sPoint *p,sCone *c);
int   PosPtCer(sPoint *p,sCercle *c);
int   PosPtDt (sPoint *p,sDroite *d,sPlan*pl);
int   PosPtPl (sPoint *p,sPlan *pl);
int   PosDtCy (sDroite *d,sCylindre *c);

float DistDtDt (sDroite *a,sDroite *b);
float DistPtDt (sPoint *p,sDroite *d);

float SitePtDt (sPoint *m,sDroite *d);



/************************************************************/
/*============================================================
	fonctions d'intersections de surface            	*/

sCercle * Plan_Cylindre (sPlan *pl, sCylindre *cy);
sCercle * Plan_Sphere (sPlan *pl,sSphere *sp);
sCercle * Plan_Cone (sPlan *pl, sCone *co);
sPoint  * Plan_Droite (sPlan *p,sDroite *d);
sDroite * Plan_Plan (sPlan *p,sPlan *q);
sPoint  * Droite_Droite (sDroite *d,sDroite *e,sPlan *p);
sPoint  * Droite_Cylindre (sDroite* d,sCylindre* c);
sPoint  * Cone_Droite (sCone *c,sDroite *d);
sDroite * Cone_Cone (sCone * ca,sCone *cb);
sPoint  * Cercle_Droite (sCercle *c,sDroite *d);


/************************************************************************/
/*========================================================================
		fonctions diverses                          		*/

/* resolution equation 2d degre ax2+bx+c=0                  		*/
void Eq2dDegre(float a,float b,float c,float* r1,float* r2);

/* arrondissement d'un reel en entier					*/
int rnd (float);


/************************************************************************/
/*========================================================================
		Variables d'erreures                        		*/

extern int GeomERROR;



/*========================================================================
		Variables d'information complementaire			*/

extern int 	GeomInt;
extern float    GeomFloat;


#endif