#include "my_malloc.h"
#include "blocks_management.h"

/* Pointeur vers le premier bloc */
static ptr_bloc memoire = NULL;

/**
 * Cree un nouveau bloc a l'aide d'un appel systeme
 * La taille du bloc cree est au minimum TAILLE_MIN pour eviter trop
 * d'appels a sbrk
 * @param size La taille du bloc a creer
 * @return Un nouveau bloc ou NULL si erreur
 */
ptr_bloc create_bloc(size_t size)
{
	char *extension;
	ptr_bloc tmp_bloc;

	if(size < TAILLE_MIN)
		size = TAILLE_MIN;

	extension = sbrk(size + sizeof(struct memory_bloc));
	if(extension ==  (void *) -1)
		return NULL;

	tmp_bloc = (ptr_bloc) extension;
	tmp_bloc->user_space_size = size;
	tmp_bloc->user_space = extension + sizeof(struct memory_bloc);
	tmp_bloc->preview_bloc = tmp_bloc;
	tmp_bloc->next_bloc = tmp_bloc;
	tmp_bloc->state = FREE;

	return tmp_bloc;
}

/**
 * Renvoie le premier entete de bloc de taille au moins egale a size dans la
 * liste des blocs vides, si aucun bloc de taille suffisante n'est trouve, on
 * renvoie NULL
 * @param size La taille minimale du bloc a trouver
 * @return Le premier entete trouve ou NULL si aucun bloc
 */
ptr_bloc find_bloc(size_t size)
{
	ptr_bloc tmp_bloc;

	if(!memoire)
		return NULL;

	tmp_bloc = memoire;

	while(tmp_bloc->user_space_size <= size || tmp_bloc->state == NOTFREE) {
		tmp_bloc = tmp_bloc->next_bloc;
		if(tmp_bloc == memoire) /* si on a fait le tour de la liste */
			return NULL;
	}

	return tmp_bloc;
}

/**
 * Insere le bloc dans la memoire, dans l'ordre des adresses
 * @param bloc Le bloc a inserer
 */
void insert_bloc(ptr_bloc bloc)
{
	ptr_bloc current_bloc;

	/* Premiere insertion */
	if(!memoire)
		memoire = bloc;

	current_bloc = memoire;

	while(current_bloc < bloc) {
		current_bloc = current_bloc->next_bloc;
		if(current_bloc == memoire) /* Tour complet de liste */
			break;
	}

	bloc->preview_bloc = current_bloc->preview_bloc;
	bloc->next_bloc = current_bloc;

	current_bloc->preview_bloc->next_bloc = bloc;
	current_bloc->preview_bloc = bloc;
}

/**
 * Il se peut que la taille du bloc soit superieure a size: par
 * exemple, si le reste est <= sizeof(struct memory_bloc), on ne peut pas creer de second bloc,
 * donc on laisse le reste dans le premier bloc
 * @param bloc Le bloc a couper
 * @param size La taille du bloc a creer
 */
void cut_bloc(ptr_bloc bloc, size_t size)
{
	ptr_bloc new_bloc;
	char *new;

	/* bonne taille, pas besoin de couper */
	if(bloc->user_space_size == size)
		return;

	/* Si il n'y a pas assez de place pour creer un second bloc de reste */
	if(bloc->user_space_size - size <= sizeof(struct memory_bloc))
		return;

	new = (char *)bloc + sizeof(struct memory_bloc) + size;
	new_bloc = (ptr_bloc) new;
	new_bloc->user_space_size = bloc->user_space_size - size - sizeof(struct memory_bloc);
	new_bloc->user_space = new + sizeof(struct memory_bloc);
	new_bloc->preview_bloc = bloc;
	new_bloc->next_bloc = bloc->next_bloc;
	new_bloc->state = FREE;

	bloc->next_bloc->preview_bloc = new_bloc;
	bloc->user_space_size = size;
	bloc->next_bloc = new_bloc;
	bloc->state = FREE;
}

/**
 * Fusionne les bloc b1 et b2, b1 doit etre juste avant b2
 * Apres l'appel de la fonction, b2 est "detruit", b1 prend la place
 * des deux blocs
 * @param b1
 * @param b2
 */
void fusion_bloc(ptr_bloc b1, ptr_bloc b2)
{
	/* TODO Et si les deux blocs ne sont pas contigus ??? */
	b1->user_space_size = b1->user_space_size + sizeof(struct memory_bloc) + b2->user_space_size;
	b1->next_bloc = b2->next_bloc;
	b2->next_bloc->preview_bloc = b1;
	b1->state = FREE;
}

/**
 * Copie l'espace utilisateur du bloc "old" dans l'espace utilisateur du bloc "new"
 * La taille de l'espace de new doit etre superieure ou egale a celle de "old"
 * @param old_bloc L'ancien bloc
 * @param new_bloc Le nouveau bloc cree
 */
void copy_bloc(ptr_bloc old_bloc, ptr_bloc new_bloc)
{
	char *old_user_space, *new_user_space;
	unsigned int i;

	if(new_bloc->user_space_size < old_bloc->user_space_size)
		return;

	old_user_space = old_bloc->user_space;
	new_user_space = new_bloc->user_space;

	for(i = 0; i < old_bloc->user_space_size; i++)
		new_user_space[i] = old_user_space[i];
}

/**
 * Renvoie le bloc qui correspond au pointeur d'espace utilisateur
 * passe en parametre
 * @return Le bloc correspondant
 */
ptr_bloc user_space_to_bloc(void *ptr)
{
	char *tmp;
	ptr_bloc bloc;

	tmp = ptr;
	tmp -= sizeof(struct memory_bloc);
	bloc = (ptr_bloc)tmp;

	return bloc;
}

/**
 * Affiche le contenu de la memoire (adresses des blocs, taille, debut des
 * espaces utilisateur, etat...)
 */
void print_alloc(void)
{
	ptr_bloc tmp;
	int nb_bloc;

	if(!memoire) {
		printf("Memoire vide\n\n\n");
		return;
	}

	tmp = memoire;
	nb_bloc = 1;

	do {
		printf("Bloc %d\n", nb_bloc);
		print_bloc(tmp);
		tmp = tmp->next_bloc;
		nb_bloc++;
	} while(tmp != memoire); /* On continue tant qu'on n'est pas revenu au debut de la liste circulaire */
}

/**
 * Affiche les informations du bloc
 * @param bloc Le bloc
 */
void print_bloc(ptr_bloc bloc)
{
	if(!bloc) {
		printf("Bloc NULL\n\n\n");
		return;
	}

	printf("Adresse: %p\n", bloc);
	printf("Espace utilisateur: %p\n", bloc->user_space);
	printf("Taille: %d\n", bloc->user_space_size);
	printf("Etat: %d\n", bloc->state);
	printf("Entete precedent: %p\n", bloc->preview_bloc);
	printf("Entete suivant: %p\n", bloc->next_bloc);
	printf("\n\n");
}