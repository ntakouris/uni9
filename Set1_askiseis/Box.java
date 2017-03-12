class Box
{
 // Instance Variables
 double length ;
 double width ;
 double height ;

 // Constructors
 Box ( double side )
 {
 width = side ;
 height = side ;
 length = side ;
 }
 
 Box (double width, double height, double length){
     this.width = width;
     this.height = height;
     this.length = length;
    }
 
 // Methods
 double calculate()
 {
 return 2 * ( width * height +
 width * length +
height * length ) ;
 }
 
 double volume(){
     return width * height * length;
 }
 
 @Override
 public String toString(){
     return "calculate: " + calculate() + " - volume: " + volume();
   }
 
}