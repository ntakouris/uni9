public class DivideByZeroException extends ArithmeticException {
 public DivideByZeroException() {}
 public DivideByZeroException(String msg) {super(msg);}
 
 @Override
 public String toString() {
return "DivideByZeroException: The denominator cannot be zero.";
} 
}