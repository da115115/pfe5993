/* ensemble de stuctures et de fonctions avancees associees a GEOM.C 	*/


#define COURBE_ELEM_MAX	5
#define MAX_DIMENSION	5	/* nombre de dimensions maxi d'un tableau
				   de points				*/


/*========================================================================
			Structure Courbe		    		*/

typedef struct {
		int*  ou [COURBE_ELEM_MAX];
		int   qui[COURBE_ELEM_MAX];
		float taille[COURBE_ELEM_MAX];
		float totale;
		int   nbElem;
	       } sCourbe;


/*========================================================================
			Structure Spline				*/

typedef struct {
		sRepere R;
		float   coeff[2][3];
		float   longeur;
	       } sSpline;


/*========================================================================
			Structure Echelle				*/

typedef struct {
		int   nb;	/* nombre d'elements			*/
		float p0;	/* taille premier element		*/
		float q;        /* raison arithmetique			*/
	       } sEchelle;


/*========================================================================
			Structure Quadri				*/

typedef struct {
	       sPoint* tab;	/* pointeur sur le tableau de point
				   decrivant la surface			*/
	       int lx;		/* lx et ly: largeur et hauteur en 	*/
	       int ly;		/* nombre de segments			*/
	      } sQuadri;


/*========================================================================
			Structure Tabdynamique				*/

typedef struct {
		void * tab;	/* pointeur sur la zone de memoire	*/
		int dimension[MAX_DIMENSION];
				/* dimensions du tableau		*/
		int 	nbdimension;
				/* dimension du tableau			*/
		void* (*e) (int, ...);
		sPoint* (*p) (int, ...);
		sNoeud* (*n) (int, ...);
		sMaille* (*m) (int, ...);
	       } sTabDynamique;



/*========================================================================
			fonctions de sCourbe		    		*/

void CourbeInit	 (sCourbe*);
void CourbeAddDt (sCourbe*,sDroite*);
void CourbeAddCer(sCourbe*,sCercle*);
sPoint* PtCourbe (sCourbe* cour,float n);


/*========================================================================
			fonctions Spline				*/

int SplineInit (sSpline* s,sPoint* A,sVecteur* ua,sPoint* B,sVecteur* ub);
sPoint* PtSpline (sSpline* s,float n);


/*========================================================================
			fonctions d'echelle		    		*/

void   EchelleInit(sEchelle*,int,float);
float  EcPos      (sEchelle*,int);


/*========================================================================
			fonctions de quadrillage	    		*/

void    QuadriInit   (sQuadri*,sPoint*,int,int);
sPoint* PtQuadri     (sQuadri*,float,float);
sPoint* SitePtQuadri (sPoint *p,sQuadri *q);


/*========================================================================
			fonctions du tableau dynamique    		*/

void 	TabDel  	(sTabDynamique*);

void    TabPtInit 	(sTabDynamique* t,int nbdimension, ...);
void 	TabPtFocus 	(sTabDynamique*);
sPoint* PtTab 		(sTabDynamique* t, ...);

void    TabNdInit 	(sTabDynamique* t,int nbdimension, ...);
void 	TabNdFocus 	(sTabDynamique*);
sNoeud* NdTab 		(sTabDynamique* t, ...);

void     TabMaInit 	(sTabDynamique* t,int nbdimension, ...);
void 	 TabMaFocus 	(sTabDynamique*);
sMaille* MaTab 		(sTabDynamique* t, ...);


/*========================================================================
			autre fonctions					*/
float Fondu	(float milieu,float pos);
int   Num_Pos (int i,int j,int k,int J,int K);
