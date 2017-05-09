#include "Reservation.h"

int Reservation::idCount = 1;

Reservation::Reservation(std::string cName, int arr, int dur, int pCount) :
    clientName{cName},
    arrival{arr},
    duration{dur},
    personCount{pCount} {
        this->id = idCount++;
}

int Reservation::getId(){
    return this->id;
}

void Reservation::setRoom(Room& room){
    this->room = &room;
}

int Reservation::getArrival(){
    return this->arrival;
}

std::string Reservation::getClientName(){
    return clientName;
}

int Reservation::getDuration(){
    return this->duration;
}

Room * Reservation::getRoom(){
    return this->room;
}

int Reservation::getPersonCount(){
    return this->personCount;
}

Reservation::~Reservation() {}
