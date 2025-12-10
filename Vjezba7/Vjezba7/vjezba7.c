#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prototipi funkcija

struct Dir;
typedef struct Dir Dir;
struct StackNode;
typedef struct StackNode StackNode;
int createRoot(Dir** root, const char* name);
int md(Dir* parent, const char* name);
int findChild(Dir* parent, const char* name, Dir** result);
int cdDir(Dir** current, StackNode** stack, const char* name);
int cdUp(Dir** current, StackNode** stack);
int dir(Dir* current);
int freeTree(Dir* node);
int readInput(char* buf, size_t size);
int push(StackNode** top, Dir* d);
int freeStack(StackNode** top);
int printPath(Dir* current);

//Struktura direktorija

struct Dir {
    char* name;
    Dir* parent;
    Dir* firstChild;
    Dir* nextSibling;
};

//Struktura stoga 

struct StackNode {
    Dir* dir;
    StackNode* next;
};

//Funkcija za stvaranje root direktorija 

int createRoot(Dir** root, const char* name) {
    if (root == NULL) return -1;
    Dir* r = (Dir*)malloc(sizeof(Dir));
    if (r == NULL) exit(EXIT_FAILURE);
    r->name = (char*)malloc(strlen(name) + 1);
    if (r->name == NULL) exit(EXIT_FAILURE);
    strcpy(r->name, name);
    r->parent = NULL;
    r->firstChild = NULL;
    r->nextSibling = NULL;
    *root = r;
    return 0;
}

//Funkcija za stvaranje novog direktorija naredbom md

int md(Dir* parent, const char* name) {
    if (parent == NULL || name == NULL) return -1;

    Dir* tmp = parent->firstChild;
    while (tmp) {
        if (strcmp(tmp->name, name) == 0) {
            printf("Direktorij '%s' vec postoji.\n", name);
            return 0;
        }
        tmp = tmp->nextSibling;
    }

    Dir* node = (Dir*)malloc(sizeof(Dir));
    if (node == NULL) exit(EXIT_FAILURE);
    node->name = (char*)malloc(strlen(name) + 1);
    if (node->name == NULL) exit(EXIT_FAILURE);
    strcpy(node->name, name);
    node->parent = parent;
    node->firstChild = NULL;
    node->nextSibling = NULL;

    node->nextSibling = parent->firstChild;
    parent->firstChild = node;

    printf("Direktorij '%s' je napravljen.\n", name);
    return 0;
}

//Funkcija za pronalazenje poddirektorija po imenu

int findChild(Dir* parent, const char* name, Dir** result) {
    if (parent == NULL || name == NULL || result == NULL) return -1;
    Dir* tmp = parent->firstChild;
    while (tmp) {
        if (strcmp(tmp->name, name) == 0) {
            *result = tmp;
            return 0;
        }
        tmp = tmp->nextSibling;
    }
    *result = NULL;
    return 1;
}

//Funkcija za mijenjanje u direktorij naredbom cd dir

int cdDir(Dir** current, StackNode** stack, const char* name) {
    if (current == NULL || *current == NULL || name == NULL) return -1;
    Dir* found = NULL;
    int res = findChild(*current, name, &found);
    if (res == 0 && found != NULL) {
        if (push(stack, *current) != 0) return -1;
        *current = found;
        return 0;
    }
    else
        return 1;
    
}

//Funkcija za vracanje u prethodni direktorij naredbom cd..

int cdUp(Dir** current, StackNode** stack) {
    if (current == NULL || *current == NULL) return -1;
    if ((*current)->parent != NULL) {
        if (push(stack, *current) != 0) return -1;
        *current = (*current)->parent;
        return 0;
    }
    else {
        printf("Nalazite se u root direktoriju.\n");
        return 1;
    }
}

//Funkcija za ispisivanje direktorija naredbom dir

int dir(Dir* current) {
    if (current == NULL) return -1;
    Dir* tmp = current->firstChild;
    if (tmp == NULL) {
        printf("Direktorij je prazan.\n");
        return 0;
    }
    printf("Sadrzaj '%s':\n", current->name);
    while (tmp) {
        printf("  %s\n", tmp->name);
        tmp = tmp->nextSibling;
    }
    return 0;
}

//Funkcija za oslobadanje memorije stabla

int freeTree(Dir* node) {
    if (node == NULL) return 0;
    Dir* child = node->firstChild;
    while (child) {
        Dir* next = child->nextSibling;
        child->parent = NULL;
        child->nextSibling = NULL;
        freeTree(child);
        child = next;
    }
    if (node->name) {
        free(node->name);
        node->name = NULL;
    }
    node->firstChild = NULL;
    node->nextSibling = NULL;
    node->parent = NULL;
    free(node);
    return 0;
}

//Funkcija za citanje unosa korisnika

int readInput(char* buf, size_t size) {
    if (buf == NULL || size == 0) return -1;
    if (fgets(buf, (int)size, stdin) == NULL) return 1;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    return 0;
}


//Funkcija za push na stog

int push(StackNode** top, Dir* d) {
    if (top == NULL) return -1;
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (node == NULL) exit(EXIT_FAILURE);
    node->dir = d;
    node->next = *top;
    *top = node;
    return 0;
}

//Funkcija za oslobadanje memorije stoga

int freeStack(StackNode** top) {
    if (top == NULL) return -1;
    StackNode* cur = *top;
    while (cur) {
        StackNode* next = cur->next;
        free(cur);
        cur = next;
    }
    *top = NULL;
    return 0;
}

//Funkcija za ispis putanje

int printPath(Dir* current) {
    if (current == NULL) return -1;
    int capacity = 16;
    Dir** parts = (Dir**)malloc(sizeof(Dir*) * capacity);
    if (parts == NULL) exit(EXIT_FAILURE);
    int count = 0;
    Dir* it = current;
    while (it) {
        if (count >= capacity) {
            capacity *= 2;
            Dir** tmp = (Dir**)realloc(parts, sizeof(Dir*) * capacity);
            if (tmp == NULL) exit(EXIT_FAILURE);
            parts = tmp;
        }
        parts[count++] = it;
        it = it->parent;
    }
    for (int i = count - 1; i >= 0; --i) {
        printf("%s", parts[i]->name);
        if (i > 0) printf("\\");
    }
    free(parts);
    return 0;
}

int main() {
    Dir* root = NULL;
    if (createRoot(&root, "root") != 0) {
        fprintf(stderr, "Greska pri stvaranju direktorija.\n");
        return EXIT_FAILURE;
    }

    Dir* current = root;
    StackNode* history = NULL;
    char buf[256];

    while (1) {
        printf("\n[");
        printPath(current);
        printf("] >\n");
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("Izaberite sto zelite napraviti: ");
        if (readInput(buf, sizeof(buf)) != 0) continue;

        if (strcmp(buf, "1") == 0) {
            printf("Unesite ime za direktorij: ");
            if (readInput(buf, sizeof(buf)) != 0) continue;
            if (strlen(buf) == 0) {
                printf("Ime ne moze biti prazno.\n");
                continue;
            }
            md(current, buf);
        }
        else if (strcmp(buf, "2") == 0) {
            printf("Unesite ime direktorija u koji zelite prijeci: ");
            if (readInput(buf, sizeof(buf)) != 0) continue;
            if (strlen(buf) == 0) {
                printf("Ime ne moze biti prazno\n");
                continue;
            }
            cdDir(&current, &history, buf);
        }
        else if (strcmp(buf, "3") == 0) {
            cdUp(&current, &history);
        }
        else if (strcmp(buf, "4") == 0) {
            dir(current);
        }
        else if (strcmp(buf, "5") == 0) {
            printf("Izlaz.\n");
            freeTree(root);
            root = NULL;
            current = NULL;
            freeStack(&history);
            history = NULL;
            return EXIT_SUCCESS;
        }
        else {
            printf("Krivi unos, unesite 1-5.\n");
        }
    }
	return 0;
}
