public class HourlyEmployee extends Employee
{
    int hoursWorked = 0;
    int hourlyPayment = 0;
    
    public void setHoursWorked(int hw){
        this.hoursWorked = hw;
    }
    
    public int getHoursWorked(){
        return this.hoursWorked;
    }
    
    public void setHourlyPayment(int hp){
        this.hourlyPayment = hp;
    }
    
    public int getHourlyPayment(){
        return hourlyPayment;
    }
    
    @Override
    public int payment(){
        return hoursWorked * hourlyPayment;
    }
    
}
