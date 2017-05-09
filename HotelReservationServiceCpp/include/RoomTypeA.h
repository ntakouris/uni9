#ifndef ROOMTYPEA_H
#define ROOMTYPEA_H

#include "Room.h"
#include "Reservation.h"

class RoomTypeA : public Room
{
    public:
        RoomTypeA(int, double);
        virtual ~RoomTypeA();
        //This kind of room only accounts duration to calculate totalCost
        virtual double totalCost();
    protected:
        double pricePerDay;
};

#endif // ROOMTYPEA_H
