#include <bits/stdc++.h>
using namespace std;


string convert(int decimal) {
    string binary = "" ;
    for (int i = decimal; i > 0; i /= 2) {
        binary = to_string(i % 2) + binary;
    }
    if (binary.length() < 8) {
        binary = string(8 - binary.length(), '0') + binary;
    }
    return binary;
}

struct Mnemonics {
    string name;
    string binary;
    int size;
} mot[20];

struct Symbol {
    string name;
    string type;
    int location;
    int size;
    int section_id;
    string is_global;
};

struct Section {
    int id;
    string name;
    int size;
};

vector<Symbol> symtab;  // Symbol Table
vector<Section> sect;   // Section Table
int lc = 0;             // Location Counter
int sec_id = 0;         // Section Id
int var_lc;             // Store location of variable in Pass2
ifstream infile;        // Input File Stream
ofstream outfile;       // Output File Stream
string word;            // Read word by word from file
string temp;            // Temporary variable
int control, size;      // Control variables for search

void init() {
    // Initializing Machine Opcode Table
    //name binary size
    mot[0] = {"ADD", "00000001", 1};
    mot[1] = {"ADDI", "00000010", 5};
    mot[2] = {"CMP", "00000011", 5};
    mot[3] = {"INC", "00000100", 1};
    mot[4] = {"JE", "00000101", 5};
    mot[5] = {"JMP", "00000110", 5};
    mot[6] = {"LOAD", "00000111", 5};
    mot[7] = {"LOADI", "00001000", 5};
    mot[8] = {"MVTI", "00001001", 5};
    mot[9] = {"MOV", "00001010", 1};
    mot[10] = {"STOP", "00001111", 1};
    mot[11] = {"STORE", "00001100", 5};
    mot[12] = {"STOREI", "00001101", 1};
}

int search_mot(const string& opcode) {
    for (int i = 0; i < 13; ++i) {
        if (mot[i].name == opcode) {
            return i;
        }
    }
    return -1;
}

int search_symbol(const string& variable) {
    for (const auto& sym : symtab) {
        if (sym.name == variable) {
            return sym.location;
        }
    }
    return -1;
}

int size_evaluation(const string& data) {
    int size = 0;
    for (char c : data) {
        if (c == ',') {
            size += 4;
        }
    }
    size += 4;
    return size;
}

string data_break(const string& data) {
    string final;
    string temp = "";
    for (char c : data) {
        if (c == ',') {
            final += convert(atoi(temp.c_str())) + ",";
            temp = "";
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        final += convert(atoi(temp.c_str()));
    }
    return final;
}

void store_symlab() {
    outfile.open("symbol.csv");
    outfile << "Name,Type,Location,Size,SectionId,IsGlobal\n";
    for (const auto& sym : symtab) {
        outfile << sym.name << ','
                << sym.type << ','
                << sym.location << ','
                << sym.size << ','
                << sym.section_id << ','
                << sym.is_global << '\n';
    }
    outfile.close();
}

void store_sec() {
    outfile.open("section.csv");
    outfile << "ID,Name,Size\n";
    for (const auto& sec : sect) {
        outfile << sec.id << ','
                << sec.name << ','
                << sec.size << '\n';
    }
    outfile.close();
}

void pass1() {
    infile.open("input.txt");
    while (infile >> word) {
        control = search_mot(word);
        if (control == -1) {
            temp = word;
            if (word.find(":") != string::npos) {
                // Label found
                Symbol s = {temp.erase(word.length() - 1, 1), "label", lc, -1, sec_id, "false"};
                symtab.push_back(s);
            } else if (word == "section") {
                infile >> word;
                ++sec_id;
                Section a = {sec_id, word, 0};
                sect.push_back(a);
                if (sec_id != 1) {
                    sect[sec_id - 2].size = lc;
                    lc = 0;
                }
            } else if (word == "global") {
                infile >> word;
                Symbol b = {word, "label", -1, -1, -1, "true"};
                symtab.push_back(b);
            } else if (word == "extern") {
                infile >> word;
                Symbol b = {word, "external", -1, -1, -1, "false"};
                symtab.push_back(b);
            } else {
                infile >> word;
                infile >> word;
                int len = size_evaluation(word);
                Symbol b = {temp, "var", lc, len, sec_id, "false"};
                symtab.push_back(b);
                lc += len;
            }
        } else {
            if (!(control == 7 || control == 12)) infile >> word;
            // LOADI and STOREI do not have any parameter
            if (control == 2 || control == 8 || control == 9) infile >> word;
            lc += mot[control].size;
        }
    }

    sect[sec_id - 1].size = lc;
    store_symlab();
    store_sec();
    infile.close();
}

void pass2() {
    infile.open("input.txt");
    outfile.open("output.txt");
    while (infile >> word) {
        control = search_mot(word);
        if (control == -1) {
            temp = word;
            if (word.find(":") != string::npos) {
                outfile << "";
            } else if (word == "global") {
                infile >> word;
                outfile << "global " << word << '\n';
            } else if (word == "extern") {
                infile >> word;
                outfile << "extern " << word << '\n';
            } else if (word == "section") {
                infile >> word;
                outfile << "section ." << word << '\n';
                lc = 0;
            } else {
                infile >> word;
                infile >> word;
                outfile << convert(lc) << " " << data_break(word) << '\n';
                int len = size_evaluation(word);
                lc += len;
            }
        } else {
            outfile << convert(lc) << " " << mot[control].binary;
            if (control == 0 || control == 3) {
                infile >> word;
                outfile << " " << word;
            } else if (control == 1 || control == 4 || control == 5 || control == 6 || control == 11) {
                infile >> word;
                var_lc = search_symbol(word);
                if (var_lc == -1) {
                    outfile << " " << convert(atoi(word.c_str()));
                } else {
                    outfile << " " << convert(var_lc);
                }
            } else if (control == 9) {
                infile >> word;
                outfile << " " << word;
                infile >> word;
                outfile << " " << word;
            }
            lc += mot[control].size;
            outfile << '\n';
        }
    }
    outfile.close();
    infile.close();
}

int main() {
    init();
    pass1();
    lc = 0;
    pass2();
    return 0;
}
