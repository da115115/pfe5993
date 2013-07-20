/*======================================================================*/
/*			 affichage des courbes				*/

void SetDataGCoCourbe   (sDataG* d, void* t, int n, int coul);
void Fen3DAddCourbe     (sFenetre3D* fen, sCourbe*tab, int n, int coul);
void far Fen3DCourbeAffiche (sFenetre3D* fen, sCourbe* c, int coul);
void SuiteAddCourbe     (sFenASuite* suite, sCourbe*s, int nb,int coul);
void EcranAddCourbe     (sCourbe* s, int nb, int coul);


/*======================================================================*/
/*			 affichage des quadrillages			*/

void SetDataGCoQuadri   (sDataG* d, void* t, int n, int coul);
void Fen3DAddQuadri (sFenetre3D* fen, sQuadri*tab, int n, int coul);
void far Fen3DQuadriAffiche (sFenetre3D* fen, sQuadri* c, int coul);;
void SuiteAddQuadri     (sFenASuite* suite, sQuadri*s, int nb,int coul);
void EcranAddQuadri     (sQuadri* s, int nb, int coul);
