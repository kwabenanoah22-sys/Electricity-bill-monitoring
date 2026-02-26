#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";
const int MAX_APPLIANCES     = 100;

struct Appliance {
    string name;
    double watts;
    double hours;
};

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

// ---- calculations ----
double dailyKwh(const Appliance& a) { return (a.watts / 1000.0) * a.hours; }

double totalDailyKwh(const Appliance arr[], int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) total += dailyKwh(arr[i]);
    return total;
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

// ---- UI helpers ----
void title(const string& t) {
    cout << "\n============================================\n";
    cout << t << "\n";
    cout << "============================================\n";
}

void showMenu() {
    cout << "\n==================== MAIN MENU ====================\n";
    cout << "1. Register electrical appliance\n";
    cout << "2. View all registered appliances\n";
    cout << "3. Search appliance by name\n";
    cout << "4. Show total daily load (kWh)\n";
    cout << "5. Billing calculation\n";
    cout << "6. Save appliances to file\n";
    cout << "7. Exit\n";
    cout << "===================================================\n";
}

// ---- actions ----
void addAppliance(Appliance arr[], int& count) {
    title("Register Appliance");
    if (count >= MAX_APPLIANCES) { cout << "Limit reached.\n"; return; }

    Appliance a;
    a.name  = readNonEmptyLine("Appliance name: ");
    a.watts = readPositiveDouble("Power rating (watts > 0): ");
    a.hours = readHours("Daily usage hours (0 - 24): ");

    arr[count++] = a;
    saveAppliances(arr, count);
    cout << "Appliance saved.\n";
}

void listAppliances(const Appliance arr[], int count) {
    title("All Registered Appliances");
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }

    cout << fixed << setprecision(2);
    cout << left << setw(4) << "#" << setw(25) << "Name"
         << setw(12) << "Watts" << setw(12) << "Hours/day" << setw(12) << "kWh/day" << "\n";
    cout << "--------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << left << setw(4) << (i + 1)
             << setw(25) << arr[i].name
             << setw(12) << arr[i].watts
             << setw(12) << arr[i].hours
             << setw(12) << dailyKwh(arr[i]) << "\n";
    }
}

void searchAppliance(const Appliance arr[], int count) {
    title("Search Appliance");
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }

    string q = lowerStr(readNonEmptyLine("Enter name to search: "));
    bool found = false;

    cout << fixed << setprecision(2);
    for (int i = 0; i < count; i++) {
        if (lowerStr(arr[i].name).find(q) != string::npos) {
            cout << "- " << arr[i].name
                 << " | " << arr[i].watts << " W"
                 << " | " << arr[i].hours << " hrs/day"
                 << " | " << dailyKwh(arr[i]) << " kWh/day\n";
            found = true;
        }
    }
    if (!found) cout << "No match found.\n";
}

void showLoad(const Appliance arr[], int count) {
    title("Load Summary");
    if (count == 0) { cout << "No appliances registered yet.\n"; return; }
    cout << fixed << setprecision(2);
    cout << "Appliances count: " << count << "\n";
    cout << "Total daily energy: " << totalDailyKwh(arr, count) << " kWh\n";
}

void billing(const Appliance arr[], int count) {
    title("Billing Calculation");
    if (count == 0) { cout << "No appliances. Register first.\n"; return; }

    double tariff = readPositiveDouble("Enter electricity tariff per kWh (positive): ");

    double dayKwh    = totalDailyKwh(arr, count);
    double dayCost   = dayKwh * tariff;
    double monthKwh  = dayKwh * 30.0;
    double monthCost = dayCost * 30.0;

    cout << fixed << setprecision(2);
    cout << "\nTariff: " << tariff << " per kWh\n";
    cout << "Total daily energy: " << dayKwh << " kWh\n";
    cout << "Total daily cost:   " << dayCost << "\n";
    cout << "Estimated 30-day energy: " << monthKwh << " kWh\n";
    cout << "Estimated 30-day cost:   " << monthCost << "\n";

    cout << "\nSave this billing summary to billing_summary.txt? (y/n): ";
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
    cout << "Electrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << count << "\n";

    while (true) {
        showMenu();
        int option = readInt("Choose an option (1-7): ");

        if (option == 1) addAppliance(appliances, count);
        else if (option == 2) listAppliances(appliances, count);
        else if (option == 3) searchAppliance(appliances, count);
        else if (option == 4) showLoad(appliances, count);
        else if (option == 5) billing(appliances, count);
        else if (option == 6) { saveAppliances(appliances, count); cout << "Appliances saved.\n"; }
        else if (option == 7) { saveAppliances(appliances, count); cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }

    return 0;
}
