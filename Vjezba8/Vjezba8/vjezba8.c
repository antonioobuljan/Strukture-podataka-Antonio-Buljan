#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct QueueNode {
    TreeNode* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

int createTreeNode(TreeNode** node, int value);
int insertElement(TreeNode** root, int value);
int findElement(TreeNode* root, int value);
int printInorder(TreeNode* root);
int printPreorder(TreeNode* root);
int printPostorder(TreeNode* root);
int initQueue(Queue* queue);
int enQueue(Queue* queue, TreeNode* node);
int deQueue(Queue* queue, TreeNode** node);
int isQueueEmpty(Queue* queue);
int printLevelOrder(TreeNode* root);
int deleteElement(TreeNode** root, int value);
int findMin(TreeNode* root, TreeNode** minNode);
int freeTree(TreeNode* root);

int main() {
    TreeNode* root = NULL;
    int choice = 0;
    int value = 0;

    do {
        printf("\n1 - Unos elementa");
        printf("\n2 - Pronadi element");
        printf("\n3 - Ispis inorder");
        printf("\n4 - Ispis preorder");
        printf("\n5 - Ispis postorder");
        printf("\n6 - Ispis level order");
        printf("\n7 - Brisanje elementa");
        printf("\n0 - Izlaz");
        printf("\nOdabir: ");

        scanf("%d", &choice);

        if (choice == 1) {
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            insertElement(&root, value);
        }
        else if (choice == 2) {
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            if (findElement(root, value))
                printf("Element postoji u stablu\n");
            else
                printf("Element ne postoji u stablu\n");
        }
        else if (choice == 3) {
            printf("Inorder ispis: ");
            printInorder(root);
            printf("\n");
        }
        else if (choice == 4) {
            printf("Preorder ispis: ");
            printPreorder(root);
            printf("\n");
        }
        else if (choice == 5) {
            printf("Postorder ispis: ");
            printPostorder(root);
            printf("\n");
        }
        else if (choice == 6) {
            printf("Level order ispis: ");
            printLevelOrder(root);
            printf("\n");
        }
        else if (choice == 7) {
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            deleteElement(&root, value);
        }

    } while (choice != 0);

    freeTree(root);
    return 0;
}

// Funkcija za stvaranje novog ?vora stabla
int createTreeNode(TreeNode** node, int value) {
    *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (*node == NULL)
        exit(EXIT_FAILURE);

    (*node)->value = value;
    (*node)->left = NULL;
    (*node)->right = NULL;
    return 0;
}

// Funkcija za unos novog elementa u stablo
int insertElement(TreeNode** root, int value) {
    if (*root == NULL) {
        createTreeNode(root, value);
        return 0;
    }

    if (value < (*root)->value)
        insertElement(&(*root)->left, value);
    else if (value > (*root)->value)
        insertElement(&(*root)->right, value);

    return 0;
}

// Funkcija za pronalazak elementa u stablu
int findElement(TreeNode* root, int value) {
    if (root == NULL)
        return 0;

    if (value == root->value)
        return 1;

    if (value < root->value)
        return findElement(root->left, value);

    return findElement(root->right, value);
}

// Funkcija za inorder ispis stabla
int printInorder(TreeNode* root) {
    if (root == NULL)
        return 0;

    printInorder(root->left);
    printf("%d ", root->value);
    printInorder(root->right);
    return 0;
}

// Funkcija za preorder ispis stabla
int printPreorder(TreeNode* root) {
    if (root == NULL)
        return 0;

    printf("%d ", root->value);
    printPreorder(root->left);
    printPreorder(root->right);
    return 0;
}

// Funkcija za postorder ispis stabla
int printPostorder(TreeNode* root) {
    if (root == NULL)
        return 0;

    printPostorder(root->left);
    printPostorder(root->right);
    printf("%d ", root->value);
    return 0;
}

// Funkcija za inicijalizaciju reda
int initQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    return 0;
}

// Funkcija za dodavanje elementa u red
int enQueue(Queue* queue, TreeNode* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL)
        exit(EXIT_FAILURE);

    newNode->treeNode = node;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    }
    else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    return 0;
}

// Funkcija za uklanjanje elementa iz reda
int deQueue(Queue* queue, TreeNode** node) {
    QueueNode* temp = NULL;

    if (queue->front == NULL)
        return 0;

    temp = queue->front;
    *node = temp->treeNode;
    queue->front = temp->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);
    return 0;
}

// Funkcija za provjeru je li red prazan
int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Funkcija za level order ispis stabla
int printLevelOrder(TreeNode* root) {
    Queue queue;
    TreeNode* current = NULL;

    if (root == NULL)
        return 0;

    initQueue(&queue);
    enQueue(&queue, root);

    while (!isQueueEmpty(&queue)) {
        deQueue(&queue, &current);
        printf("%d ", current->value);

        if (current->left != NULL)
            enQueue(&queue, current->left);
        if (current->right != NULL)
            enQueue(&queue, current->right);
    }
    return 0;
}

// Funkcija za pronalazak minimalnog elementa u stablu
int findMin(TreeNode* root, TreeNode** minNode) {
    while (root->left != NULL)
        root = root->left;

    *minNode = root;
    return 0;
}

// Funkcija za brisanje elementa iz stabla
int deleteElement(TreeNode** root, int value) {
    TreeNode* temp = NULL;

    if (*root == NULL)
        return 0;

    if (value < (*root)->value)
        deleteElement(&(*root)->left, value);
    else if (value > (*root)->value)
        deleteElement(&(*root)->right, value);
    else {
        if ((*root)->left == NULL) {
            temp = (*root)->right;
            free(*root);
            *root = temp;
        }
        else if ((*root)->right == NULL) {
            temp = (*root)->left;
            free(*root);
            *root = temp;
        }
        else {
            findMin((*root)->right, &temp);
            (*root)->value = temp->value;
            deleteElement(&(*root)->right, temp->value);
        }
    }
    return 0;
}

// Funkcija za osloba?anje stabla
int freeTree(TreeNode* root) {
    if (root == NULL)
        return 0;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return 0;
}
