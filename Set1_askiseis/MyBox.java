public class MyBox extends Box
{
    
    MyBox(double size){
        super(size, size, size);
    }
    
    MyBox(double width , double height, double length){
        super(width, height, length);
        if(width != height || height != length || width != length){
            System.out.println("Mh epitreptes times");
            throw new IllegalArgumentException();
        }
    }
    
    /* calculate() and volume() den xreiazetai overriden ylopoihsh: doulevoun genika gia 
       koutia ara kai gia kyboys.*/
    
    @Override
    public String toString(){
        return "Cube " + super.toString();
    }
}
