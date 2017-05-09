#include "RoomTypeB.h"

RoomTypeB::RoomTypeB(int cap, double ppDay, double dpDay) :
    discountPerDay{dpDay},
    RoomTypeA{cap, ppDay}
{}

bool RoomTypeB::cancelReservation(){
    return false;
}

//scalable total cost calculation, with discount per day.
//per-day cost cant go below 50% of initial per-day price
//each subsequent day has got more discount till it reaches that minimum price
double RoomTypeB::totalCost(){
    double minimumPricePerDay = pricePerDay / 2;
    double totalCost = 0;

    int prevReservationId = -1;
    int dayCount = 0;
    for(int i = 0; i < 30; i ++){
        Reservation * r = availability[i];
        if(r == nullptr){continue;}

        if(prevReservationId != r->getId()){
            prevReservationId = r->getId();
            dayCount = 0;
        }else{
            dayCount++;
        }

        double priceReduction = dayCount * discountPerDay;
        double dayPrice = pricePerDay - priceReduction;
        double finalDayPrice = dayPrice < minimumPricePerDay ? minimumPricePerDay : dayPrice;

        totalCost += finalDayPrice;
    }
    return totalCost;
}

RoomTypeB::~RoomTypeB(){}
