#include "Hotel.h"

using namespace std;

Hotel::Hotel(string n) :
    name{n}
{}

void Hotel::reservationCalendar(){
    //Δωμάτιο 01 02 03 ..
    string dom = "Domatio    ";
    cout << dom;
    for(int i = 1; i <= 30; i++){
        cout << (i < 10 ? "0" : "") << i << " ";
    }

    cout << endl;

    int roomCountDigits = getDigitsNum(rooms.size());

    for(auto iterator : rooms){
        cout << endl;
        //[[RoomId]] (spaces)
        int spacePostfix = dom.length() - getDigitsNum(iterator->getId());

        string spaces = "";
        for(int k = 0;k < spacePostfix;k++){
            spaces += " ";
        }

        cout << iterator->getId() << spaces;

        // _ * _ * _
        for(int i = 0; i < 30 ; i++){
            if(iterator->getReservations()[i] == nullptr){
                cout << "_  ";
            }else{
                cout << "*  ";
            }
        }
    }
}

int Hotel::getDigitsNum(int num){
    return to_string(num).length();
}

double Hotel::calculateIncome(int roomId){
    double income = 0;
    for(auto iterator : rooms){
        if(iterator->getId() == roomId){
            income += iterator->totalCost();
        }
    }
    return income;
}

double Hotel::calculateIncome(){
    double income = 0;
    for(auto iterator : rooms){
        income += iterator->totalCost();
    }
    return income;
}

void Hotel::addRoom(Room& room){
    rooms.push_back(&room);
}

int Hotel::addReservation(Reservation& r){
    for(auto iterator : rooms){
            if(iterator->addReservation(r)){
                reservations.push_back(&r);
                r.setRoom(*iterator);
                return iterator->getId();
            }
    }
    return 0;
}

Reservation * Hotel::findReservation(int id){
    for(auto iterator : reservations){
        if(iterator->getId() == id){
            return iterator;
        }
    }
    return nullptr;
}


Room * Hotel::findRoom(int id){
    for(auto iterator : rooms){
        if(iterator->getId() == id){
            return iterator;
        }
    }
    return nullptr;
}

bool Hotel::cancelReservation(int id){
    auto res = findReservation(id);
    if(res == nullptr){
        cout << "Reservation " << id << " not cancelled because it does not exist." << endl;
        return false;
    }

    auto r = res->getRoom();
    if(r == nullptr){
        cout << "Reservation " << id << " not cancelled because it's not assigned to any room." << endl;
        return false;
    }

    bool result = r->cancelReservation(id);

    if(result){
        cout << "Reservation " << id << " cancelled." << endl;
        reservations.remove(res);
        delete res;
    }else{
        cout << "Reservation " << id << " can not be cancelled." << endl;
    }

    return result;
}

list<Reservation*> & Hotel::getReservations(){
    return reservations;
}

list<Room*> & Hotel::getRooms(){
    return rooms;
}

Hotel::~Hotel(){}
