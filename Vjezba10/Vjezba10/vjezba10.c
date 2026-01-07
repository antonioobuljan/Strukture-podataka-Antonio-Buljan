#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototipi funkcija */

struct Country;
struct City;

int readCountries(struct Country** listHead, struct Country** treeRoot); // Funkcija za ?itanje država
int insertCountryList(struct Country** head, char* name);                // Funkcija za dodavanje države u listu
int insertCountryTree(struct Country** root, char* name);                // Funkcija za dodavanje države u stablo
int readCities(struct City** root, struct City** list, char* filename);  // Funkcija za ?itanje gradova
int insertCityTree(struct City** root, char* name, int population);      // Funkcija za dodavanje grada u stablo
int insertCityList(struct City** head, char* name, int population);      // Funkcija za dodavanje grada u listu
int searchCities(struct Country* root);                                   // Funkcija za pretragu gradova

/* Strukture */

struct City {
    char name[64];
    int population;
    struct City* left;
    struct City* right;
    struct City* next;
};

struct Country {
    char name[64];
    struct City* cityTree;
    struct City* cityList;
    struct Country* left;
    struct Country* right;
    struct Country* next;
};

/* Implementacija funkcija */

int insertCountryList(struct Country** head, char* name) {
    struct Country* newCountry = (struct Country*)malloc(sizeof(struct Country));
    if (newCountry == NULL)
        exit(EXIT_FAILURE);

    strcpy(newCountry->name, name);
    newCountry->cityTree = NULL;
    newCountry->cityList = NULL;
    newCountry->next = NULL;

    if (*head == NULL || strcmp(name, (*head)->name) < 0) {
        newCountry->next = *head;
        *head = newCountry;
        return 0;
    }

    struct Country* current = *head;
    while (current->next != NULL && strcmp(current->next->name, name) < 0)
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;

    return 0;
}

int insertCountryTree(struct Country** root, char* name) {
    if (*root == NULL) {
        *root = (struct Country*)malloc(sizeof(struct Country));
        if (*root == NULL)
            exit(EXIT_FAILURE);

        strcpy((*root)->name, name);
        (*root)->cityTree = NULL;
        (*root)->cityList = NULL;
        (*root)->left = (*root)->right = NULL;
        return 0;
    }

    if (strcmp(name, (*root)->name) < 0)
        insertCountryTree(&(*root)->left, name);
    else
        insertCountryTree(&(*root)->right, name);

    return 0;
}

int insertCityTree(struct City** root, char* name, int population) {
    if (*root == NULL) {
        *root = (struct City*)malloc(sizeof(struct City));
        if (*root == NULL)
            exit(EXIT_FAILURE);

        strcpy((*root)->name, name);
        (*root)->population = population;
        (*root)->left = (*root)->right = NULL;
        return 0;
    }

    if (population < (*root)->population ||
        (population == (*root)->population && strcmp(name, (*root)->name) < 0))
        insertCityTree(&(*root)->left, name, population);
    else
        insertCityTree(&(*root)->right, name, population);

    return 0;
}

int insertCityList(struct City** head, char* name, int population) {
    struct City* newCity = (struct City*)malloc(sizeof(struct City));
    if (newCity == NULL)
        exit(EXIT_FAILURE);

    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->next = NULL;

    if (*head == NULL ||
        population < (*head)->population ||
        (population == (*head)->population && strcmp(name, (*head)->name) < 0)) {
        newCity->next = *head;
        *head = newCity;
        return 0;
    }

    struct City* current = *head;
    while (current->next != NULL &&
        (current->next->population < population ||
            (current->next->population == population && strcmp(current->next->name, name) < 0)))
        current = current->next;

    newCity->next = current->next;
    current->next = newCity;

    return 0;
}

int readCities(struct City** tree, struct City** list, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        exit(EXIT_FAILURE);

    char cityName[64];
    int population;

    while (fscanf(file, " %[^,],%d", cityName, &population) == 2) {
        insertCityTree(tree, cityName, population);
        insertCityList(list, cityName, population);
    }

    fclose(file);
    return 0;
}

int readCountries(struct Country** listHead, struct Country** treeRoot) {
    FILE* file = fopen("drzave.txt", "r");
    if (file == NULL)
        exit(EXIT_FAILURE);

    char countryName[64];
    char filename[64];

    while (fscanf(file, "%s %s", countryName, filename) == 2) {
        insertCountryList(listHead, countryName);
        insertCountryTree(treeRoot, countryName);

        struct Country* listCountry = *listHead;
        while (listCountry != NULL && strcmp(listCountry->name, countryName) != 0)
            listCountry = listCountry->next;

        struct Country* treeCountry = *treeRoot;
        while (treeCountry != NULL && strcmp(treeCountry->name, countryName) != 0) {
            if (strcmp(countryName, treeCountry->name) < 0)
                treeCountry = treeCountry->left;
            else
                treeCountry = treeCountry->right;
        }

        readCities(&listCountry->cityTree, &treeCountry->cityList, filename);
    }

    fclose(file);
    return 0;
}

int searchCities(struct Country* root) {
    char countryName[64];
    int minPopulation;

    printf("Unesite ime drzave: ");
    scanf("%s", countryName);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPopulation);

    while (root != NULL && strcmp(root->name, countryName) != 0) {
        if (strcmp(countryName, root->name) < 0)
            root = root->left;
        else
            root = root->right;
    }

    if (root == NULL) {
        printf("Drzava ne postoji.\n");
        return 0;
    }

    struct City* current = root->cityList;
    printf("Gradovi s vise od %d stanovnika:\n", minPopulation);

    while (current != NULL) {
        if (current->population > minPopulation)
            printf("%s (%d)\n", current->name, current->population);
        current = current->next;
    }

    return 0;
}

/* Main */

int main(void) {
    struct Country* countryList = NULL;
    struct Country* countryTree = NULL;

    readCountries(&countryList, &countryTree);
    searchCities(countryTree);

    return 0;
}
