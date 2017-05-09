#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include "Room.h"

class Reservation
{
    public:
        Reservation(std::string, int, int, int);
        virtual ~Reservation();
        std::string getClientName();
        void setRoom(Room& room);
        int getArrival();
        int getDuration();
        int getPersonCount();
        int getId();
        Room * getRoom();
    private:
        static int idCount;
        std::string clientName;
        int id;
        int arrival;
        int duration;
        int personCount;
        Room * room;
};

#endif // RESERVATION_H
