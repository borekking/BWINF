package de.flo.zzz.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Utility class containing general utilities
 */
public class JavaUtils {

    /**
     * Private constructor, s.t. this class can not have objects
     */
    private JavaUtils() {
    }

    /**
     * Function for reading a file and returning its content as List of String
     * @param file The file
     * @return The file's content as List of Strings
     * @throws FileNotFoundException If the file wasn't found
     */
    public static List<String> readFile(File file) throws FileNotFoundException {
        List<String> list = new ArrayList<>();

        Scanner sc = new Scanner(file);
        while (sc.hasNext()) list.add(sc.nextLine());

        return list;
    }

    /**
     * Function for converting a list of booleans to an array of booleans
     * @param list The list of booleans
     * @return The array of booleans
     */
    public static boolean[] toPrimitiveArray(List<Boolean> list) {
        int n = list.size();
        boolean[] bools = new boolean[n];

        for (int i = 0; i < n; i++)
            bools[i] = list.get(i);

        return bools;
    }

    /**
     * Function to clone a List
     * @param l The list to clone
     * @param <T> The list's elements' type
     * @return The new list
     */
    public static <T> List<T> clone(List<T> l) {
        return new ArrayList<>(l);
    }
}