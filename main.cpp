#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;

    double dailyKwh() const {
        return (watts / 1000.0) * hours;
    }
};

void registerAppliance(Appliance arr[], int& count) {
    if (count >= MAX_APPLIANCES) {
        cout << "Limit reached. Cannot add more.\n";
        return;
    }

    Appliance a;

    cout << "Enter appliance name: ";
    getline(cin, a.name);

    cout << "Enter power rating (watts): ";
    cin >> a.watts;

    cout << "Enter daily usage hours: ";
    cin >> a.hours;

    cin.ignore(10000, '\n'); // clear leftover newline

    arr[count] = a;
    count++;

    cout << "Appliance added.\n";
}

void viewAppliances(const Appliance arr[], int count) {
    if (count == 0) {
        cout << "No appliances registered yet.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n#   " << left << setw(20) << "Name"
         << setw(10) << "Watts"
         << setw(10) << "Hours"
         << setw(10) << "kWh/day" << "\n";
    cout << "---------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << setw(4) << (i + 1)
             << setw(20) << arr[i].name
             << setw(10) << arr[i].watts
             << setw(10) << arr[i].hours
             << setw(10) << arr[i].dailyKwh()
             << "\n";
    }
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Register appliance\n";
        cout << "2. View appliances\n";
        cout << "3. Exit\n";

        int choice;
        cout << "Choose: ";
        cin >> choice;
        cin.ignore(10000, '\n'); // clear newline

        if (choice == 1) {
            registerAppliance(appliances, count);
        } else if (choice == 2) {
            viewAppliances(appliances, count);
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
