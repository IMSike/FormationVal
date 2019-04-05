#include "my_malloc.h"
#include "blocks_management.h"
#include "my_outputs.h"

/**
 * Malloc
 * @param size
 */
void *malloc(size_t size)
{
	my_putstr("entree_malloc\n");
	ptr_bloc bloc;

	if(size == 0)
		return NULL;

	bloc = find_bloc(size);

	/* Aucun bloc */
	if(!bloc) {
		my_putstr("malloc_test__aucunBloc\n");
		bloc = create_bloc(size);
		if(bloc) {
			my_putstr("malloc_test__creationBloc\n");
			insert_bloc(bloc);
			print_alloc();
		} else {
			my_putstr("malloc_test__echecCreation\n");
			return NULL;
		}
		cut_bloc(bloc, size);
		my_putstr("malloc_test__cutBloc()_exec\n");
		bloc->state = NOTFREE;
		return bloc->user_space;
	}
	else
		print_bloc(bloc);

	if(bloc->user_space_size > size)
		cut_bloc(bloc, size);
	bloc->state = NOTFREE;
	print_alloc();
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
	ptr = malloc(total);

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
		return malloc(size);

	old_bloc = user_space_to_bloc(ptr);

	/* Si l'espace dans le bloc est deja suffisant */
	if(old_bloc->user_space_size >= size) {
		/* Si il y a assez de place, on coupe le bloc */
		if(old_bloc->user_space_size > sizeof(struct memory_bloc) + size)
			cut_bloc(old_bloc, size);
		return ptr;
	} else {
		/* TODO Essayer de fusionner blocs adjascents */
		new_espace_utilisateur = malloc(size);
		if(!new_espace_utilisateur)
			return NULL;
		new_bloc = user_space_to_bloc(new_espace_utilisateur);
		copy_bloc(old_bloc, new_bloc);
		free(ptr);
		return new_espace_utilisateur;
	}
}