public class Spirtokouto extends Box
{
    double weight;
    
   Spirtokouto(double width, double height, double length, double weight){
       super(width, height, length);
       this.weight = weight;
    }
    
    @Override
    public String toString(){
        return "calculate(): " + calculate() + " - volume(): " + volume() + " - weight: " + weight; 
    }
    
}
