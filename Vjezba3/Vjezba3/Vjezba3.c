#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktura za jednu osobu
typedef struct Person {
    char firstName[20];
    char lastName[20];
    int day, month, year;
    struct Person* next;
} Person;

// prototipi funkcija
Person* insertAtStart(Person*);
Person* insertAtEnd(Person*);
Person* insertAfter(Person*, char*);
Person* insertBefore(Person*, char*);
Person* removeBySurname(Person*, char*);
int searchBySurname(Person*, char*);
int printList(Person*);
Person* sortList(Person*);
void writeToFile(Person*, const char*);
Person* readFromFile(const char*);

// glavni dio programa
int main() {
    int option;
    int status = 0;
    char surname[20];
    Person* head = NULL;

    do {
        printf("\nOdaberi opciju (9 za izlaz):\n");
        printf("1 - Dodaj na pocetak\n");
        printf("2 - Dodaj na kraj\n");
        printf("3 - Izbrisi osobu po prezimenu\n");
        printf("4 - Pronadi osobu po prezimenu\n");
        printf("5 - Ispisi listu\n");
        printf("6 - Dodaj iza odredene osobe\n");
        printf("7 - Dodaj ispred odredene osobe\n");
        printf("8 - Sortiraj listu po prezimenima\n");
        printf("9 - Spremi listu u datoteku\n");
        printf("10 - Ucitaj listu iz datoteke\n");
        printf("11 - Izlaz\n");
        scanf("%d", &option);

        switch (option) {
        case 1:
            head = insertAtStart(head);
            break;
        case 2:
            head = insertAtEnd(head);
            break;
        case 3:
            printf("Unesi prezime za brisanje: ");
            scanf("%19s", surname);
            head = removeBySurname(head, surname);
            break;
        case 4:
            printf("Unesi prezime za pretrazivanje: ");
            scanf("%19s", surname);
            if (searchBySurname(head, surname) == 1)
                printf("Osoba sa tim prezimenom ne postoji!\n");
            break;
        case 5:
            status = printList(head);
            if (status == 1)
                printf("Lista je prazna!\n");
            break;
        case 6:
            printf("Unesi prezime osobe iza koje zelis dodati novu: ");
            scanf("%19s", surname);
            head = insertAfter(head, surname);
            break;
        case 7:
            printf("Unesi prezime osobe ispred koje zelis dodati novu: ");
            scanf("%19s", surname);
            head = insertBefore(head, surname);
            break;
        case 8:
            head = sortList(head);
            printf("Lista je sortirana po prezimenima!\n");
            break;
        case 9:
            writeToFile(head, "osobe.txt");
            printf("Lista je spremljena u datoteku 'osobe.txt'\n");
            break;
        case 10:
            head = readFromFile("osobe.txt");
            printf("Lista je ucitana iz datoteke 'osobe.txt'\n");
            break;
        case 11:
            break;
        default:
            printf("Unijeli ste krivi odabir!\n");
            break;
        }
    } while (option != 11);

    // oslobadjanje memorije
    Person* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

// dodavanje na pocetak liste
Person* insertAtStart(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return head;
    }

    printf("Unesi ime: ");
    scanf("%19s", newPerson->firstName);
    printf("Unesi prezime: ");
    scanf("%19s", newPerson->lastName);
    printf("Unesi datum rodenja (dd mm yyyy): ");
    scanf("%d %d %d", &newPerson->day, &newPerson->month, &newPerson->year);

    newPerson->next = head;
    return newPerson;
}

// dodavanje na kraj liste
Person* insertAtEnd(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return head;
    }

    printf("Unesi ime: ");
    scanf("%19s", newPerson->firstName);
    printf("Unesi prezime: ");
    scanf("%19s", newPerson->lastName);
    printf("Unesi datum rodenja (dd mm yyyy): ");
    scanf("%d %d %d", &newPerson->day, &newPerson->month, &newPerson->year);
    newPerson->next = NULL;

    if (head == NULL)
        return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson;
    return head;
}

// dodavanje iza odredene osobe
Person* insertAfter(Person* head, char* surname) {
    Person* current = head;
    while (current != NULL && strcmp(current->lastName, surname) != 0)
        current = current->next;

    if (current == NULL) {
        printf("Osoba s tim prezimenom nije pronadena!\n");
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return head;
    }

    printf("Unesi ime nove osobe: ");
    scanf("%19s", newPerson->firstName);
    printf("Unesi prezime nove osobe: ");
    scanf("%19s", newPerson->lastName);
    printf("Unesi datum rodenja (dd mm yyyy): ");
    scanf("%d %d %d", &newPerson->day, &newPerson->month, &newPerson->year);

    newPerson->next = current->next;
    current->next = newPerson;
    return head;
}

// dodavanje ispred odredene osobe
Person* insertBefore(Person* head, char* surname) {
    if (head == NULL) {
        printf("Lista je prazna!\n");
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alociranju memorije!\n");
        return head;
    }

    printf("Unesi ime nove osobe: ");
    scanf("%19s", newPerson->firstName);
    printf("Unesi prezime nove osobe: ");
    scanf("%19s", newPerson->lastName);
    printf("Unesi datum rodenja (dd mm yyyy): ");
    scanf("%d %d %d", &newPerson->day, &newPerson->month, &newPerson->year);

    // ako je trazena osoba prva u listi
    if (strcmp(head->lastName, surname) == 0) {
        newPerson->next = head;
        return newPerson;
    }

    Person* prev = NULL;
    Person* current = head;

    while (current != NULL && strcmp(current->lastName, surname) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Osoba s tim prezimenom nije pronadena!\n");
        free(newPerson);
        return head;
    }

    prev->next = newPerson;
    newPerson->next = current;
    return head;
}

// sortiranje po prezimenima (abecedno)
Person* sortList(Person* head) {
    if (head == NULL || head->next == NULL)
        return head;

    int swapped;
    Person* ptr1;
    Person* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                // zamjena podataka
                Person temp = *ptr1;
                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->day = ptr1->next->day;
                ptr1->month = ptr1->next->month;
                ptr1->year = ptr1->next->year;

                strcpy(ptr1->next->firstName, temp.firstName);
                strcpy(ptr1->next->lastName, temp.lastName);
                ptr1->next->day = temp.day;
                ptr1->next->month = temp.month;
                ptr1->next->year = temp.year;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}

// upis liste u datoteku
void writeToFile(Person* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    Person* current = head;
    while (current != NULL) {
        fprintf(fp, "%s %s %d %d %d\n",
            current->firstName, current->lastName,
            current->day, current->month, current->year);
        current = current->next;
    }

    fclose(fp);
}

// citanje liste iz datoteke
Person* readFromFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;
    Person temp;

    while (fscanf(fp, "%19s %19s %d %d %d",
        temp.firstName, temp.lastName,
        &temp.day, &temp.month, &temp.year) == 5)
    {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        if (newPerson == NULL) {
            printf("Greska pri alociranju memorije!\n");
            fclose(fp);
            return head;
        }

        *newPerson = temp;
        newPerson->next = NULL;

        if (head == NULL)
            head = newPerson;
        else
            tail->next = newPerson;

        tail = newPerson;
    }

    fclose(fp);
    return head;
}

// ispis liste
int printList(Person* head) {
    if (head == NULL)
        return 1;

    printf("\n%-20s %-20s %-20s\n", "IME", "PREZIME", "DATUM RODENJA");
    printf("--------------------------------------------------------------\n");

    Person* current = head;
    while (current != NULL) {
        printf("%-20s %-20s %02d.%02d.%04d\n",
            current->firstName, current->lastName,
            current->day, current->month, current->year);
        current = current->next;
    }
    return 0;
}

// trazenje osobe po prezimenu
int searchBySurname(Person* head, char* surname) {
    Person* ptr = head;
    while (ptr != NULL) {
        if (strcmp(ptr->lastName, surname) == 0) {
            printf("Pronadena osoba: %s %s (%02d.%02d.%04d)\n",
                ptr->firstName, ptr->lastName, ptr->day, ptr->month, ptr->year);
            return 0;
        }
        ptr = ptr->next;
    }
    return 1;
}

// brisanje osobe po prezimenu
Person* removeBySurname(Person* head, char* surname) {
    if (head == NULL) {
        printf("Lista je prazna!\n");
        return NULL;
    }

    Person* current = head;
    Person* previous = NULL;

    while (current != NULL && strcmp(current->lastName, surname) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Ne postoji osoba s tim prezimenom!\n");
        return head;
    }

    if (previous == NULL)
        head = current->next;
    else
        previous->next = current->next;

    free(current);
    printf("Uspijesno ste izbrisali osobu.\n");
    return head;
}
