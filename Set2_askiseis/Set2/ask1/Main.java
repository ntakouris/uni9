import java.util.*;

public class Main
{
    
    public static void main(String[] args){
        
        List<Employee> es = new ArrayList<>();
        
        SalariedEmployee e1 = new SalariedEmployee();
        
        e1.setName("Tzortzis");
        e1.setAfm("213123112");
        
        e1.setSalary(3000);
        
        HourlyEmployee e2 = new HourlyEmployee();
        
        e2.setName("Zina");
        e2.setAfm("74576547");
        
        e2.setHoursWorked(210);
        e2.setHourlyPayment(20);
        
        es.add(e1);
        es.add(e2);
        
        for(Employee e: es){
            System.out.println(e);
        }
        
    }
    
}
