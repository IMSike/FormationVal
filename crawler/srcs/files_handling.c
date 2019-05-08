#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "crawler.h"

char  *allocateCharPtr(int wordSize)
{
  char *ptr = (char*)malloc(wordSize*sizeof(char));
  if ( ptr != NULL )
    return ptr;
  else
    perror("\n ERROR in allocateCharPtr ");
}

/* Fonction qui récupère le nombre de caractères contenu le fichier */
int getCharFilesNumbers(char *filePath)
{
  FILE *file = fopen( filePath,"r");
  if( file != NULL )
  {
    int  counter = 0;
    char c;
    while( fscanf( file,"%c",&c ) != EOF )
    {
     if ( counter < INT_MAX )
       counter++;
     else
      perror("\n ERROR in getCharFilesNumber 2 ");
    }
    return counter;
  }
  else
    perror("\n ERROR in getCharFilesNumber 1 ");
  fclose(file);
}

/* Fonction pour mettre en mémoire un fichier */
char *fileToString(char *filePath, int *bufferLength)
{
  FILE *file = fopen( filePath,"r" );
  if( file != NULL && bufferLength != NULL)
  {
    *bufferLength = getCharFilesNumbers(filePath);
    char *buffer = allocateCharPtr(*bufferLength);
    char  c;
    int   offset = 0;
    while( fscanf( file,"%c",&c ) != EOF )
    {
      buffer[offset] = c;
      offset++;
    }
    return buffer;
  }
  else
    perror("\n ERROR in fileToString ");
  fclose(file);
}

//renvoi le nombre d'occurence du caractère car dans chaine
int nb_occ(char* chaine, char car)
{
  int c,nbocc = 0,
  size = strlen(chaine);    // taille de la chaine
  for (c = 0 ; c < size ; c++)
    if (chaine[c] == car) // si le caractère n° c est bien le caractère
      nbocc ++;         // on incrémente le compteur
  return nbocc;             // on renvoie le compteur
}

void* getWhatToPrintInFile(void* arg)
{
  printf("TEST_fct_thread\n");
  t_com_s* infomations = (t_com_s*) arg;
  if(infomations->type_of_filter == 1) //filtre -o    ==> on renvoi le texte du fichier s'il contient le filtre, \0 sinon
  {
    char* text = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
    text = fileToString(infomations->path, infomations->size_of_what_to_print);
    if(strstr(text, infomations->filter) != NULL)
      infomations->what_to_print = text;
    else
      infomations->what_to_print = "\0";
  }
  else if(infomations->type_of_filter == 2) //filtre -r      ==> on renvoi les lignes du fichier qui contiennent le filtre, \0 s'il y en a aucune
  {
    char* text = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
    text = fileToString(infomations->path, infomations->size_of_what_to_print);
    char* text_to_return = malloc(sizeof(char)*(*infomations->size_of_what_to_print));
    text_to_return[0]='\0';
    if(strstr(text, infomations->filter) != NULL)
    {
      int number_of_line = nb_occ(text, '\n')+1;
      char* p = NULL;
      p = strtok(text, "\n");
      int inc = 0;
      while(inc < number_of_line && p != NULL)
      {
        if(strstr(p, infomations->filter) != NULL)
          strcat(text_to_return, p);
        p = strtok(NULL, "\n");
        inc++;
      }
    }
    else
      infomations->what_to_print = "\0";
    infomations->what_to_print = text_to_return;
  }
  else // si l'utilisateur n'utilise pas de filtre on renvoi le texte du fichier
    infomations->what_to_print = fileToString(infomations->path, infomations->size_of_what_to_print);
  //printf("||||||||%s\n", infomations->what_to_print);
  printf("TEST_fin_fct_thread\n");
  pthread_exit(0);
}



char *getWhatToPrint(int type_of_filter, char* path, char* filter, int* size_of_what_to_print)
{
  if(type_of_filter == 1) //filtre -o    ==> on renvoi le texte du fichier s'il contient le filtre, \0 sinon
  {
    char* text = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
    text = fileToString(path, size_of_what_to_print);
    if(strstr(text, filter) != NULL)
      return text;
    else
      return "\0";
  }
  else if(type_of_filter == 2) //filtre -r      ==> on renvoi les lignes du fichier qui contiennent le filtre, \0 s'il y en a aucune
  {
    char* text = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
    text = fileToString(path, size_of_what_to_print);
    char* text_to_return = malloc(sizeof(char)*(*size_of_what_to_print));
    text_to_return[0]='\0';
    if(strstr(text, filter) != NULL)
    {
      int number_of_line = nb_occ(text, '\n')+1;
      char* p = NULL;
      p = strtok(text, "\n");
      int inc = 0;
      while(inc < number_of_line && p != NULL)
      {
        if(strstr(p, filter) != NULL)
          strcat(text_to_return, p);
        p = strtok(NULL, "\n");
        inc++;
      }
    }
    else
      return "\0";
    return text_to_return;
  }
  else // si l'utilisateur n'utilise pas de filtre on renvoi le texte du fichier
    return fileToString(path, size_of_what_to_print);
}

void print_file(char* file_path) //affiche le fichier file_path
{
  FILE* fichier = NULL;
  char str[FILE_LINE_SIZE_MAX] = "";
  fichier = fopen(file_path, "r");
  if (fichier != NULL)
  {
    while (fgets(str, FILE_LINE_SIZE_MAX, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
	    printf("%s", str); // On affiche la chaîne qu'on vient de lire
    fclose(fichier);
  }
  else
    printf("ERROR : can't open file in print_file() in file_handling.c\n");
}

//affiche le fichier file_path s'il contient str
void print_file_contain(char* file_path, char* str)
{
  char* content_of_file= NULL;
  FILE* fichier = NULL;
  char temp[FILE_LINE_SIZE_MAX] = "";
  fichier = fopen(file_path, "r");
  content_of_file = malloc(sizeof(char*)*FILE_LINE_SIZE_MAX*FILE_NUMBER_OF_LINE_MAX);
  *content_of_file = '\0';
  if (fichier != NULL)
  {
   	while (fgets(temp, FILE_LINE_SIZE_MAX, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
      strcat(content_of_file, temp);
    fclose(fichier);
  }
  else
    printf("ERROR : can't open file in print_file_contain() in file_handling.c\n");
  if(strstr(content_of_file, str) != NULL)
    printf("%s\n", content_of_file);
  free(content_of_file);
}

void print_line_of_file_contain(char* file_path, char* str)
{
  FILE* fichier = NULL;
  char temp[FILE_LINE_SIZE_MAX] = "";
  fichier = fopen(file_path, "r");
  if (fichier != NULL)
  {
   	while (fgets(temp, FILE_LINE_SIZE_MAX, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
      if(strstr(temp, str))
    printf("%s", temp);
    fclose(fichier);
  }
  else
    printf("ERROR : can't open file in print_line_of_file_contain() in file_handling.c\n");
}

//renvoi 1 si file_path est un fichier, 0 sinon
int isFile(const char* file_path)
{
  FILE* file;
  file = fopen(file_path, "r");
  if(file != NULL) //si c'est un fichier
  {
    fclose(file);
  	return 1;
  }
	return 0;
}

//renvoi 1 si directory_path est un repertoire, 0 sinon
int isDirectory(const char* directory_path)
{
	DIR* directory;
	directory = opendir(directory_path);
	if(directory != NULL) //si c'est un repertoire
	{
		closedir(directory);
		return 1;
	}
	return 0;
}