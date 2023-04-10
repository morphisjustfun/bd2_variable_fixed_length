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
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo; // char[3]
    double mensualidad; // char[9]
};


class FixedRecord {
    string filename;
    fstream file;

public:
    explicit FixedRecord(const string &fileName) {
        this->filename = fileName;
        this->file = fstream(fileName, ios::in | ios::out | ios::binary);
        
        if (file.get() != EOF) {
            return;
        }
        int def = -1;
        file.clear();
        this->file.seekg(sizeof(Alumno), ios::beg);
        this->file.write((char*)&def, sizeof(free_slot));
    }

    vector<Alumno> load() {
        file.clear();
        file.seekg(sizeof(Alumno) + sizeof(free_slot));
        vector<Alumno> alumnos;
        
        while (true) {
            SKIP_END_LINE_LINUX(file);
            if (file.eof()) break; 
            int current_free;
            Alumno alumno{};
            file.read(alumno.codigo, sizeof(Alumno::codigo));
            file.read(alumno.nombre, sizeof(Alumno::nombre));
            file.read(alumno.apellidos, sizeof(Alumno::apellidos));
            file.read(alumno.carrera, sizeof(Alumno::carrera));
            file.read((char *) &alumno.ciclo, sizeof(Alumno::ciclo));
            file.read((char *) &alumno.mensualidad, sizeof(Alumno::mensualidad));
            file.read((char*)&current_free, sizeof(free_slot));
            // 331.41000000000003 to 331.41
            if (current_free == -2)
                alumnos.push_back(alumno);
        }
        return alumnos;
    }

    Alumno readRecord(int pos) {
        file.clear();
        file.seekg((pos+1) * (sizeof(Alumno) + sizeof(free_slot) + OFFSET_LINUX)-pos, ios::beg);

        Alumno alumno{};
        file.read(alumno.codigo, sizeof(Alumno::codigo));
        file.read(alumno.nombre, sizeof(Alumno::nombre));
        file.read(alumno.apellidos, sizeof(Alumno::apellidos));
        file.read(alumno.carrera, sizeof(Alumno::carrera));
        file.read((char *) &alumno.ciclo, sizeof(Alumno::ciclo));
        file.read((char *) &alumno.mensualidad, sizeof(Alumno::mensualidad));
        return alumno;
    }

    bool deleteRecord(int pos) {
        
        file.clear();
        file.seekg(0);
        int current_free;
        int new_free;
        file.seekg(sizeof(Alumno), ios::cur);
        auto p = file.tellg();
        file.read((char*)&current_free, sizeof(free_slot));
        
        file.seekg((pos+1) * (sizeof(Alumno) + sizeof(free_slot) + OFFSET_LINUX) - 1, ios::beg);
        file.seekg(sizeof(Alumno) - pos, ios::cur);
        auto p2 = file.tellg();
        file.read((char*)&new_free, sizeof(free_slot));
        if (new_free != -2) {
            return false;
        }
        //write 2 value
        file.seekg(p2, std::ios_base::beg);
        file.write((char*)&current_free, sizeof(free_slot));
        
        //write first value
        file.seekg(p, std::ios_base::beg);
        file.write((char*)&pos, sizeof(free_slot));
        return true;
    }


    ~FixedRecord() {
        file.close();
    }
    void add(Alumno record) {
        //seek to end of file
        file.clear();
        file.seekp(sizeof(Alumno), ios::beg);
        int p = file.tellg();
        int current_free;
        file.read((char*)&current_free, sizeof(free_slot));
        if (current_free == -1) {
            file.seekp(0, ios::end);
            file << "\n";
            file.write(record.codigo, sizeof(Alumno::codigo));
            file.write(record.nombre, sizeof(Alumno::nombre));
            file.write(record.apellidos, sizeof(Alumno::apellidos));
            file.write(record.carrera, sizeof(Alumno::carrera));
            file.write((char*)&record.ciclo, sizeof(Alumno::ciclo));
            file.write((char*)&record.mensualidad, sizeof(Alumno::mensualidad));
            free_slot trash = TRASH;
            file.write((char*)&trash, sizeof(free_slot));
        } else {
            file.seekg((current_free+1) * (sizeof(Alumno) + sizeof(free_slot) + OFFSET_LINUX) - 1  - current_free, ios::beg);
            int p2 = file.tellg() + 1;
            file.seekg(sizeof(Alumno), ios::cur);
            int new_free;
            file.read((char*)&new_free, sizeof(free_slot));
            file.seekg(p2, ios::beg);
            file.write(record.codigo, sizeof(Alumno::codigo));
            file.write(record.nombre, sizeof(Alumno::nombre));
            file.write(record.apellidos, sizeof(Alumno::apellidos));
            file.write(record.carrera, sizeof(Alumno::carrera));
            file.write((char*)&record.ciclo, sizeof(Alumno::ciclo));
            file.write((char*)&record.mensualidad, sizeof(Alumno::mensualidad));
            free_slot trash = TRASH;
            file.write((char*)&trash, sizeof(free_slot));
            file.seekg(p, ios::beg);
            file.write((char*)&new_free, sizeof(free_slot));
        }

        
        // file << flush; 
    }

    void print_debug() {
        file.clear();
        file.seekg(0, ios::beg);
        free_slot delete_status;
        while (true) {
            SKIP_END_LINE_LINUX(file);
            if (file.eof()) break; 
            Alumno alumno{};
            file.read(alumno.codigo, sizeof(Alumno::codigo));
            file.read(alumno.nombre, sizeof(Alumno::nombre));
            file.read(alumno.apellidos, sizeof(Alumno::apellidos));
            file.read(alumno.carrera, sizeof(Alumno::carrera));
            file.read((char *) &alumno.ciclo, sizeof(Alumno::ciclo));
            file.read((char *) &alumno.mensualidad, sizeof(Alumno::mensualidad));
            file.read((char *) &delete_status, sizeof(free_slot));
            cout << alumno.codigo << " " << alumno.nombre << " " << alumno.apellidos << " " << alumno.carrera << " " << alumno.ciclo << " " << alumno.mensualidad << " " << delete_status << endl;
        }
    }
};

int main() {
    FixedRecord fixedRecord("datos.txt");
    fixedRecord.add(Alumno{"1234", "Juan", "Perez", "Ingenieria", 1, 331.41});
    fixedRecord.add(Alumno{"1235", "Pedro", "Simpson", "Psicologia", 4, 33.41});
    fixedRecord.add(Alumno{"1236", "Maria", "Lopez", "Ingenieria", 2, 321.41});
    fixedRecord.add(Alumno{"1237", "Luis", "Berrospi", "Cocina", 2, 45.41});
    vector<Alumno> vec = fixedRecord.load();
    cout << "\nSe agregan registros:\n";
    for (auto &alumno : vec) {
        cout << alumno.codigo << " " << alumno.nombre << " " << alumno.apellidos << " " << alumno.carrera << " " << alumno.ciclo << " " << alumno.mensualidad << endl;
    }
    fixedRecord.deleteRecord(0);
    fixedRecord.deleteRecord(2);
    cout << "\nSe borran registros:\n";
    vec = fixedRecord.load();
    for (auto &alumno : vec) {
        cout << alumno.codigo << " " << alumno.nombre << " " << alumno.apellidos << " " << alumno.carrera << " " << alumno.ciclo << " " << alumno.mensualidad << endl;
    }
    fixedRecord.add(Alumno{"1237", "Mario", "Risas", "Arte", 10, 41.41});
    cout << "\nSe vuelven a agregar registros:\n";
    vec = fixedRecord.load();
    for (auto &alumno : vec) {
        cout << alumno.codigo << " " << alumno.nombre << " " << alumno.apellidos << " " << alumno.carrera << " " << alumno.ciclo << " " << alumno.mensualidad << endl;
    }

   
    return 0;
};
