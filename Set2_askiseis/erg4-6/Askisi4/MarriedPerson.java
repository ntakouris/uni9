public class MarriedPerson extends Person {
private int children;
public MarriedPerson(String lastname, String firstname, int age, float
salary, int sex, int children) {
 super(lastname, firstname, age, true, salary, sex);
 this.children = children;
} 
public int getNoOfChildren() { return children; }

@Override
public void printInfo(){
    super.printInfo();
    System.out.println("Married with ");
    if (getNoOfChildren() > 0)
 System.out.print(getNoOfChildren());
 else System.out.print("no");
 System.out.println(" children."); 
}

@Override
public void setMarried(boolean m){
    //tipota
}

public void setSalary(MarriedPerson spouse){
    
    if(spouse.getSex() != getSex()){
          setSalary(getSalary() + spouse.getSalary());
    }
    
}

} 