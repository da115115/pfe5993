/* module de definitions de fenetres et fenetre 3D permettant
   l'affichage et l'animation de tableaux de Noeuds ou de Points	*/


#define SIMPLE	0	/* fenetre sans attribus particuliers		*/
#define BORDURE	1	/* fenetre avec bordure				*/
#define ANIMATION 2	/* fenetre avec viewport plus petit		*/
#define CONTROLE   3	/* fenetre d'anim. de controle d'une fenetre anim*/

#define MAXDATAG  40	/* nombre de tableaux d'objets affichables dans */
			/* une fenetre graphique			*/

#define MAXANIME  10	/* nombre maximum de fenetre dans une suite	*/
#define MAXMAITRE 5	/* nombre de fenetre maitre pour l'animation	*/



/*========================= structure sDataG   =========================*/

typedef void (far *tGFonc) (void*, void*,int); /* fonction d'affichage	*/
					       /* d'un objet		*/


typedef struct
	{
	 void*  Tab;
	 int    Taille;
	 int    NbObjets;
	 int    Couleur;
	 tGFonc AfficheObjet;
	} sDataG;



/*========================= structure sFenetre  ========================*/

typedef struct
	{
	 int x,y,lx,ly;
	 int style;
	} sFenetre;


/*=================== structure fenetre texte sFenetreT ================*/

typedef struct
	{
	 sFenetre fenetre;
	 int texte;
	 int question;
	} sFenetreT;


/*========================= structure sFenetre3D  ======================*/

typedef struct
	{
	 sFenetre fenetre;
	 sPoint   O;
	 sPoint   focus;
	 sVecteur i,k;
	 float    angle;
	 sDataG   data[MAXDATAG];
	 int      nbdata;
	 int      (far *GetXY) (void* fen, sPoint *P, float *X, float *Y);
	} sFenetre3D;


/*==================== structure sFenetreA d'animation==================*/

typedef struct
	{
	 sFenetre3D fenD;
	 sFenetreT  fenT;
	 char titre[80];
	 float VitesseTrans[2];
	 float VitesseRot  [2];
	 int Vitesse;

	 void* fenEsclave;
	 void* fenMaitre[MAXMAITRE];
	 int   nbmaitre;
	} sFenetreA;


/*================= structure suite de fenetre d'animation==============*/

typedef struct
	{
	 sFenetreA* fenetre[MAXANIME];
	 int nbfenetre;
	 int courante;
	} sFenASuite;



/*======================================================================*/
/*			fonctions DataG					*/


void SetDataGCo (sDataG* d, void* t, int n, int tail,int coul,void far *fonc);
void SetDataGCoPt  (sDataG* d, void* t, int n, int coul);
void SetDataGCoNd  (sDataG* d, void* t, int n, int coul);
void SetDataGCoDt  (sDataG* d, void* t, int n, int coul);
void SetDataGCoCer (sDataG* d, void* t, int n, int coul);
void SetDataGCoSph (sDataG* d, void* t, int n, int coul);
void SetDataGCoCy (sDataG* d, void* t, int n, int coul);
void SetDataGCoCo (sDataG* d, void* t, int n, int coul);

void SetDataG (sDataG* A, sDataG* B);

void DataGAffiche (sDataG* d,sFenetre3D* Fen3D);



/*======================================================================*/
/*			fonctions fenetre				*/

void SetFenCo (sFenetre* fenetre,int x,int y,int lx, int ly);
void FenFocus (sFenetre* fen);
void FenLostFocus (sFenetre *fen);
void SetFen   (sFenetre* fenA,sFenetre* fenB);
void FenStyle (sFenetre *fen,int style);


/*======================================================================*/
/*			fonctions fenetre texte				*/

void SetFenTCo (sFenetreT* fenT,sFenetre* fen);
void FenTFocus (sFenetreT* fenT);
void FenTSet   (sFenetreT* fenT,char* texte);


/*======================================================================*/
/*			fonctions fenetre 3D				*/

void SetFen3DCo   (sFenetre3D* fen3D,sFenetre* fenetre,
		   sPoint* O,sVecteur* u,float angle);

void SetFen3DNdCo (sFenetre3D* fen3D,sFenetre* fenetre,
		   sPoint* O,sVecteur* u,float angle,sNoeud* tab,int nb);
void SetFen3DPtCo (sFenetre3D *fen3D,sFenetre* fenetre,
		   sPoint* O,sVecteur* u,float angle,sPoint* tab,int nb);


void SetFen3D	    (sFenetre3D* fena,sFenetre3D* fenb);

void Fen3DAddPt  (sFenetre3D* fen, sPoint* tab, int n, int coul);
void Fen3DAddNd  (sFenetre3D* fen, sNoeud* tab, int n, int coul);
void Fen3DAddDt  (sFenetre3D* fen, sDroite*tab, int n, int coul);
void Fen3DAddCer (sFenetre3D* fen, sCercle*tab, int n, int coul);
void Fen3DAddSph (sFenetre3D* fen, sSphere*tab, int n, int coul);
void Fen3DAddCy (sFenetre3D* fen, sCylindre* tab, int n, int coul);
void Fen3DAddCo (sFenetre3D* fen, sCone* tab, int n, int coul);

void Fen3DDataChange (sFenetre3D *fen,void *quoi,void *par,int nb,int coul);

void Fen3DFocus     (sFenetre3D* fenetre);
void Fen3DLostFocus (sFenetre3D* fen);
void Fen3DStyle     (sFenetre3D *fen,int style);
void Fen3DShow      (sFenetre3D* fen);

void far Fen3DPtAffiche  (sFenetre3D* fen, sPoint*  p, int coul);
void far Fen3DNdAffiche  (sFenetre3D* fen, sNoeud*  N, int coul);
void far Fen3DDtAffiche  (sFenetre3D* fen, sDroite* d, int coul);
void far Fen3DCerAffiche (sFenetre3D* fen, sCercle* c, int coul);
void far Fen3DSphAffiche (sFenetre3D* fen, sSphere* s, int coul);
void far Fen3DCyAffiche (sFenetre3D* fen, sCylindre* c, int coul);
void far Fen3DCoAffiche (sFenetre3D* fen, sCone* c, int coul);

int far Fen3DGetXYAxono (sFenetre3D* fen, sPoint* P, float* X, float* Y);

/*======================================================================*/
/*			fonctions fenetre d'animation			*/

void SetFenACo 		(sFenetreA* fenA,sFenetre3D* fenD,char* titre);
void FenAFocus 		(sFenetreA* fen);
void FenAShow  		(sFenetreA* fen);
char FenAnime  		(sFenetreA* fen);
char FenAAligne		(sFenetreA* fena,sVecteur *u);
char FenAZoom  		(sFenetreA* fena,float sens);
char FenAMoveFocus 	(sFenetreA* fena,char sens);
char FenARotate		(sFenetreA* fena,char sens);
char FenATourne		(sFenetreA* fena,char sens);
char FenASetFocus 	(sFenetreA* fena,sPoint* p);

/*	    fonctions specifiques aux fenetres de controle		*/
void SetFenControleCo (sFenetreA* controle,sFenetreA* esclave);




/*======================================================================*/
/*			fonctions suite de fenetres			*/

void SuiteInit (sFenASuite* suite);
void SuiteAnime (sFenASuite* suite);
void SuiteAdd   (sFenASuite* suite,sFenetreA* fena);

void SuiteAddPt  (sFenASuite* suite, sPoint* P, int nb, int coul);
void SuiteAddNd  (sFenASuite* suite, sNoeud* N, int nb,int coul);
void SuiteAddDt  (sFenASuite* suite, sDroite*d, int nb,int coul);
void SuiteAddCer (sFenASuite* suite, sCercle*c, int nb,int coul);
void SuiteAddSph (sFenASuite* suite, sSphere*s, int nb,int coul);
void SuiteAddCy (sFenASuite* suite, sCylindre* s, int nb,int coul);
void SuiteAddCo (sFenASuite* suite, sCone* s, int nb,int coul);

void SuiteChange (sFenASuite* suite, void* quoi, void* par, int nb,int coul);




/*======================================================================*/
/*			fonctions d'affichage globale			*/

void InitEcran (void);
void FermeEcran(void);

void EcranAddPt (sPoint*  p, int nb, int coul);
void EcranAddNd (sNoeud*  n, int nb, int coul);
void EcranAddDt (sDroite* d, int nb, int coul);
void EcranAddCer(sCercle* c, int nb, int coul);
void EcranAddSph(sSphere* s, int nb, int coul);
void EcranAddCy (sCylindre* s, int nb, int coul);
void EcranAddCo (sCone* s, int nb, int coul);

void EcranChange (void* quoi, void* par, int nb, int coul);

void NdAffiche (sNoeud* Noeud,int NbNoeud);
void PtAffiche (sPoint* Point,int NbPoint);


/*======================================================================*/
/*			Variables externes				*/

extern sFenASuite  Show_SuiteEcran;   /* suite de l'affichage par defaut*/
