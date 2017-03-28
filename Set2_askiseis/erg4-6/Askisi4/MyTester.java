import java.util.*;

abstract class MyTester {
 public static void main(String[] args) {
 MarriedPerson mp1 = new MarriedPerson("Tront", "Betty", 31, 980.5f,
Person.FEMALE, 3);
MarriedPerson mp2 = new MarriedPerson("Tront", "Kirk", 31, 2080f,
Person.MALE, 2);
MarriedPerson mp3 = new MarriedPerson("Tront", "Sonia", 31, 600f,
Person.FEMALE, 0);
mp1.printInfo();
mp2.printInfo();
mp3.printInfo();
mp1.setSalary(mp2);
mp1.printInfo();
mp1.setSalary(mp3);
mp1.printInfo();
mp1.setSalary(mp2.getSalary());
mp1.printInfo(); 


ArrayList<MarriedPerson> mpList = new ArrayList<>();
mpList.add(mp1);
mpList.add(mp2);

for(MarriedPerson p : mpList){
    p.printInfo();
}


}
} 