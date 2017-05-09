#ifndef ROOMTYPEC_H
#define ROOMTYPEC_H

#include "Reservation.h"
#include "Room.h"

//This room has both a minimum person and day number constaint
class RoomTypeC : public Room
{
    public:
        RoomTypeC(double, int, int);
        virtual ~RoomTypeC();
        //checks constraints and propagates logic to Room.class
        virtual bool addReservation(Reservation& reservation);
    private:
        int minimumPersonCount;
        int minimumDuration;
};

#endif // ROOMTYPEC_H
