/*======================================================================*/
/*============  routines de traitement des donnees sur fichier  ========*/
/*======================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "geom.h"
#include "filedata.h"

/*========= inclusion du module de lecture d'un fichier text ===========*/

#include "littext.h"
#include "littext.c"



/*======================================================================*/
/*			       LECTURE D'OBJETS				*/
/*======================================================================*/


float ReelLit (char** chaine,char * sep)
	{
	 char * p;
	 char pt[128];

	 strncpy (pt,*chaine,127);
	 pt[127]='\0';

	 p=strtok(pt,sep);
	 if (p==NULL)
	    {
	     GeomERROR=1;
	     return 0;
	    }

	 GeomERROR=OK;
	 *chaine=*chaine+(p-pt)+strlen(p);
	 return (atof(p));
	}


sPoint * PtLit (char** chaine)
	{
	 static sPoint result;
	 char   pt[128],*p,*pp;
	 char *sep=" \t\n;:=,xXyYzZ";

	 strncpy (pt,*chaine,127);
	 pt[127]='\0';

	 GeomERROR=1;

	 p=strtok(pt,sep);
	 if (p==NULL)
	   {
	    SetPtCo (&result,0,0,0);
	    return &result;
	   }

	 result.x=atof(p);
	 pp=p+strlen(p);

	 p=strtok (NULL,sep);
	 if (p==NULL)
	    {
	     result.y=result.z=0;
	     *chaine=*chaine+(pp-pt);
	     return &result;
	    }

	 result.y=atof(p);
	 pp=p+strlen(p);
	 p=strtok(NULL,sep);
	 if (p==NULL)
	    {
	     result.z=0;
	     *chaine=*chaine+(pp-pt);
	     return &result;
	    }

	 result.z=atof(p);
	 *chaine=*chaine+(p-pt)+strlen(p);
	 GeomERROR=OK;
	 return &result;
	}



sVecteur * VecLit (char** chaine)
	{
	 static sVecteur result;
	 char   pt[128],*p,*pp;
	 char *sep=" \t\n;:=,uUvVwW";

	 strncpy (pt,*chaine,127);
	 pt[127]='\0';

	 GeomERROR=1;
	 p=strtok(pt,sep);
	 if (p==NULL)
	   {
	    SetVecCo (&result,0,0,0);
	    return &result;
	   }

	 result.u=atof(p);
	 pp=p+strlen(p);

	 p=strtok (NULL,sep);
	 if (p==NULL)
	    {
	     result.v=result.w=0;
	     *chaine=*chaine+(pp-pt);
	     return &result;
	    }

	 result.v=atof(p);
	 pp=p+strlen(p);
	 p=strtok(NULL,sep);
	 if (p==NULL)
	    {
	     result.w=0;
	     *chaine=*chaine+(pp-pt);
	     return &result;
	    }

	 result.w=atof(p);
	 *chaine=*chaine+(p-pt)+strlen(p);
	 GeomERROR=OK;
	 return &result;
	}


sDroite* DtLit  (char** chaine)
	{
	 static sDroite result;
	 int i=0;
	 sPoint A,B;
	 sVecteur u;
	 boolean segment=FALSE;

	 do
	 {
	 while (**chaine==' ' || **chaine=='\t') (*chaine)++;
	 switch (**chaine) {
	   case 'a':
	   case 'A': (*chaine)++;
		     SetPt(&A,PtLit(chaine));
		     break;

	   case 'b':
	   case 'B': (*chaine)++;
		     SetPt(&B,PtLit(chaine));
		     segment=TRUE;
		     break;

	   case 'u':
	   case 'U': (*chaine)++;
		     SetVec (&u,VecLit(chaine));
		     break;

	   default : switch (i){
			case 0 : SetPt(&A,PtLit(chaine)); break;
			case 1 : SetVec(&u,VecLit(chaine)); break;
			}
	   }
	 i++;
	 } while (i<2 && GeomERROR==OK && **chaine!='\0');
	 if (i<2) GeomERROR=1;
	 if (GeomERROR!=OK) return (&result);

	 if (segment) SetDt  (&result,DtPt(&A,&B));
	 else	      SetDtCo(&result,&A,&u);
	 return (&result);
	}


sPlan * PlLit (char** chaine)
	{
	 static sPlan result;

	 SetPt (&result.P, PtLit(chaine));
	 if (GeomERROR!=OK) return (&result);
	 SetVec(&result.u, VecNorme(VecLit(chaine)));
	 return (&result);
	}


sCercle* CerLit (char** chaine)
	{
	 static sCercle result;
	 int cont=0;
	 sPoint C;
	 sVecteur i,j,k;
	 float alpha=0,beta=0,a=0,b=0;
	 boolean cercle=TRUE;
	 boolean direct=FALSE;
	 boolean court=FALSE;
	 boolean fi=FALSE;
	 boolean fj=FALSE;


	 SetVecCo (&i,0,0,0);
	 SetVecCo (&j,0,0,0);
	 SetVecCo (&k,0,0,0);

	 do
	 {
	 while (**chaine==' ' || **chaine=='\t') (*chaine)++;
	 switch (**chaine) {
	   case 'C':
	   case 'c': (*chaine)++;
		     SetPt(&C,PtLit(chaine));
		     break;

	   case 'i':
	   case 'I': (*chaine)++;
		     SetVec(&i,VecLit(chaine));
		     direct=TRUE;
		     fi=TRUE;
		     break;

	   case 'j':
	   case 'J': (*chaine)++;
		     SetVec(&j,VecLit(chaine));
		     direct=TRUE;
		     fj=TRUE;
		     break;

	   case 'k':
	   case 'K': (*chaine)++;
		     SetVec(&k,VecLit(chaine));
		     break;


	   case 'A':
	   case 'a': if (*(*chaine+1)=='l' || *(*chaine+1)=='L')
			{
			 alpha=ReelLit(chaine,"aAlLpPhH ,;:=\t\n");
			 direct=TRUE;
			}
		     else
			a=ReelLit(chaine,"aA ,;:=\t\n");
		     break;

	   case 'b':
	   case 'B': if (*(*chaine+1)=='e' || *(*chaine+1)=='E')
			beta=ReelLit(chaine,"bBeEtTaA ,;:=\t\n");
		     else
			{
			 b=ReelLit(chaine,"bB ,;:=\t\n");
			 cercle=FALSE;
			}
		     direct=TRUE;
		     break;


	   default : switch (cont){
			case 0 : SetPt(&C,PtLit(chaine)); break;
			case 1 : SetVec(&k,VecLit(chaine)); break;
			case 2 : a=ReelLit(chaine,"aA ,;:=\t\n");
			}
		court=TRUE;
	   }
	 cont++;
	 } while ( (cont<3 || court==FALSE) && GeomERROR==OK && **chaine!='\0');
	 if (cont<3 && court==TRUE) GeomERROR=1;
	 if (GeomERROR!=OK) return (&result);

	 if (fi) SetVec(&i,VecNorme(&i));
	 if (fj) SetVec(&j,VecNorme(&j));
	 SetVec(&k,VecNorme(&k));

	 if (cercle)
	    {
	     SetCerCoCer (&result,&C,&k,a);
	     if (direct)
	       {
		if (fi && !fj) SetVec(&j,VecVec(&k,&i));
		if (fj && !fi) SetVec(&i,VecVec(&j,&k));
		SetVec (&result.i,&i);
		SetVec (&result.j,&j);
		result.alpha=alpha;
		result.beta=beta;
	       }
	    }
	 else
	  {
	   SetPt (&result.C,&C);
	   if (fi && !fj) SetVec(&j,VecVec(&k,&i));
	   if (fj && !fi) SetVec(&i,VecVec(&j,&k));
	   SetVec(&result.i,&i);
	   SetVec(&result.j,&j);
	   SetVec(&result.k,&k);
	   result.a=a;
	   result.b=b;
	   result.alpha=alpha;
	   result.beta=beta;
	   if (cercle) result.cercle=1;
	   else        result.cercle=0;
	  }

	 return (&result);
	}


sCylindre* CyLit  (char** chaine)
	{
	 static sCylindre result;
	 int i=0;

	 do
	 {
	 while (**chaine==' ' || **chaine=='\t') (*chaine)++;
	 switch (**chaine) {
	   case 'P':
	   case 'p': (*chaine)++;
		     SetPt(&result.p,PtLit(chaine));
		     break;

	   case 'u':
	   case 'U': (*chaine)++;
		     SetVec(&result.u,VecNorme(VecLit(chaine)));
		     break;

	   case 'r':
	   case 'R': result.r=ReelLit(chaine,"rR ,;:=\t\n");
		     break;

	   default : switch (i){
			case 0 : SetPt(&result.p,PtLit(chaine)); break;
			case 1 : SetVec(&result.u,VecNorme(VecLit(chaine)));
				 break;
			default: result.r=ReelLit(chaine," ,;:=\t\n");
			}
	   }
	 i++;
	 } while (i<3 && GeomERROR==OK && **chaine!='\0');
	 if (i<3) GeomERROR=1;
	 return (&result);
	}


sCone* CoLit  (char** chaine)
	{
	 static sCone result;
	 int i=0;

	 do
	 {
	 while (**chaine==' ' || **chaine=='\t') (*chaine)++;
	 switch (**chaine) {
	   case 'S':
	   case 's': (*chaine)++;
		     SetPt(&result.S,PtLit(chaine));
		     break;

	   case 'u':
	   case 'U': (*chaine)++;
		     SetVec(&result.u,VecNorme(VecLit(chaine)));
		     break;

	   case 'A':
	   case 'a': (*chaine)++;
		     result.Agl=ReelLit(chaine,"aAgGlL ,;:=\t\n");
		     break;

	   default : switch (i){
			case 0 : SetPt(&result.S,PtLit(chaine)); break;
			case 1 : SetVec(&result.u,VecNorme(VecLit(chaine)));
				 break;
			default: result.Agl=ReelLit(chaine," ,;:=\t\n");
			}
	   }
	 while (i<2 && (**chaine=='\t' || **chaine=='\n' || **chaine==' '))
	    (*chaine)++;
	 i++;
	 } while (i<3 && GeomERROR==OK && **chaine!='\0');
	 if (i<3) GeomERROR=1;
	 return (&result);
	}



sSphere * SphLit (char** chaine)
	{
	 static sSphere result;

	 SetPt (&result.C, PtLit(chaine));
	 if (GeomERROR!=OK) return (&result);
	 result.R=ReelLit(chaine,(char*)" ,;:=rR\t\n");
	 return (&result);
	}


sRepere* RepLit (char** chaine)
	{
	 static sRepere result;
	 int i=0;

	 SetRepCo(&result,Pt(0,0,0),Vec(0,0,0),Vec(0,0,0),Vec(0,0,0));

	 do
	 {
	 while (**chaine==' ' || **chaine=='\t') (*chaine)++;
	 switch (**chaine) {
	   case 'O':
	   case 'o': (*chaine)++;
		     SetPt(&result.O,PtLit(chaine));
		     break;

	   case 'i':
	   case 'I': (*chaine)++;
		     SetVec(&result.i,VecNorme(VecLit(chaine)));
		     break;

	   case 'j':
	   case 'J': (*chaine)++;
		     SetVec(&result.j,VecNorme(VecLit(chaine)));
		     break;

	   case 'k':
	   case 'K': (*chaine)++;
		     SetVec(&result.k,VecNorme(VecLit(chaine)));
		     break;

	   default : switch (i){
			case 0 : SetPt(&result.O,PtLit(chaine)); break;
			case 1 : SetVec(&result.i,VecNorme(VecLit(chaine)));
				 break;
			case 2 : SetVec(&result.j,VecNorme(VecLit(chaine)));
				 break;
			case 3 : SetVec(&result.k,VecNorme(VecLit(chaine)));
				 break;
			}
	   }
	 i++;
	 } while (i<4 && GeomERROR==OK && **chaine!='\0');
	 return (&result);
	}




/*======================================================================*/
/*			       ECRITURES D'OBJETS		   	*/
/*======================================================================*/

char* EcritPt (sPoint* P, char* chaine)
	{
	 char pt[128];
	 sprintf (pt,"x=%f\t y=%f\t z=%f",P->x,P->y,P->z);
	 strcpy  (chaine,pt);
	 return (chaine);
	}


char* EcritVec (sVecteur* v, char* chaine)
	{
	 char pt[128];
	 sprintf (pt,"u=%f\t v=%f\t w=%f",v->u,v->v,v->w);
	 strcpy  (chaine,pt);
	 return (chaine);
	}


char* EcritDt  (sDroite* d, char* chaine)
	{
	 strcpy(chaine,"A  ");
	 EcritPt (&d->A,chaine+strlen(chaine));
	 if (d->segment==0)
	    {
	     strcat (chaine,"\nu   ");
	     EcritVec(&d->u,chaine+strlen(chaine));
	    }
	 else
	    {
	     strcat (chaine,"\nB   ");
	     EcritPt (&d->B,chaine+strlen(chaine));
	    }
	 return (chaine);
	}


char* EcritPl (sPlan* pl, char* chaine)
	{
	 EcritPt (&pl->P,chaine);
	 strcat  (chaine,"\n");
	 EcritVec(&pl->u,chaine+strlen(chaine));
	 return (chaine);
	}


char* EcritCer (sCercle* c,char* chaine)
	{
	 char temp[128];

	 strcpy(chaine,"C  ");
	 EcritPt (&c->C,chaine+strlen(chaine));
	 if (!c->cercle || c->alpha!=0 || c->beta!=0)
	    {
	     strcat(chaine,"\ni  ");
	     EcritVec (&c->i,chaine+strlen(chaine));
	     strcat(chaine,"\nj  ");
	     EcritVec (&c->j,chaine+strlen(chaine));
	    }
	 strcat(chaine,"\nk  ");
	 EcritVec (&c->k,chaine+strlen(chaine));

	 if (c->cercle) sprintf (temp,"\na    =%f",c->a);
	 else sprintf (temp,"\na    =%f \tb   =%f",c->a,c->b);
	 strcat(chaine,temp);

	 if (c->alpha!=0 || c->beta !=0)
	    {
	     sprintf(temp,"\nalpha=%f \tbeta=%f",c->alpha,c->beta);
	     strcat (chaine,temp);
	    }
	 return (chaine);
	}


char* EcritCy  (sCylindre *c, char* chaine)
	{
	 char pt[30];
	 strcpy (chaine,"p   ");
	 EcritPt (&c->p,chaine+strlen(chaine));
	 strcat (chaine,"\nu   ");
	 EcritVec(&c->u,chaine+strlen(chaine));
	 sprintf (pt," \tr=%f",c->r);
	 strcat  (chaine,pt);
	 return (chaine);
	}

char* EcritCo  (sCone* c, char* chaine)
	{
	 char pt[30];
	 strcpy (chaine,"S   ");
	 EcritPt (&c->S,chaine+strlen(chaine));
	 strcat (chaine,"\nu   ");
	 EcritVec(&c->u,chaine+strlen(chaine));
	 sprintf (pt," \tAgl=%f",c->Agl);
	 strcat  (chaine,pt);
	 return (chaine);
	}


char* EcritSph (sSphere* s, char* chaine)
	{
	 char pt[30];

	 EcritPt (&s->C,chaine);
	 sprintf (pt,"\t r=%f",s->R);
	 strcat  (chaine,pt);
	 return (chaine);
	}


char* EcritRep (sRepere* r, char* chaine)
	{
	 strcpy  (chaine,"O   ");
	 EcritPt (&r->O,chaine+strlen(chaine));
	 strcat  (chaine,"\ni   ");
	 EcritVec(&r->i,chaine+strlen(chaine));
	 strcat  (chaine,"\nj   ");
	 EcritVec(&r->j,chaine+strlen(chaine));
	 strcat  (chaine,"\nk   ");
	 EcritVec(&r->k,chaine+strlen(chaine));
	 return chaine;
	}


char* EcritNd  (sNoeud* n, char* chaine)
	{
	 char pt[30];

	 EcritPt (&n->P,chaine);
	 sprintf (pt,"\t num=%f",n->num);
	 strcat  (chaine,pt);
	 return (chaine);
	}

char* EcritMa  (sMaille* m, char* chaine)
	{
	 char temp[128];
	 sprintf (temp,"%d6   %d6   %d6   %d6\n%d6   %d6   %d6   %d6   mat=%d",
		  m->P[0],m->P[1],m->P[2],m->P[3],m->P[4],m->P[5],
		  m->P[6],m->P[7],m->materiau);
	 return (chaine);
	}



/*======================================================================*/
/*			       SECTION					*/
/*======================================================================*/



int InitSection (sSection* s,char* nom,boolean mode, char* nouveaunom)
	{
	 s->Ancien=fopen(nom,"rt+");
	 if (s->Ancien==NULL && mode==LECTURE)
	    {
	     return(NONOUVRABLE);
	    }

	 s->Nouveau=NULL;

	 if (mode==ECRITURE)
	    {
	     s->Modif=FALSE;
	     if (s->Ancien==NULL)
		s->Nouveau=fopen(nom,"wt+");

	     else
	    {
	    if (nouveaunom==NULL)
	       {
		s->Modif=TRUE;
		tmpnam(s->NouveauFichier);
		s->Nouveau=fopen(s->NouveauFichier,"wt+");
	       }
	    else
	       {
		s->Nouveau=fopen(nouveaunom,"wt+");
		strcpy(s->NouveauFichier,nouveaunom);
	       }
	    if (s->Nouveau==NULL)
	       {
		fclose(s->Ancien);
		s->Ancien=NULL;
		return (NONOUVRABLE);
	       }
	    }
	    }

	 strcpy(s->AncienFichier,nom);
	 s->Lecture=mode;
	 s->Buffer=NULL;
	 s->Taille=0;
	 strcpy (s->Nom,"");
	 return OK;
	}


int OuvreSection (sSection* s,char* nom)
	{
	 long   pos;
	 char*  sep=" \t\n";
	 int 	cont,i;
	 char * buffer,*pt;
	 char   ligne[128];


	 if (!s->Lecture)
	    {
	     if (s->Nouveau==NULL) {GeomERROR=INEXISTANTE;
				    return (INEXISTANTE);}
	     if (s->Ancien==NULL) {GeomERROR=OK; return (OK);}
	    }

	 if (s->Buffer!=NULL)
	    {
	     free(s->Buffer);
	     s->Buffer=NULL;
	    }
	 if (s->Ancien==NULL) {GeomERROR=NONOUVRABLE; return (NONOUVRABLE);}

	 GeomERROR=INEXISTANTE;
	 rewind (s->Ancien);
	 pos=0;

	 while (!feof(s->Ancien))
	       {
		buffer=fgets(ligne,128,s->Ancien);
		if (buffer==NULL || *buffer!='[') continue;
		buffer=strtok(ligne," \n\t][");
		if (strcmp(nom,buffer)==0)
		   {
		    pos=ftell(s->Ancien);
		    break;
		   }
	       }

	 if (pos==0) return (INEXISTANTE);

	 /* calcul de la taille de memoire dynamique a alouer	*/

	 cont=0;
	 pt=NULL;
	 while (!feof(s->Ancien))
	       {
		if (pt==NULL) pt=fgets(ligne,128,s->Ancien);
		pt=strtok(ligne,sep);

		if (pt==NULL) continue;
		if (*pt=='[') break;
		if (*pt=='#')
		   {
		    pt=NULL;
		    continue;
		   }

		cont=cont+strlen(pt)+1;

		pt=strtok (NULL,sep);
		while (*pt!='#'&& pt!=NULL)
		      {
		       cont=cont+strlen(pt)+1;
		       pt=strtok(NULL,sep);
		      }

		while (!feof(s->Ancien))
		      {
		       pt=fgets(ligne,128,s->Ancien);
		       if (*pt!=' ' && *pt!='\t') break;
		       pt=strtok(ligne,sep);
		       while (*pt!='#' && pt!=NULL)
			     {
			      cont=cont+strlen(pt)+1;
			      pt=strtok(NULL,sep);
			     }
		       pt=NULL;
		      }

		cont++;
	       }

	 s->Taille=cont+2;

	 fseek(s->Ancien,pos,SEEK_SET);	/* on se repositionne au debut de
					   la section			  */
	 buffer=(char*)calloc(cont+2,sizeof(char)); /* allocation memoire */
	 if (buffer==NULL)
	    {
	     printf ("Pas assez de place memoire pour charger la section %s\n",nom);
	     printf ("qui represente %dl octets !\n",cont);
	     fclose (s->Ancien);
	     exit (-1);
	    }


	 *buffer='\0';
	 pt=NULL;
	 while (!feof(s->Ancien))
	       {
		if (pt==NULL) pt=fgets(ligne,128,s->Ancien);
		pt=strtok(ligne,sep);

		if (pt==NULL) continue;
		if (*pt=='[') break;
		if (*pt=='#')
		   {
		    pt=NULL;
		    continue;
		   }

		strcat(buffer,pt);
		strcat(buffer,"\n");

		pt=strtok (NULL,sep);
		while (*pt!='#'&& pt!=NULL)
		      {
		       strcat (buffer,pt);
		       strcat (buffer," ");
		       pt=strtok(NULL,sep);
		      }

		while (!feof(s->Ancien))
		      {
		       pt=fgets(ligne,128,s->Ancien);
		       if (*pt!=' ' && *pt!='\t') break;
		       pt=strtok(ligne,sep);
		       while (*pt!='#' && pt!=NULL)
			     {
			      strcat (buffer,pt);
			      strcat (buffer," ");
			      pt=strtok(NULL,sep);
			     }
		       pt=NULL;
		      }

		strcat (buffer,"\n");
	       }

	 s->Buffer=buffer;
	 strcpy(s->Nom,nom);
	 GeomERROR=OK;
	 return (OK);
	}


void FermeSection (sSection* s)
	{
	 if (s->Buffer!=NULL) free(s->Buffer);
	 s->Buffer=NULL;
	 if (s->Nouveau!=NULL) fclose(s->Nouveau);
	 if (s->Ancien!=NULL)  fclose(s->Ancien);
	 s->Nouveau=NULL;
	 s->Ancien=NULL;
	 if (s->Modif && (!s->Lecture))
	    {
	     remove(s->AncienFichier);
	     rename(s->NouveauFichier,s->AncienFichier);
	    }
	 strcpy (s->AncienFichier,"");
	 strcpy (s->NouveauFichier,"");
	 strcpy (s->Nom,"");
	 s->Taille=0;
	}


char *	ChercheInfo  (sSection* s,char* clef)
	{
	 char *pt,*pt1,*pt2;
	 static char mot[256];
	 int i;

	 pt=s->Buffer;
	 if (pt==NULL) return (NULL);

	 while (*pt!='\0')
	       {
		sscanf(pt,"%s",mot);
		pt=pt+strlen(mot)+1;

		if (strcmp(mot,clef)==0)
		   {
		    i=0;
		    while (*(pt+i)!='\n')
			  {
			   *(mot+i)=*(pt+i);
			   i++;
			  }
		    if (i==0) i=1;
		    *(mot+i-1)='\0';
		    return mot;
		   }
		while (*pt!='\n') pt++;
		pt++;
	       }

	 GeomERROR=INEXISTANTE;
	 return NULL;
	}



/*======================================================================*/
/*		FONCTIONS DE LECTURE D'UN FICHIER DE DONNEES		*/
/*======================================================================*/


sPoint* PtSection (sSection *s,char* nom)
	{
	 static sPoint result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetPt (&result,PtLit(&pt));

	 return &result;
	}


sVecteur* VecSection (sSection *s,char* nom)
	{
	 static sVecteur result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetVec (&result,VecLit(&pt));

	 return &result;
	}


sDroite* DtSection  (sSection *s,char* nom)
	{
	 static sDroite result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetDt (&result,DtLit(&pt));

	 return &result;
	}



sPlan* PlSection (sSection *s,char* nom)
	{
	 static sPlan result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetPl (&result,PlLit(&pt));

	 return &result;
	}


sCercle* CerSection (sSection *s,char* nom)
	{
	 static sCercle result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetCer (&result,CerLit(&pt));

	 return &result;
	}



sCylindre* CySection (sSection *s,char* nom)
	{
	 static sCylindre result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetCy (&result,CyLit(&pt));

	 return &result;
	}


sCone* CoSection (sSection *s,char* nom)
	{
	 static sCone result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetCo (&result,CoLit(&pt));

	 return &result;
	}


sSphere* SphSection (sSection *s,char* nom)
	{
	 static sSphere result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetSph (&result,SphLit(&pt));

	 return &result;
	}



sRepere* RepSection (sSection *s,char* nom)
	{
	 static sRepere result;
	 char *pt;

	 if (s==NULL) return &result;

	 pt=ChercheInfo (s,nom);
	 if (pt==NULL) return NULL;

	 SetRep (&result,RepLit(&pt));

	 return &result;
	}




/*======================================================================*/
/*		FONCTIONS DE CREATION D'UN FICHIER DE DONNEES		*/
/*======================================================================*/


void	CreeSection (sSection* s,char* nom,char* commentaires)
	{
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaires!=NULL) fprintf (s->Nouveau,"# %s\n",commentaires);
	 fprintf(s->Nouveau,"[%s]\n",nom);
	}


void	CreeInfo    (sSection* s,char* clef,char* info,char* commentaires)
	{
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaires!=NULL) fprintf (s->Nouveau,"# %s\n",commentaires);
	 fprintf(s->Nouveau,"%s\t%s\n",clef,info);
	}


void	CreeComment (sSection* s,char* commentaires)
	{
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaires!=NULL) fprintf (s->Nouveau,"# %s\n",commentaires);
	 else 			 fprintf (s->Nouveau,"\n");
	}


void	CopySection (sSection* s,char* nom,char* clef_fin)
	{
	 char temp[128];
	 char*pt;
	 FILE* fic;

	 if (s->Nouveau==NULL || s->Lecture) return;
	 if (s->Ancien==NULL) return;

	 fic=Fichier;
	 Fichier=s->Ancien;
	 rewind(Fichier);

	 /* recherche de la section */
	 sprintf(temp,"[%s]",nom);
	 do
	   {
	    if (Cherche(temp," \n\t")==NULL) {Fichier=fic; return;}

	    strcpy (LigneTravail,Ligne);
	    PremierMot(" \t\n");
	   }
	 while (strcmp(Mot,temp)!=0);

	 /* copie de la section ligne par ligne */
	 while (LitLigne()!=NULL)
	    {
	     pt=MotLigneSuivant(" \t\n");
	     if (pt!=NULL && *Ligne=='[')
		break;	/* fin de la section */
	     if (pt!=NULL && clef_fin!=NULL &&
		 strcmp(pt,clef_fin)==0 && pt==LigneTravail)
		break;  /* clef clef_fin atteinte */

	     fprintf(s->Nouveau,Ligne);
	    }

	 Fichier=fic;
	}


void	CopyInfos   (sSection* s,char* section, char* debut, char* fin)
	{
	 char temp[128];
	 char*pt;
	 FILE* fic;

	 if (s->Nouveau==NULL || s->Lecture) return;
	 if (s->Ancien==NULL) return;

	 fic=Fichier;
	 Fichier=s->Ancien;
	 rewind(Fichier);

	 /* recherche de la section */
	 sprintf(temp,"[%s]",section);
	 do
	   {
	    if (Cherche(temp," \n\t")==NULL) {Fichier=fic; return;}

	    strcpy (LigneTravail,Ligne);
	    PremierMot(" \t\n");
	   }
	 while (strcmp(Mot,temp)!=0 && Mot==LigneTravail);

	 /* recherche du mot clef debut */
	 if (debut==NULL)
	   {
	    LitLigne();
	    pt=MotLigneSuivant(" \t\n");
	   }
	 else
	   {
	    do
	      {
	       LitLigne();
	       if (Ligne==NULL) {Fichier=fic; return;}
	       pt=MotSuivant(" \t\n");
	       if (pt!=NULL && *Ligne=='[') {Fichier=fic; return;}
	      }
	    while (strcmp(pt,debut)!=0 || pt!=LigneTravail);
	   }

	 /* copie de la section ligne par ligne */
	 do
	   {
	    if (pt!=NULL && *Ligne=='[') break; /* fin de la section */
	    if (pt!=NULL && fin!=NULL &&
		strcmp(pt,fin)==0 && pt==LigneTravail)
	       break;  /* clef fin atteinte */

	    fprintf(s->Nouveau,Ligne);

	    LitLigne();
	    if (Ligne!=NULL) pt=MotLigneSuivant(" \t\n");
	   }
	 while (Ligne!=NULL);

	 Fichier=fic;
	}


/*========================================================================
			CREATIONS DES OBJETS DE GEOM
========================================================================*/

void	CreePt	(sSection* s, sPoint* p, char* nom, char* commentaire)
	{
	 char temp[128];
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 fprintf(s->Nouveau,"%s\t%s\n",nom,EcritPt (p,temp));
	}


void	CreeVec	(sSection* s, sVecteur* v, char* nom, char* commentaire)
	{
	 char temp[128];
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 fprintf(s->Nouveau,"%s\t%s\n",nom,EcritVec (v,temp));
	}

void	CreeDt	(sSection* s, sDroite* d, char* nom, char* commentaire)
	{
	 char temp[255];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritDt (d,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}


void	CreePl	(sSection* s, sPlan*  p, char* nom, char* commentaire)
	{
	 char temp[128];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritPl (p,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}

void	CreeCer	(sSection* s, sCercle* c, char* nom, char* commentaire)
	{
	 char temp[350];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritCer (c,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}

void	CreeCy	(sSection* s, sCylindre* c, char* nom, char* commentaire)
	{
	 char temp[255];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritCy (c,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}

void	CreeCo	(sSection* s, sCone* c, char* nom, char* commentaire)
	{
	 char temp[255];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritCo (c,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}


void	CreeSph	(sSection* s, sSphere* sp, char* nom, char* commentaire)
	{
	 char temp[128];
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 fprintf(s->Nouveau,"%s\t%s\n",nom,EcritSph (sp,temp));
	}

void	CreeRep	(sSection* s, sRepere* r, char* nom, char* commentaire)
	{
	 char temp[256];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritRep (r,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}

void	CreeNd	(sSection* s, sNoeud* n, char* nom, char* commentaire)
	{
	 char temp[128];
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 fprintf(s->Nouveau,"%s\t%s\n",nom,EcritNd (n,temp));
	}

void	CreeMa	(sSection* s, sMaille* m, char* nom, char* commentaire)
	{
	 char temp[255];
	 char* pt;
	 if (s->Nouveau==NULL || s->Lecture) return;

	 if (commentaire!=NULL) fprintf (s->Nouveau,"# %s\n",commentaire);
	 EcritMa (m,temp);
	 pt=strtok(temp,"\n");
	 fprintf (s->Nouveau,"%s\t%s\n",nom,pt);

	 while ((pt=strtok(NULL,"\n"))!=NULL)
	    fprintf (s->Nouveau,"\t\t%s\n",pt);
	}


