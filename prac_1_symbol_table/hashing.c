#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 11

struct symb {
    int add;         // Address
    char label[10];  // Label
} sy[MAX];


void search();
void Iprob(int a[], int key, int num);
int create(int num);
void display(int a[]);

int main() {
    int a[MAX], num, key, i, ch;
    char ans;
    
    // Initialize the symbol table
    for (i = 0; i < MAX; i++) {
        a[i] = 0;
        sy[i].add = 0;
        strcpy(sy[i].label, "");
    }


    do {
        printf("\nEnter your choice:");
        printf("\n1 - Create entry in the symbol table\n2 - Search in the symbol table\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                do {
                    printf("\nEnter the address: ");
                    scanf("%d", &num);
                    key = create(num);
                    printf("Enter the label: ");
                    scanf("%s", sy[key].label);
                    Iprob(a, key, num);
                    printf("\nContinue? 0 for no, 1 for yes: ");
                    scanf(" %c", &ans);
                } while (ans == '1');
                
                display(a);
                break;

            case 2:
                search();
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (ch <= 2);

    return 0;
}

int create(int num) {
    int key = num % MAX;
    return key;
}

void Iprob(int a[], int key, int num) {
    int i, flag = 0, count = 0;

    if (a[key] == 0) {
        a[key] = num;
        sy[key].add = num;
    } else {
        for (i = 0; i < MAX; i++) {
            if (a[i] != 0) count++;
        }

        if (count == MAX) {
            printf("\nHash table is full");
            display(a);
            exit(1);
        }

        for (i = key + 1; i < MAX; i++) {
            if (a[i] == 0) {
                a[i] = num;
                flag = 1;
                sy[i].add = num;
                break;
            }
        }

        if (flag == 0) {
            for (i = 0; i < key; i++) {
                if (a[i] == 0) {
                    a[i] = num;
                    sy[i].add = num;
                    break;
                }
            }
        }
    }
}

void display(int a[]) {
    FILE *fp;
    int i;

    fp = fopen("symbol.txt", "w");
    printf("\nSymbol Table:");
    printf("\nHash Index | Address | Label\n");
    
    for (i = 0; i < MAX; i++) {
        if (a[i] != 0) {
            printf("%d          | %d      | %s\n", i, sy[i].add, sy[i].label);
            fprintf(fp, "%d %d %s\n", i, sy[i].add, sy[i].label);
        }
    }
    fclose(fp);
}


void search() {
    FILE *fp;
    char la[10];
    int found = 0, i;

    printf("Enter the label: ");
    scanf("%s", la);

    fp = fopen("symbol.txt", "r");

    for (i = 0; i < MAX; i++) {
        fscanf(fp, "%d %d %s", &i, &sy[i].add, sy[i].label);
        if (sy[i].add != 0 && strcmp(sy[i].label, la) == 0) {
            printf("\nThe label '%s' is present in the symbol table at address: %d\n", la, sy[i].add);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nThe label '%s' is not present in the symbol table\n", la);
    }

    fclose(fp);
}
