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
Person* removeBySurname(Person*, char*);
int searchBySurname(Person*, char*);
int printList(Person*);

//glavni dio koda

int main() {
    int option;
    int status = 0;
    char surname[20];
    Person* head = NULL;

    do {
        printf("\nOdaberi opciju (6 za izlaz):\n");
        printf("1 - Dodaj na pocetak\n");
        printf("2 - Dodaj na kraj\n");
        printf("3 - Izbrisi osobu po prezimenu\n");
        printf("4 - Pronadi osobu po prezimenu\n");
        printf("5 - Ispisi listu\n");
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
            scanf("%19s",surname);
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
            break;
        default:
            printf("Unijeli ste krivi odabir!\n");
            break;
        }
    } while (option != 6);

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
