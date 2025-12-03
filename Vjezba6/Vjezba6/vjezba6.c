#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prototipovi funkcija

int insertInvoiceSorted(struct Invoice** head, const char* date);
int insertProductSorted(struct Product** head, const char* name, int qty, float price);
int loadInvoice(struct Invoice** head, const char* filename);
int loadAllInvoices(struct Invoice** head);
int printInvoice(struct Invoice* head);
int searchProductRange(struct Invoice* head, const char* productName, const char* from, const char* to);
int summarizeInvoices(struct Invoice* head);
int freeAllInvoices(struct Invoice* head);
int bestSellerInRange(struct Invoice* head, const char* from, const char* to);
int top5InRange(struct Invoice* head, const char* from, const char* to);
int addInvoiceManual(struct Invoice** head);

typedef struct Product {
    char name[50];
    int qty;
    float price;
    struct Product* next;
} Product;

typedef struct Invoice {
    char date[11];
    Product* products;
    struct Invoice* next;
} Invoice;

//Funkcija za umetanje novih ra?una

int insertInvoiceSorted(Invoice** head, const char* date) {
    Invoice* inv = (Invoice*)malloc(sizeof(Invoice));
    if (!inv) {
        perror("Neuspjela alokacija memorije za Invoice");
        exit(EXIT_FAILURE);
    }

    strcpy(inv->date, date);
    inv->products = NULL;
    inv->next = NULL;

    if (*head == NULL || strcmp(date, (*head)->date) < 0) {
        inv->next = *head;
        *head = inv;
        return 0;
    }

    Invoice* cur = *head;
    while (cur->next != NULL && strcmp(cur->next->date, date) < 0)
        cur = cur->next;

    inv->next = cur->next;
    cur->next = inv;
    return 0;
}

//Funkcija za umetanje artikla u listu artikala

int insertProductSorted(Product** head, const char* name, int qty, float price) {
    Product* p = (Product*)malloc(sizeof(Product));
    if (!p) {
        perror("Neuspjela alokacija memorije za Product");
        exit(EXIT_FAILURE);
    }

    strcpy(p->name, name);
    p->qty = qty;
    p->price = price;
    p->next = NULL;

    if (*head == NULL || strcmp(name, (*head)->name) < 0) {
        p->next = *head;
        *head = p;
        return 0;
    }

    Product* cur = *head;
    while (cur->next != NULL && strcmp(cur->next->name, name) < 0)
        cur = cur->next;

    p->next = cur->next;
    cur->next = p;
    return 0;
}

//Funkcija za u?itavanje ra?una

int loadInvoice(Invoice** head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Ne mogu otvoriti datoteku %s\n", filename);
        return -1;
    }

    char dateLine[20];

    if (fgets(dateLine, sizeof(dateLine), f) == NULL) { fclose(f); return -1; }
    dateLine[strcspn(dateLine, "\r\n")] = 0;

    if (insertInvoiceSorted(head, dateLine) != 0) { fclose(f); return -1; }

    Invoice* inv = *head;
    while (inv->next != NULL) inv = inv->next;

    char line[200];
    char name[50];
    int qty;
    float price;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%49[^,],%d,%f", name, &qty, &price) == 3) {
            insertProductSorted(&inv->products, name, qty, price);
        }
    }

    fclose(f);
    return 0;
}

//Funkcija za ?itanje datoteke 

int loadAllInvoices(Invoice** head) {
    FILE* f = fopen("invoices.txt", "r");
    if (!f) {
        fprintf(stderr, "Ne mogu otvoriti invoices.txt\n");
        return -1;
    }

    char filename[200];

    while (fgets(filename, sizeof(filename), f)) {
        filename[strcspn(filename, "\r\n")] = 0;
        if (strlen(filename) == 0) continue;

        loadInvoice(head, filename);
    }

    fclose(f);
    return 0;
}

//Funkcija za traženje datuma i ispis ra?una

int printInvoice(Invoice* head) {
    char date[11];
    printf("Unesi datum (YYYY-MM-DD): ");
    scanf("%10s", date);

    while (head) {
        if (strcmp(head->date, date) == 0) {
            printf("\nRacun %s:\n", head->date);

            Product* p = head->products;
            while (p) {
                printf("%s  %d kom  %.2f EUR\n", p->name, p->qty, p->price);
                p = p->next;
            }
            return 0;
        }
        head = head->next;
    }

    printf("Nema tog racuna.\n");
    return 0;
}

//Funkcija za zbrajanje koli?ine i cijene

int searchProductRange(Invoice* head, const char* productName, const char* from, const char* to) {
    int totalQty = 0;
    float totalSpent = 0.0f;

    while (head) {
        if (strcmp(head->date, from) >= 0 && strcmp(head->date, to) <= 0) {
            Product* p = head->products;
            while (p) {
                if (strcmp(p->name, productName) == 0) {
                    totalQty += p->qty;
                    totalSpent += p->qty * p->price;
                }
                p = p->next;
            }
        }
        head = head->next;
    }

    printf("\nARTIKL: %s\n", productName);
    printf("Razdoblje: %s  ->  %s\n", from, to);
    printf("Ukupno kupljeno: %d kom\n", totalQty);
    printf("Ukupno potroseno: %.2f EUR\n", totalSpent);

    return 0;
}

//Funkcija za zbrajanje svih artikle kroz ra?un

int summarizeInvoices(Invoice* head) {
    struct Stat { char name[50]; int qty; float total; };
    struct Stat stats[300] = { 0 };
    int n = 0;
    float grandTotal = 0.0f;

    while (head) {
        Product* p = head->products;
        while (p) {
            int found = 0;
            for (int j = 0; j < n; j++) {
                if (strcmp(stats[j].name, p->name) == 0) {
                    stats[j].qty += p->qty;
                    stats[j].total += p->qty * p->price;
                    found = 1;
                    break;
                }
            }
            if (!found && n < 300) {
                strcpy(stats[n].name, p->name);
                stats[n].qty = p->qty;
                stats[n].total = p->qty * p->price;
                n++;
            }
            p = p->next;
        }
        head = head->next;
    }

    printf("\n--- OBRACUN SVIH RACUNA ---\n");
    for (int j = 0; j < n; j++) {
        printf("%s  %d kom  %.2f EUR\n", stats[j].name, stats[j].qty, stats[j].total);
        grandTotal += stats[j].total;
    }

    printf("-------------------------------\n");
    printf("UKUPNO POTROSENO: %.2f EUR\n", grandTotal);
    return 0;
}

//Funkcija za pronalaženje artikala koji su prodani po najve?oj koli?ini

int bestSellerInRange(Invoice* head, const char* from, const char* to) {
    struct Stat { char name[50]; int qty; float total; };
    struct Stat stats[300] = { 0 };
    int n = 0;

    while (head) {
        if (strcmp(head->date, from) >= 0 && strcmp(head->date, to) <= 0) {
            Product* p = head->products;
            while (p) {
                int found = 0;
                for (int j = 0; j < n; j++) {
                    if (strcmp(stats[j].name, p->name) == 0) {
                        stats[j].qty += p->qty;
                        stats[j].total += p->qty * p->price;
                        found = 1;
                        break;
                    }
                }
                if (!found && n < 300) {
                    strcpy(stats[n].name, p->name);
                    stats[n].qty = p->qty;
                    stats[n].total = p->qty * p->price;
                    n++;
                }
                p = p->next;
            }
        }
        head = head->next;
    }

    if (n == 0) {
        printf("Nema podataka u zadanom razdoblju.\n");
        return 0;
    }

    int maxQty = 0;
    for (int j = 0; j < n; j++) if (stats[j].qty > maxQty) maxQty = stats[j].qty;

    printf("\n--- NAJPRODAVANIJI ARTIKL(I) U RASPOONU %s -> %s ---\n", from, to);
    for (int j = 0; j < n; j++) {
        if (stats[j].qty == maxQty) {
            printf("%s  |  Ukupna kolicina: %d kom  |  Ukupno potroseno: %.2f EUR\n",
                stats[j].name, stats[j].qty, stats[j].total);
        }
    }

    return 0;
}

//Funkciaj za ispis top 5 artikala

int top5InRange(Invoice* head, const char* from, const char* to) {
    struct Stat { char name[50]; int qty; float total; };
    struct Stat stats[300] = { 0 };
    int n = 0;

    while (head) {
        if (strcmp(head->date, from) >= 0 && strcmp(head->date, to) <= 0) {
            Product* p = head->products;
            while (p) {
                int found = 0;
                for (int j = 0; j < n; j++) {
                    if (strcmp(stats[j].name, p->name) == 0) {
                        stats[j].qty += p->qty;
                        stats[j].total += p->qty * p->price;
                        found = 1;
                        break;
                    }
                }
                if (!found && n < 300) {
                    strcpy(stats[n].name, p->name);
                    stats[n].qty = p->qty;
                    stats[n].total = p->qty * p->price;
                    n++;
                }
                p = p->next;
            }
        }
        head = head->next;
    }

    if (n == 0) {
        printf("Nema podataka u zadanom razdoblju.\n");
        return 0;
    }

    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            if (stats[j].qty > stats[idx].qty) idx = j;
        }
        if (idx != i) {
            struct Stat tmp = stats[i];
            stats[i] = stats[idx];
            stats[idx] = tmp;
        }
    }

    int limit = (n < 5) ? n : 5;
    printf("\n--- TOP %d ARTIKALA U RASPOONU %s -> %s ---\n", limit, from, to);
    for (int j = 0; j < limit; j++) {
        printf("%d. %s  |  Ukupna kolicina: %d kom  |  Ukupno potroseno: %.2f EUR\n",
            j + 1, stats[j].name, stats[j].qty, stats[j].total);
    }

    return 0;
}

//Funkcija za ru?no unošenje ra?una


int addInvoiceManual(Invoice** head) {
    char date[20];
    printf("Unesite datum novog racuna (YYYY-MM-DD): ");
    if (scanf("%19s", date) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        printf("Neispravan unos datuma.\n");
        return -1;
    }
    int c; while ((c = getchar()) != '\n' && c != EOF); 

    if (insertInvoiceSorted(head, date) != 0) {
        fprintf(stderr, "Greska pri umetnju racuna.\n");
        return -1;
    }

    Invoice* inv = *head;
    while (inv->next != NULL) inv = inv->next;

    printf("Unesite artikle.\n");
    printf("Format: (1): Naziv,kolicina,cijena  ili  korak po korak: naziv [ENTER] kolicina [ENTER] cijena [ENTER]\n");
    printf("Za kraj ostavite naziv praznim i pritisnite ENTER.\n");

    char line[256];
    while (1) {
        printf("Naziv artikla (ili prazno za kraj): ");
        if (!fgets(line, sizeof(line), stdin)) break; 
        line[strcspn(line, "\r\n")] = 0;

        if (line[0] == '\0') {
            break;
        }

		char* tokens[50] = { 0 };
        int tokcount = 0;
        char tmp[256];
        strncpy(tmp, line, sizeof(tmp) - 1); tmp[sizeof(tmp) - 1] = 0;

        char* p = strtok(tmp, ",");
        while (p && tokcount < 50) {
            tokens[tokcount++] = p;
            p = strtok(NULL, ",");
        }

        char name[50];
        int qty = 0;
        float price = 0.0f;
        int parsed_ok = 0;

        if (tokcount >= 3) {
            strncpy(name, tokens[0], sizeof(name) - 1); name[sizeof(name) - 1] = 0;
            qty = atoi(tokens[1]);

            char price_str[128] = "";
            for (int i = 2; i < tokcount; ++i) {
                if (i > 2) strncat(price_str, ",", sizeof(price_str) - strlen(price_str) - 1);
                strncat(price_str, tokens[i], sizeof(price_str) - strlen(price_str) - 1);
            }
            for (char* qptr = price_str; *qptr; ++qptr) if (*qptr == ',') *qptr = '.';
            price = (float)strtod(price_str, NULL);
            parsed_ok = 1;
        }
        else {
            strncpy(name, line, sizeof(name) - 1); name[sizeof(name) - 1] = 0;

            char qtyline[64];
            printf("Kolicina: ");
            if (!fgets(qtyline, sizeof(qtyline), stdin)) break;
            qtyline[strcspn(qtyline, "\r\n")] = 0;
            if (qtyline[0] == '\0') { printf("Kolicina ne moze biti prazna. Preskacemo artikl.\n"); continue; }
            qty = atoi(qtyline);

            char priceline[64];
            printf("Cijena (npr. 1.20 ili 1,20): ");
            if (!fgets(priceline, sizeof(priceline), stdin)) break;
            priceline[strcspn(priceline, "\r\n")] = 0;
            if (priceline[0] == '\0') { printf("Cijena ne moze biti prazna. Preskacemo artikl.\n"); continue; }
            for (char* qptr = priceline; *qptr; ++qptr) if (*qptr == ',') *qptr = '.';
            price = (float)strtod(priceline, NULL);

            parsed_ok = 1;
        }

        if (!parsed_ok) {
            printf("Neuspjelo parsiranje unosa. Pokusajte ponovno.\n");
            continue;
        }

      
        char* start = name;
        while (*start == ' ') start++;
        if (start != name) memmove(name, start, strlen(start) + 1);
        size_t ln = strlen(name);
        while (ln > 0 && name[ln - 1] == ' ') { name[ln - 1] = '\0'; ln--; }

        insertProductSorted(&inv->products, name, qty, price);
        printf("Dodano: %s  %d kom  %.2f EUR\n", name, qty, price);
    }

    printf("Racun dodan.\n");
    return 0;
}


//Funkcija za osloba?anje memorije ra?una i artikala

int freeAllInvoices(Invoice* head) {
    Invoice* nextInv;
    Product* nextProd;

    while (head) {
        Product* p = head->products;
        while (p) {
            nextProd = p->next;
            free(p);
            p = nextProd;
        }
        nextInv = head->next;
        free(head);
        head = nextInv;
    }
    return 0;
}

int main() {
    Invoice* invoices = NULL;

    if (loadAllInvoices(&invoices) != 0) {
        printf("Greska pri ucitavanju.\n");
    }

    int choice;
    char name[50];
    char from[20], to[20];

    do {
        printf("\n1 - Ispis racuna\n");
        printf("2 - Pretraga artikla\n");
        printf("3 - Obracun svih racuna\n");
        printf("4 - Najprodavaniji artikl u rasponu datuma (i Top 5)\n");
        printf("5 - Dodaj novi racun rucno\n");
        printf("0 - Izlaz iz programa\n");
        printf("Izbor: ");
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            choice = -1;
            continue;
        }

        if (choice == 1)
            printInvoice(invoices);
        else if (choice == 2) {
            printf("Unesi artikl: ");
            scanf("%49s", name);

            printf("Od datuma (YYYY-MM-DD): ");
            scanf("%19s", from);

            printf("Do datuma (YYYY-MM-DD): ");
            scanf("%19s", to);

            searchProductRange(invoices, name, from, to);
        }
        else if (choice == 3)
            summarizeInvoices(invoices);
        else if (choice == 4) {
            printf("Od datuma (YYYY-MM-DD): ");
            scanf("%19s", from);
            printf("Do datuma (YYYY-MM-DD): ");
            scanf("%19s", to);

            bestSellerInRange(invoices, from, to);
            top5InRange(invoices, from, to);
        }
        else if (choice == 5) {
            addInvoiceManual(&invoices);
        }
        else if (choice == 0) {
            printf("Izlaz...\n");
            break;
        }
        else {
            printf("Nepoznata opcija. Pritisnite 0 za izlaz.\n");
        }

    } while (1);

    freeAllInvoices(invoices);
    return 0;
}
