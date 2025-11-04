#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define _CRT_SECURE_NO_WARNINGS

//Struktura za stog
typedef struct list {
	int number;
	struct list* next;
} list;

//prototipi funkcija
int writeInFile(char* filename); //prototip funkcije za pisanje u datoteku
int push(list** head, int number); //prototip funkcije za dodavanje na stog
int pop(list** head); //prototip funkcije za uklanjanje sa stoga
int freeStack(list** head); //prototip funkcije za oslobadjanje stoga 
int calculatePostfix(char* filename); //prototip funkcije za racunanje postfix izraza

//glavni dio programa	
int main()
{
	char fileName[50];
	printf("Unesite ime datoteke: ");		//Unošenje imena datoteke

	// ispravka scanf_s sintakse
	if (scanf_s("%49s", fileName, (unsigned)_countof(fileName)) != 1) {
		printf("Greska pri unosu imena datoteke.\n");
		return 1;
	}

	// poziv funkcije za pisanje u datoteku
	if (writeInFile(fileName) != 0) {
		printf("Greska pri pisanju u datoteku.\n");
		return 1;
	}

	// poziv funkcije za racunanje postfix izraza
	int result = calculatePostfix(fileName);
	if (result == -1) {
		printf("Doslo je do greske pri racunanju.\n");
		return 1;
	}

	printf("Rezultat: %d\n", result);

	return 0;
}

//funkcija za pisanje u datoteku
int writeInFile(char* filename)
{
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		perror("Greska pri otvaranju datoteke");
		return -1;
	}

	printf("Unesite postfix izraz (npr. 23 34 + 45 *): ");
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF); // ciscenje buffer-a

	char expression[100];
	if (fgets(expression, sizeof(expression), stdin) == NULL) {
		fclose(file);
		return -1;
	}

	// uklanjanje \n sa kraja stringa
	size_t len = strlen(expression);
	if (len > 0 && expression[len - 1] == '\n') {
		expression[len - 1] = '\0';
	}

	fputs(expression, file);
	fclose(file);
	return 0;
}

//funkcija za dodavanje na stog
int push(list** head, int number)
{
	list* newElement = (list*)malloc(sizeof(list));
	if (newElement == NULL)
		return -1;
	newElement->number = number;
	newElement->next = *head;
	*head = newElement;
	return 0;
}

//funkcija za uklanjanje sa stoga
int pop(list** head)
{
	if (*head == NULL) {
		printf("Stog je prazan!\n");
		return -1;
	}
	list* temp = *head;
	int number = temp->number;
	*head = (*head)->next;
	free(temp);
	return number;
}

//funkcija za oslobadjanje stoga
int freeStack(list** head)
{
	list* current = *head;
	list* next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
	return 0;
}

//funkcija za racunanje postfix izraza
int calculatePostfix(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Greska pri otvaranju datoteke.\n");
		return -1;
	}
	list* stack = NULL;
	int c; // mora biti int da primi EOF
	while ((c = fgetc(file)) != EOF) {
		char operator = (char)c;

		if (operator == ' ' || operator == '\n') continue;

		if (operator >= '0' && operator <= '9') {
			push(&stack, operator - '0'); // samo jednobrojni
		}
		else {
			int b = pop(&stack);
			int a = pop(&stack);
			int result;
			switch (operator) {
			case '+': result = a + b; break;
			case '-': result = a - b; break;
			case '*': result = a * b; break;
			case '/':
				if (b == 0) {
					printf("Greska: dijeljenje s nulom.\n");
					fclose(file);
					freeStack(&stack);
					return -1;
				}
				result = a / b; break;
			default:
				printf("Nepoznat operator: %c\n", operator);
				fclose(file);
				freeStack(&stack);
				return -1;
			}
			push(&stack, result);
		}
	}
	fclose(file);

	int finalResult = pop(&stack);
	if (stack != NULL) {
		printf("Greska: izraz nije ispravan.\n");
		freeStack(&stack);
		return -1;
	}
	return finalResult;
}
