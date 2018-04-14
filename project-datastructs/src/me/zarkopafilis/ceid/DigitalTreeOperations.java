package me.zarkopafilis.ceid;

import me.zarkopafilis.ceid.algo.*;

import java.util.List;

import static me.zarkopafilis.ceid.IOHelper.*;

public class DigitalTreeOperations {

    static long time;

    public static void main(String[] args) {
        List<String> words = readWordsFile();
        DigitalTree dt = new DigitalTree();
        time = System.nanoTime();
        for (String w : words) {
            dt.insert(w);
        }

        System.out.println("Time took to build digital tree: " + (System.nanoTime() - time) + " ns");

        while (true){
            System.out.println("0 -> search");
            System.out.println("1 -> add");
            System.out.println("2 -> remove");

            int n = scanInt();

            System.out.println("Enter word> ");
            String w = scanString();

            switch (n){
                case 0:
                    time = System.nanoTime();
                    System.out.println(dt.search(w) ? "exists" : "not found");
                    System.out.println("Took: " + (System.nanoTime() - time) + " ns");
                    break;
                case 1:
                    time = System.nanoTime();
                    dt.insert(w);
                    System.out.println("Took: " + (System.nanoTime() - time) + " ns");
                    break;
                case 2:
                    time = System.nanoTime();
                    dt.delete(w);
                    System.out.println("Took: " + (System.nanoTime() - time) + " ns");
                    break;
            }

        }

    }
}
