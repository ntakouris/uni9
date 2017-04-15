import java.util.Set;

public class RoomTypeB extends RoomTypeA
{
  protected double discountPerDay;  
    
      public RoomTypeB(int capacity, double pricePerDay, double discountPerDay){
        super(capacity, pricePerDay);
        this.discountPerDay = discountPerDay;
    }
    
  //scalable total cost calculation, with discount per day.
  //per-day cost cant go below 50% of initial per-day price
  //each subsequent day has got more discount till it reaches that minimum price
  @Override
    public double totalCost(){
        double minimumPricePerDay = pricePerDay / 2;
        double totalCost = 0L;
        
        int prevReservationId = -1;
        int dayCount = 0;
        for(Reservation r : availability){
            if(r == null){continue;}
            
            if(prevReservationId != r.getId()){
                prevReservationId = r.getId();
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
    
    @Override
    public boolean cancelReservation(int resId){
        return false;
    }
}
