#include "RoomTypeD.h"

RoomTypeD::RoomTypeD(int cap, double ppPrice, int mDuration) :
    minimumDuration{mDuration},
    Room{cap, ppPrice}
{}

bool RoomTypeD::addReservation(Reservation& reservation){
    if(reservation.getDuration() >= minimumDuration){
        return Room::addReservation(reservation);
    }
    return false;
}

RoomTypeD::~RoomTypeD(){}
