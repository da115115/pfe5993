/*--------------------------------------------------------------------------*/
/*--------------------------------  ILIAQUE   ------------------------------*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <graphics.h>

#include "geom.h"
#include "geom_adv.h"
#include "filedata.h"
#include "maille.h"

#include "show.h"
#include "show_adv.h"


/*------------------  structures d'echanges de l'iliaque  --------------*/

sPoint*  Pt_IlioPelvien;	/* point projection de Pt_CoSup
					   sur le cylindre pelvien 			*/

sPlan*   Pl_Sacre;  		/* plan de l'articulation sacro-iliaque */

sDroite* Dt_Mediale;     	/* droite mediale de l'iliaque          */
sDroite* Dt_IlAnteroInt;  	/* droite des epines anterieures	  */
sDroite* Dt_IlAnteroInf;  	/* droite de raccordement avec la branche
					   pubienne                       */
sCercle* Ce_IlAnteroInf;  	/* portion d'ellipse inferieure 	  */




/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void iliaque (int COTE)
 {

  float pos,x,y,y0,y1,y2;
  float fTemp,fTemp1,disSup,disInf,dis;
  int i,j,k;
  int a,b,c,d,e,f,g,h,mat;
  sPoint p,p1,p2,p3,p4;
  sVecteur u1,u2,u3;
  sPoint m1,m2,pTemp,pTemp1;
  sCercle ce;
  sPlan   Pl,Pl1;
  sDroite  p1p2,p2m1,m2p3,p2p3,dDir;
  int    Flag,Etape,nbinf,nbsup,num;
  sSection Data;
  sTabDynamique tDir;
  sPoint  tDis[3][2];
  sQuadri qDir,qDis;

  sDroite  dTemp,dTemp1,dTemp2;
  sVecteur vTemp,vTemp1;

  static sPoint  pt1,pt2,pt4,pt5;
  static sPlan   pl1,pl2;
  static sDroite dt1,dt2,dt3,dt4;
  static sCercle ce1,ce3,ce5;
	 sCourbe cb1,cb2;

	/* numeros materiaux						*/
  int CORTICAL,SPONGIEUX;

	/* numero de la piece		   				*/
  int ILIAQUE;


	/* dimension de la piece					*/
  int IL_LARGEUR,IL_LARGEUR_SEUL;
  int IL_HAUTEUR;
  int IL_EPAISSEUR;
  int CO_EPAISSEUR;

  float Ep_Il;
  float Ep_Mediane;
  float	IL_CONE_INF,IL_CONE_SUP;
  int	IL_CONE_HAUTEUR;

  /* dimensions articulation sacro-iliaque en nombre de mailles */
  int nLH,nLV,nlh,nlv;


  /*-----------------------  Structures a determiner  ---------------------*/

  /* varaiables d'affichage */
  sDroite DtAff[6];

  /* variables principales */

  sDroite DtTemp,DtTemp1;
  sDroite*pDtTemp;
  sPoint  PtTemp,PtTemp1;
  sPoint* pPtTemp;
  sPlan   PlTemp;
  sVecteur VecTemp,VecTemp1;


  sDroite * Dt_Base=&dt1;	/* droite inf cotyle-sacroiliaque	*/

  sCourbe * Cb_Inf=&cb1;	/* courbe des arretes inferieures	*/
  sCourbe * Cb_Ver=&cb2;	/* courbe verticale pour le maillage en
				   hauteur				*/
  sCourbe Cb_Post;		/* courbe posterieure de l'iliaque 	*/
  sPoint  Pt_IlEpPostSup;
  sPoint  *Pt_IlEpAntSup=&pt4;
  sPoint  *Pt_IlMedianeAntSup=&pt5;


  sPlan   * Pl_InterCone=&pl2;  /* plan inter cone                   	*/
  sCercle * Ce_IlSupInt=&ce1;	/* cercle superieur et interieur       	*/
  sCercle * Ce_IlSupExt=&ce5;   /* cercle superieur et exterieur	*/

  sPoint  * Pt_IlCoExt=&pt2;	/* point de la face externe sur le cotyle*/
  sPoint  Pt_IlExtPostInf;      /* point de la face posterieur,
				   face a Pt_SacreAntInf		*/
  sPoint  Pt_IlExtPostSup;
  sPoint  Pt_EpExtPostInf;
  sPoint  Pt_EpExtPostSup;

  sDroite Dt_IlExtInf;


  sPoint Pt_SacreAntInf;	/* points de l'articulations sacro-iliaque */
  sPoint Pt_SacreAntSup;
  sPoint Pt_SacrePostInf;
  sPoint Pt_SacrePostSup;


  sTabDynamique tFosse;		/* tableau en memoire dynamique contenant
				   le quadrillage de la fosse interne	*/
  sQuadri   qFosse;		/* quadrillage associe 			*/

  sTabDynamique tCone;		/* tableau associe au quadrillage de la face*/
  sQuadri   qCone;		/* externe en appuie sur les cones	*/

  sTabDynamique tPost;		/* tableau dynamique de la face externe	*/
  sQuadri   qPost;		/* quadrillage associe			*/

  sTabDynamique tPostInt;
  sTabDynamique tPostExt;
  sQuadri qPostInt;
  sQuadri qPostExt;

  sVecteur  SupAnt,SupPost,InfAnt,InfPost;
				/* direction des 4 angles de l'aile	*/



  /*------------------  structures d'echange de l'iliaque    -----------*/

  Pt_IlioPelvien=&pt1;	/* point projection de Pt_CoSup sur le cylindre */

  Pl_Sacre=&pl1;        /* plan de l'articulation sacro-iliaque 	*/

  Dt_Mediale=&dt2;      /* droite mediale de l'iliaque    	      	*/
  Dt_IlAnteroInt=&dt3;  /* droite des epines anterieures		*/
  Dt_IlAnteroInf=&dt4;  /* droite de raccordement avec la branche
					     pubienne                   */
  Ce_IlAnteroInf=&ce3;  /* portion d'ellipse inferieure 		*/


  /*======================================================================
    ==================       INITIALISATION    ===========================
    ====================================================================*/

  /*--------------------------  DEFAUT.CFG -----------------------------*/
  if (InitSection (&Data,cDefaut,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de configuration %s introuvable !\n",cDefaut);
      exit (-1);
     }

	/* numeros materiaux						*/
  OuvreSection (&Data,"fichier_commande");
  CORTICAL=atoi(ChercheInfo(&Data,"CORTICAL"));
  SPONGIEUX=atoi(ChercheInfo(&Data,"SPONGIEUX"));

  OuvreSection (&Data,"iliaque");

	/* numero de la piece		   				*/
  ILIAQUE=atoi	(ChercheInfo(&Data,"ILIAQUE"));


	/* dimension de la piece					*/
  IL_LARGEUR_SEUL=atoi(ChercheInfo(&Data,"IL_LARGEUR_SEUL"));
  IL_HAUTEUR=atoi(ChercheInfo(&Data,"IL_HAUTEUR"));
  IL_EPAISSEUR=atoi(ChercheInfo(&Data,"IL_EPAISSEUR"));

  IL_CONE_INF=atof(ChercheInfo(&Data,"IL_CONE_INF"));
  IL_CONE_SUP=atof(ChercheInfo(&Data,"IL_CONE_SUP"));
  IL_CONE_HAUTEUR=atoi(ChercheInfo(&Data,"IL_CONE_HAUTEUR"));


  OuvreSection (&Data,"cotyle");
  CO_EPAISSEUR=atoi(ChercheInfo(&Data,"CO_EPAISSEUR"));
  IL_LARGEUR=CO_EPAISSEUR+IL_LARGEUR_SEUL;

  /* donnees du sacrum */
  OuvreSection (&Data,"sacrum");
  nLH=atoi(ChercheInfo(&Data,"nLH"));
  nLV=atoi(ChercheInfo(&Data,"nLV"));
  nlh=atoi(ChercheInfo(&Data,"nlh"));
  nlv=atoi(ChercheInfo(&Data,"nlv"));


  FermeSection (&Data);



  /*-----------------  Initialisation elements calcules   -----------------*/

  /*--------------------------  GEOM.CFG -----------------------------*/
  if (InitSection (&Data,cFormes,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de formes %s introuvable !\n",cFormes);
      exit (-1);
     }



  /* creation artificielle de l'interface iliaque_cotyle */
  if (COTE==1) OuvreSection (&Data,"Cotyle_Droit");
  else         OuvreSection (&Data,"Cotyle_Gauche");
  if (GeomERROR!=OK) return;
  SetCer (&ce,CerSection(&Data,"Ce_Cotyle"));
  SetCerCoCer (&ce,&ce.C,VecK(COTE*-1,&ce.k),ce.a*11/9);
  SetPt  (&p1,PtSection (&Data,"E1"));
  SetPt  (&p2,PtSection (&Data,"E2"));
  SetPt  (&p3,PtSection (&Data,"N1"));
  SetPt  (&p4,PtSection (&Data,"N2"));
  SetCer(&ce,CerCutPt(&ce,&p1,&p2));
  SetDt (&p1p2,DtPt(&p3,&p4));

  /*!!!!!!!!!!!!!!  ILIAQUE _COTYLE ATIFICIELLE   !!!!!!!!!!!!!!!!!!!*/
/*  TabNdInit(&Iliaque_Cotyle,2,CO_EPAISSEUR+1,IL_EPAISSEUR+1);
  for (j=0;j<=IL_EPAISSEUR;j++)
     {
      SetPt(&pTemp,PtDt(&p1p2,(float)j/IL_EPAISSEUR));
      SetPt(&pTemp1,PtCer(&ce,(float)j/IL_EPAISSEUR));
      SetDt(&dTemp,DtPt(&pTemp,&pTemp1));
      for (i=0;i<=CO_EPAISSEUR;i++)
	 {
	  SetPt(&p,PtDt(&dTemp,(float)i/CO_EPAISSEUR));
	  SetNdCo(NdTab(&Iliaque_Cotyle,i,j),&p,j*(CO_EPAISSEUR+1)+i+1);
	 }
     }

*/



  if (COTE==1) OuvreSection (&Data,"Iliaque_Droit");
  else         OuvreSection (&Data,"Iliaque_Gauche");
  if (GeomERROR!=OK) return;

  Ep_Il=atof (ChercheInfo(&Data,"Ep_Il"));
  Ep_Mediane=atof (ChercheInfo(&Data,"Ep_Mediane"));


  SetCer (Ce_IlSupInt,CerSection(&Data,"Ce_IlSupInt"));
  SetSph (Sp_IlFosse,SphSection (&Data,"Sp_IlFosse"));
  SetPt  (Pt_IlEpAntSup,PtSection(&Data,"Pt_IlEpAntSup"));
  SetPt  (Pt_IlMedianeAntSup,PtSection(&Data,"Pt_IlMedianeAntSup"));
  SetPt  (&Pt_IlEpPostSup,PtSection(&Data,"Pt_IlEpPostSup"));


  SetCer (Ce_IlSupExt,CerSection(&Data,"Ce_IlSupExt"));
  SetCo  (Co_IlAnt,CoSection (&Data,"Co_IlAnt"));
  SetCo  (Co_IlPost,CoSection (&Data,"Co_IlPost"));
  SetPt  (&Pt_IlExtPostInf,PtSection(&Data,"Pt_IlExtPostInf"));
  SetPt  (&Pt_IlExtPostSup,PtSection(&Data,"Pt_IlExtPostSup"));
  SetPt  (&Pt_EpExtPostInf,PtSection(&Data,"Pt_EpExtPostInf"));
  SetPt  (&Pt_EpExtPostSup,PtSection(&Data,"Pt_EpExtPostSup"));



  /* Initialisation des points communs avec le Cotyle */
  SetPt (Pt_CoSup,&NdTab(&Iliaque_Cotyle,CO_EPAISSEUR,0)->P);
  SetPt (Pt_IlCoExt,&NdTab(&Iliaque_Cotyle,CO_EPAISSEUR,IL_EPAISSEUR)->P);
  SetPt (Pt_IlioPelvien,&NdTab(&Iliaque_Cotyle,0,0)->P);


  /* formes generales */
  if (COTE==1) OuvreSection (&Data,"General_Droit");
  else         OuvreSection (&Data,"General_Gauche");
  if (GeomERROR!=OK) return;

  SetCer (Ce_Pelvien,CerSection(&Data,"Ce_Pelvien"));

  /* points du sacrum*/
  SetPt (&Pt_SacreAntInf,&Sacro_Iliaque[0][0].P);
  SetPt (&Pt_SacreAntSup,&Sacro_Iliaque[nLH][0].P);
  SetPt (&Pt_SacrePostInf,&Sacro_Iliaque[0][nLV].P);
  SetPt (&Pt_SacrePostSup,&Sacro_Iliaque[nlv][nLV].P);


  /* droite iliaque-atero-inferieure					*/
  SetDtCo(Dt_IlAnteroInf,Pt_CoSup,VecAB(Pt_CoSup,Pt_IlioPelvien));
  SetDt  (Dt_IlAnteroInf,DtCutPt(Dt_IlAnteroInf,Pt_IlioPelvien,Pt_CoSup));

  /* droite de base et cercle de la courbe inferieure */
  SetDt  (Dt_Base,DtPt(&Pt_SacreAntInf,Pt_CoSup));
  SetCer (Ce_IlAnteroInf,CerCutPt(Ce_Pelvien,&Pt_SacreAntInf,
					    Pt_IlioPelvien));

  CourbeInit   (Cb_Inf);
  CourbeAddCer (Cb_Inf,Ce_IlAnteroInf);
  CourbeAddDt  (Cb_Inf,Dt_IlAnteroInf);


  FermeSection(&Data);



/*===========================================================================
=======================                     =================================
=======================  BLOQUE ANTERIEUR   ===============================*/
/*=====================                     =================================
===========================================================================*/


  /*----------------------------------------------------------------------*/
  /*-------------------    FOSSE INTERNE    ------------------------------*/
  /*----------------------------------------------------------------------*/


EcranAddNd (Iliaque_Cotyle.tab,(CO_EPAISSEUR+1)*(IL_EPAISSEUR+1),YELLOW);
EcranAddCer    (Ce_IlSupInt,1,BROWN);
EcranAddCourbe (Cb_Inf,1,BROWN);
EcranAddDt (Dt_Base,1,RED);


  /* creation d'un quadrillage de la fosse interne associe au tableau
     de point dynamique tFosse						*/

  /* initialisation du tableau dynamique		*/
  TabPtInit (&tFosse,2,IL_LARGEUR+1,IL_HAUTEUR+1);

  /* boucle d'iteration dans la largeur */
  for (i=0;i<=IL_LARGEUR;i++)
	{
	 /* on determine les points p1 et p3 des arretes	*/

	 pos=(float)i/IL_LARGEUR;
	 if (i>=IL_LARGEUR_SEUL)
	    SetPt (&p1,&NdTab(&Iliaque_Cotyle,i-IL_LARGEUR_SEUL,0)->P);
	 else
	    SetPt (&p1,PtCer(Ce_IlAnteroInf,(float)i/IL_LARGEUR_SEUL));
	 SetPt (&p3,PtCer(Ce_IlSupInt,pos));

	 /* on determine p2 de la droite de base		*/

	 SetVec (&u1,&Dt_Base->u);
	 SetVec (&u2,VecNorme(VecAB(&p1,&p3)));
	 SetVec (&u3,VecNorme(VecVec(&u1,&u2)));
	 SetVec (&u1,VecVec(&u2,&u3));
	 SetPlCo(&PlTemp,&p1,&u1);
	 SetPt  (&p2,Plan_Droite(&PlTemp,Dt_Base));


	 /* calcul des points tangeants  a la sphere. Si ils existent,
	    le Flag est positionne a 1 */

	 SetPl (&PlTemp,PlPt3(&p3,&Sp_IlFosse->C,&p2));
	 SetCer(&ce,Plan_Sphere(&PlTemp,Sp_IlFosse));


	 SetPt (&m1,PtTanCer(&p2,&ce,GAUCHE));
	 SetPt (&m2,PtTanCer(&p3,&ce,DROITE));


	 SetDt (&DtTemp,DtPt(&p2,&p3));

/*if (i==0) EcranAddDt(&DtTemp,1,BLUE);
else      EcranChange(&DtTemp,&DtTemp,1,BLUE);*/

	 if ((PosPtDt(&m1,&DtTemp,&PlTemp)==GAUCHE) ||
	     (PosPtDt(&m2,&DtTemp,&PlTemp)==GAUCHE))
	    Flag=0;
	 else if (PosPtCer(&p2,&ce)==INTERNE || PosPtCer(&p3,&ce)==INTERNE)
	    Flag=2;
	 else
	    Flag=1;

			/* construction de la courbe en hauteur 	*/

	 SetDt (&p1p2,DtPt(&p1,&p2));
	 SetDt (&p2m1,DtPt(&p2,&m1));
	 SetDt (&m2p3,DtPt(&m2,&p3));
	 SetDt (&p2p3,DtPt(&p2,&p3));

/*if (i==0) EcranAddCer(&ce,1,RED);
else      EcranChange(&ce,&ce,1,RED);*/

	 SetCer(&ce,CerCutPt(&ce,&m1,&m2));

/*EcranChange(&ce,&ce,1,WHITE);*/

	 CourbeInit (Cb_Ver);
	 CourbeAddDt(Cb_Ver,&p1p2);
	 if (Flag!=0)
	    {
	     CourbeAddDt (Cb_Ver,&p2m1);
	     if (Flag==1) CourbeAddCer(Cb_Ver,&ce);
	     CourbeAddDt (Cb_Ver,&m2p3);
	    }
	 else
	    CourbeAddDt (Cb_Ver,&p2p3);

			/* calcul des points dans la hauteur 		*/

/*if (i==0) EcranAddCourbe(Cb_Ver,1,GREEN);
else      EcranChange   (Cb_Ver,Cb_Ver,1,GREEN);*/

	 for (j=0;j<=IL_HAUTEUR;j++)
	     {
	      pos=(float)j/IL_HAUTEUR;
	      SetPt (tFosse.e(i,j),PtCourbe(Cb_Ver,pos));
	     }
	}

  /* maillage de la partie non liee au cotyle */

  /* initialisation de la courbe posterieure */
  SetDt (&dTemp, DtPt(&Pt_SacreAntInf,&Pt_SacreAntSup));
  SetDt (&dTemp1,DtPt(&Pt_SacreAntSup,Pt_IlMedianeAntSup));
  CourbeInit(&Cb_Post);
  CourbeAddDt(&Cb_Post,&dTemp);
  CourbeAddDt(&Cb_Post,&dTemp1);

 /*!!!!!!!!!!!!!  BRUTUS ME VOILA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  for (j=1; j<IL_HAUTEUR; j++)
     {
      if (j<=nLH) SetPt(&p,&Sacro_Iliaque[j][0].P);
      else      SetPt(&p,PtDt(&dTemp1,(float)(j-nLH)/(IL_HAUTEUR-nLH)));
      SetPt (tFosse.e(0,j),&p);
     }




  /* initialisation du quadrillage		*/

  QuadriInit (&qFosse,tFosse.tab,IL_LARGEUR+1,IL_HAUTEUR+1);

EcranAddQuadri (&qFosse,1,MAGENTA);
EcranAddCourbe (&Cb_Post,1,RED);




  /*----------------------------------------------------------------------*/
  /*-------------------    FACE  EXTERNE    ------------------------------*/
  /*----------------------------------------------------------------------*/

  /* Initialisation de la droite  faisant face a Ce_IlAnteroInf */
  SetPt  (&pTemp,&NdTab(&Iliaque_Cotyle,0,IL_EPAISSEUR)->P);
  SetDt  (&Dt_IlExtInf,DtPt(&Pt_IlExtPostInf,&pTemp));



EcranAddCer (Ce_IlSupExt,1,LIGHTBLUE);

/*EcranAddCo (Co_IlAnt,1,BLUE);
EcranAddCo (Co_IlPost,1,MAGENTA);*/



  /* initialisation du tableau de points indiquant les directions de
     projection */
  TabPtInit (&tDir,2,IL_LARGEUR+1,IL_HAUTEUR+1);
  SetDt (&dTemp,DtPt(&Pt_IlExtPostInf,Pt_IlCoExt));
  for (i=0;i<=IL_LARGEUR;i++)
      {
/*       if (i<IL_LARGEUR_SEUL)*/
	   SetPt (&p1,PtDt(&dTemp,(float)i/IL_LARGEUR));
/*       else
	   SetPt (&p1,&NdTab(&Iliaque_Cotyle,
			      i-IL_LARGEUR_SEUL,IL_EPAISSEUR)->P);*/

       SetPt (&p3,PtCer(Ce_IlSupExt,(float)i/IL_LARGEUR));

       SetDt(&dDir,DtPt(&p1,&p3));

       for (j=0;j<=IL_HAUTEUR;j++)
	   {
	    SetPt (&pTemp,PtDt(&dDir,(float)j/IL_HAUTEUR));
	    SetPt (PtTab(&tDir,i,j),&pTemp);
	   }
      }
  QuadriInit (&qDir,tDir.tab,IL_LARGEUR+1,IL_HAUTEUR+1);

/*EcranAddQuadri (&qDir,1,BLUE);*/




  /* Initialisation des vecteurs dans les angles */
  SetVec(&InfAnt,VecAB(Pt_CoSup,Pt_IlCoExt));
  SetVec(&InfPost,VecAB(&Pt_SacreAntInf,&Pt_IlExtPostInf));
  SetVec(&SupAnt,VecAB(Pt_IlEpAntSup,PtCer(Ce_IlSupExt,1)));
  SetVec(&SupPost,VecAB(Pt_IlMedianeAntSup,PtCer(Ce_IlSupExt,0)));


  /* initialisation du quadrillage de points indiquant les epaisseurs maxi
     de l'iliaque*/
  SetPt (&tDis[0][0],Pt(0,0,Norme(&InfPost)));
  SetPt (&tDis[1][0],Pt(1,0,Norme(VecAB(Pt_IlioPelvien,
					PtDt(&Dt_IlExtInf,1)))));
  SetPt (&tDis[2][0],Pt(2,0,Norme(&InfAnt)));
  SetPt (&tDis[0][1],Pt(0,1,Norme(&SupPost)));
  SetPt (&tDis[1][1],Pt(1,1,Norme(&SupAnt)));
  SetPt (&tDis[2][1],Pt(2,1,Norme(&SupAnt)));
  QuadriInit (&qDis,tDis,3,2);




  SetVec(&InfAnt,VecNorme(&InfAnt));
  SetVec(&SupAnt,VecNorme(&SupAnt));
  SetVec(&InfPost,VecNorme(&InfPost));
  SetVec(&SupPost,VecNorme(&SupPost));




  /* initialisation des plans inter-cones				*/

  pDtTemp=Cone_Cone (Co_IlPost,Co_IlAnt);

SetDt (pDtTemp,DtCutPt(pDtTemp,&pDtTemp->A,Pt(0,0,15)));
SetDt (pDtTemp+1,DtCutPt(pDtTemp+1,&(pDtTemp+1)->A,Pt(0,0,15)));
EcranAddDt (pDtTemp,2,LIGHTGRAY);

  /* Flag=0 : les deux cones sont tangents ou ne se coupent pas. 	*/
  if (GeomERROR!=OK)
     {
      Flag=0;
      if (GeomERROR==DISTANT)
	 SetPlCo (Pl_InterCone,&Co_IlAnt->S,
				VecVec(&pDtTemp->u,&(pDtTemp+1)->u));
      else
	 SetPlCo (Pl_InterCone,&Co_IlAnt->S,VecVec(&pDtTemp->u,&Co_IlAnt->u));
     }

  /* Flag=1 : les cones se coupent					*/
  else
     {
      Flag=1;
      SetPlCo (Pl_InterCone,&Co_IlAnt->S,
			    VecVec(&(pDtTemp+1)->u,&pDtTemp->u));

      SetPlCo (&Pl ,&Co_IlAnt->S,VecVec(&(pDtTemp+1)->u,&Pl_InterCone->u));
      SetPlCo (&Pl1,&Co_IlAnt->S,VecVec(&pDtTemp->u    ,&Pl_InterCone->u));
     }



  /* creation du quadrillage en appuie sur les cones			*/
  TabPtInit (&tCone,2,IL_LARGEUR+1,IL_CONE_HAUTEUR+1);

  for (j=0;j<=IL_CONE_HAUTEUR;j++)
      {
       y=IL_CONE_INF+(float)j/(float)IL_CONE_HAUTEUR*(IL_CONE_SUP-IL_CONE_INF);
       Etape=0;

       for (i=0;i<=IL_LARGEUR;i++)
	   {
	    x=(float)i/(float)IL_LARGEUR;

	    SetPt (&PtTemp,PtQuadri(&qFosse,x,y));
	    SetPt (&pTemp1,PtQuadri(&qDir,x,y));
	    SetVec(&VecTemp,VecNorme(VecAB(&PtTemp,&pTemp1)));
	    fTemp=Scal(&VecTemp,&InfPost);
	    if (fTemp<0)
	       SetVec (&VecTemp,VecK(-1,&VecTemp));

/*	    SetVec(&VecTemp,VecK((1-x)*(1-y),&InfPost));
	    SetVec(&VecTemp,VecPlus(&VecTemp,VecK((1-x)*y,&SupPost)));
	    SetVec(&VecTemp,VecPlus(&VecTemp,VecK(x*y,&SupAnt)));
	    SetVec(&VecTemp,VecPlus(&VecTemp,VecK(x*(1-y),&InfAnt)));
	    SetVec(&VecTemp,VecNorme(&VecTemp));*/

	    SetDt (&DtTemp,Dt(&PtTemp,&VecTemp));

	    dis=PtQuadri(&qDis,x,y)->z;

/*SetDt(&DtTemp,DtPt(&PtTemp,&pTemp1));
if (i==0 && j==0)
   EcranAddDt(&DtTemp,1,LIGHTBLUE);
else EcranChange (&DtTemp,&DtTemp,1,LIGHTBLUE);*/

	    /* determination de la projection du point i,j
	       sur les cones						*/

	    if (Etape==0)
	       {
		pPtTemp=Cone_Droite(Co_IlPost,&DtTemp);
		fTemp=Norme(VecAB(&PtTemp,pPtTemp));
		if (GeomERROR!=OK || fTemp>dis)
		   SetPt(pPtTemp,PtVec(&PtTemp,VecK(dis,&VecTemp)));

		if (Flag==0 && PosPtPl(pPtTemp,Pl_InterCone)==INTERNE)
		   Etape=1;

		else if (Flag==1 && PosPtPl (pPtTemp,&Pl)==INTERNE)
		    Etape=1;

/*SetDt(&DtTemp,DtPt(&PtTemp,pPtTemp));
if (i==0 && j==0)
   EcranAddDt(&DtTemp,1,LIGHTBLUE);
else EcranChange (&DtTemp,&DtTemp,1,LIGHTBLUE);*/
	       }

	    if (Etape==1)
	       {
		if (Flag==0)
		   pPtTemp=Cone_Droite(Co_IlAnt,&DtTemp);

		else
		   {
		    pPtTemp=Plan_Droite(Pl_InterCone,&DtTemp);
		    if (PosPtPl(pPtTemp,&Pl1)==INTERNE)
		       Etape=2;
		   }
/*SetDt(&DtTemp,DtPt(&PtTemp,pPtTemp));
EcranChange (&DtTemp,&DtTemp,1,WHITE);*/
	       }

	    if (Etape==2)
	       {
		pPtTemp=Cone_Droite(Co_IlAnt,&DtTemp);
/*SetDt(&DtTemp,DtPt(&PtTemp,pPtTemp));
EcranChange (&DtTemp,&DtTemp,1,RED);*/
	       }

	    fTemp=Norme(VecAB(&PtTemp,pPtTemp));
	    if (fTemp>dis)
	       SetPt(pPtTemp,PtVec(&PtTemp,VecK(dis,&VecTemp)));

	    SetPt (tCone.e(i,j),pPtTemp);
	   }
      }

  QuadriInit (&qCone,tCone.tab,IL_LARGEUR+1,IL_CONE_HAUTEUR+1);

/*EcranAddQuadri (&qCone,1,YELLOW);*/


  /*======================================================================*/
  /*======   creation du quadrillage de la FACE EXTERNE  =================*/

  TabPtInit (&tPost,2,IL_LARGEUR+1,IL_HAUTEUR+1);
  for (i=0;i<=IL_LARGEUR;i++)
	{
	 x=(float)i/((float)IL_LARGEUR);

	 if (i<IL_LARGEUR_SEUL)
	    {
	     SetPt (&p1,PtDt(&Dt_IlExtInf,(float)i/IL_LARGEUR_SEUL));
	     SetPt (&pTemp,PtCer(Ce_IlAnteroInf,(float)i/IL_LARGEUR_SEUL));
	    }
	 else
	    {
	     SetPt (&p1,&NdTab(&Iliaque_Cotyle,
			      i-IL_LARGEUR_SEUL,IL_EPAISSEUR)->P);
	     SetPt (&pTemp,&NdTab(&Iliaque_Cotyle,i-IL_LARGEUR_SEUL,0)->P);
	    }


	 SetPt (&p2,PtQuadri(&qCone,x,0));
	 SetPt (&m2,PtQuadri(&qCone,x,1));
	 SetPt (&p3,PtCer(Ce_IlSupExt,x));

	 SetDt (&p1p2,DtPt(&p1,&p2));
	 SetDt (&m2p3,DtPt(&m2,&p3));

	 SetPt (&pTemp1,PtCer(Ce_IlSupInt,pos));
	 disSup=Norme(VecAB(&pTemp1,&p3));
	 disInf=Norme(VecAB(&pTemp,&p1));



	 for (j=0;j<=IL_HAUTEUR;j++)
	     {
	      y=(float)j/((float)IL_HAUTEUR);

	      /* partie basse */
	      if (y<IL_CONE_INF)
		 {
		  pos=y/IL_CONE_INF;
		  SetPt (&PtTemp,PtDt(&p1p2,pos));
		 }

	      /* partie en appuie sur les cones */
	      else if (y<IL_CONE_SUP)
		 {
		  pos=(y-IL_CONE_INF)/(IL_CONE_SUP-IL_CONE_INF);
		  SetPt (&PtTemp,PtQuadri(&qCone,x,pos));
		 }

	      /* partie haute */
	      else
		 {
		  pos=(y-IL_CONE_SUP)/(1-IL_CONE_SUP);
		  SetPt (&PtTemp,PtDt(&m2p3,pos));
		 }

	      SetPt (tPost.e(i,j),&PtTemp);
	     }
	}


  /*!!!!!!!!!!!!!!!!!!!!  BRUTUS DE NOUVEAU !!!!!!!!!!!!!!!!!!!!!!!!!*/
  SetVec (&vTemp,VecAB(&Pt_SacreAntInf,&Pt_IlExtPostInf));
  SetPt (&pTemp,PtVec(&Pt_SacreAntSup,&vTemp));

  SetDt (&DtTemp,DtPt(PtDt(&Dt_IlExtInf,0),&pTemp));
  SetDt (&DtTemp1,DtPt(&pTemp,&Pt_IlExtPostSup));
  CourbeInit (&Cb_Post);
  CourbeAddDt (&Cb_Post,&DtTemp);
  CourbeAddDt (&Cb_Post,&DtTemp1);

EcranAddCourbe (&Cb_Post,1,WHITE);

  for (j=0;j<=IL_HAUTEUR;j++)
      SetPt (tPost.e(0,j),PtCourbe(&Cb_Post,(float)j/IL_HAUTEUR));


  /* initialisation du quadrillage */
  QuadriInit (&qPost,tPost.tab,IL_LARGEUR+1,IL_HAUTEUR+1);
EcranAddQuadri (&qPost,1,RED);


  TabDel (&tCone);
  TabDel (&tDir);




  /*----------------------------------------------------------------------*/
  /*------------------------  Maillage interne  --------------------------*/
  /*----------------------------------------------------------------------*/

  TabNdFocus (&Noeud);

  for (i=0;i<=IL_LARGEUR;i++)
      {
       for (j=0;j<=IL_HAUTEUR;j++)
	   {
	    if (j==0 && i<=CO_EPAISSEUR) continue;

	    SetPt (&p1,PtTab(&tFosse,IL_LARGEUR-i,j));
	    SetPt (&p2,PtTab(&tPost,IL_LARGEUR-i,j));

	    SetDt  (&dTemp,DtPt(&p1,&p2));

	    for (k=0;k<=IL_EPAISSEUR;k++)
		{
		 pos=1/(float)(IL_EPAISSEUR)*k;

		 SetPt (&p,PtDt(&dTemp,pos));

		 num=i+k*(IL_LARGEUR+nLV+1)+(IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1)*j;
		 num=num+ILIAQUE*1000+1;
		 SetNdCo (Noeud.e(NbNoeud),&p,num);
		 NbNoeud++;
		}
	   }
      }


  /*----------------------------------------------------------------------*/
  /*---------------------  Numerotation des mailles  ---------------------*/
  /*----------------------------------------------------------------------*/

  TabMaFocus (&Maille);

  mat=SPONGIEUX;

  for (i=0;i<IL_LARGEUR;i++)
      {
       for (j=0;j<IL_HAUTEUR;j++)
	   {
	    for (k=0;k<IL_EPAISSEUR;k++)
		{

		 if (i<=CO_EPAISSEUR && j==0)
		   {
		    a=NdTab(&Iliaque_Cotyle,CO_EPAISSEUR-i,k)->num;
		    e=NdTab(&Iliaque_Cotyle,CO_EPAISSEUR-i,k+1)->num;
		   }
		 else
		   {
		 a=i+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 a=a+ILIAQUE*1000+1;

		 e=i+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 e=e+ILIAQUE*1000+1;
		   }


		 if (i<CO_EPAISSEUR && j==0)
		   {
		    b=NdTab(&Iliaque_Cotyle,CO_EPAISSEUR-i-1,k)->num;
		    f=NdTab(&Iliaque_Cotyle,CO_EPAISSEUR-i-1,k+1)->num;
		   }
		 else
		   {
		 b=i+1+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 b=b+ILIAQUE*1000+1;

		 f=i+1+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 f=f+ILIAQUE*1000+1;
		   }



		 c=i+1+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 c=c+ILIAQUE*1000+1;

		 d=i+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 d=d+ILIAQUE*1000+1;


		 g=i+1+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 g=g+ILIAQUE*1000+1;

		 h=i+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 h=h+ILIAQUE*1000+1;




		 if (i==IL_LARGEUR-1 && j<=nLH && k==0)
		    b=Sacro_Iliaque[j][0].num;

		 if (i==IL_LARGEUR-1 && j<nLH && k==0)
		    c=Sacro_Iliaque[j+1][0].num;


		 SetMaCo (Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
		 NbMaille++;
		}
	   }
      }

/*===========================================================================
=======================                     =================================
=======================  BLOQUE POSTERIEUR  ===============================*/
/*=====================                     =================================
===========================================================================*/

/* face INTERIEURE */

  TabPtInit(&tPostInt,2,nLV,IL_HAUTEUR+1);
  SetDt (&dTemp,DtPt(Pt_IlMedianeAntSup,&Pt_IlEpPostSup));

  for (i=1; i<=nLV; i++)
     {
      if (i<=nlh) SetPt (&pTemp,&Sacro_Iliaque[nLH][i].P);
      else        SetPt (&pTemp,&Sacro_Iliaque[nlv][i].P);
      SetDt (&dTemp1,DtPt(&pTemp,PtDt(&dTemp,(float)i/nLV)));

      for (j=0; j<=IL_HAUTEUR; j++)
	 {
	  if (i<=nlh && j<=nLH)
	     SetPt (tPostInt.e(i-1,j),&Sacro_Iliaque[j][i].P);

	  else if (j<=nlv)
	     SetPt (tPostInt.e(i-1,j),&Sacro_Iliaque[j][i].P);

	  else if (i<=nlh)
	     SetPt (tPostInt.e(i-1,j),
		    PtDt(&dTemp1,(float)(j-nLH)/(IL_HAUTEUR-nLH)));

	  else
	     SetPt (tPostInt.e(i-1,j),
		    PtDt(&dTemp1,(float)(j-nlv)/(IL_HAUTEUR-nlv)));
	 }
     }

  QuadriInit (&qPostInt,tPostInt.tab,nLV,IL_HAUTEUR+1);

EcranAddQuadri(&qPostInt,1,LIGHTBLUE);


/* face EXTERIEURE */

  TabPtInit(&tPostExt,2,nLV,IL_HAUTEUR+1);
  SetDt (&dTemp,DtPt(&Pt_EpExtPostInf,&Pt_EpExtPostSup));

  for (j=0; j<=IL_HAUTEUR; j++)
     {
      pos=(float)j/IL_HAUTEUR;
      SetPt (&pTemp, PtCourbe(&Cb_Post,pos));
      SetPt (&pTemp1,PtDt (&dTemp,pos));
      SetDt (&dTemp1,DtPt(&pTemp,&pTemp1));

      for (i=1; i<=nLV; i++)
	 SetPt(tPostExt.e(i-1,j),PtDt(&dTemp1,(float)i/nLV));
     }

  QuadriInit (&qPostExt,tPostExt.tab,nLV,IL_HAUTEUR+1);

EcranAddQuadri(&qPostExt,1,LIGHTGREEN);




  /*----------------------------------------------------------------------*/
  /*------------------------  Maillage interne  --------------------------*/
  /*----------------------------------------------------------------------*/

  TabNdFocus (&Noeud);

  for (i=0;i<nLV;i++)
      {
       for (j=0;j<=IL_HAUTEUR;j++)
	   {
	    SetPt (&p1,PtTab(&tPostInt,i,j));
	    SetPt (&p2,PtTab(&tPostExt,i,j));

	    SetDt  (&dTemp,DtPt(&p1,&p2));

	    for (k=0;k<=IL_EPAISSEUR;k++)
		{
		 pos=1/(float)(IL_EPAISSEUR)*k;

		 SetPt (&p,PtDt(&dTemp,pos));

		 num=i+IL_LARGEUR+1+k*(IL_LARGEUR+nLV+1)+(IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1)*j;
		 num=num+ILIAQUE*1000+1;
		 SetNdCo (Noeud.e(NbNoeud),&p,num);
		 NbNoeud++;
		}
	   }
      }


  /*----------------------------------------------------------------------*/
  /*---------------------  Numerotation des mailles  ---------------------*/
  /*----------------------------------------------------------------------*/

  TabMaFocus (&Maille);

  mat=SPONGIEUX;

  for (i=0;i<nLV;i++)
      {
       for (j=0;j<IL_HAUTEUR;j++)
	   {
	    for (k=0;k<IL_EPAISSEUR;k++)
		{
		 a=i+IL_LARGEUR+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 a=a+ILIAQUE*1000+1;

		 b=i+IL_LARGEUR+1+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 b=b+ILIAQUE*1000+1;

		 c=i+IL_LARGEUR+1+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 c=c+ILIAQUE*1000+1;

		 d=i+IL_LARGEUR+k*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 d=d+ILIAQUE*1000+1;

		 e=i+IL_LARGEUR+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 e=e+ILIAQUE*1000+1;

		 f=i+IL_LARGEUR+1+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*j;
		 f=f+ILIAQUE*1000+1;

		 g=i+IL_LARGEUR+1+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 g=g+ILIAQUE*1000+1;

		 h=i+IL_LARGEUR+(k+1)*(IL_LARGEUR+nLV+1)+((IL_LARGEUR+nLV+1)*(IL_EPAISSEUR+1))*(j+1);
		 h=h+ILIAQUE*1000+1;


		 if ((i<=nlh && j<=nLH) || j<=nlv)
		    a=Sacro_Iliaque[j][i].num;

		 if ((i<nlh && j<=nLH) || j<=nlv)
		    b=Sacro_Iliaque[j][i+1].num;

		 if ((i<nlh && j<nLH) || j<nlv)
		    a=Sacro_Iliaque[j+1][i+1].num;

		 if ((i<=nlh && j<nLH) || j<nlv)
		    b=Sacro_Iliaque[j+1][i].num;


		 SetMaCo (Maille.e(NbMaille),a,b,c,d,e,f,g,h,mat);
		 NbMaille++;
		}
	   }
      }




  TabDel (&tPostExt);
  TabDel (&tPostInt);
  TabDel (&tFosse);
  TabDel (&tPost);
  return;
}



/*============== fonctions retournants le nombre de noeuds ===============
===================  et de mailles crees pour l'iliaque  ===============*/


int IliaqueNbNoeud (void)
 {
  int cont,x,y,z;
  sSection Data;

  if (InitSection (&Data,cDefaut,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de donnees par defaut %s introuvable !\n",cDefaut);
      exit (-1);
     }

  OuvreSection (&Data,"iliaque");

	/* pas de numerotation						*/
  x=atoi(ChercheInfo(&Data,"IL_LARGEUR_SEUL"));
  y=atoi(ChercheInfo(&Data,"IL_HAUTEUR"));
  z=atoi(ChercheInfo(&Data,"IL_EPAISSEUR"));

  OuvreSection (&Data,"sacrum");
  x=x+atoi(ChercheInfo(&Data,"nLV"));

  OuvreSection (&Data,"cotyle");
  x=x+atoi(ChercheInfo(&Data,"CO_EPAISSEUR"));

  FermeSection (&Data);

  cont=(x+1)*(y+1)*(z+1);
  return cont;
 }


int IliaqueNbMaille (void)
 {
  int cont,x,y,z;
  sSection Data;

  if (InitSection (&Data,cDefaut,LECTURE,NULL)!=OK)
     {
      printf ("Fichier de donnees par defaut %s introuvable !\n",cDefaut);
      exit (-1);
     }

  OuvreSection (&Data,"iliaque");

	/* pas de numerotation						*/

  x=atoi(ChercheInfo(&Data,"IL_LARGEUR_SEUL"));
  y=atoi(ChercheInfo(&Data,"IL_HAUTEUR"));
  z=atoi(ChercheInfo(&Data,"IL_EPAISSEUR"));

  OuvreSection (&Data,"sacrum");
  x=x+atoi(ChercheInfo(&Data,"nLV"));

  OuvreSection (&Data,"cotyle");
  x=x+atoi(ChercheInfo(&Data,"CO_EPAISSEUR"));


  FermeSection (&Data);

  cont=x*y*z;
  return cont;
 }
