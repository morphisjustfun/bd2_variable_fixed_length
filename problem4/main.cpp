#include <bits/stdc++.h>
#include <filesystem>

using namespace std;

struct Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;
};

struct Metadata {
    int recordPos;
    int recordSize;
};

class VariableRecord {
    fstream file;
    fstream metadata;
public:
    explicit VariableRecord(const string &filename, const string &metadataFileName) {
        file.open(filename, ios::in | ios::out | ios::binary);
        metadata.open(metadataFileName, ios::in | ios::out | ios::binary);
    }

    void add(const Matricula &record) {
        file.seekp(0, ios::end);
        int codigoLength = record.codigo.length();
        int observacionesLength = record.observaciones.length();

        // pack codigo = codigoLength + codigo
        file.write((char *) &codigoLength, sizeof(codigoLength));
        file.write(record.codigo.c_str(), codigoLength);

        // pack ciclo = ciclo
        file.write((char *) &record.ciclo, sizeof(record.ciclo));

        // pack mensualidad = mensualidad
        file.write((char *) &record.mensualidad, sizeof(record.mensualidad));

        // pack observaciones = observacionesLength + observaciones
        file.write((char *) &observacionesLength, sizeof(observacionesLength));
        file.write(record.observaciones.c_str(), observacionesLength);


        // back sizeof(Metadata) bytes from end
        metadata.seekp(0, ios::end);

        // write metadata
        // recordPos = current file position
        // recordSize = sizeof(codigoLength) + codigoLength + sizeof(record.ciclo) + sizeof(record.mensualidad) + sizeof(observacionesLength) + observacionesLength
        int recordSize = sizeof(codigoLength) + codigoLength + sizeof(record.ciclo) + sizeof(record.mensualidad) + sizeof(observacionesLength) + observacionesLength;
        int recordPos = file.tellp() - recordSize;
        metadata.write((char *) &recordPos, sizeof(recordPos));
        metadata.write((char *) &recordSize, sizeof(recordSize));
    }

    vector<Matricula> load() {
        vector<Matricula> records;
        file.seekg(0, ios::beg);

        while (true) {
            Matricula record;
            int codigoLength;
            int observacionesLength;

            // unpack codigo = codigoLength + codigo
            file.read((char *) &codigoLength, sizeof(codigoLength));
            char codigo[codigoLength];
            file.read(codigo, codigoLength);
            record.codigo = string(codigo, codigoLength);

            // unpack ciclo = ciclo
            file.read((char *) &record.ciclo, sizeof(record.ciclo));

            // unpack mensualidad = mensualidad
            file.read((char *) &record.mensualidad, sizeof(record.mensualidad));

            // unpack observaciones = observacionesLength + observaciones
            file.read((char *) &observacionesLength, sizeof(observacionesLength));
            char observaciones[observacionesLength];
            file.read(observaciones, observacionesLength);
            record.observaciones = string(observaciones, observacionesLength);

            records.push_back(record);

            if (file.peek() == EOF) {
                break;
            }
        }
        return records;
    }

    Matricula readRecord(int recordPos) {
        file.clear();
        file.seekg(0, ios::beg);
        // get the recordpos and recordsize from metadata
        metadata.seekg(sizeof(Metadata) * recordPos, ios::beg);

        Metadata metadataRecord;
        metadata.read((char *) &metadataRecord.recordPos, sizeof(metadataRecord.recordPos));
        metadata.read((char *) &metadataRecord.recordSize, sizeof(metadataRecord.recordSize));

        // read the record from file
        file.seekg(metadataRecord.recordPos, ios::beg);
        Matricula record;
        int codigoLength;
        // unpack codigo = codigoLength + codigo
        file.read((char *) &codigoLength, sizeof(codigoLength));
        char codigo[codigoLength];
        file.read(codigo, codigoLength);
        record.codigo = string(codigo, codigoLength);
        // unpack ciclo = ciclo
        file.read((char *) &record.ciclo, sizeof(record.ciclo));
        // unpack mensualidad = mensualidad
        file.read((char *) &record.mensualidad, sizeof(record.mensualidad));
        // unpack observaciones = observacionesLength + observaciones
        int observacionesLength;
        file.read((char *) &observacionesLength, sizeof(observacionesLength));
        char observaciones[observacionesLength];
        file.read(observaciones, observacionesLength);
        record.observaciones = string(observaciones, observacionesLength);
        return record;
    }

    ~VariableRecord() {
        file.close();
        metadata.close();
    }
};


int main() {
    VariableRecord variableRecord("datos.txt", "metadata.txt");
    Matricula matricula1{
            "2019-1-12345",
            2019,
            1000.0,
            "Sin observaciones"
    };

    Matricula matricula2{
            "2019-1-12346",
            2019,
            1000.0,
            "Probando probnado"
    };

    Matricula matricula3{
            "3019-1-12347",
            2893,
            1510.0,
            "Ni idea"
    };

    variableRecord.add(matricula1);
    variableRecord.add(matricula2);
    variableRecord.add(matricula3);

    vector<Matricula> records = variableRecord.load();
    cout << "Records size" << endl;
    cout << records.size() << endl;
    cout << "Print all records" << endl;
    for (auto &record : records) {
        cout << record.codigo << " " << record.ciclo << " " << record.mensualidad << " " << record.observaciones << endl;
    }
    cout << endl;

    Matricula record2 = variableRecord.readRecord(2);
    cout << "Print record 2" << endl;
    cout << record2.codigo << " " << record2.ciclo << " " << record2.mensualidad << " " << record2.observaciones << endl;

    Matricula record0 = variableRecord.readRecord(0);
    cout << "Print record 0" << endl;
    cout << record0.codigo << " " << record0.ciclo << " " << record0.mensualidad << " " << record0.observaciones << endl;
    return 0;
}
