package me.zarkopafilis.ceid;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class IOHelper {
    public static List<Integer> readIntegersFile() {
        String integersFile = "C:\\Users\\Zarkopafilis\\Desktop\\uni9\\uni9\\project-datastructs\\data\\integers.txt";

        /* Read File */
        List<Integer> ints = new ArrayList<>();
        try (BufferedReader br = Files.newBufferedReader(Paths.get(integersFile))) {
            ints = br.lines().map(it -> Integer.parseInt(it)).collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }
        return ints;
    }

    public static List<String> readWordsFile() {
        String integersFile = "C:\\Users\\Zarkopafilis\\Desktop\\uni9\\uni9\\project-datastructs\\data\\words.txt";

        /* Read File */
        List<String> words = new ArrayList<>();
        try (BufferedReader br = Files.newBufferedReader(Paths.get(integersFile))) {
            words = br.lines().collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }
        return words;
    }

    public static int scanInt() {
        return Integer.parseInt(new Scanner(System.in).nextLine());
    }
    public static String scanString() {
        return new Scanner(System.in).nextLine();
    }

}
