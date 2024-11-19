#include<bits/stdc++.h>
using namespace std;
const int MAX = 11;

struct Symbol {
    int address;  
    char label[10]; 
};

class SymbolTable {
private:
    int table[MAX];
    Symbol symbols[MAX];

    int create(int num) const {
        return num % MAX;
    }

    void handleCollision(int key, int num, const char *label) {
        int i, flag = 0, count = 0;

        if (table[key] == 0) {
            table[key] = num;
            symbols[key].address = num;
            strcpy(symbols[key].label, label);
        } else {
            for (i = 0; i < MAX; ++i) {
                if (table[i] != 0) count++;
            }

            if (count == MAX) {
                cout << "\nHash table is full\n";
                display();
                exit(1);
            }

            for (i = key + 1; i < MAX; ++i) {
                if (table[i] == 0) {
                    table[i] = num;
                    symbols[i].address = num;
                    strcpy(symbols[i].label, label);
                    flag = 1;
                    break;
                }
            }

            if (flag == 0) {
                for (i = 0; i < key; ++i) {
                    if (table[i] == 0) {
                        table[i] = num;
                        symbols[i].address = num;
                        strcpy(symbols[i].label, label);
                        break;
                    }
                }
            }
        }
    }

    void display() const {
        FILE *fp = fopen("symbol.txt", "w");
        if (!fp) {
            cerr << "Error opening file\n";
            exit(1);
        }

        cout << "\nSymbol Table:";
        cout << "\nHash Index | Address | Label\n";

        for (int i = 0; i < MAX; ++i) {
            if (table[i] != 0) {
                cout << i << "          | " << symbols[i].address << "      | " << symbols[i].label << "\n";
                fprintf(fp, "%d %d %s\n", i, symbols[i].address, symbols[i].label);
            }
        }
        fclose(fp);
    }

public:
    SymbolTable() {
        memset(table, 0, sizeof(table));
        memset(symbols, 0, sizeof(symbols));
    }

    void createEntry() {
        int num;
        char label[10];
        char ans;

        do {
            cout << "\nEnter the address: ";
            cin >> num;
            int key = create(num);
            cout << "Enter the label: ";
            cin >> label;
            handleCollision(key, num, label);
            cout << "\nContinue? 0 for no, 1 for yes: ";
            cin >> ans;
        } while (ans == '1');

        display();
    }

    void search() const {
        FILE *fp = fopen("symbol.txt", "r");
        if (!fp) {
            cerr << "Error opening file\n";
            return;
        }

        char label[10];
        bool found = false;

        cout << "Enter the label: ";
        cin >> label;

        int index, address;
        char tempLabel[10];
        while (fscanf(fp, "%d %d %s", &index, &address, tempLabel) != EOF) {
            if (strcmp(tempLabel, label) == 0) {
                cout << "\nThe label '" << label << "' is present in the symbol table at address: " << address << "\n";
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\nThe label '" << label << "' is not present in the symbol table\n";
        }

        fclose(fp);
    }
};

int main() {
    SymbolTable table;
    int choice;

    do {
        cout << "\nEnter your choice:";
        cout << "\n1 - Create entry in the symbol table\n2 - Search in the symbol table\n";
        cin >> choice;

        switch (choice) {
            case 1:
                table.createEntry();
                break;
            case 2:
                table.search();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice <= 2);

    return 0;
}
