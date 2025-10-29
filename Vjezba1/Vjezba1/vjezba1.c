#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_POINTS 50

typedef struct {
    char firstName[50];
    char lastName[50];
    int score;
} Stud;


int countStudents(FILE * f1);
Stud* loadStudents(FILE * f1, int studentNumber);
float calculatePercentage(int points);
int printStudents(Stud * studentList, int studentNumber);
FILE* openFile(const char* filename);

int main() {

    FILE* file = openFile("students.txt");
    if (file == NULL) {
        printf("Doslo je do greske pri otvaranju datoteke\n");
        return 1;
    }

    int numOfStudents = countStudents(file);
    if (numOfStudents == 0) {
        printf("Datoteka je prazna.\n");
        fclose(file);
        return 0;
    }

    rewind(file);

    Stud* student = loadStudents(file, numOfStudents);
    fclose(file);

    if (student == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return 1;
    }

    printStudents(student, numOfStudents);

    free(student);
    return 0;
}

// Otvaranje datoteke
FILE* openFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    return fp;
}

// Brojanje redaka
int countStudents(FILE * f1) {
    int count = 0;
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), f1)) {
        count++;
    }

    return count;
}

// Učitavanje studenata u dinamički niz
Stud* loadStudents(FILE * f1, int studentNumber) {
    Stud* list = (Stud*)malloc(studentNumber * sizeof(Stud));
    if (list == NULL) {
        return NULL;
    }

    for (int i = 0; i < studentNumber; i++) {
        fscanf(f1, "%s %s %d",
            list[i].firstName,
            list[i].lastName,
            &list[i].score);
    }

    return list;
}

// Računanje postotka
float calculatePercentage(int points) {
    return (float)points / MAX_POINTS * 100.0f;
}

// Ispisivanje studenata i broja redaka
int printStudents(Stud * studentList, int studentNumber) {
    printf("NAME\t\tSURNAME\t\tABS POINTS\tRELATIVE (%%)\n");

    for (int i = 0; i < studentNumber; i++) {
        printf("%s\t\t%s\t\t%d\t\t%.2f\n",
            studentList[i].firstName,
            studentList[i].lastName,
            studentList[i].score,
            calculatePercentage(studentList[i].score));
    }

    return studentNumber;
}
