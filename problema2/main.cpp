#include <bits/stdc++.h>

#define OFFSET_LINUX 1
#define OFFSET_WINDOWS 2

#define SKIP_END_LINE_LINUX(file) file.get()
#define SKIP_END_LINE_WINDOWS(file) file.get(); file.get()

#define WRITE_END_LINE_LINUX(file) file << endl
#define WRITE_END_LINE_WINDOWS(file) file << '\r' << '\n'

using namespace std;

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo; // char[3]
    double mensualidad; // char[9]
};

struct AlumnoRecord {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    char ciclo[3];
    char mensualidad[9];
    char freeListIndex[3];
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
        vector<Alumno> alumnos;
        AlumnoRecord record{};

        while (file.read((char *) &record, sizeof(AlumnoRecord))) {
            SKIP_END_LINE_LINUX(file);
            Alumno alumno{};
            strcpy(alumno.codigo, record.codigo);
            strcpy(alumno.nombre, record.nombre);
            strcpy(alumno.apellidos, record.apellidos);
            strcpy(alumno.carrera, record.carrera);
            alumno.ciclo = stoi(record.ciclo);
            // 331.41000000000003 to 331.41
            alumno.mensualidad = stod(record.mensualidad);
            alumnos.push_back(alumno);
        }
        return alumnos;
    }

    Alumno readRecord(int pos) {
        file.clear();
        file.seekg(pos * (sizeof(AlumnoRecord) + OFFSET_LINUX), ios::beg);

        AlumnoRecord record{};
        file.read((char *) &record, sizeof(AlumnoRecord));
        Alumno alumno{};
        memccpy(alumno.codigo, record.codigo, 0, sizeof(AlumnoRecord::codigo));
        memccpy(alumno.nombre, record.nombre, 0, sizeof(AlumnoRecord::nombre));
        memccpy(alumno.apellidos, record.apellidos, 0, sizeof(AlumnoRecord::apellidos));
        memccpy(alumno.carrera, record.carrera, 0, sizeof(AlumnoRecord::carrera));
        alumno.ciclo = stoi(record.ciclo);
        alumno.mensualidad = stod(record.mensualidad);
        return alumno;
    }

    ~FixedRecord() {
        file.close();
    }
};

int main() {
    FixedRecord fixedRecord("../datos.txt");
//    vector<Alumno> records = fixedRecord.load();
    Alumno xd = fixedRecord.readRecord(1);
    return 0;
}
