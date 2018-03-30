package me.zarkopafilis.ceid;

import me.zarkopafilis.ceid.algo.MergeSort;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    public static void main(String[] args) {
        String integersFile = "C:\\Users\\Zarkopafilis\\Desktop\\uni9\\uni9\\project-datastructs\\data\\integers.txt";

        List<Integer> ints = new ArrayList<>();
        try (BufferedReader br = Files.newBufferedReader(Paths.get(integersFile))) {
            //br returns as stream and convert it into a List
            ints = br.lines().map(it -> Integer.parseInt(it)).collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }

        Integer[] arr = new Integer[ints.size()];
        ints.toArray(arr);
        MergeSort mergeSort = new MergeSort(arr, 0, ints.size() - 1);
        mergeSort.start();

        Stream.of(mergeSort.getArray()).reduce((last, curr) -> {
            if (last > curr) {
                throw new AssertionError("MergeSort did not sort elements");
            }
            return curr;
        });

    }
}
