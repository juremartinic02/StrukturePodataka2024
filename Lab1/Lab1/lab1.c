#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define ERROR_OPPENING_FILE -1

//Stavimo 1024 jer je to maksimalni broj znakova u redu
#define BUFFER_SIZE 1024

struct _student;
typedef struct _student {
	char ime[100];
} Student;

int count_rows(char* filename);

int main()
{
	printf("Hello world!\n");

	return EXIT_SUCCESS;
}

int count_rows(char* filename)
{
	FILE* file = NULL;

	char buffer[BUFFER_SIZE];
	int count = 0;

	file = fopen(filename, "r");
	if (!file)
	{
		printf("Error opening file!\n");
		return ERROR_OPPENING_FILE;
	}

	while (!feof(file))
	{
		fgets(buffer, BUFFER_SIZE, file);
		++count;
	}


	fclose(file);
	return EXIT_SUCCESS;
}