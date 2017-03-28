public class SalariedEmployee extends Employee
{
    
    int salary = 0;
    
    void setSalary(int s){
        this.salary = s;
    }
    
    @Override
    public int payment(){
        return this.salary;
    }
    
}
