#if !defined(_LITTEXT_H)
#define _LITTEXT_H


#define TAILLELIGNE	90

#define INEXISTANT	-1
#define MODEDIRECT	0
#define MODEINDIRECTE	-1

extern int LITTEXT_Error;
extern int LITTEXT_ErrorMode;


extern char* Ligne;
extern char* Mot;
extern char* LigneTravail;

char* LitLigne (void);
char* PremierMot (char* sep);
char* MotSuivant (char* sep);
char* MotLigneSuivant (char* sep);
int   Change (char* chaine,char ancien,char nouveau);
char* Cherche(char* texte,char* sep);

int   Ouvre (char* nom);
void  Ferme (void);



#endif