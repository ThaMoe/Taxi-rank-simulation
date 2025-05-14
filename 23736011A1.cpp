// Name: Thaamir Moerat
// student numder: 23736011

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>  // for exit
#include <queue>    // for queue
#include <iomanip>  // for setw
using namespace std;

// Structure to represent a passenger
struct Passenger {
    int arrivalTime;      // time when the passenger arrives at the station
    char route;           // s c or l
    int boardingTime;     // time the passenger takes to board the taxi
    string name;          // name of the passenger
};

// convert a string to an integer
int stringToInt(const string& str) {
    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        char ch = str[i];
        if (ch >= '0' && ch <= '9') {
            result = result * 10 + (ch - '0');  // convert each character to a digit
        } else {
            cout << "Invalid number: " << str << endl;  // in case of invalid character
            exit(1);  // exit the program if invalid input is found
        }
    }
    return result;
}

// read passenger data from a file
vector<Passenger> readPassengerData() {
    vector<Passenger> passengers;  // vector to store all passengers
    ifstream file("C:\\data\\taxiData.txt");  // open the file with passanger data

    if (!file) {
        cout << "Error opening file!" << endl;  // handle error if file doesn't open
        return passengers;  // return empty vector if the file can't be opened
    }

    int countS = 0, countL = 0, countC = 0;  // counters for each rout type

    string line;
    // read each line of the file
    while (getline(file, line)) {
        if (line.empty()) continue;  // skip empty lines

        stringstream ss(line);
        string arrivalStr, routeStr, boardingStr;

        // extract each part of the line
        getline(ss, arrivalStr, ',');
        getline(ss, routeStr, ',');
        getline(ss, boardingStr, ',');

        Passenger p;  // create new passenger object
        p.arrivalTime = stringToInt(arrivalStr);  // convert arrival time string to integer
        p.route = routeStr[0];  // extract the route type
        p.boardingTime = stringToInt(boardingStr);  // convert boarding time to integer

        // generate the passenger name based on their route
        stringstream nameStream;
        if (p.route == 'S') {
            countS++;  // Increment count
            nameStream << "S(" << countS << ")";
        } else if (p.route == 'L') {
            countL++;
            nameStream << "L(" << countL << ")";
        } else if (p.route == 'C') {
            countC++;
            nameStream << "C(" << countC << ")";
        }

        p.name = nameStream.str();  // set passenger's name

        passengers.push_back(p);  // add passenger to the vector
    }

    file.close();  // close the file
    return passengers;  // return the list of passengers
}

int main() {
    // read the passenger data from the file
    vector<Passenger> passengers = readPassengerData();

    // create queues for each route
    queue<Passenger> queueS;
    queue<Passenger> queueL;
    queue<Passenger> queueC;

    // initialize taxi capacities for each route
    int taxiCapS = 5;
    int taxiCapL = 5;
    int taxiCapC = 5;

    int currentTime = 0;  // start simulation at time = 0

    // print the header for the output table
    cout << left << setw(8) << "Time"
         << setw(15) << "Next"
         << setw(5) << "S"
         << setw(5) << "L"
         << setw(5) << "C"
         << setw(15) << "WaitingQ S"
         << setw(15) << "WaitingQ L"
         << setw(15) << "WaitingQ C"
         << setw(15) << "TaxiCap S"
         << setw(15) << "TaxiCap L"
         << setw(15) << "TaxiCap C"
         << endl;
    cout << string(120, '-') << endl;

    // simulate the process for 40 time units
    while (currentTime <= 40) {
        bool printedLine = false;  // indicate if a line should be printed
        string nextPassengers = "";  // track the next passengers for this time unit

        // process boarding for each route
        if (!queueS.empty()) {
            Passenger& frontS = queueS.front();  // get the first passenger in the queue
            if (frontS.boardingTime == 0) {  // if boarding time is 0, the passenger can board
                if (taxiCapS > 0) {  // if there's space in the taxi, the passenger boards
                    queueS.pop();  // remove passenger from queue
                    taxiCapS--;  // decrease available capacity for 'S'
                } else {  // if no space, reset taxi capacity to 5
                    taxiCapS = 5;
                }
                printedLine = true;  // mark that a line should be printed
            } else {
                frontS.boardingTime--;  // if not yet time to board, decrease the boarding time
            }
        }


        if (!queueL.empty()) {
            Passenger& frontL = queueL.front();
            if (frontL.boardingTime == 0) {
                if (taxiCapL > 0) {
                    queueL.pop();
                    taxiCapL--;
                } else {
                    taxiCapL = 5;
                }
                printedLine = true;
            } else {
                frontL.boardingTime--;
            }
        }

        if (!queueC.empty()) {
            Passenger& frontC = queueC.front();
            if (frontC.boardingTime == 0) {
                if (taxiCapC > 0) {
                    queueC.pop();
                    taxiCapC--;
                } else {
                    taxiCapC = 5;
                }
                printedLine = true;
            } else {
                frontC.boardingTime--;
            }
        }

        // add new passengers to the queues
        for (int i = 0; i < passengers.size(); i++) {
            if (passengers[i].arrivalTime == currentTime) {
                Passenger p = passengers[i];
                nextPassengers += p.name + " ";  // add passenger's name to the list
                if (p.route == 'S') {
                    queueS.push(p);  // add to the short rout queue
                } else if (p.route == 'L') {
                    queueL.push(p);  // add to the long route queue
                } else if (p.route == 'C') {
                    queueC.push(p);  // add to the city route queue
                }
                printedLine = true;
            }
        }

        // print the current status of the simulation for this time unit
        if (printedLine || !nextPassengers.empty()) {

            cout << left << setw(8) << currentTime
                 << setw(15) << nextPassengers
                 << setw(5) << (queueS.empty() ? 0 : queueS.size())
                 << setw(5) << (queueL.empty() ? 0 : queueL.size())
                 << setw(5) << (queueC.empty() ? 0 : queueC.size())
                 << setw(15) << (queueS.empty() ? 0 : queueS.size())
                 << setw(15) << (queueL.empty() ? 0 : queueL.size())
                 << setw(15) << (queueC.empty() ? 0 : queueC.size())
                 << setw(15) << taxiCapS
                 << setw(15) << taxiCapL
                 << setw(15) << taxiCapC
                 << endl;
        }

        // reset taxi capacities if they reach 0
        if (taxiCapS == 0) taxiCapS = 5;
        if (taxiCapL == 0) taxiCapL = 5;
        if (taxiCapC == 0) taxiCapC = 5;

        currentTime++;  // move to the next time unit
    }

    return 0;  // end of program
}
