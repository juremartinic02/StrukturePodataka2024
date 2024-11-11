#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<crtdbg.h>

#define MAX_LINE 1024
#define FILE_NOT_OPEN -1
#define EMPTY_LISTS -2
#define ERROR_MEMORY_ALLOCATION -3

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
    int coefficient;
    int exponent;
    Position next;
} Element;

/*
    Funkcija readFile()
    --------------------------
        Funkcija cita .txt datoteku pomocu fopen funckije i sa fgets uzima sve sto se u njoj nalazi.
        Funckcijom changeStringIntoList mijenjamo oblik procitanog stringa iz .txt datoteke u oblik liste sto nam kasnije sluzi za manipulaciju iste
*/
int readFile(Position headPolynome1, Position headPolynome2, char* fileName);

/*
    Funckija printPolynome()
    -----------------------------
        Funckcija ispisuje polinome po pravilima za oblik polinoma
    */
int printPolynome(char* polynomeName, Position first);

/*
    Funckija addPolynomes()
    --------------------------------
        Funkcija zbraja polinome po matematickim pravilima za zbrajanje polinoma
*/
int addPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);

/*
    Funckija multiplyPolynomes()
    --------------------------------
        Funkcija mnozi polinome po matematickim pravilima za mnozenje polinoma
*/
int multiplyPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);

/*
    Funkcija freeMemory()
    --------------------------------
        Funkcija oslobadja alociranu memoriju koju smo koristili kroz kod
*/
int freeMemory(Position head);

/*
    Funckija changeStringIntoList()
    ----------------------------------------------
        Funckija mijenja oblik procitanog stringa iz .txt datoteke u format koji nam pase kako bi manipulirali polinomima kasnije
            (zbrajanje i oduzimanje). Da nema ove funkcije morali bi koristiti puno vise loopova i condition-a kako bi proveli funckije nad samim polinomima
*/
int changeStringIntoList(Position head, char* buffer);

/*
    Funckija createNewElement()
    -------------------------------------
        Funckija stvara novu inacicu strukture pomocu alokacije memorije
*/
Position createNewElement(int coefficient, int exponent);

/*
    Funckija insertSorted()
    --------------------------------
        Funkcija insertSorted provjerava da li su polinomi sortirani po pravilu o sortiranju polinoma pa ih ubacuje u funkciju multiplyPolynome
*/
int insertSorted(Position head, Position newElement);

/*
* 
    Funckija mergeAfter()
    ------------------------------------------
    Funckija osigurava da polinomi u listi budu slozeni pravilnim redosljedom(od polinoma sa najmanjom potencijom do polinoma sa najvecom potencijom)
    tj., funckija provjerava da li sljedeci polinom u listi moze nalaziti na toj poziciji,
     te ako ne moze zadrzava ga da polinom koji treba doci na tu poziciju dodje na tu poziciju
*/
int mergeAfter(Position current, Position newElement);

/*
    Funckija insertAfter()
    ------------------------------------
        Funckija ubacuje novi element nakon nekog elementa
*/
int insertAfter(Position current, Position newElement);

/*
    Funckija deleteAfter()
    ----------------------------------
        Funckija brise sljedeci element u listi
*/
int deleteAfter(Position previous);

/*
    Funckija createAndInsertAfter()
    ---------------------------------------
        Funkcija kreira novi element u vezanoj listi i ubacuje ga na sljedece mjesto u listi
*/
int createAndInsertAfter(int coefficient, int exponent, Position current);

int main() {
    Element headPolynome1 = { 0, 0, NULL };
    Element headPolynome2 = { 0, 0, NULL };
    Element headPolynomeAdd = { 0, 0, NULL };
    Element headPolynomeMultiplication = { 0, 0, NULL };
    char* fileName = "polynomes.txt";

    if (readFile(&headPolynome1, &headPolynome2, fileName) == EXIT_SUCCESS) {
        printPolynome("First polynome", headPolynome1.next);
        printPolynome("Second polynome", headPolynome2.next);

        addPolynomes(&headPolynomeAdd, headPolynome1.next, headPolynome2.next);
        multiplyPolynomes(&headPolynomeMultiplication, headPolynome1.next, headPolynome2.next);

        printPolynome("Added polynomes", headPolynomeAdd.next);
        printPolynome("Multiplied polynomes", headPolynomeMultiplication.next);

        freeMemory(&headPolynome1);
        freeMemory(&headPolynome2);
        freeMemory(&headPolynomeAdd);
        freeMemory(&headPolynomeMultiplication);
    }

    _CrtDumpMemoryLeaks();
    return EXIT_SUCCESS;
}

int readFile(Position headPolynome1, Position headPolynome2, char* fileName) {
    FILE* file = NULL;
    file = fopen(fileName, "r");
    char buffer[MAX_LINE] = { 0 };
    if (!file) {
        printf("Can't open file!\n");
        return FILE_NOT_OPEN;
    }

    fgets(buffer, MAX_LINE, file);
    changeStringIntoList(headPolynome1, buffer);

    fgets(buffer, MAX_LINE, file);
    changeStringIntoList(headPolynome2, buffer);

    fclose(file);
    return EXIT_SUCCESS;
}

int printPolynome(char* polynomeName, Position first) {
    printf("%s: ", polynomeName);

    if (!first) {
        printf("0\n");
        return EXIT_SUCCESS;
    }

    int isFirst = 1;

    while (first) {
        if (first->coefficient == 0) {
            first = first->next;
            continue;
        }

        if (!isFirst && first->coefficient > 0)
            printf(" + ");
        else if (!isFirst && first->coefficient < 0)
            printf(" - ");
        else if (first->coefficient < 0)
            printf("-");

        int absCoef = abs(first->coefficient);

        if (absCoef != 1 || first->exponent == 0)
            printf("%d", absCoef);

        if (first->exponent > 0) {
            printf("x");
            if (first->exponent > 1)
                printf("^%d", first->exponent);
        }

        isFirst = 0;
        first = first->next;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

int addPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
    while (firstElementPoly1 && firstElementPoly2) {
        if (firstElementPoly1->exponent == firstElementPoly2->exponent) {
            createAndInsertAfter(firstElementPoly1->coefficient + firstElementPoly2->coefficient,
                firstElementPoly1->exponent, resultHead);
            firstElementPoly1 = firstElementPoly1->next;
            firstElementPoly2 = firstElementPoly2->next;
        }
        else if (firstElementPoly1->exponent > firstElementPoly2->exponent) {
            createAndInsertAfter(firstElementPoly1->coefficient, firstElementPoly1->exponent, resultHead);
            firstElementPoly1 = firstElementPoly1->next;
        }
        else {
            createAndInsertAfter(firstElementPoly2->coefficient, firstElementPoly2->exponent, resultHead);
            firstElementPoly2 = firstElementPoly2->next;
        }
    }

    while (firstElementPoly1) {
        createAndInsertAfter(firstElementPoly1->coefficient, firstElementPoly1->exponent, resultHead);
        firstElementPoly1 = firstElementPoly1->next;
    }

    while (firstElementPoly2) {
        createAndInsertAfter(firstElementPoly2->coefficient, firstElementPoly2->exponent, resultHead);
        firstElementPoly2 = firstElementPoly2->next;
    }

    return EXIT_SUCCESS;
}

int multiplyPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
    for (Position p1 = firstElementPoly1; p1; p1 = p1->next) {
        for (Position p2 = firstElementPoly2; p2; p2 = p2->next) {
            Position newElement = createNewElement(p1->coefficient * p2->coefficient, p1->exponent + p2->exponent);
            insertSorted(resultHead, newElement);
        }
    }
    return EXIT_SUCCESS;
}

int changeStringIntoList(Position head, char* buffer) {
    char* currentBuffer = buffer;
    int coefficient = 0;
    int exponent = 0;
    int numBytes = 0;
    Position newElement = NULL;

    while (strlen(currentBuffer) > 0) {
        while (*currentBuffer == ' ' || *currentBuffer == '\t' || *currentBuffer == '\n') {
            currentBuffer++;
        }

        if (*currentBuffer == '\0') break;

        if (sscanf(currentBuffer, "-x^%d%n", &exponent, &numBytes) == 1) {
            coefficient = -1;
        }
        else if (sscanf(currentBuffer, "+x^%d%n", &exponent, &numBytes) == 1 ||
            sscanf(currentBuffer, "x^%d%n", &exponent, &numBytes) == 1) {
            coefficient = 1;
        }
        else if (strncmp(currentBuffer, "-x", 2) == 0) {
            coefficient = -1;
            exponent = 1;
            numBytes = 2;
        }
        else if (strncmp(currentBuffer, "+x", 2) == 0 ||
            strncmp(currentBuffer, "x", 1) == 0) {
            coefficient = 1;
            exponent = 1;
            numBytes = (*currentBuffer == '+') ? 2 : 1;
        }
        else if (sscanf(currentBuffer, "%dx^%d%n", &coefficient, &exponent, &numBytes) == 2) {
        }
        else if (sscanf(currentBuffer, "%d%n", &coefficient, &numBytes) == 1) {
            exponent = 0;
        }
        else {
            printf("Invalid input format at: %s\n", currentBuffer);
            return EXIT_FAILURE;
        }

        if (coefficient != 0) {
            newElement = createNewElement(coefficient, exponent);
            if (!newElement) {
                return ERROR_MEMORY_ALLOCATION;
            }
            insertSorted(head, newElement);
        }

        currentBuffer += numBytes;
    }

    return EXIT_SUCCESS;
}

Position createNewElement(int coefficient, int exponent) {
    Position element = NULL;
    element = (Position)malloc(sizeof(Element));
    if (!element)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    element->coefficient = coefficient;
    element->exponent = exponent;
    element->next = NULL;
    return element;
}

int insertSorted(Position head, Position newElement) {
    Position current = head;
    while (current->next && current->next->exponent > newElement->exponent)
        current = current->next;
    mergeAfter(current, newElement);
    return EXIT_SUCCESS;
}

int mergeAfter(Position current, Position newElement) {
    if (current->next && current->next->exponent == newElement->exponent) {
        current->next->coefficient += newElement->coefficient;
        free(newElement);
    }
    else 
    {
        insertAfter(current, newElement);
    }

    return EXIT_SUCCESS;
}

int insertAfter(Position current, Position newElement) {
    newElement->next = current->next;
    current->next = newElement;
    return EXIT_SUCCESS;
}

int freeMemory(Position head) {
    while (head->next)
        deleteAfter(head);
    return EXIT_SUCCESS;
}

int deleteAfter(Position previous) {
    Position toDelete = previous->next;
    previous->next = toDelete->next;
    free(toDelete);
    return EXIT_SUCCESS;
}

int createAndInsertAfter(int coefficient, int exponent, Position current) {
    Position newElement = createNewElement(coefficient, exponent);
    if (!newElement)
    {
        printf("Memory allocation failed!\n");
        return ERROR_MEMORY_ALLOCATION;
    }

    insertAfter(current, newElement);
    return EXIT_SUCCESS;
}
