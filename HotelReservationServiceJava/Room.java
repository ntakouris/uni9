
import java.util.Arrays;
import java.util.stream.Stream;

public class Room
{
    //keeps track of assigned ids
    private static int idCount = 1;
    
    int id;
    int capacity;
    //One slot for each day of the month
    Reservation[] availability = new Reservation[30];
    double perPersonPrice;
    
    public Room(){
        this.id = idCount++;
    }
    
    public Room(int capacity, double perPersonPrice){
        this();
        this.capacity = capacity;
        this.perPersonPrice = perPersonPrice;
    }
    
    public int getId(){
        return id;
    }
    
    public Reservation[] getReservations(){
        return availability;
    }
    
    //Adds reservation only if there are available slots for the duration
    public boolean addReservation(Reservation res){
        boolean result = true;
        
        for(int i = res.getArrival() - 1; i < res.getArrival() + res.getDuration() - 1; i++){
            result &= (availability[i] == null);
        }
        if(result){
            for(int i = res.getArrival(); i < res.getArrival() + res.getDuration() - 1; i++){
                availability[i] = res;
            }
        }
        return result;
    }
    
    //remove reservation from availability array
    public boolean cancelReservation(int resId){
        for(int i = 0; i < 30; i++){
            if(availability[i] != null && availability[i].getId() == resId){
                availability[i] = null;
            }
        }
        return true;
    }
    
    //Returns completeness percentage of availability array
    public int completenessPercentage(){
        return Arrays.stream(availability).filter(x -> x != null)
                        .mapToInt(x -> 1).sum() * 10 / 3;
    }
    
    
    //returns total cost of all reservations on availability array
    public double totalCost(){
        return Arrays.stream(availability).filter(x -> x != null)
                        .mapToDouble(x -> perPersonPrice * x.getPersonCount()).sum();
    }
    
}
