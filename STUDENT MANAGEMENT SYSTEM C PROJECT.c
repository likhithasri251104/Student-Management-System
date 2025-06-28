#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

typedef struct {
    char id[20];
    char name[50];
    int age;
    float marks;
} Student;

// Predefined student list
Student students[] = {
    {"AP22110010208", "Pavan", 20, 88.5},
    {"AP22110010447", "Likitha", 21, 91.0},
    {"AP22110010240", "Pranay", 19, 85.75},
    {"AP22110010232", "Niteesh", 22, 87.5},
    {"AP22110010252", "Chetana", 20, 93.25},
    {"AP22110010227", "Charitha", 21, 89.0},
    {"AP22110010310", "Harsha", 20, 86.0},
    {"AP22110010199", "Tejaswi", 22, 90.0},
    {"AP22110010488", "Nikhil", 21, 84.0},
    {"AP22110010511", "Shravani", 20, 95.0}
};

int predefined_count = sizeof(students) / sizeof(students[0]);

void addStudentsFromList();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    do {
        printf("\n--- Student Management System ---\n");
        printf("1. Add All Students from Predefined List\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudentsFromList(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while(choice != 6);

    return 0;
}

void addStudentsFromList() {
    FILE *fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < predefined_count; i++) {
        fwrite(&students[i], sizeof(Student), 1, fp);
    }
    fclose(fp);
    printf("%d students added from predefined list.\n", predefined_count);
}

void displayStudents() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }
    Student s;
    printf("\nRoll No\t\tName\t\tAge\tMarks\n");
    printf("------------------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%-15s %-15s %d\t%.2f\n", s.id, s.name, s.age, s.marks);
    }
    fclose(fp);
}

void searchStudent() {
    char id[20];
    printf("Enter Roll Number to search: ");
    scanf("%s", id);
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }
    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0) {
            printf("Student found:\n");
            printf("Roll No: %s\nName: %s\nAge: %d\nMarks: %.2f\n", s.id, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) {
        printf("Student with Roll No %s not found.\n", id);
    }
}

void updateStudent() {
    char id[20];
    printf("Enter Roll Number to update: ");
    scanf("%s", id);

    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        printf("No records found.\n");
        return;
    }
    Student s;
    int found = 0;
    long pos;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(Student);

            printf("Enter new name: ");
            getchar();
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;
            printf("Enter new age: ");
            scanf("%d", &s.age);
            printf("Enter new marks: ");
            scanf("%f", &s.marks);

            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(Student), 1, fp);
            printf("Student record updated.\n");
            break;
        }
    }
    if (!found) {
        printf("Student with Roll No %s not found.\n", id);
    }
    fclose(fp);
}

void deleteStudent() {
    char id[20];
    printf("Enter Roll Number to delete: ");
    scanf("%s", id);

    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (fp == NULL || temp == NULL) {
        printf("Error accessing file.\n");
        return;
    }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0) {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILENAME);
        rename("temp.dat", FILENAME);
        printf("Student record deleted.\n");
    } else {
        remove("temp.dat");
        printf("Student with Roll No %s not found.\n", id);
    }
}
