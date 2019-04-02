#ifndef _BLOCKS_MANAGEMENT_H_
#define _BLOCKS_MANAGEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAILLE_ENTETE sizeof(struct entete)
#define TAILLE_MIN 1024

#ifndef M_MXFAST
#define M_MXFAST 1
#endif

#ifndef M_NLBLOCKS
#define M_NLBLOCKS 2
#endif

/* Pour designer l'etat d'un bloc */
enum etat {
	LIBRE = 0, OCCUPE = 1
};

/* Represente l'entete d'un bloc */
typedef struct entete entete_t;
typedef entete_t* entete_p;

struct entete {
	size_t taille_espace_utilisateur;
	void *espace_utilisateur;
	entete_p entete_precedent;
	entete_p entete_suivant;
	enum etat etat;
};

/* Prototypes */
static entete_p creer_bloc(size_t size);
static entete_p recherche_bloc(size_t size);
static void insere_bloc(entete_p bloc);
static void coupe_bloc(entete_p bloc, size_t size);
static void fusion_bloc(entete_p b1, entete_p b2);
static void copie_bloc(entete_p old_bloc, entete_p new_bloc);
static entete_p espace_utilisateur_to_bloc(void *ptr);
static void affiche_memoire();
static void affiche_bloc(entete_p bloc);

#endif