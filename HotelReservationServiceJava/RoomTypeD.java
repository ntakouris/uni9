//This room only has a minimum duration of days
public class RoomTypeD extends Room
{

    int minimumDuration;
    
    public RoomTypeD(int capacity, double perPersonPrice, int minimumDuration){
        super(capacity, perPersonPrice);
        this.minimumDuration = minimumDuration;
    }
    
    //check duration and propagate logic to Room.class
    @Override
    public boolean addReservation(Reservation res){
        if(res.getDuration() >= minimumDuration){
            return super.addReservation(res);
        }
        return false;
    }
    
}
