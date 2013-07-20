#define TAILLELIGNE	200

extern char* Ligne;
extern char* Mot;
extern char* LigneTravail;

char* LitLigne ();
char* PremierMot (char* sep);
char* MotSuivant (char* sep);
char* MotLigneSuivant (char* sep);
int   Change (char* chaine,char ancien,char nouveau);
char* Cherche(char* texte,char* sep);
int   Ouvre (char* nom);
void  Ferme (void);