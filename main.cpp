#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;

    double dailyKwh() const {
        return (watts / 1000.0) * hours;
    }
};

string trim(string s) {
    while (!s.empty() && (s[0] == ' ' || s[0] == '\t' || s[0] == '\r' || s[0] == '\n')) s.erase(0, 1);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) s.pop_back();
    return s;
}

void saveAppliances(const Appliance arr[], int count) {
    ofstream out(APPLIANCES_FILE.c_str());
    if (!out.is_open()) {
        cout << "Error: could not write " << APPLIANCES_FILE << "\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        out << arr[i].name << "|" << arr[i].watts << "|" << arr[i].hours << "\n";
    }
    out.close();
}

void loadAppliances(Appliance arr[], int& count) {
    count = 0;
    ifstream in(APPLIANCES_FILE.c_str());
    if (!in.is_open()) return; // first run ok

    string line;
    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        int p1 = (int)line.find('|');
        int p2 = (p1 == -1) ? -1 : (int)line.find('|', p1 + 1);
        if (p1 == -1 || p2 == -1) continue;

        string name = trim(line.substr(0, p1));
        string wStr = trim(line.substr(p1 + 1, p2 - p1 - 1));
        string hStr = trim(line.substr(p2 + 1));
        if (name.empty()) continue;

        double w, h;
        try { w = stod(wStr); h = stod(hStr); } catch (...) { continue; }
        if (w <= 0 || h < 0 || h > 24) continue;

        if (count < MAX_APPLIANCES) {
            arr[count].name = name;
            arr[count].watts = w;
            arr[count].hours = h;
            count++;
        }
    }
    in.close();
}

double totalDailyKwh(const Appliance arr[], int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) total += arr[i].dailyKwh();
    return total;
}

void registerAppliance(Appliance arr[], int& count) {
    if (count >= MAX_APPLIANCES) { cout << "Limit reached.\n"; return; }

    Appliance a;
    cout << "Enter appliance name: ";
    getline(cin, a.name);

    cout << "Enter power rating (watts): ";
    cin >> a.watts;

    cout << "Enter daily usage hours: ";
    cin >> a.hours;

    cin.ignore(10000, '\n'); // clear newline

    arr[count++] = a;
    saveAppliances(arr, count);
    cout << "Appliance added and saved.\n";
}

void viewAppliances(const Appliance arr[], int count) {
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }

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

void showTotalLoad(const Appliance arr[], int count) {
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }
    cout << fixed << setprecision(2);
    cout << "Total daily load = " << totalDailyKwh(arr, count) << " kWh\n";
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    loadAppliances(appliances, count);
    cout << "Loaded appliances: " << count << "\n";

    while (true) {
        cout << "\n=== MENU ===\n";
        cout << "1. Register appliance\n";
        cout << "2. View appliances\n";
        cout << "3. Show total daily load (kWh)\n";
        cout << "4. Save appliances to file\n";
        cout << "5. Exit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) registerAppliance(appliances, count);
        else if (choice == 2) viewAppliances(appliances, count);
        else if (choice == 3) showTotalLoad(appliances, count);
        else if (choice == 4) { saveAppliances(appliances, count); cout << "Saved.\n"; }
        else if (choice == 5) { saveAppliances(appliances, count); cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}