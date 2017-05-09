#ifndef ROOMTYPED_H
#define ROOMTYPED_H

#include "Reservation.h"
#include "Room.h"

//This room only has a minimum duration of days
class RoomTypeD : public Room
{
    public:
        RoomTypeD(int, double, int);
        virtual ~RoomTypeD();
        //checks constraints and propagates logic to Room.class
        virtual bool addReservation(Reservation& reservation);
    private:
        int minimumDuration;
};

#endif // ROOMTYPED_H
