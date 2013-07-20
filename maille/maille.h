/* C“t‚ gauche ou c“t‚ droit */

extern int COTE;
extern int COTYLE;
extern int CORTICAL,SPONGIEUX;

/* noms de fichiers */
extern char    cFormes[80];
extern char    cPoints[80];
extern char    cFinal [80];
extern char    cDefaut[80];


/*-----------------------  Tableau du maillage  ---------------------------*/

extern sTabDynamique Noeud;
extern sTabDynamique Maille;

extern int NbNoeud;
extern int NbMaille;



/*----------------------  Tableaux d'echanges  ----------------------------*/
extern sTabDynamique Iliaque_Cotyle;
extern sNoeud Sacro_Iliaque[3][6];



/*------------------------  Variables globales  ---------------------------*/

extern sPoint* Pt_IlEpineAnteroSup;	/* epine iliaque antero posterieure*/
extern sPoint* Pt_IlEpinePostSup;       /* epine iliaque postero superieure*/
extern sPoint* Pt_IlEpinePostInf;	/* epine iliaque postero inferieure*/
extern sPoint* Pt_IlMedianeAnteroSup;   /* point sup de la ligne mediane   */
extern sPoint* Pt_IlMedioAnt;           /* point le plus elev‚ de la face
							 anterieure        */
extern sPoint* Pt_SacreAntInf;          /* point antero-inf. de l'articula-
					   -tion sacro-iliaque		   */
extern sPoint* Pt_CoCentre;             /* centre du cotyle                */

extern sPoint* Pt_CoSup;		/* Point superieur du bord du cotyle*/

extern sPoint* Pt_CorneAnt;		/* point corne anterieure cotyle   */

extern sPoint* Pt_CornePost;		/* point corne posterieure cotyle  */


extern sCercle* Ce_Pelvien;  /* Cercle pelvien contenu dans le plan pelvien*/
extern sCercle* Ce_Cotyle2;  /* Cercle cotylo‹dien */

extern sPlan* Pl_Pelvien;       /* plan pelvien de r‚f‚rence            */
extern sPlan* Pl_Pub; 		/* plan pelvien donnant la ligne arqu‚e */
extern sPlan* Pl_Scan;		/* plan des coupes scannaires           */
extern sPlan* Pl_CoBord;        /* plan determinant le bord du cotyle   */
extern sPlan* Pl_CoFond;        /* plan de fond de cotyle               */


extern sCone* Co_IlAnt;         /* Cone anterieur                       */
extern sCone* Co_IlPost;        /* cone posterieur                      */


extern sSphere *Sp_IlFosse;     /* sphere de la fosse iliaque           */
extern sSphere *Sp_Cotyle;      /* sphere du cotyle                     */
extern sSphere *Sp_CoFond;	/* sphere du fond du cotyle		*/

extern sCylindre *Cy_Pelvien;	/* Cylindre pelvien			*/


  /*------------------  structures d'echanges de l'iliaque  ------------*/

extern sPoint*  Pt_IlioPelvien;	/* point projection de Pt_CoSup
					   sur le cylindre pelvien 			*/

extern sPlan*   Pl_Sacre;  	/* plan de l'articulation sacro-iliaque */

extern sDroite* Dt_Mediale;    	/* droite mediale de l'iliaque          */
extern sDroite* Dt_IlAnteroInt;	/* droite des epines anterieures	*/
extern sDroite* Dt_IlAnteroInf;	/* droite de raccordement avec la branche
				   pubienne 				*/
extern sCercle* Ce_IlAnteroInf;	/* portion d'ellipse inferieure 	*/
