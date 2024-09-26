package de.flo.zzz.util;

import org.jlinalg.Vector;
import org.jlinalg.f2.F2;

/**
 * Utility class for functions involving the hamming-weight
 */
public class HammingUtils {

    /**
     * Private constructor, s.t. this class can not have objects
     */
    private HammingUtils() {
    }

    /**
     * Function to obtain the hamming weight of a given vector
     * @param vector The vector
     * @param rows The amount of rows in the vector
     * @return The vector's hamming weight
     */
    public static int getHammingWeight(Vector<F2> vector, int rows) {
        int counter = 0;
        for (int i = 1; i <= rows; i++)
            if (vector.getEntry(i) == F2.ONE)
                counter++;
        return counter;
    }

    /**
     * Function to obtain the hamming weight of a boolean array
     * @param arr The boolean array
     * @return The boolean array's hamming weight
     */
    public static int getHammingWeight(boolean[] arr) {
        int counter = 0;
        for (boolean b : arr)
            if (b)
                counter++;
        return counter;
    }
}