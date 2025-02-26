#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

struct Student {
    int id;
    char name[50];
    int age;
    float marks;
};

// Function Prototypes
void addStudent();
void displayStudents();
void deleteStudent();
void updateStudent();
void clearInputBuffer();

int main() {
    int choice;
    do {
        printf("\n===== Student Record Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Delete Student\n");
        printf("4. Update Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer(); // Clears input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: deleteStudent(); break;
            case 4: updateStudent(); break;
            case 5: printf("Exiting... Goodbye!\n"); break;
            default: printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to add a new student
void addStudent() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    printf("Enter Student ID: ");
    scanf("%d", &s.id);
    clearInputBuffer(); // Clear buffer before taking string input

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // Remove newline character

    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, file);
    fclose(file);
    printf("Student record added successfully!\n");
}

// Function to display all students
void displayStudents() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    printf("\n===== Student Records =====\n");
    printf("%-10s %-25s %-5s %-5s\n", "ID", "Name", "Age", "Marks");
    printf("------------------------------------------------\n");

    while (fread(&s, sizeof(struct Student), 1, file)) {
        printf("%-10d %-25s %-5d %-5.2f\n", s.id, s.name, s.age, s.marks);
    }

    fclose(file);
}

// Function to delete a student by ID
void deleteStudent() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("No records to delete!\n");
        return;
    }

    int id, found = 0;
    struct Student s;
    FILE *tempFile = fopen("temp.dat", "wb");

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.id == id) {
            found = 1;
        } else {
            fwrite(&s, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("Student record deleted successfully!\n");
    } else {
        printf("Student ID not found!\n");
    }
}

// Function to update student details
void updateStudent() {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    int id, found = 0;
    struct Student s;

    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    clearInputBuffer(); // Clear buffer before taking string input

    while (fread(&s, sizeof(struct Student), 1, file)) {
        if (s.id == id) {
            found = 1;

            printf("Enter New Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter New Age: ");
            scanf("%d", &s.age);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            fseek(file, -sizeof(struct Student), SEEK_CUR);
            fwrite(&s, sizeof(struct Student), 1, file);
            printf("Student record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student ID not found!\n");
    }

    fclose(file);
}

// Function to clear input buffer (prevents issues with fgets() after scanf)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
