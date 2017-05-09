#ifndef HOTEL_H
#define HOTEL_H

//If multiple 'vsnprintf' defined
#define __USE_MINGW_ANSI_STDIO 0

#include <iostream>
#include <string>
#include <list>

#include "Reservation.h"
#include "Room.h"

using namespace std;

class Hotel
{
    public:
        Hotel(string);
        virtual ~Hotel();
        //pretty-prints a map of reservations and rooms, in relation to each day of the month
        void reservationCalendar();
        //calculates income for all the rooms
        double calculateIncome();
        //calculates income from only the room which has the specified id
        double calculateIncome(int roomId);
        void addRoom(Room& room);
        //adds reservation to first available room
        int addReservation(Reservation& r);
        //tries to cancel reservation. If success, it prints it on the screen and returns true
        //else it returns false and prints on the screen
        bool cancelReservation(int id);
        //returns a room with id or nullptr
        Room * findRoom(int id);
        //returns a reservation by id or nullptr
        Reservation * findReservation(int id);
        list<Room*> & getRooms();
        list<Reservation*> & getReservations();
    private:
        string name;
        list<Room*> rooms;
        list<Reservation*> reservations;
        //returns digits of some integer 'num'
        //utility method, only used for pretty-print reasons
        int getDigitsNum(int num);
};

#endif // HOTEL_H
