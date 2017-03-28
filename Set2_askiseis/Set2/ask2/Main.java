import java.util.*;

public class Main
{
    
    public static void main(String[] args){
        
       Scanner s = new Scanner(System.in);
        
        try{
            
        int x = s.nextInt();
        int y = s.nextInt();
        int z = s.nextInt();
        
        System.out.println(
        (3 * (x-1) * y) / (x - 2 * z)
        );
        
        }catch(NumberFormatException e){
            System.out.println("Input is not a number");
            System.exit(1);
        }catch(ArithmeticException e){
            System.out.println("Paranomastis 0");
        }
        
    }
    
}
