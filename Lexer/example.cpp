#include <iostream>
#include <string>
#include <cctype>
using namespace std;

void lexicalAnalyze(string s) {
    string operators = "+-*/=";
    string delimiters = ";";
    
    for(int i = 0; i < s.length(); i++) {
        string token = "";
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
            continue;
        
        if (operators.find(s[i]) != string::npos) {
            token += s[i];
            cout << "Operator: " << token << endl;
            continue;
        }

        if (delimiters.find(s[i]) != string::npos) {
            token += s[i];
            cout << "Delimiter: " << token << endl;
            continue;
        }

        if (isdigit(s[i])) {
            while(i < s.length() && (isdigit(s[i]) || s[i] == '.')) {
                token += s[i];
                i++;
            }
            cout << "Number: " << token << endl;
            i--;
            continue;
        }

        if (isalpha(s[i])) {
            while(i < s.length() && isalpha(s[i])) {
                token += s[i];
                i++;
            }
            cout << "Identifier: " << token << endl;
            i--;
            continue;
        }
    }
    
}

int main() {
    string s = "int a = 10; float b = 20.5; if (a < b) {}";
    lexicalAnalyze(s);
    return 0;
}