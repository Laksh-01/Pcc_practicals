#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MOT 4

// Function to convert decimal to binary (8 bits)
char* convert(int decimal) {
    static char binary[9];  
    int i;
    for (i = 7; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0';
        decimal /= 2;
    }
    binary[8] = '\0';
    return binary;
}

// Mnemonics Table (MOT)
struct Mnemonics {
    char name[10];
    char binary[9];
    int size;
} mot[MAX_MOT];

int lc = 0; 

// Function to initialize Machine Opcode Table (MOT)
void init() {
    // Initialize Mnemonics Table (MOT)
    strcpy(mot[0].name, "ADD");
    strcpy(mot[0].binary, "00000001");
    mot[0].size = 1;

    strcpy(mot[1].name, "LOAD");
    strcpy(mot[1].binary, "00000111");
    mot[1].size = 5;

    strcpy(mot[2].name, "MOV");
    strcpy(mot[2].binary, "00001010");
    mot[2].size = 1;

    strcpy(mot[3].name, "STOP");
    strcpy(mot[3].binary, "00001111");
    mot[3].size = 1;
}

// Function to search for mnemonic in MOT
int search_mot(const char* opcode) {
    for (int i = 0; i < MAX_MOT; ++i) {
        if (strcmp(mot[i].name, opcode) == 0) {
            return i;
        }
    }
    return -1;
}

// Pass1 function to handle first pass (for our case, just reading input)
void pass1() {
    int num1, num2;

    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    printf("Binary representation of num1: %s\n", convert(num1));
    printf("Binary representation of num2: %s\n", convert(num2));

    lc += mot[1].size;  // Simulate a 'LOAD' operation (LOAD num1)
    lc += mot[0].size;  // Simulate an 'ADD' operation (ADD num2)
}

// Pass2 function to handle second pass (machine code generation)
void pass2() {
    // In a real pass2, we'd generate machine code
    printf("\nMachine Code for Addition Operation:\n");
    printf("%s %s\n", mot[1].binary, convert(5)); // Simulate LOAD operation with 5 (for example)
    printf("%s %s\n", mot[0].binary, convert(6)); // Simulate ADD operation with 6 (for example)
    printf("%s\n", mot[3].binary);  // Simulate STOP operation

    // Load r1,num1
    // ADD r1,num2
    // STOP
}

int main() {
    init();
    pass1();
    lc = 0;
    pass2();
    return 0;
}
