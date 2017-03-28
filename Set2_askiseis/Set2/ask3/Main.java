import java.io.*;
import java.util.*;

public class Main
{

    public static void main(String[] args) throws Exception{  
        File src = new File("src.txt");
        
        long srcBytes = src.length();
        
        if(!src.exists()){
            throw new FileNotFoundException("src.txt not found");
        }
        
        File out = new File("output.txt");
        if(out.exists()){
            out.delete();
        }
        
        out.createNewFile();
        
        int spaces = 0;
        long outInitial = out.length();
        
        FileReader reader = new FileReader(src);
        
        BufferedReader bufferedReader = new BufferedReader(reader);

        String contents = "";

        String line;
        while ((line = bufferedReader.readLine()) != null)
        {
            contents += line + "\n\n";
        }
      
        for (char c : contents.toCharArray()) {
            if (c == ' ') {
                 spaces++;
            }
        }
        
        
        contents = contents.replaceAll(" ", "@");
        
        bufferedReader.close();
        
        FileWriter writer = new FileWriter(out); 

        writer.write(contents); 
        writer.flush();
        
        writer.close();
        
        long outFinal = out.length();
        
        System.out.println("Spaces: " + spaces);
        System.out.println("Src: " + srcBytes);
        System.out.println("Initial Out: " + outInitial);
        System.out.println("Final Out: " + outFinal);
    }
    
}
