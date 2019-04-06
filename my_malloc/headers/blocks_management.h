#ifndef _BLOCKS_MANAGEMENT_H_
#define _BLOCKS_MANAGEMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAILLE_MIN 1024

/* Pour designer l'etat d'un bloc */
typedef enum 	stateOfBloc {FREE = 0, NOTFREE = 1}	enumState;

/* Represente l'entete d'un bloc */
typedef struct 	memory_bloc 	m_bloc;
typedef 	m_bloc* 	ptr_bloc;

struct 	memory_bloc
{
	size_t 		user_space_size;
	void 		*user_space;
	ptr_bloc 	preview_bloc;
	ptr_bloc 	next_bloc;
	enumState 	state;
};

/* Prototypes */
ptr_bloc 	create_bloc(size_t size);
ptr_bloc 	find_bloc(size_t size);
void 		insert_bloc(ptr_bloc bloc);
void 		cut_bloc(ptr_bloc bloc, size_t size);
void 		fusion_bloc(ptr_bloc b1, ptr_bloc b2);
void 		copy_bloc(ptr_bloc old_bloc, ptr_bloc new_bloc);
ptr_bloc 	user_space_to_bloc(void *ptr);
void 		print_alloc(void);
void 		print_bloc(ptr_bloc bloc);

#endif