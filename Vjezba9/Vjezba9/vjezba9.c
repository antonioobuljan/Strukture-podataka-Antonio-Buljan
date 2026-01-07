#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Prototipi funkcija

struct TreeNode;

struct TreeNode* insert(struct TreeNode* root, int value);     // Funkcija za dodavanje elementa u stablo
struct TreeNode* createNode(int value);                        // Funkcija za kreiranje novog ?vora
int replace(struct TreeNode* node);                            // Funkcija za zamjenu vrijednosti sumom potomaka
int inorderWrite(struct TreeNode* node, FILE* file);           // Funkcija za inorder ispis u datoteku
int freeTree(struct TreeNode* node);                            // Funkcija za osloba?anje memorije
int generateRandomNumber(void);                                 // Funkcija za generiranje slu?ajnog broja

//Struktura binarnog stabla

struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
};

//Red za level-order

struct TreeNode* queue[100];
int front = 0;
int rear = 0;

 // Funkcija za kreiranje novog ?vora

struct TreeNode* createNode(int value) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (node == NULL)
        exit(EXIT_FAILURE);

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Funkcija za dodavanje elementa u stablo

struct TreeNode* insert(struct TreeNode* root, int value) {
    struct TreeNode* newNode = createNode(value);

    if (root == NULL) {
        queue[rear++] = newNode;
        return newNode;
    }

    struct TreeNode* current = queue[front];

    if (current->left == NULL) {
        current->left = newNode;
        queue[rear++] = newNode;
    }
    else if (current->right == NULL) {
        current->right = newNode;
        queue[rear++] = newNode;
        front++;
    }

    return root;
}

// Funkcija za zamjenu vrijednosti sumom potomaka

int replace(struct TreeNode* node) {
    if (node == NULL)
        return 0;

    int leftSum = replace(node->left);
    int rightSum = replace(node->right);

    int oldValue = node->value;
    node->value = leftSum + rightSum;

    return oldValue + node->value;
}

// Funkcija za inorder ispis u datoteku

int inorderWrite(struct TreeNode* node, FILE* file) {
    if (node == NULL)
        return 0;

    inorderWrite(node->left, file);
    fprintf(file, "%d ", node->value);
    inorderWrite(node->right, file);

    return 0;
}

// Funkcija za osloba?anje memorije

int freeTree(struct TreeNode* node) {
    if (node == NULL)
        return 0;

    freeTree(node->left);
    freeTree(node->right);
    free(node);

    return 0;
}

// Funkcija za generiranje slu?ajnog broja u rasponu <10, 90>
int generateRandomNumber(void) {
    return rand() % 81 + 10;  
}

// Main program

int main(void) {
    struct TreeNode* root = NULL;
    FILE* file = fopen("inorder.txt", "w");
    if (file == NULL)
        exit(EXIT_FAILURE);

    //a) Predefinirani niz
    int numbers[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int n = sizeof(numbers) / sizeof(int);

    for (int i = 0; i < n; i++)
        root = insert(root, numbers[i]);

    fprintf(file, "Inorder prije zamjene (predefinirani niz):\n");
    inorderWrite(root, file);

    //b) Zamjena vrijednosti
    replace(root);

    fprintf(file, "\nInorder nakon zamjene:\n");
    inorderWrite(root, file);

    //c) Random brojevi
    struct TreeNode* randomRoot = NULL;
    front = rear = 0;

    srand((unsigned)time(NULL));

    fprintf(file, "\n\nRandom brojevi:\n");
    for (int i = 0; i < 10; i++) {
        int r = generateRandomNumber();
        fprintf(file, "%d ", r);
        randomRoot = insert(randomRoot, r);
    }

    fprintf(file, "\nInorder prije zamjene (random):\n");
    inorderWrite(randomRoot, file);

    replace(randomRoot);

    fprintf(file, "\nInorder nakon zamjene (random):\n");
    inorderWrite(randomRoot, file);

    fclose(file);

    freeTree(root);
    freeTree(randomRoot);

    printf("Program je uspjesno zavrsen.\n");

    return 0;
}
