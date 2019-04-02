#include "my_malloc.h"
#include "blocks_management.h"
 
/* Pointeur vers le premier bloc */
static entete_p memoire = NULL;
 
/* Mallopt */
static int maxfast = 0;
static int numblks = 100;
static int mallopt_premiere_allocation = 0; /* 0 si aucune premiere petite allocation a ete faite, 1 sinon */
 
/**
 * Malloc
 * @param size
 */
void *mon_malloc(size_t size)
{	
	entete_p bloc;
 
	if(size == 0)
		return NULL;
 
	bloc = recherche_bloc(size);
 
	/* Aucun bloc */
	if(!bloc) {
		bloc = creer_bloc(size);
		if(bloc) {
			insere_bloc(bloc);
		} else {
			return NULL;
		}
		coupe_bloc(bloc, size);
		bloc->etat = OCCUPE;
		return bloc->espace_utilisateur;
	}
 
	if(bloc->taille_espace_utilisateur > size)
		coupe_bloc(bloc, size);
	bloc->etat = OCCUPE;
	return bloc->espace_utilisateur;
}
 
/**
 * Free
 * @param ptr
 */
void mon_free(void *ptr)
{
	entete_p bloc;
 
	bloc = espace_utilisateur_to_bloc(ptr);
 
	bloc->etat = LIBRE;
 
	if(bloc->entete_suivant->etat == LIBRE && bloc < bloc->entete_suivant) {
		fusion_bloc(bloc, bloc->entete_suivant);
	}
 
	if(bloc->entete_precedent->etat == LIBRE && bloc->entete_precedent < bloc) {
		fusion_bloc(bloc->entete_precedent, bloc);
	}
}
 
/**
 * Calloc
 * @param nmemb Nombre d'elements
 * @param size Taille des elements
 */
void *mon_calloc(size_t nmemb, size_t size)
{
	size_t total;
	void *ptr;
 
	total = nmemb * size;
	ptr = mon_malloc(total);
 
	if(!ptr)
		return NULL;
 
	return memset(ptr, 0, total); /* TODO ? */
}
 
/**
 * Realloc
 * @param ptr
 * @param size
 */
void *mon_realloc(void *ptr, size_t size)
{
	entete_p old_bloc;
	entete_p new_bloc;
	void *new_espace_utilisateur;
 
	if(!ptr)
		return mon_malloc(size);
 
	old_bloc = espace_utilisateur_to_bloc(ptr);
 
	/* Si l'espace dans le bloc est deja suffisant */
	if(old_bloc->taille_espace_utilisateur >= size) {
		/* Si il y a assez de place, on coupe le bloc */
		if(old_bloc->taille_espace_utilisateur > TAILLE_ENTETE + size)
			coupe_bloc(old_bloc, size);
		return ptr;
	} else {
		/* TODO Essayer de fusionner blocs adjascents */
		new_espace_utilisateur = mon_malloc(size);
		if(!new_espace_utilisateur)
			return NULL;
		new_bloc = espace_utilisateur_to_bloc(new_espace_utilisateur);
		copie_bloc(old_bloc, new_bloc);
		mon_free(ptr);
		return new_espace_utilisateur;
	}
}
 
/**
 * Mallopt
 * @param cmd La variable a modifier
 * @param val La nouvelle valeur de la variable
 * @return 0 si erreur, 1 sinon
 */
int mon_mallopt(int cmd, int val)
{
	/* Si un premier bloc a deja ete alloue */
	if(mallopt_premiere_allocation == 1)
		return 0;
 
	if(val < 0)
		return 0;
 
	switch(cmd) {
		case M_MXFAST:
			maxfast = val;
			break;
		case M_NLBLOCKS:
			numblks = val;
			break;
		default:
			return 0;
	}
 
	return 1;
}
 
