#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";
const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;

    double dailyKwh() const { return (watts / 1000.0) * hours; }
};

// ---- simple helpers ----
void clearLine() {
    cin.clear();
    cin.ignore(10000, '\n');
}

string trim(string s) {
    while (!s.empty() && (s[0] == ' ' || s[0] == '\t' || s[0] == '\r' || s[0] == '\n')) s.erase(0, 1);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) s.pop_back();
    return s;
}

string lowerStr(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = char(s[i] - 'A' + 'a');
    }
    return s;
}

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) { clearLine(); return x; }
        cout << "Invalid number. Try again.\n";
        clearLine();
    }
}

double readDouble(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) { clearLine(); return x; }
        cout << "Invalid number. Try again.\n";
        clearLine();
    }
}

string readNonEmptyLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (!s.empty()) return s;
        cout << "Input must not be empty.\n";
    }
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        double v = readDouble(prompt);
        if (v > 0) return v;
        cout << "Value must be greater than 0.\n";
    }
}

double readHours(const string& prompt) {
    while (true) {
        double h = readDouble(prompt);
        if (h >= 0 && h <= 24) return h;
        cout << "Hours must be between 0 and 24.\n";
    }
}

// ---- file ops ----
void saveAppliances(const Appliance arr[], int count) {
    ofstream out(APPLIANCES_FILE.c_str());
    if (!out.is_open()) { cout << "Error: could not write " << APPLIANCES_FILE << "\n"; return; }
    for (int i = 0; i < count; i++)
        out << arr[i].name << "|" << arr[i].watts << "|" << arr[i].hours << "\n";
    out.close();
}

void loadAppliances(Appliance arr[], int& count) {
    count = 0;
    ifstream in(APPLIANCES_FILE.c_str());
    if (!in.is_open()) return;

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

// ---- calculations ----
double totalDailyKwh(const Appliance arr[], int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) total += arr[i].dailyKwh();
    return total;
}

// ---- actions ----
void registerAppliance(Appliance arr[], int& count) {
    if (count >= MAX_APPLIANCES) { cout << "Limit reached.\n"; return; }

    Appliance a;
    a.name  = readNonEmptyLine("Appliance name: ");
    a.watts = readPositiveDouble("Power rating (watts > 0): ");
    a.hours = readHours("Daily usage hours (0 - 24): ");

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

void searchAppliance(const Appliance arr[], int count) {
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }

    string q = lowerStr(readNonEmptyLine("Enter name to search: "));
    bool found = false;

    cout << fixed << setprecision(2);
    for (int i = 0; i < count; i++) {
        if (lowerStr(arr[i].name).find(q) != string::npos) {
            cout << "- " << arr[i].name
                 << " | " << arr[i].watts << " W"
                 << " | " << arr[i].hours << " hrs/day"
                 << " | " << arr[i].dailyKwh() << " kWh/day\n";
            found = true;
        }
    }
    if (!found) cout << "No match found.\n";
}

void showTotalLoad(const Appliance arr[], int count) {
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }
    cout << fixed << setprecision(2);
    cout << "Total daily load = " << totalDailyKwh(arr, count) << " kWh\n";
}

void appendBillingSummary(double tariff, int count, double dayKwh, double dayCost, double monthKwh, double monthCost) {
    ofstream out(BILLING_FILE.c_str(), ios::app);
    if (!out.is_open()) { cout << "Error: could not append " << BILLING_FILE << "\n"; return; }

    out << "================ BILLING SUMMARY ================\n";
    out << fixed << setprecision(2);
    out << "Tariff: " << tariff << " per kWh\n";
    out << "Appliances count: " << count << "\n";
    out << "Total daily energy: " << dayKwh << " kWh\n";
    out << "Total daily cost:  " << dayCost << "\n";
    out << "Estimated 30-day energy: " << monthKwh << " kWh\n";
    out << "Estimated 30-day cost:  " << monthCost << "\n";
    out << "=================================================\n\n";
    out.close();
}

void billing(const Appliance arr[], int count) {
    if (count == 0) { cout << "No appliances. Register first.\n"; return; }

    double tariff = readPositiveDouble("Enter electricity tariff per kWh (positive): ");

    double dayKwh    = totalDailyKwh(arr, count);
    double dayCost   = dayKwh * tariff;
    double monthKwh  = dayKwh * 30.0;
    double monthCost = dayCost * 30.0;

    cout << fixed << setprecision(2);
    cout << "\n--- Billing Summary ---\n";
    cout << "Tariff: " << tariff << " per kWh\n";
    cout << "Total daily energy: " << dayKwh << " kWh\n";
    cout << "Total daily cost:   " << dayCost << "\n";
    cout << "30-day energy:      " << monthKwh << " kWh\n";
    cout << "30-day cost:        " << monthCost << "\n";

    cout << "Save summary to billing_summary.txt? (y/n): ";
    char ch; cin >> ch; clearLine();

    if (ch == 'y' || ch == 'Y') {
        appendBillingSummary(tariff, count, dayKwh, dayCost, monthKwh, monthCost);
        cout << "Billing summary saved.\n";
    } else {
        cout << "Not saved.\n";
    }
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
        cout << "3. Search appliance by name\n";
        cout << "4. Show total daily load (kWh)\n";
        cout << "5. Billing calculation\n";
        cout << "6. Save appliances to file\n";
        cout << "7. Exit\n";

        int choice = readInt("Choose: ");

        if (choice == 1) registerAppliance(appliances, count);
        else if (choice == 2) viewAppliances(appliances, count);
        else if (choice == 3) searchAppliance(appliances, count);
        else if (choice == 4) showTotalLoad(appliances, count);
        else if (choice == 5) billing(appliances, count);
        else if (choice == 6) { saveAppliances(appliances, count); cout << "Saved.\n"; }
        else if (choice == 7) { saveAppliances(appliances, count); cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }

    return 0;
}
