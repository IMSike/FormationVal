#include <dirent.h>
#include "crawler.h"

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
	char* content_of_file="";
	FILE* fichier = NULL;
    	char temp[FILE_LINE_SIZE_MAX] = "";
    	fichier = fopen(file_path, "r");
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
				printf("%s\n", temp);
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