#include <stdio.h>
#include <string.h>

// Label counter for TAC generation
int labelCounter = 0;
int tempCounter = 0;

// Function to generate new labels and temporary variables
char* newLabel() {
    static char label[10];
    sprintf(label, "L%d", labelCounter++);
    return label;
}

char* newTemp() {
    static char temp[10];
    sprintf(temp, "t%d", tempCounter++);
    return temp;
}

// Function prototypes
void generateAssignment(char* id, char* expr);
void generateIfElse(char* condition, char* trueBlock, char* falseBlock);
void generateWhileLoop(char* condition, char* body);

int main() {
    // Example usage
    generateAssignment("x", "a + b");
    generateIfElse("a < b", "x = a + b", "x = a - b");
    generateWhileLoop("a < b", "a = a + 1");

    return 0;
}

// Function to generate TAC for assignment statements
void generateAssignment(char* id, char* expr) {
    printf("%s = %s\n", id, expr);
}

// Function to generate TAC for if-then-else statements
void generateIfElse(char* condition, char* trueBlock, char* falseBlock) {
    char* L1 = newLabel();
    char* L2 = newLabel();
    char* L3 = newLabel();

    printf("if %s goto %s\n", condition, L1);
    printf("goto %s\n", L2);
    printf("%s: %s\n", L1, trueBlock);
    printf("goto %s\n", L3);
    printf("%s: %s\n", L2, falseBlock);
    printf("%s:\n", L3);
}

// Function to generate TAC for while loops
void generateWhileLoop(char* condition, char* body) {
    char* L1 = newLabel();
    char* L2 = newLabel();

    printf("%s: if not %s goto %s\n", L1, condition, L2);
    printf("%s\n", body);
    printf("goto %s\n", L1);
    printf("%s:\n", L2);
}
