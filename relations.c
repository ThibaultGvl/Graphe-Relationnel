#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define OMEGA 0
#define NTYPE 17
#define TYPESTRING OMEGA, "relation inconnue", "frère ou soeur", "cousin ou cousine de ", "père ou mère de", "oncle ou tante de ", "époux ou épouse de", "ami(e) de ", "vit avec ", "connait ", "supérieur de", "collègue de ", "locataire de ", "travaille à ", "est propriétaire de ", "est situé à ", "découvert à " 
#define LONG_NOM_MAX 128

//Création d'un tableau pour la représentation de la table statique par adressage associatif
char* tab_type_string[NTYPE] = { TYPESTRING };
//enum qui défini les différents types de relations entre sommets
typedef enum {
    FRERE = 2, COUSIN, PARENT,
    ONCLE, EPOUX, AMI, VIT, CONNAIT, CHEF,
    COLLEGUE, LOCATAIRE, TRAVAILLE,
    PROPRIETAIRE, SITUE, DECOUVERT
} rtype;

//On regarde en fonction de l'enum si l'id passé en paramètre correspond à un lien de parenté
//Pré : Aucune
bool est_lien_parente(rtype id) {
    bool est_parent = false;
    if (id >= 2 && id <= 6) {
        est_parent = true;
    }
    return est_parent;
}

//On regarde en fonction de l'enum si l'id passé en paramètre correspond à un lien professionnel
//Pré : Aucune
bool est_lien_professionel(rtype id) {
    bool est_pro = false;
    if (id == 10 || id == 11) {
        est_pro = true;
    }
    return est_pro;
}
//On regarde en fonction de l'enum si l'id passé en paramètre correspond à une connaissance
//Pré : Aucune
bool est_lien_connaissance(rtype id) {
    bool connaissance = false;
    if (id >= 7 && id <= 9) {
        connaissance = true;
    }
    return connaissance;
}

//On retourne l'élément de la chaine de la table associé à l'id passé paramètre
//Pré : Aucune
char* toString(rtype id) {
    //On vérifie que l'id correponde à un index tu tableau
    if (id >= NTYPE || id < 0) {
        printf("Erreur: type de relation invalide");
        exit(EXIT_FAILURE);
    }
    return tab_type_string[id];
}

//structure liste chaînée d'adjacences
//Pré : Aucune
typedef struct s_node {
    void* val; // pointeur quelconque
    struct s_node* suiv; // élément suivant
} *listeg;

//On créé une listeg NULL pour l'instant pas besoin d'initialisation
//Pré : Aucune
listeg listegnouv() {
    return NULL;
}

//Fonction d'ajout en tête dans une liste de pointeurs nuls
//Pré : Aucune
listeg adjtete(listeg lst, void* x) {
    //On alloue l'espace mémoire nécéssaire
    listeg nouv = (listeg)malloc(sizeof(struct s_node));
    //On stocke assigne la valeur de x à val
    nouv->val = x;
    //On place nouv en tête
    nouv->suiv = lst;
    return nouv;
}
//Fonction d'ajout en queue dans une liste de pointeurs nuls
//Pré : Aucune
listeg adjqueue(listeg lst, void* x) {
    //On regarde si on est à la fin de la liste on ajoute en tête
    if (lst == NULL) return adjtete(lst, x);
    //Et récursivement on ajoute en queue
    lst->suiv = adjqueue(lst->suiv, x);
    return lst;
}


//Fonction de suppression en tête dans une liste de pointeurs nuls
//Pré : Aucune
listeg suptete(listeg lst) {
    //L'élément suivant est placé en tête
    listeg nouv_tete = lst->suiv;
    //On libére l'espace mémoire alloué pour lst
    free(lst);
    //On renvoie la nouvelle tête
    return nouv_tete;
}

//On renvoie l'élément en tête
//Pré : Aucune
void* tete(listeg lst) {
    return lst->val;
}

//On renvoie l'élément en queue en utilisant la fonction tete et en appelant récursivement la fonction queue
//Pré : Aucune
void* queue(listeg lst) {
    if (lst == NULL) return tete(lst);
    lst->suiv = queue(lst->suiv);
    return lst->val;
}

//Fonction de recherche dans la liste d'adjacences
//Pré : Aucune
listeg rech(listeg lst, void* x, int(*comp)(void*, void*)) {
    //On alloue l'espace nécéssaire pour la listeg à renvoyer
    listeg nouv = listegnouv();
    //On défini une listeg tampon pour parcourir la listeg
    listeg tmp = lst;
    //On parcours la liste !
    while (tmp != NULL) {
        //On compare la tête à l'élément recherché
        if (comp((void*)tete(tmp), x) == 0) {
            //Si c'est égale, on assigne nouv à l'élément trouvé
            nouv = adjtete(nouv, tete(tmp));
        }
        //Sinon on continue de parcourir
        tmp = tmp->suiv;
    }
    //On retourne l'élément trouvé
    return nouv;
}

//Fonction récursive de calcul de longueur sur liste chainée
//Pré : Aucune
int longueur(listeg lst) {
    if (lst == NULL) return 0;
    return 1 + longueur(lst->suiv);
}

//Fonction de test de vacuité
//Pré : Aucune
bool estvide(listeg lst) {
    return lst == NULL;
}

//Fonction recursive de déstruction de la listeg
//Pré : Aucune
void detruire(listeg lst) {
    while (lst != NULL) lst = suptete(lst);
}

typedef enum { PERSONNE = 0, OBJET = 1, ADRESSE = 2, VILLE = 3 } etype;
char* etype_tab[4] = { "personne", "objet", "adresse", "ville" };
typedef struct s_entite {
    char nom[LONG_NOM_MAX]; // le nom de l’entité p.ex « Peugeot 106 »
    etype ident; // l’identifiant associé, p.ex OBJET
} *Entite;

typedef struct s_sommet {
    struct s_node* larcs; //Pointeur sur une liste chainée de noeuds dont chacun pointe sur un arc
    Entite x; //Entite du sommet
} *Sommet;

typedef struct s_arc {
    rtype t; //Type de relation
    Entite x; // Entite avec qui le sommet à une relation
} *Arc;

typedef struct s_relations {
    listeg rel; //Pointeur sur une liste chainée de noeuds dont chacun pointe sur un sommet
} *Relations;

//Constructeur d'entité
//Pré : Aucune
Entite creerEntite(char* s, etype e) {
    //On vérifie si le nom est valide
    if (strlen(s) > LONG_NOM_MAX) {
        printf("Erreur le nom est trop long");
        exit(EXIT_FAILURE);
    }
    //On vérifie si le type est valide
    if (e < 0 || e > 3) {
        printf("Erreur le type est invalide");
        exit(EXIT_FAILURE);
    }
    //On créé notre nouvelle entite
    Entite nouv = (Entite)malloc(sizeof(struct s_entite));
    strcpy(nouv->nom, s);
    nouv->ident = e;
    return nouv;
}
//Constructeur de sommet
//Pré : Aucune
Sommet nouvSommet(Entite e) {
    Sommet nouv = (Sommet)malloc(sizeof(struct s_sommet));
    nouv->x = e;
    nouv->larcs = listegnouv();
    return nouv;
}

//Constructeur d'arc
//Pré : Aucune
Arc nouvArc(Entite e, rtype type) {
    //On vérifie si le type de relation est valide
    if (type >= NTYPE || type < 0) {
        printf("Erreur: type de relation invalide");
        exit(EXIT_FAILURE);
    }
    Arc nouv = (Arc)malloc(sizeof(struct s_arc));
    nouv->t = type;
    nouv->x = e;
    return nouv;
}

//Initialisation de la structure relation (du graphe)
//Pré : Aucune
void relationInit(Relations* g) {
    (*g) = (Relations)malloc(sizeof(struct s_relations));
    (*g)->rel = listegnouv();
}

//Fonction de libération de mémoire du graphe relations
//Pré : Aucune
void relationFree(Relations* g) {
    //On récupère la liste des sommets graphe
    listeg noeud_rel = (*g)->rel;
    //On parcourt cette liste
    while (noeud_rel != NULL) {
        //On récupère le sommet du noeud courant 
        Sommet s = (Sommet)noeud_rel->val;
        //On récupère la liste chainée d'arcs du sommet
        listeg noeud_arc = s->larcs;
        //On la parcourt
        while (noeud_arc != NULL) {
            //On récupère l'arc du noeud courant
            Arc a = (Arc)noeud_arc->val;
            //On le libère
            free(a);
            //Au suivant
            noeud_arc = noeud_arc->suiv;
        }
        //On libère la liste chainée d'arcs du sommet courant
        detruire(s->larcs);
        //On libère l'entité du sommet courant
        free(s->x);
        //On libère le sommet courant
        free(s);
        //Au suivant
        noeud_rel = noeud_rel->suiv;
    }
    //On écrase la liste chainée de sommets
    detruire((*g)->rel);
    //On libère le pointeur vers le graphe
    free(*g);
}


//Fonction de comparaison Entite
int compEntite(void* e, void* string) {
    return strcmp(((Entite)e)->nom, (char*)string);
}

//Fonction de comparaison Sommet
int compSommet(void* s, void* string) {
    return strcmp(((Sommet)s)->x->nom, (char*)string);
}

//Fonction de comparaison Arc
int compArc(void* a, void* string) {
    return strcmp(((Arc)a)->x->nom, (char*)string);
}
//Fonction d'ajout d'entité dans le graphe
void adjEntite(Relations g, char* nom, etype t) {
    //On vérifie si le graphe en paramètre existe
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //Création Entite
    Entite nouv = creerEntite(nom, t);
    //On vérifie qu'il n'existe pas déjà une entité sous ce nom
    listeg tmp = rech(g->rel, (void*)nom, compEntite);
    if (longueur(tmp) == 0) {
        //Si ce n'est pas le cas on créé un sommet pour cette entité
        Sommet s = nouvSommet(nouv);
        //On l'ajoute en tête de la liste de noeuds
        g->rel = adjtete(g->rel, (void*)s);
    }
    detruire(tmp);
}

// PRE: id doit être cohérent avec les types des sommets correspondants à x et y
// p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE: strcmp(nom1,nom2)!=0
//Fonction d'ajout de relations entre sommets (arc)
void adjRelation(Relations g, char* nom1, char* nom2, rtype id) {
    //On vérifie si le graphe en paramètre existe
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On cherche le sommet portant le nom1
    listeg sommets = rech(g->rel, (void*)nom1, compSommet);
    //Si le sommet n'existe pas, on affiche une erreur
    if (estvide(sommets)) {
        printf("Pas de sommet de nom : %s", nom1);
        exit(EXIT_FAILURE);
    }
    Sommet tmp = (Sommet)(sommets->val);
    detruire(sommets);
    listeg nouv = rech(tmp->larcs, (void*)nom2, compArc);
    Arc arcn = nouv != NULL ? nouv->val : NULL;
    //On cherche le sommet portant le nom2
    sommets = rech(g->rel, (void*)nom2, compSommet);
    //Si le sommet n'existe pas, on affiche une erreur
    if (estvide(sommets)) {
        printf("Pas de sommet de nom : %s", nom2);
        exit(EXIT_FAILURE);
    }
    Sommet cible = (Sommet)(sommets->val);
    detruire(sommets);
    listeg nouv2 = rech(cible->larcs, (void*)nom1, compArc);
    Arc arcn2 = nouv != NULL ? nouv->val : NULL;
    //On regarde s'il existe déjà une relation entre les individus
    if (nouv == NULL) {
        //Si non : On créé un nouvel arc de nom1 à nom2
        Entite e = (Entite)cible->x;
        arcn = nouvArc(e, id);
        tmp->larcs = adjtete(tmp->larcs, (void*)arcn);
    }
    else {
        //Si oui : On change le type de relation qui les unis
        arcn->t = id;
        detruire(nouv);
    }
    if (nouv2 == NULL) {
        //Si non : On créé un nouvel arc de nom2 à nom1
        Entite e1 = (Entite)tmp->x;
        arcn2 = nouvArc(e1, id);
        cible->larcs = adjtete(cible->larcs, (void*)arcn2);
    }
    else {
        //Si oui : On change le type de relation qui les unis
        arcn2->t = id;
        detruire(nouv2);
    }
}
//Fonction qui renvoie les arcs d'un Sommets
listeg en_relation(Relations g, char* x) {
    //On vérifie si le graphe en paramètre existe
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On recherche le sommet dans la liste d'adjacences relative aux sommets
    listeg tmp = rech(g->rel, (void*)x, compSommet);
    //S'il n'existe pas on affiche une erreur et on quitte
    if (estvide(tmp)) {
        printf("Pas de sommet de nom : %s", x);
        exit(EXIT_FAILURE);
    }
    Sommet cur = (Sommet)tmp->val;
    detruire(tmp);
    //On retourne sa liste d'adjacences relative aux arcs
    return cur->larcs;
}

//Fonction qui renvoie tous les chemins de longueur 2 de x à y
listeg chemin2(Relations g, char* x, char* y) {
    // On vérifie si le graphe en paramètre existec
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }

    // On recherche le sommet dans la liste d'adjacences relative aux sommets
    listeg tmp = rech(g->rel, (void*)x, compSommet);
    // S'il n'existe pas, on affiche une erreur et on quitte
    if (estvide(tmp)) {
        printf("Pas de sommet de nom : %s", x);
        exit(EXIT_FAILURE);
    }
    // On libère tmp
    detruire(tmp);

    // On fait la même chose pour y
    tmp = rech(g->rel, (void*)y, compSommet);
    if (estvide(tmp)) {
        printf("Pas de sommet de nom : %s", y);
        exit(EXIT_FAILURE);
    }
    // On libère tmp qui ne nous servira plus
    detruire(tmp);

    // On récupère les arcs des deux sommets
    listeg arcsx = en_relation(g, x);
    listeg arcsy = en_relation(g, y);
    // On crée notre liste vide de retour
    listeg chem = listegnouv();

    // On itère sur les arcs de x et les arcs de y
    while (arcsx != NULL) {
        //On recherche dans les arcs de x s'il existe un des arcs de y
        listeg exist = rech(arcsy, (void*)((Arc)tete(arcsx))->x->nom, compArc);
        //Si c'est le cas
        if (!estvide(exist)) {
            //On ajoute l'entite commune 
            chem = adjtete(chem, ((Arc)tete(arcsx))->x);
        }
        //On libère
        detruire(exist);
        //Au suivant
        arcsx = arcsx->suiv;
    }
    //On retourne les relations communes directes 
    return chem;
}


// PRE : strcmp(x,y)!=0 
// PRE: les sommets correspondants à x et y sont de type PERSONNE 
bool ont_lien_parente(Relations g, char* x, char* y) {
    //On vérifie si le graphe en paramètre existe, si non on affiche une erreur et on quitte
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On récupère les arcs du sommet dont l'entite est de nom x (pas de vérification de celui-ci l'existence voir pré)
    listeg cur = en_relation(g, x);
    //On cherche s'il existe un arc avec une entite de nom y dans les arcs du sommet d'entite de nom x
    listeg y_arc = rech(cur, (void*)y, compArc);
    //Pas de relation directe entre x et y
    if (y_arc == NULL) {
        //On libère y_arcs
        detruire(y_arc);
        //Pas de lien de parenté connu
        return false;
    }
    else {
        //Si on a une relation on regarde si c'est un lien de parenté
        bool retour = est_lien_parente(((Arc)y_arc->val)->t);
        //On libère y_arcs
        detruire(y_arc);
        //On renvoit si oui ou non ils ont un lien de parenté
        return retour;
    }
}

// PRE : strcmp(x,y)!=0 
// PRE: les sommets correspondants à x et y sont de type PERSONNE 
bool se_connaissent(Relations g, char* x, char* y) {
    //On vérifie si le graphe en paramètre existe, si non on affiche une erreur et on quitte
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On cherche récupère les arcs du sommet dont l'entite est de nom x (pas de vérification de celui-ci l'existence voir pré)
    listeg cur = en_relation(g, x);
    //On cherche s'il existe un arc avec une entite de nom y dans les arcs du sommet d'entite de nom x
    listeg y_arc = rech(cur, (void*)y, compArc);
    //On inititalise la valeur de retour à false par défaut
    bool retour = false;
    //Si pas de relation
    if (estvide(y_arc)) {
        //On récupére les relations communes à x et y
        listeg chem = chemin2(g, x, y);
        //On les stocke dans une variable tampon pour les libérer par la suite
        listeg chemtmp = chem;
        //On parcourt les relations
        while (chem != NULL) {
            //Si elles ont toutes les deux un lien de parenté avec la relations commune courante
            if (ont_lien_parente(g, x, ((Entite)chem->val)->nom) && ont_lien_parente(g, y, ((Entite)chem->val)->nom)) {
                //Elles se connaissent
                retour = true;
            }
            //On passe au suivant
            chem = chem->suiv;
        }
        //On libère les relations communes
        detruire(chemtmp);
    }
    else {
        //Si elles ont une relations directe de parenté, connaissance ou professionnel : elles se connaissent
        //Equivalent à retour = true dans la representation actuelle du graphe mais pour être fidèle au cahier des charge :
        retour = est_lien_connaissance(((Arc)y_arc->val)->t) || est_lien_parente(((Arc)y_arc->val)->t) || est_lien_professionel(((Arc)y_arc->val)->t);
    }
    //On libère y_arc
    detruire(y_arc);
    //On retourne le booléen
    return retour;
}

// PRE : strcmp(x,y)!=0 
// PRE: les sommets correspondants à x et y sont de type PERSONNE 
bool se_connaissent_proba(Relations g, char* x, char* y) {
    //On vérifie si le graphe en paramètre existe, si non on affiche une erreur et on quitte
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On inititalise la valeur de retour à false par défaut
    bool connait = false;
    //On récupére les relations communes à x et y
    listeg chem = chemin2(g, x, y);
    //On les stocke dans une variable tampon pour les libérer par la suite
    listeg chemtmp = chem;
    //On parcourt les relations communes 
    while (chem != NULL) {
        //Si l'une des deux et pas l'autre à un lien de parenté avec la relation commune courante
        if (ont_lien_parente(g, x, ((Entite)chem->val)->nom) ^ ont_lien_parente(g, y, ((Entite)chem->val)->nom)) {
            //Elles se connaissent probablement
            connait = true;
        }
        //Au suivant
        chem = chem->suiv;
    }
    //On libère les relations communes
    detruire(chemtmp);
    //On renvoit le boolean
    return connait;
}

// PRE : strcmp(x,y)!=0 
// PRE: les sommets correspondants à x et y sont de type PERSONNE 
bool se_connaissent_peutetre(Relations g, char* x, char* y) {
    //On vérifie si le graphe en paramètre existe, si non on affiche une erreur et on quitte
    if (g == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    //On inititalise la valeur de retour à false par défaut
    bool connait = false;
    //Si elles ne se connaissent ni ne se connaissent probablement
    if (!se_connaissent_proba(g, x, y) && !se_connaissent(g, x, y)) {
        //On récupère les relations communes
        listeg chem = chemin2(g, x, y);
        //Variable tampon pour free
        listeg chemtmp = chem;
        //On les parcourt
        while (chem != NULL) {
            //Si elles connaissent toutes les deux la relation
            if (se_connaissent(g, x, ((Entite)chem->val)->nom) && se_connaissent(g, y, ((Entite)chem->val)->nom)) {
                //Elles se connaissent peut-être
                connait = true;
            }
            //Au suivant
            chem = chem->suiv;
        }
        //On libère
        detruire(chemtmp);
    }
    //On retourne le boolean 
    return connait;
}

//Pré : Aucune
void affichelg(listeg l, void(*aff)(void*)) {
    listeg tmp = l;
    while (tmp != NULL) {
        aff((void*)tete(tmp));
        tmp = tmp->suiv;
    }
}

//Pré : x est un pointeur void vers l'adresse d'une Entite
void afficheEntite(void* x) {
    printf("%s :%s\n", ((Entite)x)->nom, etype_tab[((Entite)x)->ident]);
}

//Pré : x est un pointeur void vers l'adresse d'un Arc
void afficheArc(void* x) {
    printf("--%s-->%s:%s\n", tab_type_string[((Arc)x)->t], ((Arc)x)->x->nom, etype_tab[((Arc)x)->x->ident]);
}

//Pré : Aucune
void affiche_degre_relations(Relations r, char* x) {
    //On vérifie si le graphe en paramètre existe, si non on affiche une erreur et on quitte
    if (r == NULL) {
        printf("Erreur, graphe inexistant.");
        exit(EXIT_FAILURE);
    }
    listeg som = rech(r->rel, (void*)x, compSommet);
    //On vérifie s'il existe un sommet lié à une entité de ce nom dans le graphe
    if (estvide(som)) {
        printf("Pas de sommet de nom : %s", x);
        exit(EXIT_FAILURE);
    }
    //On utilise uniquement pour le test d'existence, on libère
    detruire(som);
    //On affiche le nom de l'entite dont on veut connaitre les relations
    printf("%s\n", x);
    //Liste dans laquelle on stockera les sommets déjà vus
    listeg visite = listegnouv();
    //Liste dans laquelle on stockera les sommets que l'on s'appretera à visiter
    listeg a_traiter = listegnouv();
    //Variable pour afficher le degrès courant de la relation
    int deg = 1;
    //On affiche 1 car on va afficher les relations directes
    printf("-- %d\n", deg);
    //Liste qui contient les sommets ayant une relation de degrès courant avec x, pour l'instant les relations directes
    listeg a_visiter = en_relation(r, x);
    //Variable tampon pour libérer la mémoire
    listeg a_visitersup = listegnouv();
    //On parcourt les sommets à visiter
    while (a_visiter != NULL) {
        //On regarde si on a déjà visité ce sommet
        listeg tmprech = rech(visite, (void*)((Arc)tete(a_visiter))->x->nom, compArc);
        //Si ce n'est pas le cas
        if (estvide(tmprech) && strcmp(((Arc)tete(a_visiter))->x->nom, x) != 0) {
            //On affiche le nom de son entite
            printf("    %s\n", ((Arc)tete(a_visiter))->x->nom);
            //On ajoute dans la liste des arcs visités
            visite = adjqueue(visite, tete(a_visiter));
            //On récupère les arcs de ce sommet
            listeg tmp = en_relation(r, ((Arc)tete(a_visiter))->x->nom);
            //On les parcourt
            while (tmp != NULL) {
                //On regarde si on déjà visité la relation courante du sommet qu'on vient d'afficher
                listeg rechtemp = rech(visite, (void*)((Arc)tete(tmp))->x->nom, compArc);
                //On vérifie aussi si ce n'est pas le sommet courant
                if (estvide(rechtemp) && strcmp(((Arc)tete(tmp))->x->nom, x) != 0) {
                    //On ajoute en bout de file ce nouveau sommet à visiter
                    a_traiter = adjqueue(a_traiter, tete(tmp));
                }
                //On libère la liste utilisée pour vérifier si la relation du sommet qui vient d'être affiché a deja été visité
                detruire(rechtemp);
                //Au suivant
                tmp = tmp->suiv;
            }
        }
        //Au suivant
        a_visiter = a_visiter->suiv;
        //On libère la liste de vérification de l'existence du somment qu'on vient d'afficher (si il n'était pas deja visité)
        detruire(tmprech);
        //On regarde si on a visité tous les arcs de degrès courants
        if (estvide(a_visiter)) {
            //On mets les sommets de degrès suivant dans a_visiter
            a_visiter = a_traiter;
            //Si a_traiter est vide
            if (!estvide(a_traiter)) {
                //On incrèmente le degrès
                deg++;
                //Et on l'affiche
                printf("-- %d\n", deg);
            }
            a_traiter = listegnouv();
            //On libère les sommets qu'on a déjà visités
            detruire(a_visitersup);
            //On récupère les suivant pour les supprimer au prochain passage
            a_visitersup = a_visiter;
        }
        //On a plus rien à traiter on libère tout
        if (estvide(a_visiter) && estvide(a_traiter)) {
            //Deja vide mais meilleure lisibilité
            detruire(a_visiter);
            //On libère les sommets visités
            detruire(visite);
            //Deja vide mais meilleure lisibilité
            detruire(a_traiter);
        }
    }
}


int main()
{
    int i, j;
    Relations r; relationInit(&r);
    // ajouter les entites de l'exemple
    char* tabe[] = { "KARL","LUDOVIC","CELINE","CHLOE","GILDAS","CEDRIC","SEVERINE",
        "PEUGEOT 106" ,"1, RUE DE LA RUE","STRASBOURG" };
    for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
    adjEntite(r, tabe[7], OBJET);
    adjEntite(r, tabe[8], ADRESSE);
    adjEntite(r, tabe[9], VILLE);
    // ajouter les relations de l'exemple
    adjRelation(r, tabe[0], tabe[1], FRERE);
    adjRelation(r, tabe[0], tabe[2], AMI);
    adjRelation(r, tabe[0], tabe[3], CONNAIT);
    adjRelation(r, tabe[0], tabe[5], COUSIN);
    adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
    adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
    adjRelation(r, tabe[3], tabe[4], VIT);
    adjRelation(r, tabe[5], tabe[6], EPOUX);
    adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
    adjRelation(r, tabe[7], tabe[8], DECOUVERT);
    adjRelation(r, tabe[8], tabe[9], SITUE);

    // explorer les relations
    printf("%s est en relation avec:\n", tabe[0]);
    affichelg(en_relation(r, tabe[0]), afficheArc);
    printf("\n");

    for (i = 0; i < 7; i++) for (j = i + 1; j < 10; j++)
    {
        printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
        listeg ch = chemin2(r, tabe[i], tabe[j]);
        affichelg(ch, afficheEntite);
        printf("\n");
        detruire(ch);
    }
    printf("\n\n");

    for (i = 0; i < 10; i++) for (j = i + 1; j < 10; j++)
    {
        printf("<%s> et <%s> ont lien de parente: %s\n",
            tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
    }
    printf("\n");
    for (i = 0; i < 7; i++)
    {
        for (j = i + 1; j < 7; j++)
        {
            printf("<%s> et <%s> se connaissent: %s\n",
                tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
            printf("<%s> et <%s> se connaissent tres probablement: %s\n",
                tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
            printf("<%s> et <%s> se connaissent peut etre: %s\n",
                tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
        }
        printf("\n");
    }

    affiche_degre_relations(r, tabe[3]);

    relationFree(&r);

    printf("\nPRESS RETURN\n");
    char buff[64]; fscanf(stdin, "%s", buff);
    return 0;
}
