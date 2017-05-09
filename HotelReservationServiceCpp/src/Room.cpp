#include "Room.h"
#include "Reservation.h"

int Room::idCount = 1;

Room::Room(int cap, double ppPrice) :
    capacity{cap},
    perPersonPrice{ppPrice}
    {
        this->id = idCount++;
    }

int Room::getId(){
    return this->id;
}


Reservation * (&Room::getReservations())[30]{
    return availability;
}

bool Room::addReservation(Reservation& reservation){
    bool result = true;

    for(int i = reservation.getArrival() - 1; i < reservation.getArrival() + reservation.getDuration() - 1; i++){
        result &= (availability[i] == nullptr);
    }
    if(result){
        for(int i = reservation.getArrival(); i < reservation.getArrival() + reservation.getDuration() - 1; i++){
            availability[i] = &reservation;
        }
    }
    return result;
}

bool Room::cancelReservation(int resId){
    for(int i = 0; i < 30; i++){
        if(availability[i] != nullptr && availability[i]->getId() == resId){
            availability[i] = nullptr;
        }
    }
    return true;
}

double Room::totalCost(){
    double cost = 0;
    for(int i = 0; i < 30; i++){
        if(availability[i] != nullptr){
            cost += perPersonPrice * availability[i]->getPersonCount();
        }
    }
    return cost;
}

int Room::completenessPercentage(){
    int nullEntries = 0;
    for(int i = 0; i < 30; i++){
        if(availability[i] == nullptr){
            nullEntries++;
        }
    }
    return (30 - nullEntries) * (10 / 3);
}

Room::~Room(){}
