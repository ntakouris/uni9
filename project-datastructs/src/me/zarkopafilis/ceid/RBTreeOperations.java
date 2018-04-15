package me.zarkopafilis.ceid;

import me.zarkopafilis.ceid.algo.*;

import java.util.List;

import static me.zarkopafilis.ceid.IOHelper.readIntegersFile;
import static me.zarkopafilis.ceid.IOHelper.scanInt;

public class RBTreeOperations {

    static long time;

    public static void main(String[] args) {
        List<Integer> ints = readIntegersFile();

        RedBlackTree tree = new RedBlackTree();
        time = System.nanoTime();
        for (Integer i : ints) {
            tree.insert(i);
        }
        System.out.println("Time took to construct red black tree: " + (System.nanoTime() - time) + " ns");

        while (true){
            System.out.println("0 -> insert");
            System.out.println("1 -> search");

            int n = scanInt();
            if(n < 0 ){
                break;
            }

            System.out.println("Number> ");

            int x = scanInt();

            switch (n){
                case 0:
                    time = System.nanoTime();
                    tree.insert(x);
                    System.out.println("Time took: " + (System.nanoTime() - time) + " ns");
                    break;
                case 1:
                    time = System.nanoTime();
                    System.out.println("Hops: " + tree.search(x) + " - Time took: " + (System.nanoTime() - time) + " ns");
                    break;
            }
        }
    }

}
