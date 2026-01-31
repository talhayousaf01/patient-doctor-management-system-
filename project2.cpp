/*

Talha yousaf
NOTE ABOUT CSV HEADERS:
The CSV files are edited using Microsoft Excel.
Excel automatically keeps a header row when saving CSV files.
Therefore, the program explicitly skips the first line of each CSV
file to ignore the header. This behavior is intentional and correct.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class User {
public:
    int login_id;
    string password;
    string first_name;
    string last_name;

    User() {}
    User(int id, string pw, string fn, string ln)
        : login_id(id), password(pw), first_name(fn), last_name(ln) {}
};

class Physician : public User {
public:
    string title;
    string expertise;

    Physician(int id, string pw, string fn, string ln, string t, string e)
        : User(id, pw, fn, ln), title(t), expertise(e) {}

    void print() {
        cout << login_id << " | " << first_name << " " << last_name
            << " | " << title << " | " << expertise << endl;
    }
};

class Patient : public User {
public:
    double weight;
    double height;
    string blood_pressure;
    int physician_id;

    Patient(int id, string pw, string fn, string ln, double w, double h, string bp, int pid)
        : User(id, pw, fn, ln), weight(w), height(h), blood_pressure(bp), physician_id(pid) {}

    void print() {
        cout << login_id << " | " << first_name << " " << last_name
            << " | Weight: " << weight
            << " | Height: " << height
            << " | BP: " << blood_pressure
            << " | PhysicianID: " << physician_id << endl;
    }
};

class HospitalSystem {
private:
    vector<Physician> physicians;
    vector<Patient> patients;

public:
    void run() {
        loadPhysicians();
        loadPatients();

        cout << "=== Hospital System ===" << endl;

        int id;
        string pw;

        cout << "Login ID: ";
        cin >> id;
        cout << "Password: ";
        cin >> pw;

        // Try physician login
        for (auto& doc : physicians) {
            if (doc.login_id == id && doc.password == pw) {
                physicianMenu(doc);
                savePatients();
                return;
            }
        }

        // Try patient login
        for (auto& pt : patients) {
            if (pt.login_id == id && pt.password == pw) {
                patientMenu(pt);
                savePatients();
                return;
            }
        }

        cout << "Invalid login." << endl;
    }

    // CSV LOADING 
    void loadPhysicians() {
        ifstream file("C:\\Users\\talha\\source\\repos\\Project2\\x64\\Debug\\Physician.csv");
        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> data(6);
            for (int i = 0; i < 6; i++) getline(ss, data[i], ',');

            physicians.emplace_back(
                stoi(data[0]), data[1], data[2], data[3], data[4], data[5]
            );
        }
        cout << "Loaded physicians count = " << physicians.size() << endl;

    }

    void loadPatients() {
        ifstream file("C:\\Users\\talha\\source\\repos\\Project2\\x64\\Debug\\Patient.csv"
        );
        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> data(8);
            for (int i = 0; i < 8; i++) getline(ss, data[i], ',');

            patients.emplace_back(
                stoi(data[0]), data[1], data[2], data[3],
                stod(data[4]), stod(data[5]), data[6], stoi(data[7])
            );
        }
        cout << "Loaded patients count = " << patients.size() << endl;

    }

    void savePatients() {
        ofstream file("C:\\Users\\talha\\source\\repos\\Project2\\x64\\Debug\\Patient.csv"
        );
        file << "login_id,password,first_name,last_name,weight,height,blood_pressure,physician_id\n";

        for (auto& p : patients) {
            file << p.login_id << "," << p.password << ","
                << p.first_name << "," << p.last_name << ","
                << p.weight << "," << p.height << ","
                << p.blood_pressure << "," << p.physician_id << "\n";
        }
    }

    //  PATIENT MENU 
    void patientMenu(Patient& pt) {
        cout << "\nWelcome, " << pt.first_name << "!\n";
        cout << "Your current physician ID: " << pt.physician_id << endl;

        cout << "\nAvailable Physicians:\n";
        for (auto& doc : physicians) doc.print();

        cout << "Enter new physician ID: ";
        int newId;
        cin >> newId;

        pt.physician_id = newId;
        cout << "Physician updated successfully!\n";
    }

    //  PHYSICIAN MENU 
    void physicianMenu(Physician& doc) {
        cout << "\nWelcome Dr. " << doc.last_name << "!\n";

        cout << "\nYour Patients:\n";
        vector<Patient*> myPatients;
        for (auto& p : patients)
            if (p.physician_id == doc.login_id)
                myPatients.push_back(&p);

        if (myPatients.empty()) {
            cout << "You have no patients assigned.\n";
            return;
        }

        for (auto p : myPatients) p->print();

        cout << "\nEnter patient ID to update: ";
        int pid;
        cin >> pid;

        Patient* target = nullptr;
        for (auto p : myPatients) {
            if (p->login_id == pid) target = p;
        }

        if (!target) {
            cout << "Invalid patient ID.\n";
            return;
        }

        cout << "New weight: ";
        cin >> target->weight;
        cout << "New height: ";
        cin >> target->height;
        cout << "New blood pressure: ";
        cin >> target->blood_pressure;

        cout << "Patient data updated.\n";
    }
};

int main() {
    HospitalSystem sys;
    sys.run();
    return 0;
}