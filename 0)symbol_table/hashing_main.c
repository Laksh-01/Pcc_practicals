#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

// Structure for storing symbol data
struct symb {
    int add;
    char label[10];
} sy[MAX];

// Function to display the symbol table
void display(int a[]) {
    printf("\nSymbol Table:\nHash Index | Address | Label\n");
    for (int i = 0; i < MAX; i++) {
        if (a[i] != 0) {
            printf("%d          | %d      | %s\n", i, sy[i].add, sy[i].label);
        }
    }
}

// Function to create a key for the hash map
int create_key(int num) {
    return num % MAX;
}

// Function to insert a value into the hash map with linear probing
// Function to insert a value into the hash map with linear probing
void Insert_into_hashMap(int a[], int key, int num, char* label) {
    int i, flag = 0;
    if (a[key] == 0) {
        a[key] = num;
        sy[key].add = num;
        strcpy(sy[key].label, label);
    } else {
        // If the key's hashed position is already occupied, handle the collision
        for (i = key + 1; i < MAX; i++) {
            if (a[i] == 0) {
                a[i] = num;
                sy[i].add = num;
                strcpy(sy[i].label, label);  // Store the label at the found empty slot
                flag = 1;
                break;
            }
        }

        //come back circularly and place where it is free
        if (!flag) {
            for (i = 0; i < key; i++) {
                if (a[i] == 0) {
                    a[i] = num;
                    sy[i].add = num;
                    strcpy(sy[i].label, label);  // Store the label in the wrapped-around slot
                    break;
                }
            }
        }
    }
}


// Function to search for a label in the symbol table
void search() {
    char la[10];
    int found = 0;
    printf("Enter the label: ");
    scanf("%s", la);

    for (int i = 0; i < MAX; i++) {
        if (sy[i].add != 0 && strcmp(sy[i].label, la) == 0) {
            printf("\nThe label '%s' is present in the symbol table at address: %d\n", la, sy[i].add);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nThe label '%s' is not present in the symbol table\n", la);
    }
}

int main() {
    int a[MAX] = {0}, num, key, ch;
    char ans;

    memset(sy, 0, sizeof(sy));

    do {
        printf("\nEnter your choice:\n1 - Create entry in the symbol table\n2 - Search in the symbol table\n");
        scanf("%d", &ch);

        switch (ch) {
           // In the main function, under case 1:
        case 1:
            do {
                // Ask for an address from the user
                printf("\nEnter the address: ");
                scanf("%d", &num);

                // Create a key for mapping
                key = create_key(num);

                // Correctly declare and take the label as input
                char lab[10];
                printf("Enter the label: ");
                scanf("%s", lab);

                // Now insert the key value and label into the map
                Insert_into_hashMap(a, key, num, lab);

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
