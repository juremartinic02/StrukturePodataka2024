// Direktiva kojom iskljucujemo upozorenje za koristenje funckija za manipulaciju datoteka i stringova
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define ERROR_OPENING_FILE -1
#define ERROR_ALLOCATING_MEMORY -2
#define MAX_NAME_LENGTH 100

// Stavimo 1024 jer je to maksimalni broj znakova u redu
#define BUFFER_SIZE 1024

// Definiranje strukture student
typedef struct _student {
    char ime[MAX_NAME_LENGTH];
    char prezime[MAX_NAME_LENGTH];
    double brojBodova;
} Student;

// Definiramo StudentPos kao pokazivac na strukturu zbog citljivosti koda
typedef Student* StudentPos;

/*
    Funckija: count_rows
    --------------------------
        Vraca broj redaka u datoteci.
        (u ovom slucaju formatirana na nacin da 1 redak = 1 student sto kasnije koristimo da bi izracunali broj studenata)
*/

int count_rows(char* filename);

/*
    Funckija: allocate_students
    ------------------------------------
    Vraca pokazivac na alociranu memoriju.
*/

StudentPos allocate_students(int numStudents);

/*
    Funkcija: read_students
    --------------------------------
    Vraca pokazivac na niz struktura Student (u slucaju uspjesne alokacije memorije)
*/
StudentPos read_students(char* filename, int numStudents, double* maxBodovi);

/*
    Funkcija: print_students
    ---------------------------------
    Ispisuje korisniku na ekran procitane i obradjene podatke.
*/
void print_students(StudentPos students, int numStudents, double maxBodovi);

int main()
{
    char filename[MAX_NAME_LENGTH] = "studenti.txt";
    int numStudents = 0;
    double maxBodovi = 0.0;

    numStudents = count_rows(filename);
    if (numStudents <= 0)
    {
        printf("Neuspjesno citanje datoteke ili nema studenata u datoteci za procitati.\n");
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
    // Osigurava cinjenicu da datoteka postoji / se otvorila, u suprotnom pointer skuplja trash vrijednost
    FILE* file = NULL;

    // "Cuva" redak teksta prije nego sto program procita drugi. Bez ovoga fgets nema gdje smjestiti prvi procitani redak
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
        // Pitanje: da li bi mi ovdje mogli staviti count++?
        ++count;
    }

    fclose(file);
    return count;
}

StudentPos allocate_students(int numStudents)
{
    StudentPos students = (StudentPos)malloc(numStudents * sizeof(Student));

    if (!students)
    {
        printf("Nesupjela alokacija memorije!");
        return ERROR_ALLOCATING_MEMORY;
    }

    return students;
}

StudentPos read_students(char* filename, int numStudents, double* maxBodovi)
{
    FILE* file = NULL;

    // Inicijaliziramo pokazivac na strukturu kako ne bi pokupio neku trash vrijednost prije alokacije memorije
    StudentPos students = NULL;
    int i = 0;

    students = allocate_students(numStudents);
    if (!students)
    {
        return ERROR_ALLOCATING_MEMORY;
    }

    file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file!\n");

        // Dobra praksa je oslobadjati memoriju ako je neuspjelo otvaranje datoteke
        free(students);
        return NULL;
    }

    while (i < numStudents)
    {
        // Pokazivac na trenutnog studenta kako mozemo koristit operator -> zbog preglednosti koda. U suprotnosti moramo pisati students[i].ime
        StudentPos student = &students[i];

        if (fscanf(file, "%s %s %lf", student->ime, student->prezime, &student->brojBodova) != 3)
        {
            printf("Neuspjelo citanje podataka iz datoteke!");
            break;
        }

        // Citanje stvarnog broja bodova sa memorije jer broj bodova pocinje od 0
        // Ovo je nacin kako maksimalan broj bodova dobivamo tako da usporedjujemo broj bodova od studenata
        // Mogli smo i izbjec ovo da smo define-ali proizvoljan maksimalni broj bodova na testu u main-u
        if (student->brojBodova > *maxBodovi)
        {
            *maxBodovi = student->brojBodova;
        }

        i++;
    }

    fclose(file);
    return students;
}

void print_students(StudentPos students, int numStudents, double maxBodovi)
{
    int i = 0;
    printf("Ime\tPrezime\t\tApsolutni Bodovi\tRelativni Bodovi\n");

    for (i = 0; i < numStudents; i++)
    {
        double relativniBodovi = (students[i].brojBodova / maxBodovi) * 100;
        printf("%s\t%s\t\t%.2f\t\t\t%.2f%%\n", students[i].ime, students[i].prezime, students[i].brojBodova, relativniBodovi);
    }
}
