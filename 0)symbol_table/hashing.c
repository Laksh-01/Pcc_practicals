#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10


// Created a structure to create a HashMap of max size as MAX
struct symb {
    int add;
    char label[10];
} sy[MAX];



// Displays the contents that are stored into the symbol 
void display(int a[]) {
    FILE *fp = fopen("symbol.txt", "w");
    if (!fp) {
        printf("Error opening file.\n");
        exit(1);
    }
    
    printf("\nSymbol Table:\nHash Index | Address | Label\n");
    for (int i = 0; i < MAX; i++) {
        if (a[i] != 0) {
            printf("%d          | %d      | %s\n", i, sy[i].add, sy[i].label);
            fprintf(fp, "%d %d %s\n", i, sy[i].add, sy[i].label);
        }
    }
    fclose(fp);
}




int create_key(int num) {
    return num % MAX;
}


void Insert_into_hashMap(int a[], int key, int num) {
    int i, flag = 0;
    if (a[key] == 0) {
        a[key] = num;
        sy[key].add = num;
    } else {
        // Linear probing to resolve collision
        for (i = key + 1; i < MAX && !flag; i++) {
            if (a[i] == 0) {
                a[i] = num;
                sy[i].add = num;
                flag = 1;
            }
        }

        // Wrap around if no slot found in the right half
        if (!flag) {
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


void search() {
    FILE *fp = fopen("symbol.txt", "r");
    if (!fp) {
        printf("Error opening file.\n");
        exit(1);
    }

    char la[10];
    printf("Enter the label: ");
    scanf("%s", la);

    int found = 0;
    while (fscanf(fp, "%d %d %s", &sy[0].add, &sy[0].add, sy[0].label) != EOF) {
        // The correct entry is checked by comparing the label
        if (strcmp(sy[0].label, la) == 0) {
            printf("\nThe label '%s' is present in the symbol table at address: %d\n", la, sy[0].add);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nThe label '%s' is not present in the symbol table\n", la);
    }
    fclose(fp);
}


int main() {
    int a[MAX] = {0}, num, key, ch;
    char ans;

    memset(sy, 0, sizeof(sy));

    do {
        printf("\nEnter your choice:\n1 - Create entry in the symbol table\n2 - Search in the symbol table\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                do {

                    // ask for an address from user
                    printf("\nEnter the address: ");
                    scanf("%d", &num);

                    //create a key for mapping
                    key = create_key(num);


                    printf("Enter the label: ");
                    scanf("%s", sy[key].label);

                    //Now insert the key value into the map
                    Insert_into_hashMap(a, key, num);


                    printf("\nContinue ? 0 for no, 1 for yes: ");
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







