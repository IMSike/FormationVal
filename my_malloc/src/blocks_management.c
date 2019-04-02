#include "my_malloc.h"
#include "blocks_management.h"
 
/* Pointeur vers le premier bloc */
static entete_p memoire = NULL;
 
/* Mallopt */
static int maxfast = 0;
static int numblks = 100;
static int mallopt_premiere_allocation = 0; /* 0 si aucune premiere petite allocation a ete faite, 1 sinon */

/**
 * Cree un nouveau bloc a l'aide d'un appel systeme
 * La taille du bloc cree est au minimum TAILLE_MIN pour eviter trop
 * d'appels a sbrk
 * @param size La taille du bloc a creer
 * @return Un nouveau bloc ou NULL si erreur
 */
static entete_p creer_bloc(size_t size)
{
	char *extension;
	entete_p extension_entete;
 
	if(size < TAILLE_MIN)
		size = TAILLE_MIN;
 
	extension = sbrk(size + TAILLE_ENTETE);
	if(extension ==  (void *) -1)
		return NULL;
 
	extension_entete = (entete_p) extension;
	extension_entete->taille_espace_utilisateur = size;
	extension_entete->espace_utilisateur = extension + TAILLE_ENTETE;
	extension_entete->entete_precedent = extension_entete;
	extension_entete->entete_suivant = extension_entete;
	extension_entete->etat = LIBRE;
 
	return extension_entete;
}
 
/**
 * Renvoie le premier entete de bloc de taille au moins egale a size dans la
 * liste des blocs vides, si aucun bloc de taille suffisante n'est trouve, on
 * renvoie NULL
 * @param size La taille minimale du bloc a trouver
 * @return Le premier entete trouve ou NULL si aucun bloc
 */
static entete_p recherche_bloc(size_t size)
{
	entete_p tmp;
 
	if(!memoire)
		return NULL;
 
	tmp = memoire;
 
	while(tmp->taille_espace_utilisateur <= size || tmp->etat == OCCUPE) {
		tmp = tmp->entete_suivant;
		if(tmp == memoire) /* si on a fait le tour de la liste */
			return NULL;
	}
 
	return tmp;
}
 
/**
 * Insere le bloc dans la memoire, dans l'ordre des adresses
 * @param bloc Le bloc a inserer
 */
static void insere_bloc(entete_p bloc)
{
	entete_p courant;
 
	/* Premiere insertion */
	if(!memoire)
		memoire = bloc;
 
	courant = memoire;
 
	while(courant < bloc) {
		courant = courant->entete_suivant;
		if(courant == memoire) /* Tour complet de liste */
			break;
	}
 
	bloc->entete_precedent = courant->entete_precedent;
	bloc->entete_suivant = courant;
 
	courant->entete_precedent->entete_suivant = bloc;
	courant->entete_precedent = bloc;
}
 
/**
 * Il se peut que la taille du bloc soit superieure a size: par
 * exemple, si le reste est <= TAILLE_ENTETE, on ne peut pas creer de second bloc,
 * donc on laisse le reste dans le premier bloc
 * @param bloc Le bloc a couper
 * @param size La taille du bloc a creer
 */
static void coupe_bloc(entete_p bloc, size_t size)
{
	entete_p nouveau_entete;
	char *nouveau;
 
	/* bonne taille, pas besoin de couper */
	if(bloc->taille_espace_utilisateur == size)
		return;
 
	/* Si il n'y a pas assez de place pour creer un second bloc de reste */
	if(bloc->taille_espace_utilisateur - size <= TAILLE_ENTETE)
		return;
 
	nouveau = (char *)bloc + TAILLE_ENTETE + size;
	nouveau_entete = (entete_p) nouveau;
	nouveau_entete->taille_espace_utilisateur = bloc->taille_espace_utilisateur - size - TAILLE_ENTETE;
	nouveau_entete->espace_utilisateur = nouveau + TAILLE_ENTETE;
	nouveau_entete->entete_precedent = bloc;
	nouveau_entete->entete_suivant = bloc->entete_suivant;
	nouveau_entete->etat = LIBRE;
 
	bloc->entete_suivant->entete_precedent = nouveau_entete;
	bloc->taille_espace_utilisateur = size;
	bloc->entete_suivant = nouveau_entete;
	bloc->etat = LIBRE;
}
 
/**
 * Fusionne les bloc b1 et b2, b1 doit etre juste avant b2
 * Apres l'appel de la fonction, b2 est "detruit", b1 prend la place
 * des deux blocs
 * @param b1
 * @param b2
 */
static void fusion_bloc(entete_p b1, entete_p b2)
{
	/* TODO Et si les deux blocs ne sont pas contigus ??? */
	b1->taille_espace_utilisateur = b1->taille_espace_utilisateur + TAILLE_ENTETE + b2->taille_espace_utilisateur;
	b1->entete_suivant = b2->entete_suivant;
	b2->entete_suivant->entete_precedent = b1;
	b1->etat = LIBRE;
}
 
/**
 * Copie l'espace utilisateur du bloc "old" dans l'espace utilisateur du bloc "new"
 * La taille de l'espace de new doit etre superieure ou egale a celle de "old"
 * @param old_bloc L'ancien bloc
 * @param new_bloc Le nouveau bloc cree
 */
static void copie_bloc(entete_p old_bloc, entete_p new_bloc)
{
	char *old_espace_utilisateur, *new_espace_utilisateur;
	unsigned int i;
 
	if(new_bloc->taille_espace_utilisateur < old_bloc->taille_espace_utilisateur)
		return;
 
	old_espace_utilisateur = old_bloc->espace_utilisateur;
	new_espace_utilisateur = new_bloc->espace_utilisateur;
 
	for(i = 0; i < old_bloc->taille_espace_utilisateur; i++)
		new_espace_utilisateur[i] = old_espace_utilisateur[i];
}
 
/**
 * Renvoie le bloc qui correspond au pointeur d'espace utilisateur
 * passe en parametre
 * @return Le bloc correspondant
 */
static entete_p espace_utilisateur_to_bloc(void *ptr)
{
	char *tmp;
	entete_p bloc;
 
	tmp = ptr;
	tmp -= TAILLE_ENTETE;
	bloc = (entete_p)tmp;
 
	return bloc;
}
 
/**
 * Affiche le contenu de la memoire (adresses des blocs, taille, debut des
 * espaces utilisateur, etat...)
 */
static void affiche_memoire()
{
	entete_p tmp;
	int nb_bloc;
 
	if(!memoire) {
		printf("Memoire vide\n\n\n");
		return;
	}
 
	tmp = memoire;
	nb_bloc = 1;
 
	do {
		printf("Bloc %d\n", nb_bloc);
		affiche_bloc(tmp);
		tmp = tmp->entete_suivant;
		nb_bloc++;
	} while(tmp != memoire); /* On continue tant qu'on n'est pas revenu au debut de la liste circulaire */
}
 
/**
 * Affiche les informations du bloc
 * @param bloc Le bloc
 */
static void affiche_bloc(entete_p bloc)
{
	if(!bloc) {
		printf("Bloc NULL\n\n\n");
		return;
	}
 
	printf("Adresse: %p\n", bloc);
	printf("Espace utilisateur: %p\n", bloc->espace_utilisateur);
	printf("Taille: %d\n", bloc->taille_espace_utilisateur);
	printf("Etat: %d\n", bloc->etat);
	printf("Entete precedent: %p\n", bloc->entete_precedent);
	printf("Entete suivant: %p\n", bloc->entete_suivant);
	printf("\n\n");
}