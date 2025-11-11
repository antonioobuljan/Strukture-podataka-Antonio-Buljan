#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Lista za polinom

typedef struct Node {
    int coefficient;
    int exponent;
    struct Node* next;
} Node;

//Prototipi funkcija

int insertSorted(Node** head, int coefficient, int exponent);
int addPolynomials(Node* poly1, Node* poly2, Node** result);
int multiplyPolynomials(Node* poly1, Node* poly2, Node** result);
int printPolynomial(Node* head);
int freeList(Node* head);
int readFromFile(const char* filename, Node** head);

int main() {
    int choice;
    Node* poly1 = NULL;
    Node* poly2 = NULL;
    Node* result = NULL;
    char file1[50], file2[50];

    printf("     Ucitavanje polinoma iz datoteka    \n");
    printf("Unesite ime datoteke za prvi polinom: ");
    fgets(file1, sizeof(file1), stdin);
    file1[strcspn(file1, "\n")] = '\0'; 

    printf("Unesite ime datoteke za drugi polinom: ");
    fgets(file2, sizeof(file2), stdin);
    file2[strcspn(file2, "\n")] = '\0';

    //?itanje polinoma iz datoteka

    readFromFile(file1, &poly1);
    readFromFile(file2, &poly2);

    printf("\nPrvi polinom: ");
    printPolynomial(poly1);
    printf("Drugi polinom: ");
    printPolynomial(poly2);

    while (1) {
        printf("\nOdaberite opciju:\n");
        printf("1 - Zbroji polinome\n");
        printf("2 - Pomnozi polinome\n");
        printf("3 - Izlaz iz programa\n");
        printf("Vas izbor: ");

        char line[50];
        if (!fgets(line, sizeof(line), stdin)) continue;
        if (sscanf(line, "%d", &choice) != 1) {
            printf("Neispravan unos!\n");
            continue;
        }

        switch (choice) {
        case 1:
            freeList(result);
            result = NULL;
            addPolynomials(poly1, poly2, &result);
            printf("\nZbroj polinoma: ");
            printPolynomial(result);
            break;

        case 2:
            freeList(result);
            result = NULL;
            multiplyPolynomials(poly1, poly2, &result);
            printf("\nUmnozak polinoma: ");
            printPolynomial(result);
            break;

        case 3:
            printf("\nIzlaz iz programa.\n");
            freeList(poly1);
            freeList(poly2);
            freeList(result);
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Nepoznata opcija, pokusajte ponovno.\n");
            break;
        }
    }
}

//Funkcija za ?itanje polinoma iz datoteke

int readFromFile(const char* filename, Node** head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greska: ne mogu otvoriti datoteku '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    int coefficient, exponent;
    while (fscanf(file, "%d %d", &coefficient, &exponent) == 2) {
        insertSorted(head, coefficient, exponent);
    }

    fclose(file);
    return 0;
}

//Funkcija za umetanje u listu

int insertSorted(Node** head, int coefficient, int exponent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Greska pri alokaciji memorije!\n");
        exit(EXIT_FAILURE);
    }
    newNode->coefficient = coefficient;
    newNode->exponent = exponent;
    newNode->next = NULL;

    if (*head == NULL || (*head)->exponent < exponent) {
        newNode->next = *head;
        *head = newNode;
        return 0;
    }

    Node* temp = *head;
    while (temp->next != NULL && temp->next->exponent > exponent)
        temp = temp->next;

    if (temp->next != NULL && temp->next->exponent == exponent) {
        temp->next->coefficient += coefficient;
        free(newNode);
    }
    else {
        newNode->next = temp->next;
        temp->next = newNode;
    }

    return 0;
}

//Funkcija za zbroj polinoma

int addPolynomials(Node* poly1, Node* poly2, Node** result) {
    for (Node* p1 = poly1; p1 != NULL; p1 = p1->next)
        insertSorted(result, p1->coefficient, p1->exponent);
    for (Node* p2 = poly2; p2 != NULL; p2 = p2->next)
        insertSorted(result, p2->coefficient, p2->exponent);
    return 0;
}

//Funkcija za množenje polinoma

int multiplyPolynomials(Node* poly1, Node* poly2, Node** result) {
    for (Node* p1 = poly1; p1 != NULL; p1 = p1->next) {
        for (Node* p2 = poly2; p2 != NULL; p2 = p2->next) {
            insertSorted(result, p1->coefficient * p2->coefficient, p1->exponent + p2->exponent);
        }
    }
    return 0;
}

//Funkcija za ispis polinoma

int printPolynomial(Node* head) {
    if (!head) {
        printf("Polinom je prazan.\n");
        return 0;
    }
    Node* temp = head;
    while (temp) {
        printf("%dx^%d", temp->coefficient, temp->exponent);
        temp = temp->next;
        if (temp && temp->coefficient >= 0) printf(" + ");
        else if (temp) printf(" ");
    }
    printf("\n");
    return 0;
}

//Funkcija za osloba?anje memorije

int freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
