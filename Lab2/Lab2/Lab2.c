#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<crtdbg.h>

#define MAX_NAME_LENGTH 100
#define ERROR_ALLOCATING_MEMORY -1

struct _person;
typedef struct _person* PersonPosition;
typedef struct _person {
	char name[MAX_NAME_LENGTH];
	char surname[MAX_NAME_LENGTH];
	int birthYear;
	PersonPosition next;
} Person;

// PITANJE ZA PROFESORICU: da li su ovi komentari prediskriptivni?

/*
	Funkcija createNewPerson
	----------------------------------
		Dinamicki alociramo memoriju za novu osobu u jednostruko vezanoj listi
*/
PersonPosition createNewPerson(char*, char*, int);

/*
	Funkcija addNewPersonToLinkedList: (funckija 1 u menu)
	----------------------------------
		Pozivamo createNewPerson funckiju da nam osigura potrebnu memoriju za stvaranje nove inacice strukture
		Pitamo korisnika da nam popuni elemente strukture i spremamo tu novu osobu kao .next element dummy elementa head,
			sto znaci da svaki put kad pozovemo ovu funckiju ona postaje prvi stvarni element ove liste
*/
int addNewPersonToLinkedList(PersonPosition);

/*
	Funkcija printLikedList: (funckija 2 u menu)
	----------------------------------
		Funkcija koja ispisuje jednostruko vezanu listu
*/
int printLikedList(PersonPosition);

/*
	Funkcija addNewPersonToEndOfLinkedList: (funckija 3 u menu)
	----------------------------------
		Funckija alocira memoriju preko createNewPerson funckije te stvara novu osobu koju dodajemo na kraj jednostruko vezane liste
*/
int addNewPersonToEndOfLinkedList(PersonPosition);

/*
	Funckija findElementOfLinkedListBySurname: (funkcija 4 u menu)
	----------------------------------
		Funckija pita korisnika da unese prezime, program mu vraca sve osobe sa tim prezimenom ili poruku da te osobe nema

*/
int findElementOfLinkedListBySurname(PersonPosition);

/*
	Funckija deleteElementFromLinkedList: (funckija 5 u menu)
	----------------------------------
		Funkcija brise element tako sto pita korisnika da upise prezime te onda sve osobe 
*/
int deleteElementFromLinkedList(PersonPosition);

int menuForFunctions(PersonPosition);

int main()
{
	Person head = { .next = NULL };

	menuForFunctions(&head);

	return EXIT_SUCCESS;
}

PersonPosition createNewPerson(char* name, char* surname, int birthYear)
{	
	// PITANJE ZA PROFESORICU: da li smo mi ovdje mogli ostaviti spojeno PersonPosition newPerson = ... 
	// ili je dobra navika to odvajati, tj deklarirati ispred ovih char-ova na NULL
	PersonPosition newPerson = (PersonPosition)malloc(sizeof(Person));
	if (!newPerson)
	{
		printf("Memory allocation failed!\n");
		return ERROR_ALLOCATING_MEMORY;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;

	return newPerson;
}

int addNewPersonToLinkedList(PersonPosition head)
{
	PersonPosition newPerson = NULL;

	char name[MAX_NAME_LENGTH];
	char surname[MAX_NAME_LENGTH];
	int birthYear;

	printf("Unesite ime: ");
	// PITANJE ZA PROFESORICU: ovo je naravno jednostavniji nacin za rijesiti provjeru scanf return value-a, da li smijemo tako ostaviti ili moramo if funkcijom provjeravat
	// (void) ispred scanf-a sprjecava upozorenje: 'return value ignored' tako sto compileru daje do znanja da ju namjerno ignoriramo
	(void)scanf("%s", name);
	printf("Unesite prezime: ");
	(void)scanf("%s", surname);
	printf("Unesite godinu rodjenja: ");
	(void)scanf("%d", &birthYear);

	newPerson = createNewPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		return EXIT_FAILURE;
	}

	if (newPerson != NULL)
	{
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int printLikedList(PersonPosition first)
{
	printf("\nElementi jednostruko vezane liste su: \n");
	
	while (first)
	{
		printf("Ime: %s\tPrezime: %s\tGodina rodjenja: %d\n", first->name, first->surname, first->birthYear);
		first = first->next;
	}
}

int addNewPersonToEndOfLinkedList(PersonPosition head)
{
	PersonPosition newPerson = NULL;
	PersonPosition temp = head;

	char name[MAX_NAME_LENGTH];
	char surname[MAX_NAME_LENGTH];
	int birthYear;

	printf("Unesite ime: ");
	(void)scanf("%s", name);
	printf("Unesite prezime: ");
	(void)scanf("%s", surname);
	printf("Unesite godinu rodjenja: ");
	(void)scanf("%d", &birthYear);

	newPerson = createNewPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		return EXIT_FAILURE;
	}

	while (head->next != NULL)
	{
		head = head->next;
	}

	head->next = newPerson;
	newPerson->next = NULL;

	return EXIT_SUCCESS;
}

int findElementOfLinkedListBySurname(PersonPosition first)
{
	char surnameToFind[MAX_NAME_LENGTH];
	// inicijalizirana vrijednost koja nam pomaze u slucajevima kada nadjemo osobu ili obratno
	int found = 0;

	printf("Unesi prezime osobe koju zelis naci: ");
	(void)scanf("%s", surnameToFind);

	while (first != NULL) {
		if (strcmp(first->surname, surnameToFind) == 0) {
			printf("Osoba sa trazenim prezimenom je: %s %s %d\n", first->name, first->surname, first->birthYear);
			found = 1;
		}
		first = first->next;
	}

	if (!found) {
		printf("Osoba sa prezimenom '%s' nije pronadjena.\n", surnameToFind);
	}

	// PITANJE ZA PROFESORICU: da li smo ovo mogli napraviti preko return ? <argument> : <argument> i da li je to preferirani nacin
	if (found == 1)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

int deleteElementFromLinkedList(PersonPosition head)
{
	char surnameToDelete[MAX_NAME_LENGTH];
	int found = 0;
	PersonPosition current = head->next;
	PersonPosition previous = head;

	printf("Unesite prezime osobe koju zelite izbrisati: ");
	(void)scanf("%s", surnameToDelete);

	while (current != NULL) {
		if (strcmp(current->surname, surnameToDelete) == 0) {
			previous->next = current->next;
			free(current);
			found = 1;
			printf("Osoba sa prezimenom '%s' je uspjesno izbrisana.\n", surnameToDelete);
			break;
		}
		previous = current;
		current = current->next;
	}

	if (!found) {
		printf("Osoba sa prezimenom '%s' nije pronadjena.\n", surnameToDelete);
	}

	if (found == 1)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

int menuForFunctions(PersonPosition head)
{
	int choice = 0;

	while (1)
	{
		printf("Izaberi funkciju koju zelis izvrsiti: \n");
		printf("\t1. Dinamicki dodaj novi element (osobu) na pocetak jednostruko vezane liste\n");
		printf("\t2. Ispisi jednostruko vezanu listu\n");
		printf("\t3. Dinamicki dodaj novi element (osobu) na kraj jednostruko vezane liste\n");
		printf("\t4. Pronadji element u jednostruko vezanoj listi (uvjet: po prezimenu)\n");
		printf("\t5. Izbrisi odredjeni element iz jednostruko vezane liste (uvjet: prezime osobe)\n");
		printf("\t0. Izlaz\n");
		printf("Unesite broj opcije: ");

		(void)scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			addNewPersonToLinkedList(head);
			break;
		case 2:
			printLikedList(head->next);
			break;
		case 3:
			addNewPersonToEndOfLinkedList(head);
			break;
		case 4:
			findElementOfLinkedListBySurname(head->next);
			break;
		case 5:
			deleteElementFromLinkedList(head);
			break;
		case 0:
			return EXIT_SUCCESS;
		default:
			printf("Ne postoji funkcija sa tim rednim brojem za izvrsavanje!\n");
		}
	}
}


