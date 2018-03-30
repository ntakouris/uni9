package me.zarkopafilis.ceid;

import me.zarkopafilis.ceid.algo.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    static long time;

    public static void main(String[] args) {
        String integersFile = "C:\\Users\\Zarkopafilis\\Desktop\\uni9\\uni9\\project-datastructs\\data\\integers.txt";

        /* Read File */
        List<Integer> ints = new ArrayList<>();
        try (BufferedReader br = Files.newBufferedReader(Paths.get(integersFile))) {
            ints = br.lines().map(it -> Integer.parseInt(it)).collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }

        Integer[] arr = new Integer[ints.size()];
        ints.toArray(arr);
        MergeSort mergeSort = new MergeSort(arr, 0, ints.size() - 1);

        time = System.nanoTime();
        mergeSort.start();
        System.out.println("Time took to merge sort: " + (System.nanoTime() - time) + " ns");

        /* Check result validity */
        Stream.of(mergeSort.getArray()).reduce((last, curr) -> {
            if (last > curr) {
                throw new AssertionError("MergeSort did not sort elements");
            }
            return curr;
        });

        while (true){
            System.out.println("0 -> continue");
            System.out.println("1 -> Linear Serach");
            System.out.println("2 -> Binary Search");
            System.out.println("3 -> Interpolation Search");

            int n = scanInt();
            if(n < 0 ){
                break;
            }

            SearchAlgo alg = null;
            switch (n){
                case 1:
                    alg = new LinearSearch();
                    break;
                case 2:
                    alg = new BinarySearch();
                    break;
                case 3:
                    alg = new InterpolationSearch();
                    break;
            }

            System.out.println("Enter int to search: ");
            int x = scanInt();

            time = System.nanoTime();
            int pos = alg.search(mergeSort.getArray(), x); //-1 means not found
            System.out.println("Position: " + pos + " - Time took: " + (System.nanoTime() - time) + " ns");
        }

    }

    private static int scanInt() {
        return Integer.parseInt(new Scanner(System.in).nextLine());
    }
}
