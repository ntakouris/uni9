//If multiple 'vsnprinf' are defined
#define __USE_MINGW_ANSI_STDIO 0

#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "Reservation.h"
#include "Room.h"
#include "Hotel.h"
#include "RoomTypeA.h"
#include "RoomTypeB.h"
#include "RoomTypeC.h"
#include "RoomTypeD.h"
#include "RoomTypeE.h"

using namespace std;

const int randomCancelThresholdPercentage = 25;

const string possibleClientNames[] = {"Takis", "Kalamaris", "Bob", "Mr. Krab"};

const int arrivalHigh = 29;
const int arrivalLow = 1; // >= 1

const int maxDurationTryOverride = 5;
const int minDurationTryOverride = 2; // >= 1

const int personCountHigh = 3;
const int personCountLow = 1; // >= 1


int randomBetween(int low, int high){
    if(low > high){
        int temp = low;
        low = high;
        high = temp;
    }

    if(high == low){
        return low;
    }
    return rand() % (high - low + 1) + low;
}


template <typename I>
I random_element(I begin, I end)
{
    const unsigned long n = distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = rand() / divisor; } while (k >= n);

    advance(begin, k);
    return begin;
}

bool is_number(const string& s) {
  return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

int getNumber(){
    string num;
    getline(cin, num);
    while (!is_number(num)) {
        cout << "Input is not a number , enter again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        num.clear();
        getline(cin, num);
  }
  return stoi(num);
}

int main()
{
    //Don't use scientific notation (no exponent in numbers)
    cout << fixed;

    cout << "--Hotel Reservation System--" << endl;
    Hotel hotel("Some Hotel");

    srand (time(NULL));

    //Initialize and add rooms (minimum 10)
    for(int j=0;j<5;j++){
        Room * monoklino = new Room(1, 30);
        hotel.addRoom(*monoklino);
    }

    for(int j=0;j<3;j++){
        Room * diklino = new Room(2, 30);
        hotel.addRoom(*diklino);
    }

    for(int j=0;j<2;j++){
        Room * triklino = new Room(3, 30);
        hotel.addRoom(*triklino);
    }

    for(int j=0;j<2;j++){
        Room * souita = new RoomTypeA(4, 200);
        hotel.addRoom(*souita);
    }

    for(int j=0;j<2;j++){
        Room * souitaPromo = new RoomTypeB(4, 200, 50);
        hotel.addRoom(*souitaPromo);
    }

    Room * synedriakh = new RoomTypeC(20, 5, 2);
    hotel.addRoom(*synedriakh);

    for(int j=0;j<2;j++){
        Room * diklinoPromo = new RoomTypeD(2, 25, 3);
        hotel.addRoom(*diklinoPromo);
    }

    for(int j=0;j<2;j++){
        Room * diklinoRatzo = new RoomTypeE(3, 20, 2);
        hotel.addRoom(*diklinoRatzo);
    }

    while(true){
        cout << endl << endl;
        cout << "Creating new random reservation... " << endl;
        string randomClientName = possibleClientNames[randomBetween(0,3)];
        cout << "Random client name: " << randomClientName << endl;
        int randomArrival = randomBetween(arrivalLow, arrivalHigh);
        cout << "Random arrival: " << randomArrival << endl;
        int maxDurationLimit = 30 - randomArrival;
        //We need bound safety, that's why we try overrides
        maxDurationLimit = maxDurationTryOverride > maxDurationLimit ? maxDurationLimit : maxDurationTryOverride;
        int minDuration = minDurationTryOverride > maxDurationLimit ? 1 : minDurationTryOverride;

        int randomDuration = 1;
        int randomDurationBound = maxDurationLimit - minDuration;
        if(randomDurationBound > 0){
            randomDuration = randomBetween(minDuration, randomDurationBound);
        }else{
            randomDuration = 0;
        }

        cout << "Duration: " << randomDuration << endl;
        int randomPersonCount = randomBetween(personCountLow, personCountHigh);
        cout << "Persons: " << randomPersonCount << endl;

        Reservation * randRes = new Reservation(randomClientName, randomArrival, randomDuration, randomPersonCount);
        int randRoomId = hotel.addReservation(*randRes);

        if(randRoomId == 0){
            cout << "No room fitting the random reservation!" << endl;
        }else{
            cout << "Reservation added at room with id: " << randRoomId << endl;
        }

        int r = randomBetween(0,100);
        cout << "Random Number: " << r << endl;
        bool shouldCancelRandomReservation =  r < randomCancelThresholdPercentage;

        if(shouldCancelRandomReservation){
            cout << "Picking a random reservation to cancel..." << endl;
            Reservation * randomReservation = *random_element(hotel.getReservations().begin(), hotel.getReservations().end());
            bool result = hotel.cancelReservation(randomReservation->getId());
            if(result){
                cout << "Reservation " << randomReservation->getId() << " cancelled successfully." << endl;
            }else{
                cout << "Reservation " << randomReservation->getId() << " could not be cancelled." << endl;
            }
        }
            int selection = -1;
            while(selection != 1){
                cout << endl << endl;
                cout << "[1] Next Iteration" << endl;
                cout << "[2] Add Reservation" << endl;
                cout << "[3] Cancel Reservation" << endl;
                cout << "[4] View Reservations" << endl;
                cout << "[5] View Rooms" << endl;
                cout << "[6] View Reservation Calendar" << endl;
                cout << "[7] View Income" << endl;
                cout << "[8] Exit" << endl;
                cout << endl << "[*] > ";

                selection = getNumber();

                switch(selection){
                case 1: break;
                case 2:{ // new reservation
                    cout << "New Reservation Wizard" << endl;

                    cout << "Client Name: ";
                    string name;
                    getline(cin, name);

                    int arrival = -1;
                    do{
                        cout << "Arrival Day (0 - 30): ";
                        arrival = getNumber();
                    }while(arrival < 0 || arrival > 30);

                    int duration = -1;
                    do{
                        cout << "Valid Duration (in days): ";
                        duration = getNumber();
                    }while(duration < 0 || arrival + duration > 30);

                    int personCount = -1;

                    do{
                        cout << "Valid Person Count: ";
                        personCount = getNumber();
                    }while(personCount <= 0);

                    Reservation * res = new Reservation(name, arrival, duration, personCount);

                    //optional room id
                    int roomId;
                    cout << "[Optional] Enter room id or 0: ";
                    roomId = getNumber();

                    if(roomId > 0){//try to insert in valid room
                        Room * targetRoom = hotel.findRoom(roomId);
                        if(targetRoom == nullptr){
                            cout << "Target room does not exist, cancelling reservation...";
                            delete res;
                        }else{
                            bool result = targetRoom->addReservation(*res);
                            if(result){
                                cout << "Reservation id: " << res->getId() << " - Room id: " << roomId << endl;
                            }else{
                                cout << "Target is full! Cancelling reservation...";
                                delete res;
                            }
                        }
                    }else{//insert on first available room
                        cout << "Room id not specified, selecting first available room..." << endl;
                        int rId = hotel.addReservation(*res);
                        if(rId == 0){
                            cout << "Hotel full! Can't add reservation." << endl;
                            delete res;
                        }else{
                            cout << "Reservation id: " << res->getId() << " - Room id: " << rId << endl;
                        }
                    }
                    }break;
                case 3:{
                    int resId = -1;
                    do{
                        cout << "Specify positive target reservation id" << endl << "[*] > ";
                        resId = getNumber();
                    }while(resId <= 0);
                    hotel.cancelReservation(resId);
                    }break;
                case 4:{ //view reservations (id | client name | room id)
                    cout << endl;
                    for(auto iterator : hotel.getReservations()){
                        cout << iterator->getId() << " | " << iterator->getClientName() << " | " << iterator->getRoom()->getId() << endl;
                    }
                    }break;
                case 5:{//view rooms (id | completeness percentage | income)
                    cout << endl;
                    for(auto iterator : hotel.getRooms()){
                        cout << iterator->getId() << " | " << iterator->completenessPercentage() << "% | " << iterator->totalCost() << endl;
                    }
                    }break;
                case 6:{//pretty-print calendar
                    hotel.reservationCalendar();
                    }break;
                case 7:{//view income
                    cout << "[Optional] Specify positive room id or 0" << endl << "[*] > ";
                    int rId = getNumber();
                    if(rId == 0){//Income for all rooms
                        cout << "Total income for all rooms: " << hotel.calculateIncome() << endl;
                    }else{//income for specific room
                        Room * room = hotel.findRoom(rId);
                        if(room == nullptr){
                                cout << "Room with id " << rId << " does not exist!" << endl;
                        }else{
                            cout << "Income for room " << rId << " is: " << room->totalCost() << endl;
                        }
                    }
                    }break;
                case 8:
                    return 0;
                default:
                    break;
                }
            }

        }
    return 0;
}

