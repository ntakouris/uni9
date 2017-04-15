import java.util.Arrays;
import java.util.stream.Stream;

public class RoomTypeA extends Room
{
    protected double pricePerDay;
    
    public RoomTypeA(int capacity, double pricePerDay){
        super();
        this.capacity = capacity;
        this.pricePerDay = pricePerDay;
    }
    
    //This kind of room only accounts duration to calculate totalCost
    @Override
    public double totalCost(){
        return Arrays.stream(availability).filter(x -> x != null)
                        .mapToDouble(x -> pricePerDay).sum();
    }
}
