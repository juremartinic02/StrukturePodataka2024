#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_LENGTH (1024)
#define MAX_SIZE (50)

struct _stackElement;
typedef struct _stackElement* Position;
typedef struct _stackElement {
    double number;
    Position next;
} StackElement;

/*
    funckija calculatePostfixFromFile
    ------------------------------------------------
        Racuna napisani postfiks iz tekstualne datoteke "test.txt"
*/
int calculatePostfixFromFile(Position head, char* fileName, double* result);

/*
    funkcija readFile
    ---------------------------------
        Provjerava da li je moguce ucitati tekstualnu datoteku ako se tekstualna datoteka prodje provjeru onda je funckija cita
*/
int readFile(char* fileName, char* buffer);

/*
    funckija parseStringIntoPostfix
    ------------------------------------------
        Ucitan string iz tekstualne datoteke pretvara u postfiks
*/ 
int parseStringIntoPostfix(Position head, char* buffer, double* result);

/*
    funkcija checkStackAndExtractResult
    ---------------------------------------------
        Provjerava stack i ako postfiks nije zadovoljavajuc govori nam da provjerimo tekstualnu datoteku
            ako je postfiks zadovoljavajuc izvlazi ga iz datoteke i prosljedjuje sljedecoj funkciji
*/
int checkStackAndExtractResult(Position head, double* result);

Position createStackElement(double number);
 
/*
    funkcija push
    --------------------
        dodaje novi element na pocetak stacka
*/
int push(Position head, Position newStackElement);

/*
    funckija printStack
    ----------------------------
        Ispisuje stack u konzolu
*/  
int printStack(Position first);
/*
    fukcija pop
    -------------------
    Izbacuje element sa pocetka stacka te nam vraca njegovu vrijednost
*/
int pop(Position head, double* result);

/*
    funckija popAndPerformOperation
    -------------------------------------------
        izbacuje element sa pocetka stacka dok ne dodje do kraja stacka
            nad svakim elementom provodi jednu od cetiri funkcije koje smo funckiji dodijelili
*/
int popAndPerformOperation(Position head, char operation, double* result);

int main() {
    StackElement head = {
        .number = 0,
        .next = NULL };

    double result = 0;

    if (calculatePostfixFromFile(&head, "test.txt", &result) == EXIT_SUCCESS) {
        printf("Result is: %0.1lf\n", result);
    }

    return EXIT_SUCCESS;
}

int calculatePostfixFromFile(Position head, char* fileName, double* result) {
    char buffer[MAX_LENGTH] = { 0 };
    int status = 0;

    if (readFile(fileName, buffer) != EXIT_SUCCESS || !buffer) {
        return EXIT_FAILURE;
    }

    status = parseStringIntoPostfix(head, buffer, result);
    if (status != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int readFile(char* fileName, char* buffer) {
    FILE* filePointer = NULL;
    int numBytes = 0;

    filePointer = fopen(fileName, "r");
    if (!filePointer) {
        perror("Can't open file!\n");
        return EXIT_FAILURE;
    }

    // fread -> sprema sadrzaj tekstualne datoteke u buffer
    numBytes = fread(buffer, sizeof(char), MAX_LENGTH, filePointer);
    if (numBytes <= 0) {
        fclose(filePointer);
        return EXIT_FAILURE;
    }

    fclose(filePointer);

    return EXIT_SUCCESS;
}

int parseStringIntoPostfix(Position head, char* buffer, double* result) {
    char* currentBuffer = buffer;
    int status = 0;
    int numBytes = 0;
    char operation = 0;
    double number = 0.0;
    Position newStackElement = NULL;

    while (strlen(currentBuffer) > 0) {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
        if (status == 1) {
            newStackElement = createStackElement(number);
            if (!newStackElement) {
                return EXIT_FAILURE;
            }
            push(head, newStackElement);
        }
        else {
            status = sscanf(currentBuffer, " %c %n", &operation, &numBytes);
            if (status != 1) {
                printf("Error reading operator: %s\n", currentBuffer);
                return EXIT_FAILURE;
            }

            status = popAndPerformOperation(head, operation, result);
            if (status != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }

            newStackElement = createStackElement(*result);
            if (!newStackElement) {
                return EXIT_FAILURE;
            }
            push(head, newStackElement);
        }

        currentBuffer += numBytes;
    }

    return checkStackAndExtractResult(head, result);
}


int checkStackAndExtractResult(Position head, double* result) {
    int status = pop(head, result);
    if (status != EXIT_SUCCESS) {
        printf("Error: Could not extract the final result.\n");
        return EXIT_FAILURE;
    }

    if (head->next) {
        printf("Error: Invalid postfix expression. Stack is not empty after evaluation.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


Position createStackElement(double number)
{
    Position newStackElement = NULL;

    newStackElement = (Position)malloc(sizeof(StackElement));
    if (!newStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newStackElement->number = number;
    newStackElement->next = NULL;

    return newStackElement;
}

int push(Position head, Position newStackElement) {
    newStackElement->next = head->next;
    head->next = newStackElement;

    printStack(head->next);

    return EXIT_SUCCESS;
}

int printStack(Position first) {
    Position current = first;

    while (current) {
        printf(" %0.1lf", current->number);
        current = current->next;
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int pop(Position head, double* result) {
    Position toDelete = NULL;

    // provjeravamo da li je nakon zadnjeg pop-a ostao element u stack-u
    toDelete = head->next;
    if (!toDelete) {
        printf("Stack is empty! Program has nothing to pop!\n");
        return -1;
    }

    head->next = toDelete->next;
    *result = toDelete->number;
    free(toDelete);

    return EXIT_SUCCESS;
}

int popAndPerformOperation(Position head, char operation, double* result) {
    double leftOperand = 0;
    double rightOperand = 0;
    int result1 = 0;
    int result2 = 0;

    result1 = pop(head, &leftOperand);
    if (result1 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    result2 = pop(head, &rightOperand);
    if (result2 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    switch (operation) {
    case '+':
        *result = rightOperand + leftOperand;
        break;
    case '-':
        *result = rightOperand - leftOperand;
        break;
    case '*':
        *result = rightOperand * leftOperand;
        break;
    case '/':
        *result = rightOperand / leftOperand;
        break;
    default:
        printf("Operation %c not supported yet!\r\n", operation);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}