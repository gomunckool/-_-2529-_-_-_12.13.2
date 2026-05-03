#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Employee {
    string fullName;
    string position;
    int yearJoined;
    double salary;
    bool isActive = false;
    bool isDeleted = false;
};

int getValidatedInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Pomylka! Vvedit korektne chyslo.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

double getValidatedDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Pomylka! Vvedit korektne znachennya okladu.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

int hashFunction(const string& key, int size) {
    unsigned long hash = 5381;
    for (char c : key) hash = ((hash << 5) + hash) + c;
    return hash % size;
}

int findIndex(const vector<Employee>& table, const string& key) {
    int size = table.size();
    int hash = hashFunction(key, size);
    int startHash = hash;

    do {
        if (!table[hash].isActive && !table[hash].isDeleted) return -1;
        if (table[hash].isActive && table[hash].fullName == key) return hash;
        hash = (hash + 1) % size;
    } while (hash != startHash);

    return -1;
}

bool addEmployee(vector<Employee>& table, const Employee& emp) {
    int size = table.size();
    int hash = hashFunction(emp.fullName, size);
    int startHash = hash;

    do {
        if (!table[hash].isActive) {
            table[hash] = emp;
            table[hash].isActive = true;
            table[hash].isDeleted = false;
            return true;
        }
        hash = (hash + 1) % size;
    } while (hash != startHash);

    return false;
}

bool removeEmployee(vector<Employee>& table, const string& key) {
    int idx = findIndex(table, key);
    if (idx != -1) {
        table[idx].isActive = false;
        table[idx].isDeleted = true;
        return true;
    }
    return false;
}

void saveToFile(const vector<Employee>& table, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cout << "Pomylka vidkryttya faylu!\n";
        return;
    }
    for (const auto& emp : table) {
        if (emp.isActive) {
            out << emp.fullName << "\n" << emp.position << "\n" 
                << emp.yearJoined << "\n" << emp.salary << "\n";
        }
    }
    out.close();
    cout << "Dani zberezheno.\n";
}

void loadFromFile(vector<Employee>& table, const string& filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Fayl ne znaydeno.\n";
        return;
    }
    Employee emp;
    while (getline(in, emp.fullName)) {
        getline(in, emp.position);
        in >> emp.yearJoined >> emp.salary;
        in.ignore();
        addEmployee(table, emp);
    }
    in.close();
    cout << "Dani zavantazheno.\n";
}

void showEmployee(const vector<Employee>& table, const string& key) {
    int idx = findIndex(table, key);
    if (idx != -1) {
        cout << "\nPrizvyshche: " << table[idx].fullName
             << "\nPosada:      " << table[idx].position
             << "\nRik:         " << table[idx].yearJoined
             << "\nOklad:       " << fixed << setprecision(2) << table[idx].salary << endl;
    } else {
        cout << "Sluzhbovtsya ne znaydeno.\n";
    }
}

void printTable(const vector<Employee>& table) {
    cout << "\n" << left << setw(20) << "Prizvyshche" << setw(15) << "Posada" << setw(10) << "Rik" << "Oklad" << endl;
    for (const auto& emp : table) {
        if (emp.isActive) {
            cout << left << setw(20) << emp.fullName << setw(15) << emp.position 
                 << setw(10) << emp.yearJoined << emp.salary << endl;
        }
    }
}

void menu() {
    const int TABLE_SIZE = 100;
    vector<Employee> table(TABLE_SIZE);
    int choice;
    string name, filename;

    do {
        cout << "\n--- MENU KHESH-TABLYTSI ---"
             << "\n1. Dodaty sluzhbovtsya"
             << "\n2. Vyluchyty za prizvyshchem"
             << "\n3. Poshuk za prizvyshchem"
             << "\n4. Redahuvaty dani"
             << "\n5. Vyvesty vsikh"
             << "\n6. Zberehty u fayl"
             << "\n7. Zavantazhyty z faylu"
             << "\n0. Vykhid"
             << "\nVash vybir: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                Employee e;
                cout << "Prizvyshche ta initsialy: "; getline(cin, e.fullName);
                cout << "Posada: "; getline(cin, e.position);
                e.yearJoined = getValidatedInt("Rik pryyomu: ");
                e.salary = getValidatedDouble("Oklad: ");
                if (!addEmployee(table, e)) cout << "Tablytsya perepovnena!\n";
                break;
            }
            case 2:
                cout << "Vvedit prizvyshche dlya vydalennya: "; getline(cin, name);
                if (removeEmployee(table, name)) cout << "Vydaleno.\n";
                else cout << "Ne znaydeno.\n";
                break;
            case 3:
                cout << "Vvedit prizvyshche dlya poshuku: "; getline(cin, name);
                showEmployee(table, name);
                break;
            case 4: {
                cout << "Vvedit prizvyshche dlya redahuvannya: "; getline(cin, name);
                int idx = findIndex(table, name);
                if (idx != -1) {
                    cout << "Nova posada: "; getline(cin, table[idx].position);
                    table[idx].yearJoined = getValidatedInt("Novyy rik: ");
                    table[idx].salary = getValidatedDouble("Novyy oklad: ");
                } else cout << "Ne znaydeno.\n";
                break;
            }
            case 5: printTable(table); break;
            case 6:
                cout << "Imya faylu: "; getline(cin, filename);
                saveToFile(table, filename);
                break;
            case 7:
                cout << "Imya faylu: "; getline(cin, filename);
                loadFromFile(table, filename);
                break;
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
