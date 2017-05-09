#include "RoomTypeC.h"

RoomTypeC::RoomTypeC(double ppPrice, int mpCount, int mDuration) :
    minimumPersonCount{mpCount},
    minimumDuration{mDuration},
    Room(mpCount , ppPrice)
{}

bool RoomTypeC::addReservation(Reservation& reservation){
    if(reservation.getPersonCount() >= minimumPersonCount && reservation.getDuration() >= minimumDuration){
        return Room::addReservation(reservation);
    }
    return false;
}

RoomTypeC::~RoomTypeC(){}
