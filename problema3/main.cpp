#include <bits/stdc++.h>

#define OFFSET_LINUX 1
#define OFFSET_WINDOWS 2
#define TRASH -2

#define SKIP_END_LINE_LINUX(file) file.get()
#define SKIP_END_LINE_WINDOWS(file) file.get(); file.get()

#define WRITE_END_LINE_LINUX(file) file << endl
#define WRITE_END_LINE_WINDOWS(file) file << '\r' << '\n'

using namespace std;
using free_slot = int;
struct Alumno {
    string nombre;
    string apellidos;
    string carrera;
    float mensualidad; 
};


class VariableRecord {
    string filename;
    fstream file;
    char separator;

public:
    explicit VariableRecord(const string &fileName, char separator = '|') {
        this->filename = fileName;
        this->file = fstream(fileName, ios::in | ios::out | ios::binary);
        this->separator = separator;
    }

    vector<Alumno> load() {
        vector<Alumno> records;
        file.seekg(0, ios::beg);
        //skip line
        string dummy;
        getline(file, dummy);
        while (!file.eof()) {
            Alumno alumno;
            getline(file, alumno.nombre, separator);
            getline(file, alumno.apellidos, separator);
            getline(file, alumno.carrera, separator);
            file >> alumno.mensualidad;
            SKIP_END_LINE_LINUX(file);
            records.push_back(alumno);
        }
        return records;
    }

    Alumno parse(const string &data) {
        Alumno alumno;
        int i = 0;
        string token;
        stringstream ss(data);
        while (getline(ss, token, separator)) {
            if (i == 0) {
                alumno.nombre = token;
            } else if (i == 1) {
                alumno.apellidos = token;
            } else if (i == 2) {
                alumno.carrera = token;
            } else if (i == 3) {
                alumno.mensualidad = stof(token);
            }
            i++;
        }
        return alumno;
    }

    Alumno readRecord(int pos) {
        file.seekg(0, ios::beg);
        pos++;
        int i = 0;
        string data;
        while (!file.eof()) {
            getline(file, data);
            if (i == pos) {
                return(parse(data));
            }
            i++;
        }
        return Alumno();
    }


    ~VariableRecord() {
        file.close();
    }
    void addReccord(Alumno record) {
        file.seekg(0, ios::end);
        WRITE_END_LINE_LINUX(file);
        file << record.nombre << separator << record.apellidos << separator << record.carrera << separator << record.mensualidad;

    }

};

int main() {
    VariableRecord fixedRecord("datos.txt");
    fixedRecord.addReccord({"Juan", "Perez", "Ingenieria", 1000.9});
    fixedRecord.addReccord({"Juana", "Peres", "Arte", 1300.56});
    fixedRecord.addReccord({"Juanito", "Perrez", "Psicologia", 1500.1});
    Alumno alumno = fixedRecord.readRecord(3);
    cout << alumno.nombre << " " << alumno.apellidos << " " << alumno.carrera << " " << alumno.mensualidad << endl; 

    return 0;
};
