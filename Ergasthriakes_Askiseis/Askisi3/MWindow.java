public class MWindow extends Window
{
    
    protected int size = 7; 
 private String message = "No message";

 public MWindow(String message)
 {
 size = 2;
 this.message = message;
 System.out.println ("Window message = " + message);
 }

 public MWindow(int size, String message)
{
 super(size);
 this.message = message;
 System.out.println ("Window message = " + message);
 } 

 public void setSize1(int y)
 {size = y;}

 public void setSize2(int z)
{super.setSize (z);}

 public void printSize()
{System.out.println ("MSize="+size);}

public void printSize1()
{System.out.println(((Window)this).size);}

public void printSize2()
{((Window)this).printSize();} 

}