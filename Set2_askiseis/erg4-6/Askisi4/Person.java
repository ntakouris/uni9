public class Person {
private String lastname;
private String firstname;
private int age;
private boolean married;
private float salary;
    public static final int MALE = 0;
public static final int FEMALE = 1;
private int sex; 
    
public Person (String lastname, String firstname, int age, boolean married,
float salary, int sex) {
 this.lastname = new String(lastname);
 this.firstname = new String(firstname); 
 this.age = age;
 this.married = married;
 this.salary = salary;
 this.sex = sex;
} 

public String getLastName() { return lastname; }
public String getFirstName() { return firstname; }
public int getAge() { return age; }
public boolean isMarried() { return married; }
public float getSalary() { return salary; }
public int getSex(){return sex;}

public void setMarried(boolean m){
       married = m;
}

public void setSalary(float s){
    salary = s;
}


public void printInfo(){
    System.out.print(getFirstName()+" "+getLastName()+" is " + (sex == 0? "Male" : "Female")
     +getAge()+" years old, gets a "+getSalary()
     +" Euros salary and is");
     if (isMarried() == false)
     System.out.print(" not");
     System.out.println(" married.");
}

} 