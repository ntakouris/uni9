#include "RoomTypeE.h"

RoomTypeE::RoomTypeE(int cap, double ppPrice, int mpCount) :
    minimumPersonCount{mpCount},
    Room{cap, ppPrice}
{}

bool RoomTypeE::addReservation(Reservation& reservation){
    if(reservation.getPersonCount() >= minimumPersonCount){
        return Room::addReservation(reservation);
    }
    return false;
}

RoomTypeE::~RoomTypeE(){}
