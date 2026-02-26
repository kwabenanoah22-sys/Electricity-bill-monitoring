#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct Appliance {
    string name;
    double watts;
    double hours;

    double dailyKwh() const {
        return (watts / 1000.0) * hours;
    }
};

int main() {
    Appliance a;

    cout << "Enter appliance name: ";
    getline(cin, a.name);

    cout << "Enter power rating (watts): ";
    cin >> a.watts;

    cout << "Enter daily usage hours: ";
    cin >> a.hours;

    cout << fixed << setprecision(2);
    cout << "\n--- Appliance Summary ---\n";
    cout << "Name: " << a.name << "\n";
    cout << "Watts: " << a.watts << "\n";
    cout << "Hours/day: " << a.hours << "\n";
    cout << "kWh/day: " << a.dailyKwh() << "\n";

    return 0;
}