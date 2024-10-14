//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//
//#define ERROR_OPENING_FILE -1
//
//#define MAX_NAME_LENGTH 100
//
////Stavimo 1024 jer je to maksimalni broj znakova u redu
//#define BUFFER_SIZE 1024
//
////Deklaracija studenta kao strukture
//struct _student;
//typedef struct Student* StudentPos;
//typedef struct _student {
//	char ime[MAX_NAME_LENGTH];
//	char prezime[MAX_NAME_LENGTH];
//	double brojBodova;
//} Student;
//
////Definiramo StudentPos kao pokazivac na strukturu zbog ponavljanja kroz kod
//
//
////Funkcija koja broji broj redaka u file-u
//int count_rows(char* filename);
//
//StudentPos read_students(char* filename, int numStudents, double* maxBodovi);
//void print_students(StudentPos students, int numStudents, double maxBodovi);
//
//int main()
//{
//    char filename[MAX_NAME_LENGTH] = "studenti.txt"; // Promijenite ime datoteke po potrebi
//    int numStudents = 0;
//    double maxBodovi = 0.0;
//
//    //numStudents = count_rows(filename);
//    if (numStudents <= 0)
//    {
//        printf("Error reading the file or no students in the file.\n");
//        return ERROR_OPENING_FILE;
//    }
//
//    StudentPos students = read_students(filename, numStudents, &maxBodovi);
//    if (!students)
//    {
//        printf("Error reading students.\n");
//        return ERROR_OPENING_FILE;
//    }
//
//    print_students(students, numStudents, maxBodovi);
//
//    free(students); // Oslobadanje dinamicki alocirane memorije
//    return EXIT_SUCCESS;
//}
//
//int count_rows(char* filename)
//{
//	FILE* file = NULL;
//
//	char buffer[BUFFER_SIZE];
//	int count = 0;
//
//	file = fopen(filename, "r");
//	if (!file)
//	{
//		printf("Error opening file!\n");
//		return ERROR_OPENING_FILE;
//	}
//
//	while (!feof(file))
//	{
//		fgets(buffer, BUFFER_SIZE, file);
//		++count;
//	}
//
//
//	fclose(file);
//	return EXIT_SUCCESS;
//}
//
//StudentPos read_students(char* filename, int numStudents, double* maxBodovi)
//{
//    FILE* file = NULL;
//    StudentPos students = NULL;
//    int i = 0;
//
//    students = (StudentPos)malloc(numStudents * sizeof(Student));
//    if (!students)
//    {
//        printf("Memory allocation failed!\n");
//        return NULL;
//    }
//
//    file = fopen(filename, "r");
//    if (!file)
//    {
//        printf("Error opening file!\n");
//        free(students);
//        return NULL;
//    }
//
//    for (i = 0; i < numStudents; ++i)
//    {
//        fscanf(file, "%s %s %lf", students[i].ime, students[i].prezime, &students[i].brojBodova);
//        if (students[i].brojBodova > *maxBodovi)
//        {
//            *maxBodovi = students[i].brojBodova; // Pracenje maksimalnog broja bodova
//        }
//    }
//
//    fclose(file);
//    return students;
//}
//
//void print_students(StudentPos students, int numStudents, double maxBodovi)
//{
//    int i = 0;
//    printf("Ime\tPrezime\t\tApsolutni Bodovi\tRelativni Bodovi\n");
//    for (i = 0; i < numStudents; ++i)
//    {
//        double relativniBodovi = (students[i].brojBodova / maxBodovi) * 100;
//        printf("%s\t%s\t\t%.2lf\t\t\t%.2lf%%\n", students[i].ime, students[i].prezime, students[i].brojBodova, relativniBodovi);
//    }
//}

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define ERROR_OPENING_FILE -1
#define MAX_NAME_LENGTH 100

//Stavimo 1024 jer je to maksimalni broj znakova u redu
#define BUFFER_SIZE 1024

typedef struct _student {
    char ime[MAX_NAME_LENGTH];
    char prezime[MAX_NAME_LENGTH];
    double brojBodova;
} Student;

typedef Student* StudentPos;

int count_rows(char* filename);

StudentPos read_students(char* filename, int numStudents, double* maxBodovi);
void print_students(StudentPos students, int numStudents, double maxBodovi);

int main()
{
    char filename[MAX_NAME_LENGTH] = "studenti.txt";
    int numStudents = 0;
    double maxBodovi = 0.0;

    numStudents = count_rows(filename);
    if (numStudents <= 0)
    {
        printf("Error reading the file or no students in the file.\n");
        return ERROR_OPENING_FILE;
    }

    StudentPos students = read_students(filename, numStudents, &maxBodovi);
    if (!students)
    {
        printf("Error reading students.\n");
        return ERROR_OPENING_FILE;
    }

    print_students(students, numStudents, maxBodovi);

    free(students);
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
        return ERROR_OPENING_FILE;
    }

    while (fgets(buffer, BUFFER_SIZE, file))
    {
        ++count;
    }

    fclose(file);
    return count;
}

StudentPos read_students(char* filename, int numStudents, double* maxBodovi)
{
    FILE* file = NULL;
    StudentPos students = NULL;
    int i = 0;

    students = (StudentPos)malloc(numStudents * sizeof(Student));
    if (!students)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file!\n");
        free(students);
        return NULL;
    }

    for (i = 0; i < numStudents; ++i)
    {
        fscanf(file, "%s %s %lf", students[i].ime, students[i].prezime, &students[i].brojBodova);
        if (students[i].brojBodova > *maxBodovi)
        {
            *maxBodovi = students[i].brojBodova;
        }
    }

    fclose(file);
    return students;
}

void print_students(StudentPos students, int numStudents, double maxBodovi)
{
    int i = 0;
    printf("Ime\tPrezime\t\tApsolutni Bodovi\tRelativni Bodovi\n");
    for (i = 0; i < numStudents; ++i)
    {
        double relativniBodovi = (students[i].brojBodova / maxBodovi) * 100;
        printf("%s\t%s\t\t%.2lf\t\t\t%.2lf%%\n", students[i].ime, students[i].prezime, students[i].brojBodova, relativniBodovi);
    }
}
