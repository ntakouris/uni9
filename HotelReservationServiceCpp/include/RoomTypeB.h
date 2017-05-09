#ifndef ROOMTYPEB_H
#define ROOMTYPEB_H

#include "RoomTypeA.h"

class RoomTypeB : public RoomTypeA
{
    public:
        RoomTypeB(int, double, double);
        virtual ~RoomTypeB();
        virtual double totalCost();
        virtual bool cancelReservation();
    protected:
        double discountPerDay;
};

#endif // ROOMTYPEB_H
