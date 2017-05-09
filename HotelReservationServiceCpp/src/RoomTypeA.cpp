#include "RoomTypeA.h"

RoomTypeA::RoomTypeA(int cap, double ppDay) :
    pricePerDay{ppDay},
    Room(cap, 0)
{}

double RoomTypeA::totalCost(){
    double cost = 0;
    for(int i = 0; i < 30; i++){
        if(availability[i] != nullptr){
            cost += pricePerDay;
        }
    }
    return cost;
}

RoomTypeA::~RoomTypeA(){}
