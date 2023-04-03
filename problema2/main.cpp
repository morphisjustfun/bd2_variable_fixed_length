#include <bits/stdc++.h>

using namespace std;

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo; // char[3]
    float mensualidad; // char[9]
};

class FixedRecord {
    string filename;
    fstream file;

public:
    explicit FixedRecord(const string &fileName) {
        this->filename = fileName;
        this->file = fstream(fileName, ios::in | ios::out | ios::binary);
    }

    vector<Alumno> load() {
        file.clear();
        file.seekg(0, ios::beg);
        vector<Alumno> records;
        Alumno record{};
        string line;

        while (std::getline(file, line)) {
            memcpy(record.codigo, line.substr(0, 5).c_str(), 5);
            memcpy(record.nombre, line.substr(5, 11).c_str(), 11);
            memcpy(record.apellidos, line.substr(16, 20).c_str(), 20);
            memcpy(record.carrera, line.substr(36, 15).c_str(), 15);
            int ciclo = stoi(line.substr(51, 3));
            float mensualidad = stof(line.substr(54, 9));
            record.ciclo = ciclo;
            record.mensualidad = mensualidad;
            records.push_back(record);
        }
        return records;
    }

    ~FixedRecord() {
        this->file.close();
    }
};

int main() {
    FixedRecord fixedRecord("./datos.txt");
    vector<Alumno> records = fixedRecord.load();
    return 0;
}
