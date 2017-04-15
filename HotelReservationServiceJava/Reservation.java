
public class Reservation
{
    
    private static int idCount = 1;
    
    String clientName;
    int id;
    int arrival;
    int duration;//days
    int personCount;
    Room room;
    
    public Reservation(String clientName, int arrival, int duration, int personCount){
        this.clientName = clientName;
        this.arrival = arrival;
        this.duration = duration;
        this.personCount = personCount;
        
        this.id = idCount++;
    }
    
    public String getClientName(){
        return clientName;
    }
    
    public void setRoom(Room room){
        this.room = room;
    }
    
    public int getArrival(){
        return arrival;
    }
    
    public int getDuration(){
        return duration;
    }
    
    public int getPersonCount(){
        return personCount;
    }
    
    public int getId(){
        return id;
    }
    
    public Room getRoom(){
        return room;
    }
    
}
