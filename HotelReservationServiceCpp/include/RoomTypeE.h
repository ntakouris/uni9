#ifndef ROOMTYPEE_H
#define ROOMTYPEE_H

#include "Reservation.h"
#include "Room.h"

//This room only has a minimum person count
class RoomTypeE : public Room
{
    public:
        RoomTypeE(int, double, int);
        virtual ~RoomTypeE();
        //checks constraints and propagates logic to Room.class
        virtual bool addReservation(Reservation& reservation);
    private:
        int minimumPersonCount;
};

#endif // ROOMTYPEE_H
