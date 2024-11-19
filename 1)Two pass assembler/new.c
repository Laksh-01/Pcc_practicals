#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MOT 20
#define MAX_SYMTAB 100
#define MAX_SECTAB 10

// Function to convert decimal to binary (8 bits)
char* convert(int decimal) {
    static char binary[9];  // Static to return a pointer to the string
    int i;
    for (i = 7; i >= 0; i--) {
        binary[i] = (decimal % 2) + '0';
        decimal /= 2;
    }
    binary[8] = '\0';
    return binary;
}

struct Mnemonics {
    char name[10];
    char binary[9];
    int size;
} mot[MAX_MOT];

struct Symbol {
    char name[50];
    char type[20];
    int location;
    int size;
    int section_id;
    char is_global[10];
};

struct Section {
    int id;
    char name[50];
    int size;
};

struct Symbol symtab[MAX_SYMTAB];  // Symbol Table
struct Section sect[MAX_SECTAB];   // Section Table
int lc = 0;             // Location Counter
int sec_id = 0;         // Section Id
int var_lc;             // Store location of variable in Pass2
FILE *infile, *outfile; // File pointers
char word[50];          // Read word by word from file
char temp[50];          // Temporary variable
int control, size;      // Control variables for search

// Function to initialize Machine Opcode Table (MOT)
void init() {
    // Initialize Mnemonics Table (MOT)
    strcpy(mot[0].name, "ADD");
    strcpy(mot[0].binary, "00000001");
    mot[0].size = 1;

    strcpy(mot[1].name, "ADDI");
    strcpy(mot[1].binary, "00000010");
    mot[1].size = 5;

    strcpy(mot[2].name, "CMP");
    strcpy(mot[2].binary, "00000011");
    mot[2].size = 5;

    strcpy(mot[3].name, "INC");
    strcpy(mot[3].binary, "00000100");
    mot[3].size = 1;

    strcpy(mot[4].name, "JE");
    strcpy(mot[4].binary, "00000101");
    mot[4].size = 5;

    strcpy(mot[5].name, "JMP");
    strcpy(mot[5].binary, "00000110");
    mot[5].size = 5;

    strcpy(mot[6].name, "LOAD");
    strcpy(mot[6].binary, "00000111");
    mot[6].size = 5;

    strcpy(mot[7].name, "LOADI");
    strcpy(mot[7].binary, "00001000");
    mot[7].size = 5;

    strcpy(mot[8].name, "MVTI");
    strcpy(mot[8].binary, "00001001");
    mot[8].size = 5;

    strcpy(mot[9].name, "MOV");
    strcpy(mot[9].binary, "00001010");
    mot[9].size = 1;

    strcpy(mot[10].name, "STOP");
    strcpy(mot[10].binary, "00001111");
    mot[10].size = 1;

    strcpy(mot[11].name, "STORE");
    strcpy(mot[11].binary, "00001100");
    mot[11].size = 5;

    strcpy(mot[12].name, "STOREI");
    strcpy(mot[12].binary, "00001101");
    mot[12].size = 1;
}

// Function to search for mnemonic in MOT
int search_mot(const char* opcode) {
    for (int i = 0; i < 13; ++i) {
        if (strcmp(mot[i].name, opcode) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to search for symbol in symbol table
int search_symbol(const char* variable) {
    for (int i = 0; i < MAX_SYMTAB; ++i) {
        if (strcmp(symtab[i].name, variable) == 0) {
            return symtab[i].location;
        }
    }
    return -1;
}

// Function to evaluate size of data
int size_evaluation(const char* data) {
    int size = 0;
    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == ',') {
            size += 4;
        }
    }
    size += 4;
    return size;
}

// Function to break data into binary representation
char* data_break(const char* data) {
    static char final[200];
    char temp[50];
    int j = 0;

    final[0] = '\0';  // Initialize final string

    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == ',') {
            temp[j] = '\0';
            strcat(final, convert(atoi(temp)));
            strcat(final, ",");
            j = 0;
        } else {
            temp[j++] = data[i];
        }
    }
    temp[j] = '\0';
    strcat(final, convert(atoi(temp)));
    return final;
}

// Function to store symbol table to a file
void store_symlab() {
    outfile = fopen("symbol.csv", "w");
    fprintf(outfile, "Name,Type,Location,Size,SectionId,IsGlobal\n");
    for (int i = 0; i < MAX_SYMTAB; i++) {
        if (strlen(symtab[i].name) == 0) break;
        fprintf(outfile, "%s,%s,%d,%d,%d,%s\n", symtab[i].name, symtab[i].type, symtab[i].location, symtab[i].size, symtab[i].section_id, symtab[i].is_global);
    }
    fclose(outfile);
}

// Function to store section table to a file
void store_sec() {
    outfile = fopen("section.csv", "w");
    fprintf(outfile, "ID,Name,Size\n");
    for (int i = 0; i < MAX_SECTAB; i++) {
        if (strlen(sect[i].name) == 0) break;
        fprintf(outfile, "%d,%s,%d\n", sect[i].id, sect[i].name, sect[i].size);
    }
    fclose(outfile);
}

// Pass1 function to handle first pass of the assembler
void pass1() {
    infile = fopen("input.txt", "r");
    while (fscanf(infile, "%s", word) != EOF) {
        control = search_mot(word);
        if (control == -1) {
            strcpy(temp, word);
            if (strchr(word, ':') != NULL) {
                // Label found
                struct Symbol s = {0};
                strcpy(s.name, temp);
                strcpy(s.type, "label");
                s.location = lc;
                s.section_id = sec_id;
                strcpy(s.is_global, "false");
                symtab[sec_id] = s;
            } else if (strcmp(word, "section") == 0) {
                fscanf(infile, "%s", word);
                sec_id++;
                struct Section a = {sec_id, {0}, 0};
                strcpy(a.name, word);
                sect[sec_id - 1] = a;
                if (sec_id != 1) {
                    sect[sec_id - 2].size = lc;
                    lc = 0;
                }
            } else if (strcmp(word, "global") == 0) {
                fscanf(infile, "%s", word);
                struct Symbol b = {0};
                strcpy(b.name, word);
                strcpy(b.type, "label");
                strcpy(b.is_global, "true");
                symtab[sec_id] = b;
            } else if (strcmp(word, "extern") == 0) {
                fscanf(infile, "%s", word);
                struct Symbol b = {0};
                strcpy(b.name, word);
                strcpy(b.type, "external");
                strcpy(b.is_global, "false");
                symtab[sec_id] = b;
            } else {
                fscanf(infile, "%s", word);
                fscanf(infile, "%s", word);
                int len = size_evaluation(word);
                struct Symbol b = {0};
                strcpy(b.name, temp);
                strcpy(b.type, "var");
                b.location = lc;
                b.size = len;
                b.section_id = sec_id;
                strcpy(b.is_global, "false");
                symtab[sec_id] = b;
                lc += len;
            }
        } else {
            if (control != 7 && control != 12) fscanf(infile, "%s", word); // LOADI and STOREI do not have parameters
            if (control == 2 || control == 8 || control == 9) fscanf(infile, "%s", word); // Some opcodes need one more operand
            lc += mot[control].size;
        }
    }
    sect[sec_id - 1].size = lc;
    store_symlab();
    store_sec();
    fclose(infile);
}


// Pass2 function to handle second pass of the assembler
void pass2() {
    infile = fopen("input.txt", "r");
    outfile = fopen("output.txt", "w");
    while (fscanf(infile, "%s", word) != EOF) {
        control = search_mot(word);
        if (control == -1) {
            strcpy(temp, word);
            if (strchr(word, ':') != NULL) {
                fprintf(outfile, "");
            } else if (strcmp(word, "global") == 0) {
                fscanf(infile, "%s", word);
                fprintf(outfile, "global %s\n", word);
            } else if (strcmp(word, "extern") == 0) {
                fscanf(infile, "%s", word);
                fprintf(outfile, "extern %s\n", word);
            } else if (strcmp(word, "section") == 0) {
                fscanf(infile, "%s", word);
                fprintf(outfile, "section .%s\n", word);
                lc = 0;
            } else {
                fscanf(infile, "%s", word);
                fscanf(infile, "%s", word);
                fprintf(outfile, "%s %s\n", convert(lc), data_break(word));
                int len = size_evaluation(word);
                lc += len;
            }
        } else {
            fprintf(outfile, "%s %s", convert(lc), mot[control].binary);
            if (control == 0 || control == 3) {
                fscanf(infile, "%s", word);
                fprintf(outfile, " %s", word);
            } else if (control == 1 || control == 4 || control == 5 || control == 6 || control == 11) {
                fscanf(infile, "%s", word);
                var_lc = search_symbol(word);
                if (var_lc == -1) {
                    fprintf(outfile, " %s", convert(atoi(word)));
                } else {
                    fprintf(outfile, " %s", convert(var_lc));
                }
            } else if (control == 9) {
                fscanf(infile, "%s", word);
                fprintf(outfile, " %s", word);
                fscanf(infile, "%s", word);
                fprintf(outfile, " %s", word);
            }
            lc += mot[control].size;
            fprintf(outfile, "\n");
        }
    }
    fclose(outfile);
    fclose(infile);
}

int main() {
    init();
    pass1();
    lc = 0;
    pass2();
    return 0;
}
