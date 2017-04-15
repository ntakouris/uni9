import java.util.*;

public class Application
{
    final static int randomCancelThresholdPercentage = 25;
    
    final static String[] possibleClientNames = {"Τακις", "Καλαμαρης", "Μπομπ", "Κυριος Καβουρης"};
    
    final static int arrivalHigh = 29;
    final static int arrivalLow = 1; // >= 1
    
    final static int maxDurationTryOverride = 5;
    final static int minDurationTryOverride = 2; // >= 1
    
    final static int personCountHigh = 3;
    final static int personCountLow = 1; // >= 1
    
    public static void main(String[] args){
        Hotel hotel = new Hotel();
        Random r = new Random();
        
        //Initialize and add rooms (minimum 10)
        for(int j=0;j<3;j++){
            Room monoklino = new Room(1, 30);
            hotel.addRoom(monoklino);
        }
        
        for(int j=0;j<3;j++){
            Room diklino = new Room(2, 30);
            hotel.addRoom(diklino);
        }
        
        for(int j=0;j<2;j++){
            Room triklino = new Room(3, 30);
            hotel.addRoom(triklino);
        }
        
        for(int j=0;j<2;j++){
            Room souita = new RoomTypeA(4, 200);
            hotel.addRoom(souita);
        }
        
        for(int j=0;j<2;j++){
            Room souitaPromo = new RoomTypeB(4, 200, 50);
            hotel.addRoom(souitaPromo);
        }
        
        Room synedriakh = new RoomTypeC(20, 5, 2);
        hotel.addRoom(synedriakh);
        
        for(int j=0;j<2;j++){
            Room diklinoPromo = new RoomTypeD(2, 25, 3);
            hotel.addRoom(diklinoPromo);
        }
        
        for(int j=0;j<2;j++){
            Room diklinoRatzo = new RoomTypeE(3, 20, 2);
            hotel.addRoom(diklinoRatzo);
        }
        
        Scanner scanner = new Scanner(System.in);
        for(;;){
            System.out.println("Creating random reservation...");
            //randomly create and assign a reservation
            String randomClientName = possibleClientNames[r.nextInt(possibleClientNames.length)];
            System.out.println("Client Name: " + randomClientName);
            int randomArrival = r.nextInt(arrivalHigh - arrivalLow) + arrivalLow;
            System.out.println("Arrival: " + randomArrival);
            int maxDurationLimit = 30 - randomArrival;
            
            //We need bound safety, that's why we try overrides
            maxDurationLimit = maxDurationTryOverride > maxDurationLimit ? maxDurationLimit : maxDurationTryOverride;
            int minDuration = minDurationTryOverride > maxDurationLimit ? 1 : minDurationTryOverride;
            
            int randomDuration = 1;
            int randomDurationBound = maxDurationLimit - minDuration;
            if(randomDurationBound > 0){
                randomDuration = r.nextInt(randomDurationBound) + minDuration;
            }
            
            System.out.println("Duration: " + randomDuration);
            
            int randomPersonCount = r.nextInt(personCountHigh - personCountLow) + personCountLow;
            System.out.println("Person Count: " + randomPersonCount);
            
            Reservation randRes = new Reservation(randomClientName, randomArrival, randomDuration, randomPersonCount);
            int randRoomId = hotel.addReservation(randRes);
            
            if(randRoomId == 0){System.out.println("No room fitting the random reservation!");}else{System.out.println("Reservation added at room: " + randRoomId);}
            
            //25% chance to cancel reservation
            boolean shouldCancelRandomReservation = r.nextInt(100) > randomCancelThresholdPercentage;
            if(shouldCancelRandomReservation){
                System.out.println("picking out a random reservation to cancel...");
                Reservation randomReservation = hotel.getReservations().get(r.nextInt(hotel.getReservations().size()));
                boolean result = randomReservation.getRoom().cancelReservation(randomReservation.getId());
                if(result){
                    System.out.println("Reservation " + randomReservation.getId() + " cancelled successfully.");
                }else{
                    System.out.println("Reservation " + randomReservation.getId() + " could not be cancelled.");
                }
            }
            
            //User interaction
            int selection = -1;
            while(selection != 1){
                System.out.println("           ");
                System.out.println("           ");
                System.out.println("[1] Next Iteration");
                System.out.println("[2] Add Reservation");
                System.out.println("[3] Cancel Reservation");
                System.out.println("[4] View Reservations");
                System.out.println("[5] View Rooms");
                System.out.println("[6] View Reservation Calendar");
                System.out.println("[7] View Income");
                System.out.println("[8] Terminate Program");
                System.out.println("           ");
                System.out.println("[*] > ");
                //validate input
                do{
                    try{
                        selection = scanner.nextInt();
                    }catch (InputMismatchException exception) {
                        System.out.println("Integers only, please.");
                        System.out.println("[*] > ");
                        scanner.nextLine();
                    }
                }while (selection == -1);
                
                switch(selection){
                    case 2: //new reservation
                        System.out.println("New Reservation Wizard");
                        //name
                        scanner.nextLine();//discard '\n' from integer scan
                        System.out.println("Client Name: ");
                        String name = scanner.nextLine();
                        //arrival day number
                        System.out.println("Arrival Day: ");
                        int arrival = -1;
                        try{
                            arrival = scanner.nextInt();
                            
                            if(arrival <= 0 || arrival > 29){
                                throw new InputMismatchException();
                            }
                        }catch (InputMismatchException exception) {
                                System.out.println("Input Mismatch, cancelling new reservation...");
                                scanner.nextLine();
                                break;
                        }
                        //days
                        System.out.println("Duration (in days): ");
                        int duration = -1;
                        try{
                            duration = scanner.nextInt();
                            
                            if(duration <= 0 || arrival + duration > 30){
                                throw new InputMismatchException();
                            }
                        }catch (InputMismatchException exception) {
                                System.out.println("Input Mismatch, cancelling new reservation...");
                                scanner.nextLine();
                                break;
                        }
                        //number of persons
                        System.out.println("Number of persons: ");
                        int personCount = -1;
                        try{
                            personCount = scanner.nextInt();
                            
                            if(personCount <= 0){
                                throw new InputMismatchException();
                            }
                        }catch (InputMismatchException exception) {
                                System.out.println("Input Mismatch, cancelling new reservation...");
                                scanner.nextLine();
                                break;
                        }
                        
                        //optional: room id
                        System.out.println("Number of persons: ");
                        int roomId = -1;
                        try{
                            roomId = scanner.nextInt();
                        }catch (InputMismatchException exception) {
                                scanner.nextLine(); //nothing wrong though, just cleanup
                        }
                        Reservation res = new Reservation(name, arrival,duration, personCount);
                        
                        if(roomId >= 0){
                            Room targetRoom = hotel.findRoom(roomId);
                            if(targetRoom == null){
                                System.out.println("Target Room does not exist, cancelling reservation...");
                                break;
                            }
                            boolean result = targetRoom.addReservation(res);
                            if(result){
                                System.out.println("Reservation id: " + res.getId() + " - Room id: " + roomId);
                            }else{
                                System.out.println("Room full, can't add reservation!");
                            }
                        }else{
                            System.out.println("Invalid room id, selecting first available room...");
                            int rId = hotel.addReservation(res);
                            if(roomId == 0){
                                System.out.println("Hotel full, can't add reservation!");
                            }else{
                                System.out.println("Reservation id: " + res.getId() + " - Room id: " + rId);
                            }
                        }
                        break;
                    case 3:
                        //validate input
                        int resId = -1;
                        do{
                            System.out.println("Specify Reservation id");
                            System.out.println("[*] > ");
                            try{
                                resId = scanner.nextInt();
                            }catch (InputMismatchException exception) {
                                System.out.println("Positive integers only, please.");
                                scanner.nextLine();
                            }
                        }while (resId <= 0);
                        hotel.cancelReservation(resId);
                        break;
                    case 4: //view reservations (id | client name | room id)
                        System.out.println("           ");
                        for(Reservation reservation : hotel.getReservations()){
                            System.out.println(reservation.getId() + "| " + reservation.getClientName() + " | " + reservation.getRoom().getId());
                        }
                        break;
                    case 5: //view rooms (id | completeness percentage | income)
                        System.out.println("           ");
                        for(Room room : hotel.getRooms()){
                            System.out.println(room.getId() + "| " + room.completenessPercentage() + "% | " + room.totalCost());
                        }
                        break;
                    case 6: // pretty-print reservation calendar
                        hotel.reservationCalendar();
                        break;
                    case 7: //view income
                        try{
                            System.out.println("[Optional] Specify Room Id or -1 ");
                            System.out.println("[*] > ");
                            int id = scanner.nextInt();
                            
                            if(id == -1){
                                throw new InputMismatchException();
                            }
                            
                            if(hotel.findRoom(id) == null){
                                System.out.println("Room of id " + id + " does not exist!");
                                break;
                            }
                            System.out.println("Income for room id " + id + " = " + hotel.calculateIncome(id));
                        }catch (InputMismatchException exception){
                            //User did not specify room id
                            System.out.println("Total income for all rooms = " + hotel.calculateIncome());
                        }
                        break;
                    case 8: //Normal exit, that's why 0 and no break
                        System.exit(0);
                    default: //covers '1' too
                        break;
                }
            }
        }
    }
    
}
