/*======================================================================*/
/*============  routines de traitement des donnees sur fichier  ========*/
/*======================================================================*/


#define INEXISTANTE	1
#define NONOUVRABLE	2

#define LECTURE		TRUE
#define ECRITURE        FALSE
#define PRECIT		2


typedef struct
	{
	 char  Nom[128];
	 char  AncienFichier[128];
	 char  NouveauFichier[128];
	 FILE* Ancien;
	 FILE* Nouveau;
	 char* Buffer;
	 int   Taille;
	 boolean Modif;
	 boolean Lecture;
	} sSection;

/* fonctions de lecture d'un objet dans une chaine de carateres */
float		ReelLit(char** chaine, char* sep);
sPoint* 	PtLit  (char** chaine);
sVecteur*	VecLit (char** chaine);
sDroite*	DtLit  (char** chaine);
sPlan*		PlLit  (char** chaine);
sCercle* 	CerLit (char** chaine);
sCylindre*	CyLit  (char** chaine);
sCone*		CoLit  (char** chaine);
sSphere*	SphLit (char** chaine);
sRepere*	RepLit (char** chaine);


/* fonctions d'ecriture d'objet dans une chaine de caracteres */
char* EcritPt  (sPoint* P, char* chaine);
char* EcritVec (sVecteur* v, char* chaine);
char* EcritDt  (sDroite* d, char* chaine);
char* EcritPl  (sPlan* pl, char* chaine);
char* EcritCer (sCercle* c,char* chaine);
char* EcritCy  (sCylindre *c, char* chaine);
char* EcritCo  (sCone* c, char* chaine);
char* EcritSph (sSphere* s, char* chaine);
char* EcritRep (sRepere* r, char* chaine);
char* EcritNd  (sNoeud* n, char* chaine);
char* EcritMa  (sMaille* m, char* chaine);


/* fonctions de gestions des fichiers de donnees */
int	InitSection (sSection* s,char* nom,boolean mode,char* nouveaunom);
int	OuvreSection (sSection* s,char* nom);
void   	FermeSection (sSection* s);
char *	ChercheInfo  (sSection* s,char* clef);


sPoint* 	PtSection (sSection *s,char* nom);
sVecteur* 	VecSection (sSection *s,char* nom);
sDroite*	DtSection  (sSection *s,char* nom);
sPlan* 		PlSection (sSection *s,char* nom);
sCercle* 	CerSection (sSection *s,char* nom);
sCylindre* 	CySection (sSection *s,char* nom);
sCone*	 	CoSection (sSection *s,char* nom);
sSphere* 	SphSection (sSection *s,char* nom);
sRepere*	RepSection (sSection *s,char* nom);


/* fonctions de creation/modifications d'un fichier de donnees */
void	CreeSection (sSection* s,char* nom,char* commentaires);
void	CreeInfo    (sSection* s,char* clef,char* info,char* commentaires);
void	CreeComment (sSection* s,char* commentaires);
void	CopySection (sSection* s,char* nom,char* clef_fin);
void	CopyInfos   (sSection* s,char* section, char* debut, char* fin);


void	CreePt	(sSection* s, sPoint* p, char* nom, char* commentaire);
void	CreeVec	(sSection* s, sVecteur* v, char* nom, char* commentaire);
void	CreeDt	(sSection* s, sDroite* d, char* nom, char* commentaire);
void	CreePl	(sSection* s, sPlan*  p, char* nom, char* commentaire);
void	CreeCer	(sSection* s, sCercle* c, char* nom, char* commentaire);
void	CreeCy	(sSection* s, sCylindre* c, char* nom, char* commentaire);
void	CreeCo	(sSection* s, sCone* c, char* nom, char* commentaire);
void	CreeSph	(sSection* s, sSphere* sp, char* nom, char* commentaire);
void	CreeRep	(sSection* s, sRepere* r, char* nom, char* commentaire);
void	CreeNd	(sSection* s, sNoeud* n, char* nom, char* commentaire);
void	CreeMa	(sSection* s, sMaille* m, char* nom, char* commentaire);


