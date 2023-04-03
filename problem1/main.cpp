#include <bits/stdc++.h>

#define OFFSET_LINUX 1
#define OFFSET_WINDOWS 2

#define SKIP_END_LINE_LINUX(file) file.get()
#define SKIP_END_LINE_WINDOWS(file) file.get(); file.get()

#define WRITE_END_LINE_LINUX(file) file << endl
#define WRITE_END_LINE_WINDOWS(file) file << '\r' << '\n'

using namespace std;

string cleanSpacesStringAtEnd(string str) {
    int i = str.length() - 1;
    while (str[i] == ' ') {
        str.pop_back();
        i--;
    }
    return str;
}

string replaceNullWithSpaces(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\0') {
            str[i] = ' ';
        }
    }
    return str;
}

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
};

class FixedRecord {
    string filename;
    fstream file;

public:
    explicit FixedRecord(const string &filename) {
        this->filename = filename;
        file.open(filename, ios::in | ios::app | ios::binary);
    }

    vector<Alumno> load() {
        file.clear();
        file.seekg(0, ios::beg);
        vector<Alumno> records;
        Alumno record{};
        while (file.read((char *) &record, sizeof(Alumno))) {
            records.push_back(record);
            SKIP_END_LINE_LINUX(file);
        }
        return records;
    }

    void add(Alumno record) {
        string codigo = string(record.codigo, sizeof(Alumno::codigo));
        string nombre = string(record.nombre, sizeof(Alumno::nombre));
        string apellidos = string(record.apellidos, sizeof(Alumno::apellidos));
        string carrera = string(record.carrera, sizeof(Alumno::carrera));

        codigo = replaceNullWithSpaces(codigo);
        nombre = replaceNullWithSpaces(nombre);
        apellidos = replaceNullWithSpaces(apellidos);
        carrera = replaceNullWithSpaces(carrera);

        file.clear();
        file.seekp(0, ios::end);

        file << codigo << nombre << apellidos << carrera;
        WRITE_END_LINE_LINUX(file);
    }

    Alumno readRecord(const int &pos) {
        file.clear();
        Alumno record{};
        file.seekg(pos * (sizeof(Alumno) + OFFSET_LINUX), ios::beg);
        file.read((char *) &record, sizeof(Alumno));
        return record;
    }

    ~FixedRecord() {
        file.close();
    }
};

int main() {
    FixedRecord fixedRecord("../datos1.txt");

    // load test
    assert(fixedRecord.load().size() == 7);

    // read test
    Alumno record = fixedRecord.readRecord(0);

    string stringCodigo;
    string stringNombre;
    string stringApellidos;
    string stringCarrera;

    stringCodigo = cleanSpacesStringAtEnd(string(record.codigo, 5));
    stringNombre = cleanSpacesStringAtEnd(string(record.nombre, 11));
    stringApellidos = cleanSpacesStringAtEnd(string(record.apellidos, 20));
    stringCarrera = cleanSpacesStringAtEnd(string(record.carrera, 15));

    assert(stringCodigo == "0001");
    assert(stringNombre == "Howard");
    assert(stringApellidos == "Paredes Zegarra");
    assert(stringCarrera == "Computacion");

    record = fixedRecord.readRecord(1);

    stringCodigo = cleanSpacesStringAtEnd(string(record.codigo, 5));
    stringNombre = cleanSpacesStringAtEnd(string(record.nombre, 11));
    stringApellidos = cleanSpacesStringAtEnd(string(record.apellidos, 20));
    stringCarrera = cleanSpacesStringAtEnd(string(record.carrera, 15));

    assert(stringCodigo == "0002");
    assert(stringNombre == "Penny");
    assert(stringApellidos == "Vargas Cordero");
    assert(stringCarrera == "Industrial");

    Alumno newRecord = {"0008", "Leonard", "Hofstadter", "Computacion"};
    fixedRecord.add(newRecord);
    assert(fixedRecord.load().size() == 8);

    record = fixedRecord.readRecord(7);
    stringCodigo = cleanSpacesStringAtEnd(string(record.codigo, 5));
    stringNombre = cleanSpacesStringAtEnd(string(record.nombre, 11));
    stringApellidos = cleanSpacesStringAtEnd(string(record.apellidos, 20));
    stringCarrera = cleanSpacesStringAtEnd(string(record.carrera, 15));

    assert(stringCodigo == "0008");
    assert(stringNombre == "Leonard");
    assert(stringApellidos == "Hofstadter");
    assert(stringCarrera == "Computacion");

    return 0;
}
