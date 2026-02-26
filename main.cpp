#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

string readLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (!s.empty()) return s;
        cout << "Input must not be empty.\n";
    }
}

double kwhDay(const Appliance& a) { return (a.watts / 1000.0) * a.hours; }

double totalDayKwh(const Appliance arr[], int count) {
    double t = 0.0;
    for (int i = 0; i < count; i++) t += kwhDay(arr[i]);
    return t;
}

void saveAppliances(const Appliance arr[], int count) {
    ofstream out(APPLIANCES_FILE.c_str());
    if (!out.is_open()) { cout << "Error writing " << APPLIANCES_FILE << "\n"; return; }
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

void appendBilling(double tariff, int count, double dayKwh, double dayCost, double monthKwh, double monthCost) {
    ofstream out(BILLING_FILE.c_str(), ios::app);
    if (!out.is_open()) { cout << "Error appending " << BILLING_FILE << "\n"; return; }

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

void menu() {
    cout << "\n1. Register appliance\n";
    cout << "2. View appliances\n";
    cout << "3. Search appliance\n";
    cout << "4. Billing\n";
    cout << "5. Save to file\n";
    cout << "6. Exit\n";
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;
    loadAppliances(appliances, count);

    cout << "Electrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << count << "\n";

    while (true) {
        menu();
        int opt = readInt("Choose (1-6): ");

        if (opt == 1) {
            if (count >= MAX_APPLIANCES) { cout << "Limit reached.\n"; continue; }
            Appliance a;
            a.name = readLine("Name: ");
            do { a.watts = readDouble("Watts (>0): "); } while (a.watts <= 0);
            do { a.hours = readDouble("Hours/day (0-24): "); } while (a.hours < 0 || a.hours > 24);
            appliances[count++] = a;
            saveAppliances(appliances, count);
            cout << "Saved.\n";
        }
        else if (opt == 2) {
            if (count == 0) { cout << "No appliances.\n"; continue; }
            cout << fixed << setprecision(2);
            cout << left << setw(4) << "#" << setw(25) << "Name" << setw(10) << "Watts"
                 << setw(10) << "Hours" << setw(10) << "kWh/day" << "\n";
            for (int i = 0; i < count; i++) {
                cout << left << setw(4) << (i + 1) << setw(25) << appliances[i].name
                     << setw(10) << appliances[i].watts << setw(10) << appliances[i].hours
                     << setw(10) << kwhDay(appliances[i]) << "\n";
            }
        }
        else if (opt == 3) {
            if (count == 0) { cout << "No appliances.\n"; continue; }
            string q = lowerStr(readLine("Search name: "));
            bool found = false;
            cout << fixed << setprecision(2);
            for (int i = 0; i < count; i++) {
                if (lowerStr(appliances[i].name).find(q) != string::npos) {
                    cout << "- " << appliances[i].name << " | " << appliances[i].watts
                         << " W | " << appliances[i].hours << " hrs | "
                         << kwhDay(appliances[i]) << " kWh/day\n";
                    found = true;
                }
            }
            if (!found) cout << "No match.\n";
        }
        else if (opt == 4) {
            if (count == 0) { cout << "No appliances. Add some first.\n"; continue; }
            double tariff;
            do { tariff = readDouble("Tariff per kWh (>0): "); } while (tariff <= 0);

            double dayKwh = totalDayKwh(appliances, count);
            double dayCost = dayKwh * tariff;
            double monthKwh = dayKwh * 30.0;
            double monthCost = dayCost * 30.0;

            cout << fixed << setprecision(2);
            cout << "\nTariff: " << tariff << " per kWh\n";
            cout << "Daily energy: " << dayKwh << " kWh\n";
            cout << "Daily cost:   " << dayCost << "\n";
            cout << "30-day energy: " << monthKwh << " kWh\n";
            cout << "30-day cost:   " << monthCost << "\n";

            cout << "Save summary to billing_summary.txt? (y/n): ";
            char ch; cin >> ch; clearLine();
            if (ch == 'y' || ch == 'Y') {
                appendBilling(tariff, count, dayKwh, dayCost, monthKwh, monthCost);
                cout << "Billing summary saved.\n";
            } else cout << "Not saved.\n";
        }
        else if (opt == 5) {
            saveAppliances(appliances, count);
            cout << "Saved to " << APPLIANCES_FILE << ".\n";
        }
        else if (opt == 6) {
            saveAppliances(appliances, count);
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}