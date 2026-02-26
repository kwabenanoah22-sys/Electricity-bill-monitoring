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
        cout << "Limit reached.\n";
        return;
    }

    Appliance a;
    cout << "Enter appliance name: ";
    getline(cin, a.name);

    cout << "Enter power rating (watts): ";
    cin >> a.watts;

    cout << "Enter daily usage hours: ";
    cin >> a.hours;

    cin.ignore(10000, '\n'); // clear newline
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
    cout << "\n#  Name                Watts      Hours      kWh/day\n";
    cout << "----------------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << (i + 1) << ". " << arr[i].name
             << " | " << arr[i].watts
             << " | " << arr[i].hours
             << " | " << arr[i].dailyKwh() << "\n";
    }
}

// ---- NEW in Part 3: total daily energy calculation ----
double totalDailyKwh(const Appliance arr[], int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += arr[i].dailyKwh();
    }
    return total;
}

void showTotalLoad(const Appliance arr[], int count) {
    if (count == 0) {
        cout << "No appliances registered yet.\n";
        return;
    }
    cout << fixed << setprecision(2);
    cout << "Total daily load = " << totalDailyKwh(arr, count) << " kWh\n";
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Register appliance\n";
        cout << "2. View appliances\n";
        cout << "3. Show total daily load (kWh)\n";
        cout << "4. Exit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) registerAppliance(appliances, count);
        else if (choice == 2) viewAppliances(appliances, count);
        else if (choice == 3) showTotalLoad(appliances, count);
        else if (choice == 4) { cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }

    return 0;
}