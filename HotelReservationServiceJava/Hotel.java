import java.util.*;

public class Hotel
{
    String name;
    List<Room> rooms = new ArrayList<>();
    List<Reservation> reservations = new ArrayList<>();
    
    //pretty-prints a map of reservations and rooms, in relation to each day of the month
    public void reservationCalendar(){
        //Δωμάτιο 01 02 03 ..
        String dom = "Δωμάτιο    ";
        System.out.print(dom);
        for(int i = 1; i <= 30; i++){
            System.out.print((i < 10 ? "0" : "") + i + " ");
        }
        
        int roomCountDigits = getDigitsNum(rooms.size());
        
        for(Room room : rooms){
            System.out.println();
            //[[RoomId]] (spaces)
            int spacePostfix = dom.length() - Integer.toString(room.getId()).length();
            String spaces = "";
            for(int k = 0;k < spacePostfix;k++){
                spaces += " ";
            }
            
            System.out.print(room.getId() + spaces);
            
            // _ * _ * _
            for(Reservation res : room.getReservations()){
                if(res == null){
                    System.out.print("_  ");
                }else{
                    System.out.print("*  ");
                }
            }
        }    
    }
    
    //returns digits of some integer 'num'
    //utility method, only used for pretty-print reasons
    private int getDigitsNum(int num){
        return String.valueOf(num).length();
    }
    
    //calculates income from only the room which has the specified id
    public double calculateIncome(int roomId){
        return rooms.stream().filter(x -> x.getId() == roomId).mapToDouble(x -> x.totalCost()).sum();
    }
    
    //calculates income from all the rooms
    public double calculateIncome(){
        return rooms.stream().mapToDouble(x -> x.totalCost()).sum();
    }
    
    public void addRoom(Room r){
        rooms.add(r);
    }
    
    //adds reservation to first available room
    public int addReservation(Reservation r){
        for(Room room : rooms){
            if(room.addReservation(r)){
                reservations.add(r);
                r.setRoom(room);
                return room.getId();
            }
        }
        return 0;
    }
    
    //tries to cancel reservation. If success, it prints it on the screen and returns true
    //else it returns false and prints on the screen
    public boolean cancelReservation(int id){
        Reservation res = findReservation(id);
        if(res == null){
            System.out.println("Reservation " + id + " not cancelled because it does not exist.");
            return false;
        }
        
        Room r = res.getRoom();
        if(r == null){
            System.out.println("Reservation " + id + " not cancelled because it's not assigned to any room.");
            return false;
        }
        
        boolean result = r.cancelReservation(id);
        
        if(result){
            System.out.println("Reservation " + id + " cancelled.");
        }else{
            System.out.println("Reservation " + id + " can not be cancelled.");
        }
        
        return result;
    }
    
    //returns a room with id or null
    public Room findRoom(int id){
        Optional<Room> r = rooms.stream().filter(x -> x.getId() == id).findFirst();
        if(r.isPresent()){
            return r.get();
        }
        return null;
    }
    
    //returns a reservation by id or null
    public Reservation findReservation(int id){
        Optional<Reservation> r = reservations.stream().filter(x -> x.getId() == id).findFirst();
        if(r.isPresent()){
            return r.get();
        }
        return null;
    }
    
    public List<Room> getRooms(){
        return rooms;
    }
    
    public List<Reservation> getReservations(){
        return reservations;
    }
    
}
