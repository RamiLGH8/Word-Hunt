#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

typedef struct cellule2* liste2;
typedef struct cellule2{
    char combinaison[9];
    char etat[9];
    struct cellule2 *suiv;
    liste suiv2;
}cellule2;


liste pile;
char combinaison_initiale[9],etat_initial[9];
char combinaison_finale[9],etat_final[9];
bool rotat=false;

void Empiler(liste *pile,char combin[],char etat[]){
    liste newn=NULL;
    if(pile==NULL){
        pile=malloc(sizeof(cellule));
        strcpy((*pile)->combinaison,combin);
        strcpy((*pile)->etat,etat);
        (*pile)->suiv=NULL;
    }
    else{
        newn=malloc(sizeof(cellule));
        strcpy((char *)newn->combinaison,combin);
        strcpy((char *)newn->etat,etat);
        newn->suiv=*pile;
        *pile=newn;
    }

}

liste2 Recuperer_Parent(liste2 tete,liste2 child){
       bool trouver=false;
       liste tetecbn=NULL;
       liste2 Parent=NULL;
       while(trouver==false){
            tetecbn=tete->suiv2;
            while(trouver==false && tetecbn!=NULL){
                if(strcmp((char *)tetecbn->combinaison,(char *)child->combinaison)==0 && strcmp((char *)tetecbn->combinaison,(char *)child->combinaison)==0){
                    trouver=true;
                    Parent=tete;
                }
                tetecbn=tetecbn->suiv;
            }
            tete=tete->suiv;
       }
       return Parent;
}

void Ajouter_Nouv2(liste2 precedent,char combin[],char etat[]){
        liste2 nouv;
        nouv=malloc(sizeof( cellule2));
        strcpy((char *)nouv->combinaison,combin);
        strcpy((char *)nouv->etat,etat);
        precedent->suiv=nouv;
        nouv->suiv=NULL;

}


#endif // FUNCTIONS_H_INCLUDED
