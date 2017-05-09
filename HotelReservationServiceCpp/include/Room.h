#ifndef ROOM_H
#define ROOM_H

//forward declaration for cyclic dependency
class Reservation;

class Room
{
    public:
        Room(int, double);
        virtual ~Room();
        int getId();
        Reservation * (&getReservations())[30];
        //Adds reservation only if there are available slots for the duration
        virtual bool addReservation(Reservation& reservation);
        //remove reservation from availability array
        virtual bool cancelReservation(int resId);
        //returns total cost of all reservations on availability array
        virtual double totalCost();
        //Returns completeness percentage of availability array
        int completenessPercentage();
    private:
        //keep track of assigned ids (initialized to 1)
        static int idCount;
        int id;
    protected:
        //One slot for each day of month
        Reservation * availability[30] = {nullptr};
        int capacity;
        double perPersonPrice;
};

#endif // ROOM_H
