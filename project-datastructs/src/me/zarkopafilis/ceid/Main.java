package me.zarkopafilis.ceid;

import static me.zarkopafilis.ceid.IOHelper.scanInt;

public class Main {

    public static void main(String[] args){
        System.out.println("Make sure you've set the path to the words and ints files.");
        System.out.println("Merge Sort Searches -> 0");
        System.out.println("Trie -> 1");
        System.out.println("RedBlackTree -> 2");
        System.out.println("Benchmarks -> 3");

        switch (scanInt()) {
            case 0:
                UserSearches.main(args);
                break;
            case 1:
                DigitalTreeOperations.main(args);
                break;
            case 2:
                RBTreeOperations.main(args);
                break;
            case 3:
                Benchmarks.main(args);
                break;
        }
    }
}
