#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

// Helper function to calculate flight duration (in hours and minutes)
pair<int, int> calculateDuration(const string& dep_time, const string& arr_time) {
    int dep_hour, dep_min, arr_hour, arr_min;
    char colon;
    stringstream dep_ss(dep_time), arr_ss(arr_time);

    dep_ss >> dep_hour >> colon >> dep_min;
    arr_ss >> arr_hour >> colon >> arr_min;

    int dep_total_min = dep_hour * 60 + dep_min;
    int arr_total_min = arr_hour * 60 + arr_min;

    if (arr_total_min < dep_total_min) {
        arr_total_min += 24 * 60; // Adjust for next day arrival
    }

    int duration_min = arr_total_min - dep_total_min;
    return {duration_min / 60, duration_min % 60};
}

class Flight {
public:
    string company, flight_no, source, destination;
    string departure_time, arrival_time;
    int totalseats, availableseats, reservedseats;
    vector<string> passengers;
    pair<int, int> duration; // {hours, minutes}

    Flight(string comp, string fno, string src, string dest, int seats, string dep_time, string arr_time) {
        company = comp;
        flight_no = fno;
        source = src;
        destination = dest;
        totalseats = seats;
        availableseats = seats;
        reservedseats = 0;
        departure_time = dep_time;
        arrival_time = arr_time;
        duration = calculateDuration(dep_time, arr_time);
    }

    float utilization() const {
        return (reservedseats / (float)totalseats) * 100;
    }

    void reserveSeat(string passenger, string preference = "Any") {
        if (availableseats > 0) {
            passengers.push_back(passenger + " (" + preference + ")");
            reservedseats++;
            availableseats--;
            cout << "Seat reserved for " << passenger << " on flight " << flight_no << ".\n";
        } else {
            cout << "No seats available on this flight.\n";
        }
    }
    void cancelAllReservations(bool isAdmin = false) {
    if (isAdmin) {
        // Admin cancels all reservations for this flight with 100% refund
        for (const auto& passenger : passengers) {
            cout << "Refund Initiated: 100% for " << passenger << " on flight " << flight_no << ".\n";
        }
    }

    // Clear all passengers and reset reserved and available seats
    passengers.clear();
    reservedseats = 0;
    availableseats = totalseats; // Reset available seats to total seats
    cout << "All reservations have been canceled successfully.\n";
    }
    void cancelReservation(string passenger, bool isAdmin = false) {
    auto it = find_if(passengers.begin(), passengers.end(), [&](const string& p) {
        return p.find(passenger) != string::npos;
    });
    if (it != passengers.end()) {
        passengers.erase(it);
        reservedseats--;
        availableseats++;

        float refund_percentage = isAdmin ? 100.0f : 70.0f; // 100% refund for admin, 70% for passenger
        cout << "Refund Initiated: " << refund_percentage << "% for " << passenger << " on flight " << flight_no << ".\n";

        cout << "Reservation for " << passenger << " canceled successfully.\n";
    } else {
        cout << "Passenger " << passenger << " not found on this flight.\n";
    }
    }


    void displayFlight() const {
        cout << "Flight: " << flight_no << " | " << company << " | " << source << " -> " << destination << "\n";
        cout << "Departure: " << departure_time << " | Arrival: " << arrival_time
             << " | Duration: " << duration.first << "h " << duration.second << "m\n";
        cout << "Total Seats: " << totalseats << " | Reserved Seats: " << reservedseats 
             << " | Utilization: " << utilization() << "%\n";
    }

    void displayPassengers() const {
        cout << "Passengers on flight " << flight_no << ":\n";
        for (const auto& p : passengers) {
            cout << "- " << p << "\n";
        }
    }
};

bool flightExists(const vector<Flight>& flights, const string& flight_no) {
    return any_of(flights.begin(), flights.end(), [&](const Flight& f) {
        return f.flight_no == flight_no;
    });
}

void removeFlight(vector<Flight>& flights, const string& flight_no) {
    auto it = find_if(flights.begin(), flights.end(), [&](const Flight& f) {
        return f.flight_no == flight_no;
    });
    
    if (it != flights.end()) {
        flights.erase(it);
        cout << "Flight " << flight_no << " has been removed successfully.\n";
    } else {
        cout << "Flight " << flight_no << " not found.\n";
    }
}
// Function to calculate per seat cost
float setPerSeatCost(int total_cost, int total_seats) {
    // Calculate the profit (10% of total cost)
    float profit = total_cost * 0.10;

    // Calculate the total cost including profit
    float total_with_profit = total_cost + profit;

    // Calculate per seat cost
    return total_with_profit / total_seats;
}



void adminMenu(vector<Flight>& flights) {
    while (true) {
        cout << "\n--- Administrator Menu ---\n";
        cout << "1. Add Flight\n";
        cout << "2. View All Flights\n";
        cout << "3. View Passenger List\n";
        cout << "4. Find Flight with Highest Utilization\n";
        cout << "5. View Reservation Summary\n";
        cout << "6. Cancel a Flight\n";
        cout << "7. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
            string company, flight_no, source, destination, dep_time, arr_time;
            int seats, total_cost;

            cout << "Enter Flight Details:\n";
            cout << "Company: ";
            cin >> company;
            cout << "Flight Number: ";
            cin >> flight_no;
            if (flightExists(flights, flight_no)) {
                cout << "Error: Flight number already exists. Please use a unique flight number.\n";
                break;
            }
                cout << "Source: ";
                cin >> source;
                cout << "Destination: ";
                cin >> destination;
                cout << "Departure Time (HH:MM): ";
                cin >> dep_time;
                cout << "Arrival Time (HH:MM): ";
                cin >> arr_time;
                cout << "Total Seats: ";
                cin >> seats;

                // New code for total cost and per seat cost
                cout << "Enter the total cost of the flight: ";
                cin >> total_cost;

                flights.emplace_back(company, flight_no, source, destination, seats, dep_time, arr_time);

                // Calculate and display the per seat cost
                float per_seat_cost = setPerSeatCost(total_cost, seats);
                cout << "Per Seat Cost: " << fixed << setprecision(2) << per_seat_cost << "\n";

                cout << "Flight added successfully.\n";
                break;
            }

            case 2:
                cout << "Available Flights:\n";
                for (const auto& flight : flights) {
                    flight.displayFlight();
                }
                break;

            case 3: {
                string flight_no;
                cout << "Enter Flight Number to View Passenger List: ";
                cin >> flight_no;

                auto it = find_if(flights.begin(), flights.end(), [&](const Flight& f) {
                    return f.flight_no == flight_no;
                });
                if (it != flights.end()) {
                    it->displayPassengers();
                } else {
                    cout << "Flight not found.\n";
                }
                break;
            }

            case 4: {
                if (flights.empty()) {
                    cout << "No flights available.\n";
                    break;
                }
                auto max_flight = max_element(flights.begin(), flights.end(), [](const Flight& f1, const Flight& f2) {
                    return f1.utilization() < f2.utilization();
                });
                cout << "Flight with highest utilization:\n";
                max_flight->displayFlight();
                break;
            }

            case 5:
                cout << "Reservation Summary:\n";
                for (const auto& flight : flights) {
                    cout << flight.flight_no << " (" << flight.company << "): "
                         << flight.reservedseats << "/" << flight.totalseats << " seats reserved.\n";
                }
                break;
            case 6: {
                string flight_no;
                cout << "Enter Flight Number to Delete: ";
                cin >> flight_no;

                auto it = find_if(flights.begin(), flights.end(), [&](Flight& f) {
                    return f.flight_no == flight_no;
                });

                if (it != flights.end()) {
                    // Delete the flight and cancel all reservations with 100% refund
                    it->cancelAllReservations(true); // Pass 'true' for admin-side cancellation (100% refund)
                    flights.erase(it);  // Remove the flight from the list
                    cout << "Flight " << flight_no << " has been deleted successfully.\n";
                } else {
                    cout << "Flight not found.\n";
                }
            }
            break;
            }
            case 7:
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }


void passengerMenu(const vector<Flight>& flights) {
    while (true) {
        cout << "\n--- Passenger Menu ---\n";
        cout << "1. Search Flights\n";
        cout << "2. Reserve a Seat\n";
        cout << "3. Cancel a Reservation\n";
        cout << "4. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string source, destination;
                cout << "Enter Source: ";
                cin >> source;
                cout << "Enter Destination: ";
                cin >> destination;
                cout << "Available Flights:\n";
                for (const auto& flight : flights) {
                    if (flight.source == source && flight.destination == destination) {
                        flight.displayFlight();
                    }
                }
                break;
            }
            case 2: {
                string flight_no, passenger, preference;
                cout << "Enter Flight Number: ";
                cin >> flight_no;
                cout << "Enter Your Name: ";
                cin >> passenger;
                cout << "Seat Preference (Window/Aisle/Any): ";
                cin >> preference;

                auto it = find_if(flights.begin(), flights.end(), [&](const Flight& f) {
                    return f.flight_no == flight_no;
                });
                if (it != flights.end()) {
                    it->reserveSeat(passenger, preference);
                } else {
                    cout << "Flight not found.\n";
                }
                break;
            }
            case 3: {
            string flight_no, passenger;
            cout << "Enter Flight Number to Cancel Reservation: ";
            cin >> flight_no;
            cout << "Enter Your Name to Cancel Reservation: ";
            cin >> passenger;

            auto it = find_if(flights.begin(), flights.end(), [&](const Flight& f) {
                return f.flight_no == flight_no;
            });
            if (it != flights.end()) {
                it->cancelReservation(passenger, false); // Pass 'false' for passenger-side cancellation (70% refund)
            } else {
                cout << "Flight not found.\n";
            }
            break;
        }


            case 4:
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    vector<Flight> flights;

    while (true) {
        cout << "\n--- Welcome to Airline Management System ---\n";
        cout << "1. Administrator Mode\n";
        cout << "2. Passenger Mode\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                adminMenu(flights);
                break;
            case 2:
                passengerMenu(flights);
                break;
            case 3:
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
