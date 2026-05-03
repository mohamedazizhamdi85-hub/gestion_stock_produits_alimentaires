#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct infoprod {
    char nom[50];
    char reference[20];
    char datefab[11];
    char dateexp[11];
    float prix;
    int quantite;
} infoprod;
typedef struct Noeud {
    char nom[50];
    char reference[20];
    char datefab[11];
    char dateexp[11];
    float prix;
    int quantite;
    struct Noeud *suivant;
} Noeud;

int calculsla(char ch[]);
int verifdate(char ch[]);
int verifref(char ch[]);
int verifnom(char ch[]);
int verifexpfab(char dateexp[], char datefab[]);

Noeud* chargerFichier();
void sauvegarderFichier(Noeud *tete);
void libererListe(Noeud *tete);

Noeud* traitement1(Noeud *tete);
void   traitement2(Noeud *tete);
//Noeud* traitement3(Noeud *tete);
void traitement3(Noeud *tete);
Noeud* traitement4(Noeud *tete);
//Noeud* traitement5(Noeud *tete);
void traitement5(Noeud **tete);
int calculsla(char ch[]) {
    int s = 0, i;
    for (i = 0; i < strlen(ch); i++)
        if (ch[i] == '/') s++;
    return s;
}

int verifdate(char ch[]) {
    char ch1[3], ch2[3], ch3[5];
    strncpy(ch1, ch,   2); ch1[2] = '\0';
    strncpy(ch2, ch+3, 2); ch2[2] = '\0';
    strncpy(ch3, ch+6, 4); ch3[4] = '\0';
    int n1 = atoi(ch1), n2 = atoi(ch2), n3 = atoi(ch3);
    return (calculsla(ch)==2 && n1>=1 && n1<=31 && n2>=1 && n2<=12 && n3>=2026);
}

int verifref(char ch[]) {
    int i = 0;
    while (i < (int)strlen(ch) &&
           ((ch[i]>='a'&&ch[i]<='z') ||
            (ch[i]>='A'&&ch[i]<='Z') ||
            (ch[i]>='0'&&ch[i]<='9')))
        i++;
    return i == (int)strlen(ch);
}

int verifnom(char ch[]) {
    int i = 0;
    while (i < (int)strlen(ch) &&
           ((ch[i]>='A'&&ch[i]<='Z') ||
            (ch[i]>='a'&&ch[i]<='z')))
        i++;
    return i == (int)strlen(ch);
}

int verifexpfab(char dateexp[], char datefab[]) {
    char j1[3],m1[3],a1[5], j2[3],m2[3],a2[5];
    strncpy(j1,datefab,  2); j1[2]='\0';
    strncpy(m1,datefab+3,2); m1[2]='\0';
    strncpy(a1,datefab+6,4); a1[4]='\0';
    strncpy(j2,dateexp,  2); j2[2]='\0';
    strncpy(m2,dateexp+3,2); m2[2]='\0';
    strncpy(a2,dateexp+6,4); a2[4]='\0';
    int an1=atoi(a1),an2=atoi(a2),mo1=atoi(m1),mo2=atoi(m2),jo1=atoi(j1),jo2=atoi(j2);
    if (an2>an1) return 1;
    if (an2<an1) return 0;
    if (mo2>mo1) return 1;
    if (mo2<mo1) return 0;
    return jo2 > jo1;
}
Noeud* chargerFichier() {
    FILE *f = fopen("produit.bin", "rb");
    Noeud *tete = NULL, *queue = NULL;
    if (f == NULL) return NULL;
    infoprod rec;
    while (fread(&rec, sizeof(rec), 1, f) == 1) {
        Noeud *n = (Noeud*)malloc(sizeof(Noeud));
        strcpy(n->nom,       rec.nom);
        strcpy(n->reference, rec.reference);
        strcpy(n->datefab,   rec.datefab);
        strcpy(n->dateexp,   rec.dateexp);
        n->prix     = rec.prix;
        n->quantite = rec.quantite;
        n->suivant  = NULL;
        if (tete == NULL) { tete = queue = n; }
        else { queue->suivant = n; queue = n; }
    }
    fclose(f);
    return tete;
}
void sauvegarderFichier(Noeud *tete) {
    FILE *f = fopen("produit.bin", "wb");
    if (f == NULL) { printf("Erreur ouverture fichier.\n"); return; }
    Noeud *cur = tete;
    while (cur != NULL) {
        infoprod rec;
        strcpy(rec.nom,       cur->nom);
        strcpy(rec.reference, cur->reference);
        strcpy(rec.datefab,   cur->datefab);
        strcpy(rec.dateexp,   cur->dateexp);
        rec.prix     = cur->prix;
        rec.quantite = cur->quantite;
        fwrite(&rec, sizeof(rec), 1, f);
        cur = cur->suivant;
    }
    fclose(f);
}

void libererListe(Noeud *tete) {
    Noeud *tmp;
    while (tete!=NULL) { tmp = tete; tete = tete->suivant; free(tmp); }
}
Noeud* traitement1(Noeud *tete) {
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));
    printf("\n Nom du produit : ");
    scanf("%s", n->nom);
    while (!verifnom(n->nom)) {
        printf(" Redonner le nom : ");
        scanf("%s", n->nom);
    }
    printf(" Reference : ");
    scanf("%s", n->reference);
    while (!verifref(n->reference)) {
        printf(" Redonner la reference : ");
        scanf("%s", n->reference);
    }
    printf(" Date de fabrication (jj/mm/aaaa) : ");
    scanf("%s", n->datefab);
    while (!verifdate(n->datefab)) {
        printf(" Redonner la date de fabrication : ");
        scanf("%s", n->datefab);
    }
    printf(" Date d'expiration (jj/mm/aaaa) : ");
    scanf("%s", n->dateexp);
    while (!verifdate(n->dateexp) || !verifexpfab(n->dateexp, n->datefab)) {
        printf(" Redonner la date d'expiration : ");
        scanf("%s", n->dateexp);
    }
    printf(" Prix : ");
    scanf("%f", &n->prix);
    while (n->prix <= 0) {
        printf(" Redonner le prix : ");
        scanf("%f", &n->prix);
    }
    printf(" Quantite : ");
    scanf("%d", &n->quantite);
    while (n->quantite <= 0) {
        printf(" Redonner la quantite : ");
        scanf("%d", &n->quantite);
    }
    n->suivant = NULL;
    if (tete == NULL) { tete = n; }
    else {
        Noeud *cur = tete;
        while (cur->suivant!=NULL) cur = cur->suivant;
        cur->suivant = n;
    }
    sauvegarderFichier(tete);
    printf("Produit ajoute avec succes!\n");
    return tete;
}
void traitement2(Noeud *tete) {
    if (tete == NULL) { printf("Aucun produit disponible.\n"); return; }
    Noeud *cur = tete;
    printf("\nListe des produits:\n");
    while (cur != NULL) {
        printf("Nom: %-20s Ref: %-15s Fab: %s  Exp: %s  Prix: %.2f  Qte: %d\n",
               cur->nom, cur->reference, cur->datefab,
               cur->dateexp, cur->prix, cur->quantite);
        cur = cur->suivant;
    }
}
/*Noeud* traitement3(Noeud *tete) {
    char ref[20];
    printf("Reference a rechercher : ");
    scanf("%s", ref);
    while (!verifref(ref)) {
        printf("Redonner la reference : ");
        scanf("%s", ref);
    }
    Noeud *cur = tete;
    while (cur!=NULL && strcmp(cur->reference, ref) != 0)
        cur = cur->suivant;
    if (cur!=NULL)
        printf("Produit trouve -> Nom: %s, Prix: %.2f, Qte: %d\n",
               cur->nom, cur->prix, cur->quantite);
    else
        printf("Produit avec reference '%s' introuvable.\n", ref);
    return tete;
}*/
void traitement3(Noeud *tete){
	char ref[20];
    printf("Reference a rechercher : ");
    scanf("%s", ref);
    while (!verifref(ref)) {
        printf("Redonner la reference : ");
        scanf("%s", ref);
    }
    Noeud *cur=tete;
    while(cur!=NULL && strcmp(cur->reference,ref)!=0){
    	cur=cur->suivant;
	}
	if(cur!=NULL){
		printf("produit trouvee");
	}
	else{
		printf("produit non trouvee");
	}
}
Noeud* traitement4(Noeud *tete) {
    char reference[20];
    printf("Reference du produit a modifier : ");
    scanf("%s", reference);
    while (!verifref(reference)) {
        printf("Redonner la reference : ");
        scanf("%s", reference);
    }
    Noeud *cur = tete;
    while (cur && strcmp(cur->reference, reference) != 0)
        cur = cur->suivant;
    if (cur == NULL) { printf("Produit introuvable.\n"); return tete; }
    int choix;
    printf("Modifier le prix ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouveau prix : "); scanf("%f", &cur->prix);
        while (cur->prix <= 0) { printf("Redonner un prix : "); scanf("%f", &cur->prix); }
    }
    printf("Modifier la quantite ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouvelle quantite : "); scanf("%d", &cur->quantite);
        while (cur->quantite < 0) { printf("Redonner : "); scanf("%d", &cur->quantite); }
    }
    printf("Modifier la date de fabrication ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouvelle date de fabrication : "); scanf("%s", cur->datefab);
        while (!verifdate(cur->datefab)) { printf("Redonner : "); scanf("%s", cur->datefab); }
    }
    printf("Modifier la date d'expiration ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouvelle date d'expiration : "); scanf("%s", cur->dateexp);
        while (!verifdate(cur->dateexp) || !verifexpfab(cur->dateexp, cur->datefab)) {
            printf("Redonner : "); scanf("%s", cur->dateexp);
        }
    }
    printf("Modifier le nom ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouveau nom : "); scanf("%s", cur->nom);
        while (!verifnom(cur->nom)) { printf("Redonner : "); scanf("%s", cur->nom); }
    }
    printf("Modifier la reference ? 1-oui 2-non : ");
    scanf("%d", &choix);
    if (choix == 1) {
        printf("Nouvelle reference : "); scanf("%s", cur->reference);
        while (!verifref(cur->reference)) { printf("Redonner : "); scanf("%s", cur->reference); }
    }
    sauvegarderFichier(tete);
    printf("Produit modifie avec succes!\n");
    return tete;
}

void traitement5(Noeud **tete)
{
    char ref[20];
    printf("Reference du produit a supprimer : ");
    scanf("%19s", ref);
    while (!verifref(ref)) {
        printf("Redonner la reference : ");
        scanf("%19s", ref);
    }

    if (*tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    Noeud *cur = *tete;
    Noeud *prev = NULL;
    while (cur != NULL && strcmp(cur->reference, ref) != 0) {
        prev = cur;
        cur = cur->suivant;
    }
    if (cur == NULL) {
        printf("Produit introuvable.\n");
        return;
    }
    if (prev == NULL)
        *tete = cur->suivant;
    else
        prev->suivant = cur->suivant;

    free(cur);
    sauvegarderFichier(*tete);
    printf("Produit supprime avec succes!\n");
}
int main() {
    Noeud *tete = chargerFichier();
    int nc;
    printf("Bienvenue dans la gestion de stock.\n");
    printf("\n 1 - Ajouter un produit au stock");
    printf("\n 2 - Afficher la liste des produits");
    printf("\n 3 - Rechercher un produit");
    printf("\n 4 - Modifier un produit");
    printf("\n 5 - Supprimer un produit");
    printf("\n 6 - Quitter l application \n");
    printf("Votre choix : ");
    scanf("%d", &nc);

    while (nc >= 1 && nc <= 5) {
        if      (nc == 1) tete = traitement1(tete);
        else if (nc == 2) traitement2(tete);
        //else if (nc == 3) tete = traitement3(tete);
        else if (nc == 3) traitement3(tete);
        else if (nc == 4) tete = traitement4(tete);
        //else              tete = traitement5(tete);
        else			  traitement5(&tete);
        printf("\n Votre choix : ");
        scanf("%d", &nc);
    }

    if (nc == 6)
        printf("Au revoir!\n");

    libererListe(tete);
    return 0;
}
