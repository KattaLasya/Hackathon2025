#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROJECTS 100
#define MAX_LENGTH 100
#define FILE_NAME "projects.dat"

typedef struct {
    int id;
    char title[MAX_LENGTH];
    char researcher[MAX_LENGTH];
    char status[MAX_LENGTH];
} ResearchProject;

ResearchProject projects[MAX_PROJECTS];
int projectCount = 0;

void saveProjects() {
    FILE *file = fopen(FILE_NAME, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(&projectCount, sizeof(int), 1, file);
    fwrite(projects, sizeof(ResearchProject), projectCount, file);
    fclose(file);
}

void loadProjects() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }
    fread(&projectCount, sizeof(int), 1, file);
    fread(projects, sizeof(ResearchProject), projectCount, file);
    fclose(file);
}

void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("Project limit reached!\n");
        return;
    }

    ResearchProject p;
    p.id = projectCount + 1;

    printf("Enter project title: ");
    getchar(); 
    fgets(p.title, MAX_LENGTH, stdin);
    p.title[strcspn(p.title, "\n")] = '\0';

    printf("Enter researcher name: ");
    fgets(p.researcher, MAX_LENGTH, stdin);
    p.researcher[strcspn(p.researcher, "\n")] = '\0';

    printf("Enter status (Ongoing/Completed): ");
    fgets(p.status, MAX_LENGTH, stdin);
    p.status[strcspn(p.status, "\n")] = '\0';

    projects[projectCount++] = p;
    saveProjects(); 
    printf("Project added successfully!\n");
}

void listProjects() {
    printf("\n--- Research Projects ---\n");
    if (projectCount == 0) {
        printf("No projects available.\n");
        return;
    }
    int i;
    for (i = 0; i < projectCount; i++) {
        printf("ID: %d\nTitle: %s\nResearcher: %s\nStatus: %s\n\n",
               projects[i].id, projects[i].title, projects[i].researcher, projects[i].status);
    }
}

void searchProject() {
    int searchId;
    printf("Enter project ID to search: ");
    scanf("%d", &searchId);
    int i;
    for (i = 0; i < projectCount; i++) {
        if (projects[i].id == searchId) {
            printf("\n--- Project Found ---\n");
            printf("ID: %d\nTitle: %s\nResearcher: %s\nStatus: %s\n",
                   projects[i].id, projects[i].title, projects[i].researcher, projects[i].status);
            return;
        }
    }
    printf("Project with ID %d not found.\n", searchId);
}

void deleteProject() {
    int deleteId;
    printf("Enter project ID to delete: ");
    scanf("%d", &deleteId);

    int found = 0;
    int i,j;
    for (i = 0; i < projectCount; i++) {
        if (projects[i].id == deleteId) {
            found = 1;
            for (j = i; j < projectCount - 1; j++) {
                projects[j] = projects[j + 1];
            }
            projectCount--;
            saveProjects(); 
            printf("Project deleted successfully.\n");
            return;
        }
    }
    if (!found) {
        printf("Project with ID %d not found.\n", deleteId);
    }
}

int main() {
    loadProjects(); 

    int choice;
    while (1) {
        printf("\n1. Add Project\n2. List Projects\n3. Search Project\n4. Delete Project\n0. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProject();
                break;
            case 2:
                listProjects();
                break;
            case 3:
                searchProject();
                break;
            case 4:
                deleteProject();
                break;
            case 0:
                printf("Exiting...\n");
                saveProjects(); 
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

