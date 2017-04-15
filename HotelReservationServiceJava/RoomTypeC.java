//This room has both a minimum person and day number constaint
public class RoomTypeC extends Room
{

    int minimumPersonCount;
    int minimumDuration;
    
    public RoomTypeC(double perPersonPrice, int minimumPersonCount, int minimumDuration){
        super();
        this.perPersonPrice = perPersonPrice;
        this.minimumPersonCount = minimumPersonCount;
        this.minimumDuration = minimumDuration;
    }
    
    //checks constraints and propagates logic to Room.class
    @Override
    public boolean addReservation(Reservation res){
        if(res.getPersonCount() >= minimumPersonCount && res.getDuration() >= minimumDuration){
            return super.addReservation(res);
        }
        return false;
    }
    
}
