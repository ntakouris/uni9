public abstract class Employee
{
    String name = "";
    String afm = "";
    long empId;
    
    static long currentId = 0;
    
    public Employee(){
        this.empId = ++currentId;
    }
    
    public void setName(String n){
        this.name = n;
    }
    
    public String getName(){
        return this.name;
    }
    
    public void setAfm(String a){
        this.afm = a;
    }
    
    public long getEmpId(){
        return this.empId;
    }
    
    public abstract int payment();
    
    @Override
    public String toString(){
        return "Employee ID: " + empId + "\n"
            + "Employee Name: " + name + "\n" 
            + "Employee AFM: " + afm + "\n" 
            + "Employee Payment: " + payment(); 
    }
    
}
