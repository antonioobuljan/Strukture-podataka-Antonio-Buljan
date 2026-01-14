#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

struct Country;
struct City;

int createHashTable(struct Country* table[]);
int hashFunction(char* countryName);                                   //Funkcija za racunanje hash kljuca
int readCountries(struct Country* table[]);                             //Funkciaj za citanje drzava iz datoteke
int insertCountry(struct Country** head, char* name, char* filename);  //Funkcija za umetanje drzave u vezanu listu
int readCities(struct City** root, char* filename);                     //Funkcija za citanje gradova
int insertCityTree(struct City** root, char* name, int population);     //Funkcija za umetanje grada u stablo
int searchCities(struct Country* table[]);                              //Funkcija za pretrazivanje gradova
int printCityTree(struct City* root, int minPopulation);                //Funkcija za ispis stabla gradova
int freeCityTree(struct City* root);                                    //Funkcija za oslobadanje stabla gradova
int freeCountryTable(struct Country* table[]);                          //Funkcjia za oslobadanje hash tablice

struct City {
    char name[64];
    int population;
    struct City* left;
    struct City* right;
};

struct Country {
    char name[64];
    struct City* cityTree;
    struct Country* next;
};

//Funkcije

int createHashTable(struct Country* table[]) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;
    return 0;
}

int hashFunction(char* countryName) {
    int sum = 0;
    int i;

    for (i = 0; i < 5 && countryName[i] != '\0'; i++)
        sum += (int)countryName[i];

    return sum % TABLE_SIZE;
}

int insertCountry(struct Country** head, char* name, char* filename) {
    struct Country* newCountry = (struct Country*)malloc(sizeof(struct Country));
    if (newCountry == NULL)
        exit(EXIT_FAILURE);

    strcpy(newCountry->name, name);
    newCountry->cityTree = NULL;
    newCountry->next = NULL;

    readCities(&newCountry->cityTree, filename);

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

int readCities(struct City** root, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        exit(EXIT_FAILURE);

    char cityName[64];
    int population;

    while (fscanf(file, " %[^,],%d", cityName, &population) == 2)
        insertCityTree(root, cityName, population);

    fclose(file);
    return 0;
}

int readCountries(struct Country* table[]) {
    FILE* file = fopen("drzave.txt", "r");
    if (file == NULL)
        exit(EXIT_FAILURE);

    char countryName[64];
    char filename[64];

    while (fscanf(file, "%s %s", countryName, filename) == 2) {
        int index = hashFunction(countryName);
        //Provjera radi li hash funkcija
        printf("Drzava %s -> hash %d\n", countryName, index);
        insertCountry(&table[index], countryName, filename);
    }

    fclose(file);
    return 0;
}

int printCityTree(struct City* root, int minPopulation) {
    if (root == NULL)
        return 0;

    printCityTree(root->left, minPopulation);

    if (root->population > minPopulation)
        printf("%s (%d)\n", root->name, root->population);

    printCityTree(root->right, minPopulation);

    return 0;
}

int searchCities(struct Country* table[]) {
    char countryName[64];
    int minPopulation;
    int index;

    printf("Unesite ime drzave: ");
    scanf("%s", countryName);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPopulation);

    index = hashFunction(countryName);

    struct Country* current = table[index];
    while (current != NULL && strcmp(current->name, countryName) != 0)
        current = current->next;

    if (current == NULL) {
        printf("Drzava ne postoji.\n");
        return 0;
    }

    printf("Gradovi s vise od %d stanovnika:\n", minPopulation);
    printCityTree(current->cityTree, minPopulation);

    return 0;
}

int freeCityTree(struct City* root) {
    if (root == NULL)
        return 0;

    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);

    return 0;
}

int freeCountryTable(struct Country* table[]) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct Country* current = table[i];
        while (current != NULL) {
            struct Country* temp = current;
            freeCityTree(current->cityTree);
            current = current->next;
            free(temp);
        }
    }
    return 0;
}

//Main

int main() {
    struct Country* hashTable[TABLE_SIZE];

    createHashTable(hashTable);
    readCountries(hashTable);
    searchCities(hashTable);
    freeCountryTable(hashTable);

    return 0;
}
