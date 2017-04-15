//This room only has a minimum person count
public class RoomTypeE extends Room
{

    int minimumPersonCount;
    
    public RoomTypeE(int capacity, double perPersonPrice, int minimumPersonCount){
        super(capacity, perPersonPrice);
        this.minimumPersonCount = minimumPersonCount;
    }
    
    //checks person count and propagates logic to Room.class
    @Override
    public boolean addReservation(Reservation res){
        if(res.getPersonCount() >= minimumPersonCount){
            return super.addReservation(res);
        }
        return false;
    }
    
}
