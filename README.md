# Electrical Load Monitoring and Billing System

This project is a simple Electrical Load Monitoring and Billing System developed using C++. The program is used to register electrical appliances, monitor their energy consumption, and estimate electricity billing cost based on daily usage.

When the program starts, it automatically loads previously saved appliance records from a file. Users can then add new appliances, view stored appliances, search for a particular appliance, and calculate electricity bills using a given tariff rate.

The system works through a menu-driven interface with the following options:

1. Register appliance  
2. View appliances  
3. Search appliance  
4. Billing  
5. Save to file  
6. Exit  

Each appliance entered includes the appliance name, power rating in watts, and number of hours used per day. The program calculates energy consumption using the formula:

kWh/day = (Watts ÷ 1000) × Hours used per day

Electricity cost is calculated using:

Cost = Energy (kWh) × Tariff per kWh

The system also estimates electricity usage and cost for 30 days to give a monthly billing estimate.

All appliance records are stored in **appliances.txt**, while calculated billing summaries can be saved in **billing_summary.txt** for future reference. Input validation is included to ensure that appliance names are not empty, watts are greater than zero, usage hours are between 0 and 24, and tariff values are valid.

To compile the program, use:

g++ main.cpp -o billing_system

To run the program:

Linux or Mac:
./billing_system

Windows:
billing_system.exe

This project demonstrates the use of C++ structures, file handling, input validation, and menu-driven programming. It is mainly designed for educational purposes and basic electricity consumption estimation.

Author: Owusu