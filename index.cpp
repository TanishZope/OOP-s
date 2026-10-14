#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdexcept>
using namespace std;

// Class Template
template <class T>
class RecordBox {
private:
    T value;

public:
    RecordBox(T v) : value(v) {}

    T getValue() const {
        return value;
    }
};

// Base Class
class Person {
protected:
    int patientId;
    string patientName;
    int age;

public:
    Person() : patientId(0), patientName(""), age(0) {}

    Person(int id, string name, int a)
        : patientId(id), patientName(name), age(a) {
        if (a <= 0 || a > 120)
            throw invalid_argument("Invalid age.");
    }

    virtual void display() const {
        cout << "Patient ID: " << patientId
             << "\nPatient Name: " << patientName
             << "\nAge: " << age << '\n';
    }

    int getPatientId() const {
        return patientId;
    }

    string getPatientName() const {
        return patientName;
    }

    int getAge() const {
        return age;
    }

    virtual ~Person() {}
};

// Inheritance and Polymorphism
class Appointment : public Person {
private:
    string doctorName;
    string date;
    string time;
    bool active;

public:
    Appointment()
        : Person(), doctorName(""), date(""),
          time(""), active(true) {}

    Appointment(int id, string name, int a,
                string doctor, string d, string t)
        : Person(id, name, a), doctorName(doctor),
          date(d), time(t), active(true) {
        if (doctor.empty() || d.empty() || t.empty())
            throw invalid_argument("Enter all appointment details.");
    }

    void display() const override {
        Person::display();

        cout << "Doctor: " << doctorName
             << "\nDate: " << date
             << "\nTime: " << time
             << "\nStatus: "
             << (active ? "Booked" : "Cancelled")
             << '\n';
    }

    string getDoctorName() const { return doctorName; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    bool isActive() const { return active; }

    void cancel() {
        active = false;
    }
};

// Operator Overloading
class AppointmentCompare {
private:
    int id;

public:
    AppointmentCompare() : id(0) {}

    AppointmentCompare(int value) : id(value) {}

    bool operator==(const AppointmentCompare& other) const {
        return id == other.id;
    }
};

// Main Hospital Class
class Hospital {
private:
    map<int, Appointment> appointments;
    int nextId;

public:
    Hospital() : nextId(1) {}

    // Add Appointment
    void addAppointment() {
        string name, doctor, date, time;
        int age;

        cout << "Enter patient name: ";
        cin >> ws;
        getline(cin, name);

        cout << "Enter patient age: ";
        if (!(cin >> age)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw invalid_argument("Enter a valid age.");
        }

        cout << "Enter doctor name: ";
        cin >> ws;
        getline(cin, doctor);

        cout << "Enter date (DD-MM-YYYY): ";
        getline(cin, date);

        cout << "Enter appointment time: ";
        getline(cin, time);

        Appointment appt(
            nextId, name, age, doctor, date, time
        );

        appointments.emplace(nextId, appt);

        RecordBox<int> box(nextId);

        cout << "Appointment booked successfully!"
             << "\nAppointment ID: "
             << box.getValue() << '\n';

        saveAppointment(appt);
        nextId++;
    }

    // File Handling
    void saveAppointment(const Appointment& appt) {
        ofstream file("appointments.txt", ios::app);

        if (!file)
            throw runtime_error("Unable to open file.");

        file << "ID: " << appt.getPatientId()
             << " | Patient: " << appt.getPatientName()
             << " | Age: " << appt.getAge()
             << " | Doctor: " << appt.getDoctorName()
             << " | Date: " << appt.getDate()
             << " | Time: " << appt.getTime()
             << " | Status: Booked\n";

        file.close();
    }

    // Display Appointments
    void displayAppointments() const {
        if (appointments.empty()) {
            cout << "No appointments found.\n";
            return;
        }

        cout << "\n===== APPOINTMENT LIST =====\n";

        for (const auto& item : appointments) {
            const Person* ptr = &item.second;
            ptr->display();

            cout << "----------------------\n";
        }
    }

    // Search Appointment
    void searchAppointment() const {
        int id;

        cout << "Enter appointment ID: ";

        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw invalid_argument("Enter a valid ID.");
        }

        auto found = appointments.find(id);

        if (found == appointments.end()) {
            cout << "Appointment not found.\n";
            return;
        }

        AppointmentCompare a(id);
        AppointmentCompare b(found->second.getPatientId());

        if (a == b) {
            cout << "\nAppointment found:\n";
            found->second.display();
        }
    }

    // Cancel Appointment
    void cancelAppointment() {
        int id;

        cout << "Enter appointment ID to cancel: ";

        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw invalid_argument("Enter a valid ID.");
        }

        auto found = appointments.find(id);

        if (found == appointments.end()) {
            cout << "Appointment not found.\n";
        }
        else if (!found->second.isActive()) {
            cout << "Appointment already cancelled.\n";
        }
        else {
            found->second.cancel();
            cout << "Appointment cancelled successfully.\n";
            cout << "Cancellation is stored for this session only.\n";
        }
    }

    // Read Saved Records
    void readSavedRecords() const {
        ifstream file("appointments.txt");

        if (!file) {
            cout << "No saved records found.\n";
            return;
        }

        string line;

        cout << "\n===== SAVED APPOINTMENTS =====\n";

        while (getline(file, line)) {
            cout << line << '\n';
        }

        file.close();
    }

    // Main Menu
    void menu() {
        int choice;

        do {
            cout << "\n===== HOSPITAL APPOINTMENT SYSTEM =====\n";
            cout << "1. Add Appointment\n";
            cout << "2. Display Appointments\n";
            cout << "3. Search Appointment\n";
            cout << "4. Cancel Appointment\n";
            cout << "5. Read Saved Records\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Try again.\n";
                continue;
            }

            try {
                switch (choice) {
                    case 1:
                        addAppointment();
                        break;

                    case 2:
                        displayAppointments();
                        break;

                    case 3:
                        searchAppointment();
                        break;

                    case 4:
                        cancelAppointment();
                        break;

                    case 5:
                        readSavedRecords();
                        break;

                    case 0:
                        cout << "Thank you for using the system.\n";
                        break;

                    default:
                        cout << "Invalid choice.\n";
                }
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << '\n';
            }

        } while (choice != 0);
    }
};

int main() {
    Hospital hospital;
    hospital.menu();

    return 0;
}
