#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// zbog isAlpha funckije
#include<ctype.h>
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
void printLikedList(PersonPosition);

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

/*
	Funckija insertAfterElement: (funckija 6 u menu)
	----------------------------------
		Funkcija dinamicki alocira element ispred odredjenog elementa. 
			Taj element mu damo koristeci findElementOfLinkedListBySurname funckiju cime mu dajemo poziciju trenutnog elementa
				nakon kojeg mora ubaciti novo nastali element pomocu addNewPersonToLinkedList funkcije.
*/
int insertAfterElement(PersonPosition);

/*
	Funckija insertBeforeElement: (funckija 7 u menu)
	----------------------------------
		Funkcija dinamicki alocira element prije odredjenog elementa.
			Taj element mu damo koristeci findElementOfLinkedListBySurname funckiju cime mu dajemo poziciju trenutnog elementa
				prije kojeg mora ubaciti novo nastali element pomocu addNewPersonToLinkedList funkcije.
*/
int insertBeforeElement(PersonPosition);

PersonPosition getMiddle(PersonPosition);
PersonPosition mergeSortedLists(PersonPosition, PersonPosition);
PersonPosition mergeSort(PersonPosition head);

int writeListInFile(PersonPosition, char*);
int readListFromFile(PersonPosition, char*);

void freeList(PersonPosition);

/*
	Funckija menuForFunctions
	-----------------------------
		Funckija kreira izbornik u kojem se pod rednim brojevima nude funckije za manipuliranje jednostruko vezane liste
*/
int menuForFunctions(PersonPosition);

/*
	Funkcija isStringValid
	---------------------------
		Funckija provjerava korisnikov input pomocu isAlpha funckije cime osiguravamo da budemo sigurni
			da korisnik upisuje tocan tip podatka koji se od njega ocekuje
*/
int isStringValid(char*);

/*
	Funckcija getValidStringInput
	--------------------------------------
		Funckija koja koristi isStringValid funckiju u do-while loop-u sve dok korisnik ne unese ispravan tip podatka
*/
int getValidStringInput(char* prompt, char* inputBuffer, int bufferSize) {
	int success = 0;

	do {
		printf("%s", prompt);
		success = scanf("%s", inputBuffer);

		if (success != 1 || !isStringValid(inputBuffer)) {
			printf("Greska! Unesite ispravan tip varijable!\n");
			success = 0;
			while (getchar() != '\n');
		}
	} while (!success);

	return success;
}

int main()
{
	Person head = { .next = NULL };

	menuForFunctions(&head);

	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}

int isStringValid(char* input) {
	for (int i = 0; input[i] != '\0'; i++) {
		if (!isalpha(input[i])) {  // isAlpha je funckija koja za nas provjerava da li je korisnikov unos bio char
			return 0;  // vracamo 0 ako su ima znak koji je korisnik unio a da nije char 
		}
	}
	return 1;  // vracamo 1 ako su svi znakovi koje je korisnik unio tipa char
}

PersonPosition createNewPerson(char* name, char* surname, int birthYear)
{
	PersonPosition newPerson = NULL;
	newPerson = (PersonPosition)malloc(sizeof(Person));
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
	int birthYear = 0;

	getValidStringInput("Unesite ime: ", name, MAX_NAME_LENGTH);
	getValidStringInput("Unesite prezime: ", surname, MAX_NAME_LENGTH);

	printf("Unesite godinu rodjenja: ");
	(void)scanf("%d", &birthYear);

	newPerson = createNewPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		return EXIT_FAILURE;
	}

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

void printLikedList(PersonPosition first)
{
	printf("\nElementi jednostruko vezane liste su: \n");

	while (first)
	{
		printf("Ime: %s\tPrezime: %s\tGodina rodjenja: %d\n", first->name, first->surname, first->birthYear);
		first = first->next;
	}

	if (first == NULL)
	{
		printf("Lista je prazna!\n");
	}
}

int addNewPersonToEndOfLinkedList(PersonPosition head)
{
	PersonPosition newPerson = NULL;
	PersonPosition temp = head;

	char name[MAX_NAME_LENGTH];
	char surname[MAX_NAME_LENGTH];
	int birthYear = 0;

	getValidStringInput("Unesite ime: ", name, MAX_NAME_LENGTH);
	getValidStringInput("Unesite prezime: ", surname, MAX_NAME_LENGTH);

	printf("Unesite godinu rodjenja: ");
	(void)scanf("%d", &birthYear);

	newPerson = createNewPerson(name, surname, birthYear);

	if (newPerson == NULL)
	{
		return EXIT_FAILURE;
	}

	while (head->next != NULL) {
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

	getValidStringInput("Unesi prezime osobe koju zelis naci: ", surnameToFind, MAX_NAME_LENGTH);

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

	return (found == 1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int deleteElementFromLinkedList(PersonPosition head)
{
	char surnameToDelete[MAX_NAME_LENGTH];
	int found = 0;
	PersonPosition current = head->next;
	PersonPosition previous = head;

	getValidStringInput("Unesite prezime osobe koju zelite izbrisati: ", surnameToDelete, MAX_NAME_LENGTH);

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

	return (found == 1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int insertAfterElement(PersonPosition head) {
	char surnameToFind[MAX_NAME_LENGTH];
	PersonPosition current = head;
	int found = 0;

	getValidStringInput("Unesite prezime osobe nakon koje zelite dodati novu osobu: ", surnameToFind, MAX_NAME_LENGTH);

	while (current != NULL) {
		if (strcmp(current->surname, surnameToFind) == 0) {
			found = 1;
			break;
		}
		current = current->next;
	}

	if (!found) {
		printf("Osoba sa prezimenom '%s' nije pronadjena!\n", surnameToFind);
		return EXIT_FAILURE;
	}

	if (addNewPersonToLinkedList(current) == EXIT_FAILURE) {
		printf("Neuspjelo kreiranje nove osobe u vezanoj listi!\n");
		return EXIT_FAILURE;
	}

	printf("Nova osoba uspjesno dodana nakon osobe s prezimenom '%s'.\n", surnameToFind);
	return EXIT_SUCCESS;
}

int insertBeforeElement(PersonPosition head) {
	char surnameToFind[MAX_NAME_LENGTH];
	PersonPosition current = head->next;
	PersonPosition previous = head;
	int found = 0;

	getValidStringInput("Unesite prezime osobe prije koje zelite dodati novu osobu: ", surnameToFind, MAX_NAME_LENGTH);

	while (current != NULL) {
		if (strcmp(current->surname, surnameToFind) == 0) {
			found = 1;
			break;
		}
		previous = current;
		current = current->next;
	}

	if (!found) {
		printf("Osoba sa prezimenom '%s' nije pronadjena!\n", surnameToFind);
		return EXIT_FAILURE;
	}

	if (addNewPersonToLinkedList(previous) == EXIT_FAILURE) {
		printf("Neuspjelo kreiranje nove osobe u vezanoj listi!\n");
		return EXIT_FAILURE;
	}

	printf("Nova osoba uspjesno dodana prije osobe s prezimenom '%s'.\n", surnameToFind);
	return EXIT_SUCCESS;
}

PersonPosition getMiddle(PersonPosition head) {
	if (head == NULL) {
		return NULL;
	}

	PersonPosition slow = head;
	PersonPosition fast = head->next;

	while (fast != NULL && fast->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

PersonPosition mergeSortedLists(PersonPosition a, PersonPosition b) {
	if (!a) return b;
	if (!b) return a;

	if (strcmp(a->surname, b->surname) <= 0) {
		a->next = mergeSortedLists(a->next, b);
		return a;
	}
	else {
		b->next = mergeSortedLists(a, b->next);
		return b;
	}
}

PersonPosition mergeSort(PersonPosition head) {
	if (!head || !head->next) return head;

	PersonPosition middle = getMiddle(head);
	PersonPosition nextToMiddle = middle->next;

	middle->next = NULL;

	PersonPosition left = mergeSort(head);
	PersonPosition right = mergeSort(nextToMiddle);

	return mergeSortedLists(left, right);
}

int writeListInFile(PersonPosition, char*);
int readListFromFile(PersonPosition, char*);
void freeList(PersonPosition);

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
		printf("\t6. Dinamicki dodaj novi element ispred odredjenog elementa (uvjet: prezime osobe)\n");
		printf("\t7. Dinamicki dodaj novi element iza odredjenog elementa (uvjet: prezime osobe)\n");
		printf("\t8. Sortiraj jednostruko vezanu listu po prezimenima (abecedno)\n");
		printf("\t9. Upisi jednostruko vezanu listu u datoteku\n");
		printf("\t10. Citaj listu iz datoteke\n");
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
		case 6:
			insertAfterElement(head->next);
			break;
		case 7:
			insertBeforeElement(head);
			break;
		case 8:
			mergeSort(head);
			break;
		case 0:
			return EXIT_SUCCESS;
		default:
			printf("Ne postoji funkcija sa tim rednim brojem za izvrsavanje!\n");
		}
	}
}