#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <set>
#include <cstdlib> 
#include <cmath>
#include <ctime> 
#include <algorithm> 
#include <limits>
#include <thread> // for std::this_thread::sleep_for
#include <chrono> // for std::chrono::seconds (Loading Screen)
#include <fstream>

using namespace std;

std::set<int> employeeIDs; // Declare a set to store unique employee IDs

void clearScreen() {
    system("cls");
}

// Function to get the current greeting based on the time of day
string getGreeting() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    int hour = ltm->tm_hour;
    if (hour < 12) {
        return "Good Morning";
    } else if (hour < 18) {
        return "Good Afternoon";
    } else {
        return "Good Evening";
    }
}

// Function to get the current timestamp
std::string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Format the date and time
    std::ostringstream dateTimeStream;
    dateTimeStream << (1900 + ltm->tm_year) << '-' 
                   << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << '-'
                   << std::setw(2) << std::setfill('0') << ltm->tm_mday << ' '
                   << ltm->tm_hour << ':'
                   << std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
                   << std::setw(2) << std::setfill('0') << ltm->tm_sec;

    
    return dateTimeStream.str();
}


bool addEmployee(int ids) {
    if (employeeIDs.find(ids) != employeeIDs.end()) {
        // ID already exists
        std::cout << "\t\t\t\t\t\t\t\t\t\tError: Employee ID " << ids << " is already taken. Please enter a unique ID." << std::endl;
          return false; // Indicate that the ID was a duplicate
    } else {
        // ID is unique, proceed with adding employee
        employeeIDs.insert(ids); // Add the new ID to the set
        return true; // Indicate successful addition
        // Additional code to handle other employee details
    }
}

void displayHeader() {
 std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
    cout << setw(115) << "=========================" << endl;
    cout << setw(110) << "PAYROLL COUNTER" << endl;
    cout << setw(115) << "=========================" << endl;
}

void displayTableHeader() {
    cout << setfill('-') << setw(237) << "-" << setfill(' ') << endl;
    cout << setw(40) << "GENERAL INFORMATION" 
         << setw(70) << "DAYS AND WORK HOURS" 
         << setw(57) << "DEDUCTIONS" 
         << setw(55) << "PAYROLL" << endl;
    cout << setfill('-') << setw(237) << "-" << setfill(' ') << endl;
    
    cout << setw(5) << "ID" 
         << setw(15) << "Name" 
         << setw(21.5) << "Position" 
         << setw(15) << "Rate-Hour" 
         << setw(13) << "Regular"
         << setw(15) << "Overtime" 
         << setw(14) << "L-Holiday"
         << setw(14) << "S-Holiday"
         << setw(11) << "Days"
         << setw(13) << "SSS" 
         << setw(22) << "Pag-IBIG"
         << setw(19) << "PhilHealth" 
         << setw(22) << "Total Deductions" 
         << setw(17) << "Holiday Pay" 
         << setw(14) << "Net Pay" << endl;
    cout << setfill('-') << setw(237) << "-" << setfill(' ') << endl;


}

void displayTableRow(int id, const string &name, const string &position, double rate, double hours, double Lhhours, double Shhours,  double overtime, double sss, double pagIbig, double philHealth, double totalDeductions, double netPay) {
    double holidayPay = Lhhours * (rate * 2) + Shhours * (rate * 0.30);
    double hhours = Lhhours + Shhours;
    double DaysWork = (hours+Lhhours+Shhours)/8;
    string pagIbigLabel = (rate <= 1500) ? " PHP(1%)" : " PHP(2%)";
    
	cout << setw(5) << id 
         << setw(25) << name 
         << setw(12) << position 
         << setw(10) << fixed << setprecision(2) << rate <<" PHP"
         << setw(10) << setprecision(2) << hours <<" Hrs"
         << setw(10) << setprecision(2) << overtime <<" Hrs"
         << setw(10) << setprecision(2) << Lhhours <<" Hrs"
         << setw(10) << setprecision(2) << Shhours <<" Hrs"
         << setw(7)  << setprecision(0) << round(DaysWork)  <<" Days"
         << setw(10) << setprecision(2) << sss <<" PHP(14%)"
         << setw(10) << setprecision(2) << pagIbig << pagIbigLabel
         << setw(10) << setprecision(2) << philHealth <<" PHP(5%)"
         << setw(13) << setprecision(2) << totalDeductions <<" PHP"
         << setw(16) << setprecision(2) << holidayPay << " PHP" 
         << setw(13) << setprecision(2) << netPay <<" PHP" << endl;
}

double calculateHolidayPay(double rate, double LegalholidayHours, double SpecialholidayHours) {
    return LegalholidayHours * (rate * 2) + SpecialholidayHours * (rate * 0.30); 
}

double calculateNetPay(double rate, double hours, double overtime, double deductions, double LegalholidayHours, double SpecialholidayHours) {
    double overtimePay = overtime * (rate * 1.5); // Overtime pay
    double holidayPay = calculateHolidayPay(rate, LegalholidayHours, SpecialholidayHours); // Holiday pay
    double grossPay = (rate * hours) + overtimePay + holidayPay; // Include holiday pay in gross
    return grossPay - deductions; // Calculate net pay
}

double adjustRateForPosition(const string &position, double baseRate) {
    if (position == "Regular") {
        return baseRate; // Regular Employee
    } else if (position == "Supervisor") {
        return baseRate * 1.15; // 15% increase for Supervisor
    } else if (position == "Manager") {
        return baseRate * 2.5; // 25% increase
    }
    return baseRate; // No increase for other positions
}


void generatePayrollReceipt(int id, const string &name, const string &position, double rate, double hours, double Lhhours, double Shhours,  double overtime, double sss, double pagIbig, double philHealth, double totalDeductions, double netPay) {
    double overtimePay = overtime * (rate * 1.5);
    double hPay = Lhhours * (rate * 2) + Shhours * (rate * 0.30);
    double grossPay = (rate * hours) + overtimePay;
    double DaysWork = (hours+Lhhours+Shhours)/8;
    string pagIbigLabel2 = (rate <= 1500) ? " Pag-IBIG Deduction (1%):" : " Pag-IBIG Deduction (2%):";
    
    // Create an ostringstream to capture the entire receipt output
    ostringstream receiptStream1, receiptStream2, receiptStream3, receiptStream4;
    
    receiptStream1 << name;
    string namC = receiptStream1.str();
    int CCnam = namC.size();
    
    receiptStream2 << position;
    string PosC = receiptStream2.str();
    int CCpos = PosC.size();
    
    receiptStream3 << id;
    string idC = receiptStream3.str();
    int CCid = idC.size();
    
    receiptStream4 << sss;
    string sC = receiptStream4.str();
    int CCs = sC.size();
    
    
    
    std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
    cout << setw(115) << "=========================" << endl;
    cout << setw(110) << "PAYROLL RECEIPT" << endl;
    cout << setw(115) << "=========================" << endl;
    
    cout << setw(132) << "\n"; 
    
    cout << setw(82) << "Name: " << name << setw(CCnam) << "Hours Worked:" << setw(10) << fixed << setprecision(2) << hours << "hrs" << endl; 
    cout << setw(82) << "Position: " << position << setw(CCnam+CCpos+CCid) << "Holidays:" << setw(10)<< endl;
	cout << setw(82) << "Employee ID: " << id << setw(CCnam+CCpos+CCpos) << "*Legal:" << setw(10) << Lhhours << "hrs" << endl;
	cout << setw(82) << "             " << setw(CCnam+CCpos+CCpos+CCid) << "*Special:" << setw(10) << Shhours << "hrs" << endl;
	cout << setw(82) << "             " << setw(CCnam+CCpos+CCpos+CCid) << "Days Present:" << setw(7) << fixed << setprecision(0) << round(DaysWork)<< " days" << endl;
    
	cout << setw(132) << "\n"; 
	               
    cout << setw(139) << "============= BENEFITS ==================== DEDUCTIONS ================\n";
    
    cout << setw(132) << "\n"; 
    
    cout << setw(82) << "Gross Pay:" << setw(10) << fixed << setprecision(2) << grossPay << " PHP" << setw(25+CCs) << "SSS Deduction (14%):" << setw(10) << fixed << setprecision(2) << sss << " PHP" << endl;
    cout << setw(82) << "Overtime Pay:" << setw(10) << fixed << setprecision(2) << overtimePay << " PHP" << setw(25+CCs) << pagIbigLabel2 << setw(10) << fixed << setprecision(2) << pagIbig << " PHP" << endl;
    cout << setw(82) << "Holiday Pay:" << setw(10) << fixed << setprecision(2) << hPay << " PHP" << setw(25+CCs) << "PhilHealth Deduction (5%):" << setw(10) << fixed << setprecision(2) << philHealth << " PHP" << endl;
    
    cout << setw(132) << "\n"; 
    	
    cout << setw(139) << "-----------------------------------------------------------------------\n";
    cout << setw(96) << "Total Deductions:" << setw(20) << fixed << setprecision(2) << totalDeductions << " PHP" << endl;
    cout << setw(139) << "-----------------------------------------------------------------------\n";

    cout << setw(96) << "NET PAY:" << setw(20) << fixed << setprecision(2) << netPay << " PHP" << endl;
    cout << setw(139) << "-----------------------------------------------------------------------\n";
     
      // Open a file stream
      
    stringstream filename;
    filename << "payroll_receipt_" << id << ".txt";
    
    ofstream file(filename.str());
    
    if (file.is_open()) {
        file << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
        )" << '\n';

        file << setw(115) << "=========================" << endl;
        file << setw(110) << "PAYROLL RECEIPT" << endl;
        file << setw(115) << "=========================" << endl;

        file << setw(132) << "\n";

        file << setw(82) << "Name: " << name << setw(CCnam) << "Hours Worked:" << setw(10) << fixed << setprecision(2) << hours << "hrs" << endl;
        file << setw(82) << "Position: " << position << setw(CCnam + CCpos + CCid) << "Holidays:" << setw(10) << endl;
        file << setw(82) << "Employee ID: " << id << setw(CCnam + CCpos + CCpos) << "*Legal:" << setw(10) << Lhhours << "hrs" << endl;
        file << setw(82) << "             " << setw(CCnam + CCpos + CCpos + CCid) << "*Special:" << setw(10) << Shhours << "hrs" << endl;
        file << setw(82) << "             " << setw(CCnam + CCpos + CCpos + CCid) << "Days Present:" << setw(7) << fixed << setprecision(0) << round(DaysWork) << " days" << endl;

        file << setw(132) << "\n";

        file << setw(139) << "============= BENEFITS ==================== DEDUCTIONS ================\n";

        file << setw(132) << "\n";

        file << setw(82) << "Gross Pay:" << setw(10) << fixed << setprecision(2) << grossPay << " PHP" << setw(25 + CCs) << "SSS Deduction (14%):" << setw(10) << fixed << setprecision(2) << sss << " PHP" << endl;
        file << setw(82) << "Overtime Pay:" << setw(10) << fixed << setprecision(2) << overtimePay << " PHP" << setw(25 + CCs) << pagIbigLabel2 << setw(10) << fixed << setprecision(2) << pagIbig << " PHP" << endl;
        file << setw(82) << "Holiday Pay:" << setw(10) << fixed << setprecision(2) << hPay << " PHP" << setw(25 + CCs) << "PhilHealth Deduction (5%):" << setw(10) << fixed << setprecision(2) << philHealth << " PHP" << endl;

        file << setw(132) << "\n";

        file << setw(139) << "-----------------------------------------------------------------------\n";
        file << setw(96) << "Total Deductions:" << setw(20) << fixed << setprecision(2) << totalDeductions << " PHP" << endl;
        file << setw(139) << "-----------------------------------------------------------------------\n";

        file << setw(96) << "NET PAY:" << setw(20) << fixed << setprecision(2) << netPay << " PHP" << endl;
        file << setw(139) << "-----------------------------------------------------------------------\n";

        // Close the file
        file.close();
        cout << "\tPayroll receipt sucessfully Printed "<< endl;
    } else {
        cout << "\tUnable to open file." << endl;
    }
    
}

int main() {
    // Login functionality
    string userName;
    string password;
    const string correctPassword = "NoobMaster69";
    const int maxAttempts = 5;

std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
    cout << "\n\t\t\t\t\t\t\t\tEnter your Username: ";
    getline(cin, userName);

    int attempts = 0;
    while (attempts < maxAttempts) {
        cout << "\t\t\t\t\t\t\t\tEnter password: ";
        cin >> password;
        clearScreen();

        if (password == correctPassword) {
std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
            cout << "\n\t\t\t\t\t\t\t\tWelcome Boss " << userName << "!" << endl;
            cout << "\t\t\t\t\t\t\t\tLoading";
            for (int i = 0; i < 3; ++i) {
                cout << ".";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            cout << endl;
            break; 
        } else { 
        std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
            cout << "\n\t\t\t\t\t\t\t\tEnter your name: " << userName;
            cout << "\n\t\t\t\t\t\t\t\tIncorrect password. Try again." << endl;
            attempts++;
        }
    }

    if (attempts == maxAttempts) {
    	std::cout << R"(
                                                     _______ _    _ ______   _   _  ____   ____  ____     _____ ____  __  __ _____        _   ___     __
                                                    |__   __| |  | |  ____| | \ | |/ __ \ / __ \|  _ \   / ____/ __ \|  \/  |  __ \ /\   | \ | \ \   / /
                                                       | |  | |__| | |__    |  \| | |  | | |  | | |_) | | |   | |  | | \  / | |__) /  \  |  \| |\ \_/ / 
                                                       | |  |  __  |  __|   | . ` | |  | | |  | |  _ <  | |   | |  | | |\/| |  ___/ /\ \ | . ` | \   /  
                                                       | |  | |  | | |____  | |\  | |__| | |__| | |_) | | |___| |__| | |  | | |  / ____ \| |\  |  | |   
                                                       |_|  |_|  |_|______| |_| \_|\____/ \____/|____/   \_____\____/|_|  |_|_| /_/    \_\_| \_|  |_|
	)" << '\n'; 
	
        cout << "\n\t\tToo many failed attempts. Exiting the program." << endl;
        return 1; 
    }

    clearScreen();
    displayHeader();

    const int MAX_EMPLOYEES = 100;
    string names[MAX_EMPLOYEES];
    string positions[MAX_EMPLOYEES];
    double rates[MAX_EMPLOYEES];
    double hours[MAX_EMPLOYEES];
    double LholidayHours[MAX_EMPLOYEES] = {0};
    double SholidayHours[MAX_EMPLOYEES] = {0};
    double daysWorked[MAX_EMPLOYEES];
    double overtime[MAX_EMPLOYEES];
    double sss[MAX_EMPLOYEES] = {0};
    double pagIbig[MAX_EMPLOYEES] = {0};
    double philHealth[MAX_EMPLOYEES] = {0};
    double totalDeductions[MAX_EMPLOYEES] = {0};
    double netPays[MAX_EMPLOYEES];
    int ids[MAX_EMPLOYEES];
    int count = 0;

    char choice;
    do {
        clearScreen();
        displayHeader();

        string greeting = getGreeting();
        string currentDateandTime = getCurrentDateTime();
        
        cout << "\n" << setw(88) << greeting << " Boss " << userName << "!" << setw(30) << currentDateandTime << endl;
        cout << "\n\t\t\t\t\t\t\t\t\t\t1. Add Employee\n";
        cout << "\t\t\t\t\t\t\t\t\t\t2. Manage Deductions\n";
        cout << "\t\t\t\t\t\t\t\t\t\t3. Display Payroll\n";
        cout << "\t\t\t\t\t\t\t\t\t\t4. Edit Employee\n";
        cout << "\t\t\t\t\t\t\t\t\t\t5. Delete Employee\n";
        cout << "\t\t\t\t\t\t\t\t\t\t6. Exit\n";
        cout << "\t\t\t\t\t\t\t\t\t\tChoose an option: ";
        cin >> choice;

        switch (choice) {
            case '1':
                if (count < MAX_EMPLOYEES) {
                    clearScreen();
                    displayHeader();
                    
                     cout << "\n" << setw(88) << greeting << " Boss " << userName << "!" << setw(30) << currentDateandTime << endl;

            cout << "\n\t\t\t\t\t\t\t\t\t\tEnter employee ID: ";
            while (true) {
                cin >> ids[count];

                // Check for valid numeric input
                if (cin.fail()) {
                    cout << "\t\t\t\t\t\t\t\t\t\tInvalid input! Please enter a numeric Employee ID: ";
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
                    continue; // Skip to next iteration
                }

                // Try to add the employee; repeat if addEmployee returns false (duplicate ID)
                if (addEmployee(ids[count])) {
                    // Employee added successfully, increment count
                     break; // Exit the while loop
                } else {
                    cout << "\t\t\t\t\t\t\t\t\tPlease enter a different Employee ID: ";
                }
            }
       

                    cout << "\t\t\t\t\t\t\t\t\t\tEnter employee name: ";
                    cin.ignore();
                    
					while (true) {
                    getline(cin, names[count]);

                    // Check if the name is empty
                     if (!names[count].empty()) {
                     break; // Valid input, exit the loop
                     } else {
                     cout << "\t\t\t\t\t\t\t\t\t\tInvalid input! Please enter a name: ";
                     }
                    }

                    cout << "\t\t\t\t\t\t\t\t\t\tEnter employee position (Regular, Supervisor, Manager): ";
                    while (true) {
                    getline(cin, positions[count]);

                    // Convert input to lowercase for case-insensitive comparison
                    string positionLower = positions[count];
                    transform(positionLower.begin(), positionLower.end(), positionLower.begin(), ::tolower);

                   // Check if the input matches any of the allowed values
                   if (positionLower == "regular" || positionLower == "supervisor" || positionLower == "manager") {
                   // Convert to title case to ensure uniform storage (e.g., "Regular" instead of "regular")
                   if (positionLower == "regular") positions[count] = "Regular";
                   else if (positionLower == "supervisor") positions[count] = "Supervisor";
                   else if (positionLower == "manager") positions[count] = "Manager";
        
                   break; // Valid input, exit the loop
                   } else {
                   cout << "\t\t\t\t\t\t\t\t\t\tInvalid position! Please enter 'Regular', 'Supervisor', or 'Manager': ";
                  }
                 }

                    cout << "\t\t\t\t\t\t\t\t\t\tEnter hourly rate: ";
                    cin >> rates[count];

                    rates[count] = adjustRateForPosition(positions[count], rates[count]);

                    cout << "\t\t\t\t\t\t\t\t\t\tEnter hours worked: ";
                    cin >> hours[count];
                    
                    cout << "\t\t\t\t\t\t\t\t\t\tEnter overtime hours: ";
                    cin >> overtime[count];
                    
                    cout << "\t\t\t\t\t\t\t\t\t\tEnter Legal Holiday Hours worked: ";
                    cin >> LholidayHours[count];
                    
                    cout << "\t\t\t\t\t\t\t\t\t\tEnter Special Holiday Hours worked: ";
                    cin >> SholidayHours[count];
                
                
                    sss[count] = rates[count] * 0.14;
                    
                    if(pagIbig[count]<=1500) {
                    pagIbig[count] = rates[count] * 0.01;
                    	}
                    else if(pagIbig[count]>=1500 && pagIbig[count]<=5000) {
                    pagIbig[count] = rates[count] * 0.02;	
                    }
                    else if (pagIbig[count]>=5000) {
                    	pagIbig[count] = rates[count] + 200;
                    }
                    
                    philHealth[count] = rates[count] * 0.05;

                    totalDeductions[count] = sss[count] + pagIbig[count] + philHealth[count];
                    
                    netPays[count] = calculateNetPay(rates[count], hours[count], overtime[count], totalDeductions[count], LholidayHours[count], SholidayHours[count]);
                    count++;
                } else {
                    cout << "\t\t\t\t\t\t\t\t\t\tMaximum employee limit reached!" << endl;
                }
                break;

            case '2': {
            	
            	 clearScreen();
                 displayHeader();
                 
                if (count == 0) {
                    cout << "\n\t\t\t\t\t\t\t\t\t\tNo employees to manage deductions for!" << endl;
                cout << endl << "\t\t\t\t\t\t\t\t\t\tPress ENTER to continue...";
                cin.ignore();
                cin.get();
                break;
                }
                
               displayTableHeader();
               for (int i = 0; i < count; i++) {
                    displayTableRow(ids[i], names[i], positions[i], rates[i], hours[i], LholidayHours[i], SholidayHours[i], overtime[i], sss[i], pagIbig[i], philHealth[i], totalDeductions[i], netPays[i]);
                }

                
                int empId;
                cout << "\n\tEnter employee ID to manage deductions: ";
                cin >> empId;

                int empIndex = -1;
                for (int i = 0; i < count; i++) {
                    if (ids[i] == empId) {
                        empIndex = i;
                        break;
                    }
                }

                if (empIndex == -1) {
                    cout << "\tInvalid employee ID!" << endl;
                    break;
                }

                double deductionType; 
                cout << "\n\tCurrent deductions for " << names[empIndex] << ": " << totalDeductions[empIndex] << endl;
                cout << "\tSSS: " << sss[empIndex] << ", Pag-IBIG: " << pagIbig[empIndex] << ", PhilHealth: " << philHealth[empIndex] << endl;
                cout << "\n\tChoose deduction type (1 for flat amount, 2 for percentage, 0 to cancel): ";
                cin >> deductionType;

                if (deductionType == 1) {
                    double flatDeduction;
                    cout << "\tEnter flat deduction amount: ";
                    cin >> flatDeduction;
                    totalDeductions[empIndex] = flatDeduction;
                    
                } else if (deductionType == 2) {
                    double percentage;
                    cout << "\tEnter deduction percentage: ";
                    cin >> percentage;
                    double grossPay = (rates[empIndex] * hours[empIndex]) + (overtime[empIndex] * (rates[empIndex] * 1.5));
                    totalDeductions[empIndex] = (percentage / 100) * grossPay;
                    
                } else if (deductionType == 0) {
                    cout << "\tCancelled deduction update." << endl;
                } else {
                    cout << "\ttInvalid deduction type! No changes made." << endl;
                }

                netPays[empIndex] = calculateNetPay(rates[empIndex], hours[empIndex], overtime[empIndex], totalDeductions[empIndex], LholidayHours[count],SholidayHours[count]);
                break;
            }

            case '3':
                clearScreen();
                displayHeader();
             
                
                if (count == 0) {
                    cout << "\n\t\t\t\t\t\t\t\t\t\tNo employees Added!" << endl;
                    cout << endl << "\t\t\t\t\t\t\t\t\t\tPress ENTER to continue...";
                    cin.ignore();
                    cin.get();
                    break;
                }
                
                displayTableHeader();

                for (int i = 0; i < count; i++) {
                    displayTableRow(ids[i], names[i], positions[i], rates[i], hours[i], LholidayHours[i], SholidayHours[i], overtime[i], sss[i], pagIbig[i], philHealth[i], totalDeductions[i], netPays[i]);
                }
                
             char receiptChoice;
             do {
             cout << "\n\tDo you want to generate a payroll receipt? (Y/N): ";
            cin >> receiptChoice;

            if (receiptChoice == 'y' || receiptChoice == 'Y') {
            int receiptId;
            int receiptIndex = -1;

    // Loop until a valid Employee ID is entered
    while (receiptIndex == -1) {
        cout << "\n\tEnter the Employee ID for the receipt: ";
        cin >> receiptId;

        for (int i = 0; i < count; i++) {
            if (ids[i] == receiptId) {
                receiptIndex = i;
                break;
            }
        }

        if (receiptIndex == -1) {
            cout << "\n\tInvalid Employee ID! Please enter a valid Employee ID." << endl;
        }
    }

    clearScreen();
    generatePayrollReceipt(ids[receiptIndex], names[receiptIndex], positions[receiptIndex], rates[receiptIndex], hours[receiptIndex], LholidayHours[receiptIndex], SholidayHours[receiptIndex], overtime[receiptIndex], sss[receiptIndex], pagIbig[receiptIndex], philHealth[receiptIndex], totalDeductions[receiptIndex], netPays[receiptIndex]);

          } else if (receiptChoice == 'n' || receiptChoice == 'N') {
        // Go back to menu (exit the loop)
            break;
        
           } else {
            cout << "\n\tPlease enter a valid choice (Y/N)." << endl;
        }
            } while (true);
  
                
                cout << endl << "\n\tPress ENTER to continue...";
                cin.ignore();
                cin.get();
                break;

            case '4': {
            	clearScreen();
                displayHeader();
                 
                if (count == 0) {
                    cout << "\n\t\t\t\t\t\t\t\t\t\tNo employees to edit!" << endl;
                    cout << endl << "\t\t\t\t\t\t\t\t\t\tPress ENTER to continue...";
                    cin.ignore();
                    cin.get();
                    break;
                }

                int editId;
                
                displayTableHeader();

                for (int i = 0; i < count; i++) {
                    displayTableRow(ids[i], names[i], positions[i], rates[i], hours[i], LholidayHours[i], SholidayHours[i], overtime[i], sss[i], pagIbig[i], philHealth[i], totalDeductions[i], netPays[i]);
                }
                
                cout << "\n\tEnter Employee ID to edit: ";
                cin >> editId;

                int editIndex = -1;
                for (int i = 0; i < count; i++) {
                    if (ids[i] == editId) {
                        editIndex = i;
                        break;
                    }
                }

                if (editIndex == -1) {
                    cout << "\tEmployee ID not found!" << endl;
                    break;
                }

                // Update employee information here (similar to adding an employee)
                cout << "\tEnter new hourly rate: ";
                cin >> rates[editIndex];
                rates[editIndex] = adjustRateForPosition(positions[editIndex], rates[editIndex]);

                cout << "\tEnter new Hours worked: ";
                cin >> hours[editIndex];

                cout << "\tEnter new Overtime Hours: ";
                cin >> overtime[editIndex];
                
                cout << "\tEnter new Legal Holiday Hours worked: ";
                cin >> LholidayHours[editIndex];
                    
                cout << "\tEnter new Special Holiday Hours worked: ";
                cin >> SholidayHours[editIndex];
                

                sss[editIndex] = rates[editIndex] * 0.0363;
                
                if(pagIbig[editIndex]<=1500) {
                    pagIbig[editIndex] = rates[editIndex] * 0.01;
                    	}
                    else if(pagIbig[editIndex]>=1500 && pagIbig[editIndex]<=5000) {
                    pagIbig[editIndex] = rates[editIndex] * 0.02;	
                    }
                    else if (pagIbig[editIndex]>=5000) {
                    	pagIbig[editIndex] = rates[editIndex] + 200;
                    }
                    
                philHealth[editIndex] = rates[editIndex] * 0.05;

                totalDeductions[editIndex] = sss[editIndex] + pagIbig[editIndex] + philHealth[editIndex];
                netPays[editIndex] = calculateNetPay(rates[editIndex], hours[editIndex], overtime[editIndex], totalDeductions[editIndex], LholidayHours[editIndex], SholidayHours[editIndex]);
                break;
            }

            case '5': {
            	
            	clearScreen();
                displayHeader();
                 
                if (count == 0) {
                    cout << "\n\t\t\t\t\t\t\t\t\t\tNo employees to delete!" << endl;
                    cout << endl << "\t\t\t\t\t\t\t\t\t\tPress ENTER to continue...";
                    cin.ignore();
                    cin.get();
                    break;
                }
                
             displayTableHeader();
             for (int i = 0; i < count; i++) {
                    displayTableRow(ids[i], names[i], positions[i], rates[i], hours[i], LholidayHours[i], SholidayHours[i], overtime[i], sss[i], pagIbig[i], philHealth[i], totalDeductions[i], netPays[i]);
                }


                int deleteId;
                cout << "\n\tEnter Employee ID to delete: ";
                cin >> deleteId;

                int deleteIndex = -1;
                for (int i = 0; i < count; i++) {
                    if (ids[i] == deleteId) {
                        deleteIndex = i;
                        break;
                    }
                }

                if (deleteIndex == -1) {
                    cout << "\tEmployee ID not found!" << endl;
                } else {
                    for (int i = deleteIndex; i < count - 1; i++) {
                        ids[i] = ids[i + 1];
                        names[i] = names[i + 1];
                        positions[i] = positions[i + 1];
                        rates[i] = rates[i + 1];
                        hours[i] = hours[i + 1];
                        overtime[i] = overtime[i + 1];
                        sss[i] = sss[i + 1];
                        pagIbig[i] = pagIbig[i + 1];
                        philHealth[i] = philHealth[i + 1];
                        totalDeductions[i] = totalDeductions[i + 1];
                        netPays[i] = netPays[i + 1];
                    }
                    count--;
                    
                cout << "\tDeleting Employee";
                for (int i = 0; i < 3; ++i) {
                cout << ".";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
                    cout << "\n\n\tEmployee deleted successfully!" << endl;
                    
                    cout << endl << "\n\tPress ENTER to continue...";
                    cin.ignore();
                    cin.get();
                    break;
                }
                break;
            }

            case '6':
            	clearScreen();
                displayHeader();
                cout << "\n\t\t\t\t\t\t\t\t\t\tExiting the program";
                for (int i = 0; i < 3; ++i) {
                cout << ".";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
                break;

            default:
                cout << "\t\t\t\t\t\t\t\t\t\tInvalid choice! Please try again." << endl;
                break;
        }
    } while (choice != '6');
    
    

    return 0; 
    
}

