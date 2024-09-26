package de.flo.zzz.bitSequence;

import de.flo.zzz.util.JavaUtils;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Utility class for BS', providing some useful functions
 */
public class BitSequenceUtils {

    /**
     * Private constructor, s.t. this class can not have objects
     */
    private BitSequenceUtils() {
    }

    /**
     * Function to do the XOR in a list of BitSequence
     * @param sequences The list of BitSequences
     * @return The resulting XOR
     */
    public static BitSequence xor(List<BitSequence> sequences) {
        int amount = sequences.size();

        // Special cases: 0, 1
        if (amount == 0) return null;
        if (amount == 1) return sequences.get(0);

        // Bit Amount of first list
        int bitAmount = sequences.get(0).getSize();

        // Make sure all BitSequences have the same size
        // -> O(amount-1)
        for (int i = 1; i < amount; i++) {
            int currentBitAmount = sequences.get(i).getSize();

            if (currentBitAmount != bitAmount) {
                throw new RuntimeException("BitSequence do not have the same length!");
            }
        }

        // O((amount-1) * bitAmount)
        BitSequence xor = sequences.get(0).xor(sequences.get(1));

        for (int i = 2; i < amount; i++) {
            BitSequence current = sequences.get(i);
            xor = xor.xor(current); // <- O(bitAmount);
        }

        return xor;
    }

    /**
     * Function to sort list of BitSequences in place, increasing
     * @param list The list to sort
     */
    public static void sortBitSequences(List<BitSequence> list) {
        list.sort(BitSequence::compareTo);
    }

    /**
     * Function to check if a String is a binary String
     * @param str String to check
     * @return If str is a binary String
     */
    public static boolean isBinary(String str) {
        return str.matches("[0|1]*");
    }

    /**
     * Function to convert a binary String to a boolean-array
     * @param binaryNumber Binary String to convert
     * @return The boolean-array
     * @throws IllegalArgumentException If the String is not binary
     */
    public static boolean[] getBooleanArray(String binaryNumber) {
        // Check string is a binary string
        if(!isBinary(binaryNumber)) {
            throw new IllegalArgumentException("\"" + binaryNumber + "\" is not a binary number!");
        }

        // Convert binary string to list of booleans using streams
        List<Boolean> bitList = Arrays.stream(binaryNumber.split(""))
                .map(s -> s.equals("1")).collect(Collectors.toList());

        // Convert list of boolean to boolean array and return array
        return JavaUtils.toPrimitiveArray(bitList);
    }

    /**
     * Function to create binary String by given boolean-array
     * @param bits Boolean-array to convert
     * @return The resulting binary string
     */
    public static String getBinaryString(boolean[] bits) {
        char[] chars = new char[bits.length];

        for (int i = 0; i < bits.length; i++) {
            chars[i] = bits[i] ? '1' : '0';
        }

        return new String(chars);
    }
}