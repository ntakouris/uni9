package me.zarkopafilis.ceid;

import me.zarkopafilis.ceid.algo.*;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import static me.zarkopafilis.ceid.IOHelper.readIntegersFile;

public class Benchmarks {
    static long time;

    static long worse;
    static long mean;

    /* Test Integer set for benchmarks */
    static Collection<Integer> elements;

    public static void main(String[] args) {
        populateElements();
        List<Integer> ints = readIntegersFile();

        RedBlackTree tree = new RedBlackTree();
        time = System.nanoTime();
        for (Integer i : ints) {
            tree.insert(i);
        }
        System.out.println("Time took to construct red black tree: " + (System.nanoTime() - time) + " ns");



        return;
//
//        Integer[] arr = new Integer[ints.size()];
//        ints.toArray(arr);
//        MergeSort mergeSort = new MergeSort(arr, 0, ints.size() - 1);
//
//        time = System.nanoTime();
//        mergeSort.start();
//        System.out.println("Time took to merge sort: " + (System.nanoTime() - time) + " ns");
//
//        /* Check result validity */
//        Stream.of(mergeSort.getArray()).reduce((last, curr) -> {
//            if (last > curr) {
//                throw new AssertionError("MergeSort did not sort elements");
//            }
//            return curr;
//        });
//
//        SearchAlgo linear = new LinearSearch();
//        SearchAlgo binary = new BinarySearch();
//        SearchAlgo interp = new InterpolationSearch();
//
//        benchmark("Linear Search", linear, mergeSort.getArray());
//        benchmark("Binary Search", binary, mergeSort.getArray());
//        benchmark("Interpolation Search", interp, mergeSort.getArray());
    }

    private static synchronized void benchmark(String searchName, SearchAlgo alg, Integer[] array) {
        worse = 0;
        mean = 0;

        for (Integer integer : elements) {
            time = System.nanoTime();
            alg.search(array, integer);
            long diff = System.nanoTime() - time;

            if (diff > worse) {
                worse = diff;
            }

            if (mean == 0) {
                mean = diff;
            }else{
                mean = (mean + diff) / 2;
            }
        }

        System.out.println("> " + searchName);
        System.out.println(">    > Mean Time: " + mean + " ns");
        System.out.println(">    > Worse Time: " + worse + " ns");
        System.out.println();
        System.out.println();
    }

    private static void populateElements() {
        elements = new ArrayList<>(10000);
        for(int i = 0; i < 100000; i++) {
            elements.add(i);
        }
    }

}
