#include "my_malloc.h"
#include "blocks_management.h"

/* Pointeur vers le premier bloc */
static ptr_bloc memoire = NULL;

/**
 * Malloc
 * @param size
 */
void *malloc(size_t size)
{
	ptr_bloc bloc;

	if(size == 0)
		return NULL;

	bloc = find_bloc(size);

	/* Aucun bloc */
	if(!bloc) {
		bloc = create_bloc(size);
		if(bloc) {
			insert_bloc(bloc);
		} else {
			return NULL;
		}
		cut_bloc(bloc, size);
		bloc->state = NOTFREE;
		return bloc->user_space;
	}

	if(bloc->user_space_size > size)
		cut_bloc(bloc, size);
	bloc->state = NOTFREE;
	return bloc->user_space;
}

/**
 * Free
 * @param ptr
 */
void free(void *ptr)
{
	ptr_bloc bloc;

	bloc = user_space_to_bloc(ptr);

	bloc->state = FREE;

	if(bloc->next_bloc->state == FREE && bloc < bloc->next_bloc) {
		fusion_bloc(bloc, bloc->next_bloc);
	}

	if(bloc->preview_bloc->state == FREE && bloc->preview_bloc < bloc) {
		fusion_bloc(bloc->preview_bloc, bloc);
	}
}

/**
 * Calloc
 * @param nmemb Nombre d'elements
 * @param size Taille des elements
 */
void *calloc(size_t nmemb, size_t size)
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
void *realloc(void *ptr, size_t size)
{
	ptr_bloc old_bloc;
	ptr_bloc new_bloc;
	void *new_espace_utilisateur;

	if(!ptr)
		return mon_malloc(size);

	old_bloc = user_space_to_bloc(ptr);

	/* Si l'espace dans le bloc est deja suffisant */
	if(old_bloc->user_space_size >= size) {
		/* Si il y a assez de place, on coupe le bloc */
		if(old_bloc->user_space_size > sizeof(struct memory_bloc) + size)
			cut_bloc(old_bloc, size);
		return ptr;
	} else {
		/* TODO Essayer de fusionner blocs adjascents */
		new_espace_utilisateur = mon_malloc(size);
		if(!new_espace_utilisateur)
			return NULL;
		new_bloc = user_space_to_bloc(new_espace_utilisateur);
		copy_bloc(old_bloc, new_bloc);
		mon_free(ptr);
		return new_espace_utilisateur;
	}
}