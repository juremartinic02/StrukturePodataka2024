#include "definicijaStruktura.h"

/*
    Funkcija createReciept
    -------------------------------
        Alocira memoriju za novi racun, postavlja datum izdavanja,
        inicijalizira listu artikala za racun i postavlja pokazivace.
*/
RecieptPosition createReciept(Date date);

/*
    Funkcija createArticle
    -------------------------------
        Alocira memoriju za novi artikl, kopira podatke o artiklu
        (ime, kolicina i cijena) i inicijalizira pokazivace.
*/
ArticlePosition createArticle(char* name, int quantity, double price);

/*
    Funkcija insertSortedReciept
    -------------------------------
        Ubacuje novi racun u sortiranu listu racuna prema datumu izdavanja.
        Ako racun s istim datumom vec postoji, artikli se spajaju.
*/
int insertSortedReciept(RecieptPosition head, RecieptPosition newReciept);

/*
    Funkcija insertSortedArticle
    -------------------------------
        Ubacuje novi artikl u sortiranu listu artikala prema nazivu artikla.
*/
int insertSortedArticle(ArticlePosition head, ArticlePosition newArticle);

/*
    Funkcija readReciepts
    -------------------------------
        Ucitava listu racuna iz datoteke. Svaki redak datoteke
        predstavlja naziv druge datoteke iz koje se ucitava pojedinacni racun.
*/
int readReciepts(char* fileName, RecieptPosition head);

/*
    Funkcija readSingleReciept
    -------------------------------
        Ucitava pojedinacni racun iz datoteke. Datoteka sadrzi datum izdavanja
        i listu artikala s kolicinom i cijenom.
*/
int readSingleReciept(char* fileName, RecieptPosition head);

/*
    Funkcija printReciepts
    -------------------------------
        Ispisuje sve racune u listi. Svaki racun sadrzi datum
        i listu artikala s nazivom, kolicinom i cijenom.
*/
void printReciepts(RecieptPosition head);

/*
    Funkcija freeReciepts
    -------------------------------
        Oslobadja memoriju zauzetu za listu racuna i sve povezane artikle.
*/
void freeReciepts(RecieptPosition head);

/*
    Funkcija artikliUpit
    -------------------------------
        Omogucava korisniku da unese naziv artikla i vremenski raspon.
        Racuna i ispisuje ukupnu potrosnju i kolicinu za odabrani artikl.
*/
void articleQuery(RecieptPosition head);

/*
    Funkcija calculateArticleExpenses
    -------------------------------
        Racuna ukupnu cijenu i kolicinu odabranog artikla u datom vremenskom
        rasponu. Rezultati se ispisuju na ekran.
*/
float calculateArticleExpenses(RecieptPosition head, const char* articleName,
    int startDay, int startMonth, int startYear,
    int endDay, int endMonth, int endYear);

/*
    Funkcija exportToCSV
    -------------------------------
        Izvozi sve racune i pripadajuce artikle u CSV datoteku.
        Korisnik unosi naziv izlazne datoteke.
*/
void exportToCSV(RecieptPosition head);

/*
    Funkcija exportSpecificDateRangeToCSV
    -------------------------------
        Izvozi racune i pripadajuce artikle unutar zadanog vremenskog
        raspona u CSV datoteku. Korisnik unosi naziv izlazne datoteke.
*/
void exportSpecificDateRangeToCSV(RecieptPosition head);

/*
    Funkcija logAction
    -------------------------------
        Dodaje zapis o korisnickim akcijama u log.txt datoteku.
*/
void logAction(char* action);

/*
    Funckija dateCompare
    -------------------------------------
        Omogucuje lakse usporedjivanje datuma koje se lako implementira u ostale funkcije
*/
int dateCompare(Date*, Date*);



int main() {
    RecieptPosition recieptHead = NULL;
    recieptHead = (RecieptPosition)malloc(sizeof(Reciept));
    if (!recieptHead) {
        printf("Memory allocation failed!\n");
        return MEMORY_ERROR;
    }
    recieptHead->next = NULL;

    if (readReciepts("racuni.txt", recieptHead) == FILE_ERROR) {
        printf("Error reading receipts!\n");
        freeReciepts(recieptHead);
        return FILE_ERROR;
    }

    int choice;
    do {
        printf("\nOdaberite opciju:\n");
        printf("1. Ispis svih racuna\n");
        printf("2. Upit za artikl\n");
        printf("3. Export svih racuna u CSV\n");
        printf("4. Export racuna u zadanom vremenskom rasponu u CSV\n");
        printf("0. Izlaz\n");
        (void)scanf("%d", &choice);

        switch (choice) {
        case 1:
            printReciepts(recieptHead);
            break;
        case 2:
            articleQuery(recieptHead);
            break;
        case 3:
            exportToCSV(recieptHead);
            break;
        case 4:
            exportSpecificDateRangeToCSV(recieptHead);
            break;
        case 0:
            break;
        default:
            printf("Pogresan unos!\n");
        }
    } while (choice != 0);

    freeReciepts(recieptHead);


    return 0;
}

RecieptPosition createReciept(Date date) {
    RecieptPosition newReciept = (RecieptPosition)malloc(sizeof(Reciept));
    if (!newReciept) {
        printf("Memory allocation failed!\n");
        return MEMORY_ERROR;
    }
    newReciept->dateOfIssue = date;
    newReciept->articleHead = (ArticlePosition)malloc(sizeof(Article));
    if (!newReciept->articleHead) {
        printf("Memory allocation failed!\n");
        free(newReciept);
        return MEMORY_ERROR;
    }
    newReciept->articleHead->next = NULL;
    newReciept->next = NULL;

    return newReciept;
}

ArticlePosition createArticle(char* name, int quantity, double price) {
    ArticlePosition newArticle = NULL;
    newArticle = (ArticlePosition)malloc(sizeof(Article));
    if (!newArticle) {
        printf("Memory allocation failed!\n");
        return MEMORY_ERROR;
    }
    strcpy(newArticle->name, name);
    newArticle->quantity = quantity;
    newArticle->price = price;
    newArticle->next = NULL;

    return newArticle;
}

int insertSortedReciept(RecieptPosition head, RecieptPosition newReciept) {
    RecieptPosition temp = head;

    while (temp->next != NULL) {
        int comparisonResult = dateCompare(&(temp->next->dateOfIssue), &(newReciept->dateOfIssue));
        if (comparisonResult == 0) {
            ArticlePosition currentNewArticle = newReciept->articleHead->next;
            while (currentNewArticle) {
                ArticlePosition existingArticle = temp->next->articleHead->next;
                int articleFound = 0;

                while (existingArticle) {
                    if (strcmp(existingArticle->name, currentNewArticle->name) == 0) {
                        existingArticle->quantity += currentNewArticle->quantity;
                        existingArticle->price += currentNewArticle->price;
                        articleFound = 1;
                        break;
                    }
                    existingArticle = existingArticle->next;
                }

                if (!articleFound) {
                    insertSortedArticle(temp->next->articleHead,
                        createArticle(currentNewArticle->name,
                            currentNewArticle->quantity,
                            currentNewArticle->price));
                }

                currentNewArticle = currentNewArticle->next;
            }

            freeReciepts(newReciept);
            return 0;
        }

        if (comparisonResult < 0) {
            break;
        }

        temp = temp->next;
    }

    newReciept->next = temp->next;
    temp->next = newReciept;

    return 0;
}


int insertSortedArticle(ArticlePosition head, ArticlePosition newArticle) {
    ArticlePosition temp = head;
    while (temp->next != NULL && strcmp(temp->next->name, newArticle->name) < 0) {
        temp = temp->next;
    }

    newArticle->next = temp->next;
    temp->next = newArticle;

    return 0;
}

int readReciepts(char* fileName, RecieptPosition head) {
    FILE* file = NULL;
    file = fopen(fileName, "r");
    if (!file) {
        printf("Could not open file %s!\n", fileName);
        return FILE_ERROR;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (readSingleReciept(buffer, head) == FILE_ERROR) {
            fclose(file);
            return FILE_ERROR;
        }
    }

    fclose(file);
    return 0;
}

int readSingleReciept(char* fileName, RecieptPosition head) {
    FILE* file = NULL;
    file = fopen(fileName, "r");
    if (!file) {
        printf("Could not open file %s!\n", fileName);
        return FILE_ERROR;
    }

    char buffer[MAX_LINE_LENGTH];
    Date date;
    fgets(buffer, MAX_LINE_LENGTH, file);
    (void)sscanf(buffer, "%d-%d-%d", &date.day, &date.month, &date.year);

    RecieptPosition newReciept = createReciept(date);
    if (!newReciept) {
        fclose(file);
        return MEMORY_ERROR;
    }

    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        char name[MAX_NAME_LENGTH];
        int quantity;
        double price;

        (void)sscanf(buffer, "%s %d %lf", name, &quantity, &price);
        ArticlePosition newArticle = createArticle(name, quantity, price);
        if (!newArticle) {
            fclose(file);
            return MEMORY_ERROR;
        }

        insertSortedArticle(newReciept->articleHead, newArticle);
    }

    insertSortedReciept(head, newReciept);

    fclose(file);
    return 0;
}

void printReciepts(RecieptPosition head) {
    RecieptPosition temp = head->next;
    while (temp) {
        printf("Date: %02d-%02d-%04d\n", temp->dateOfIssue.day, temp->dateOfIssue.month, temp->dateOfIssue.year);

        ArticlePosition article = temp->articleHead->next;
        while (article) {
            printf("\t%s %d %.2f\n", article->name, article->quantity, article->price);
            article = article->next;
        }

        temp = temp->next;
    }
}

void freeReciepts(RecieptPosition head) {
    if (!head) return;

    RecieptPosition tempReceipt = head->next;
    while (tempReceipt) {
        ArticlePosition article = tempReceipt->articleHead->next;
        ArticlePosition tempArticle = NULL;
        while (article) {
            tempArticle = article;
            article = article->next;
            free(tempArticle);
        }

        free(tempReceipt->articleHead);

        RecieptPosition toFree = tempReceipt;
        tempReceipt = tempReceipt->next;
        free(toFree);
    }

    free(head);
}

void articleQuery(RecieptPosition head) {
    char articleName[MAX_NAME_LENGTH];
    int startDay, startMonth, startYear;
    int endDay, endMonth, endYear;

    printf("Unesite naziv artikla: ");
    (void)scanf("%s", articleName);

    printf("Unesite pocetni datum (dan mjesec godina): ");
    (void)scanf("%d %d %d", &startDay, &startMonth, &startYear);

    printf("Unesite zavrsni datum (dan mjesec godina): ");
    (void)scanf("%d %d %d", &endDay, &endMonth, &endYear);

    calculateArticleExpenses(head, articleName,
        startDay, startMonth, startYear,
        endDay, endMonth, endYear);
}

float calculateArticleExpenses(RecieptPosition head, const char* articleName,
    int startDay, int startMonth, int startYear,
    int endDay, int endMonth, int endYear) {
    double totalExpense = 0.0;
    int totalQuantity = 0;

    RecieptPosition currentReciept = head->next;
    while (currentReciept) {
        if ((currentReciept->dateOfIssue.year > startYear ||
            (currentReciept->dateOfIssue.year == startYear &&
                (currentReciept->dateOfIssue.month > startMonth ||
                    (currentReciept->dateOfIssue.month == startMonth &&
                        currentReciept->dateOfIssue.day >= startDay))) &&
            (currentReciept->dateOfIssue.year < endYear ||
                (currentReciept->dateOfIssue.year == endYear &&
                    (currentReciept->dateOfIssue.month < endMonth ||
                        (currentReciept->dateOfIssue.month == endMonth &&
                            currentReciept->dateOfIssue.day <= endDay)))))) {

            ArticlePosition currentArticle = currentReciept->articleHead->next;
            while (currentArticle) {
                if (strcmp(currentArticle->name, articleName) == 0) {
                    totalExpense += currentArticle->quantity * currentArticle->price;
                    totalQuantity += currentArticle->quantity;
                }
                currentArticle = currentArticle->next;
            }
        }
        currentReciept = currentReciept->next;
    }

    printf("Artikl: %s\n", articleName);
    printf("Ukupna potrosnja: %.2f\n", totalExpense);
    printf("Ukupna kolicina: %d\n", totalQuantity);

    return totalExpense;
}


void exportToCSV(RecieptPosition head) {
    char filename[MAX_FILENAME_LENGTH];
    printf("Unesite naziv CSV datoteke (npr. izvjestaj.csv): ");
    (void)scanf("%s", filename);

    FILE* csvFile = NULL;
    csvFile = fopen(filename, "w");
    if (!csvFile) {
        printf("Greska prilikom otvaranja datoteke za pisanje!\n");
        return;
    }

    fprintf(csvFile, "Datum,Artikl,Kolicina,Cijena,Ukupno\n");

    RecieptPosition currentReciept = head->next;
    while (currentReciept) {
        ArticlePosition currentArticle = currentReciept->articleHead->next;
        while (currentArticle) {
            fprintf(csvFile, "%02d-%02d-%04d,%s,%d,%.2f,%.2f\n",
                currentReciept->dateOfIssue.day,
                currentReciept->dateOfIssue.month,
                currentReciept->dateOfIssue.year,
                currentArticle->name,
                currentArticle->quantity,
                currentArticle->price,
                currentArticle->quantity * currentArticle->price);
            currentArticle = currentArticle->next;
        }
        currentReciept = currentReciept->next;
    }

    fclose(csvFile);
    printf("CSV datoteka %s uspjesno kreirana!\n", filename);

    char logMessage[MAX_LINE_LENGTH];
    snprintf(logMessage, MAX_LINE_LENGTH, "Korisnik je izvezao sve racune u datoteku %s.", filename);
    logAction(logMessage);
}


void exportSpecificDateRangeToCSV(RecieptPosition head) {
    int startDay, startMonth, startYear;
    int endDay, endMonth, endYear;
    char filename[MAX_FILENAME_LENGTH];

    printf("Unesite pocetni datum (dan mjesec godina): ");
    (void)scanf("%d %d %d", &startDay, &startMonth, &startYear);

    printf("Unesite zavrsni datum (dan mjesec godina): ");
    (void)scanf("%d %d %d", &endDay, &endMonth, &endYear);

    printf("Unesite naziv CSV datoteke (npr. izvjestaj_period.csv): ");
    (void)scanf("%s", filename);

    FILE* csvFile = NULL;
    csvFile = fopen(filename, "w");
    if (!csvFile) {
        printf("Greska prilikom otvaranja datoteke za pisanje!\n");
        return;
    }

    fprintf(csvFile, "Datum,Artikl,Kolicina,Cijena,Ukupno\n");

    RecieptPosition currentReciept = head->next;
    while (currentReciept) {
        if ((currentReciept->dateOfIssue.year > startYear ||
            (currentReciept->dateOfIssue.year == startYear &&
                (currentReciept->dateOfIssue.month > startMonth ||
                    (currentReciept->dateOfIssue.month == startMonth &&
                        currentReciept->dateOfIssue.day >= startDay))) &&
            (currentReciept->dateOfIssue.year < endYear ||
                (currentReciept->dateOfIssue.year == endYear &&
                    (currentReciept->dateOfIssue.month < endMonth ||
                        (currentReciept->dateOfIssue.month == endMonth &&
                            currentReciept->dateOfIssue.day <= endDay)))))) {

            ArticlePosition currentArticle = currentReciept->articleHead->next;
            while (currentArticle) {
                fprintf(csvFile, "%02d-%02d-%04d,%s,%d,%.2f,%.2f\n",
                    currentReciept->dateOfIssue.day,
                    currentReciept->dateOfIssue.month,
                    currentReciept->dateOfIssue.year,
                    currentArticle->name,
                    currentArticle->quantity,
                    currentArticle->price,
                    currentArticle->quantity * currentArticle->price);
                currentArticle = currentArticle->next;
            }
        }
        currentReciept = currentReciept->next;
    }

    fclose(csvFile);
    printf("CSV datoteka %s uspjesno kreirana!\n", filename);
}

void logAction(char* action) {
    FILE* logFile = fopen("log.txt", "a");
    if (!logFile) {
        printf("Greska prilikom otvaranja datoteke za logiranje!\n");
        logFile = fopen("log.txt", "w");
        if (!logFile) {
            printf("Greska pri kreiranju log.txt datoteke!\n");
            return;
        }
    }
    fprintf(logFile, "%s\n", action);
    fclose(logFile);
}

int dateCompare(Date* date1, Date* date2) {
    int result = date1->year - date2->year;
    if (result == 0) {
        result = date1->month - date2->month;
        if (result == 0) {
            result = date1->day - date2->day;
        }
    }
    return result;
}
