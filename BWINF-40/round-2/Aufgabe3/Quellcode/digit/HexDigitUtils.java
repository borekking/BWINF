package de.flo.hexMax.digit;

import de.flo.hexMax.util.JavaUtils;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * Class containing utility functions related to hex digits
 */
public class HexDigitUtils {

    /**
     * Prevent making objects of this class since it is a utility class
     */
    private HexDigitUtils() {
    }

    /**
     * Map: Hex-digits' names to corresponding hex-digit
     */
    private final static Map<Character, HexDigit> nameToHexDigit = new HashMap<>();

    /**
     * Hex-digits sorted decreasing (from biggest to smallest)
     */
    private final static List<HexDigit> hexDigitsDecreasing = new ArrayList<>();

    /**
     * Function applying a Comparator to a property of all hex-digits
     * @param function Function for getting hex-digits' property
     * @param comparator Comparator for comparing the hex-digits' properties
     * @param <T> The properties' type
     * @return A new sorted list of hex-digits
     */
    public static <T> List<HexDigit> getHexDigits(Function<HexDigit, T> function, Comparator<T> comparator) {
        List<HexDigit> values = getHexDigits();
        return JavaUtils.sortByProperty(values, function, comparator);
    }

    /**
     * @param comparator The Comparator used to compare two values of the hex digits
     * @return A new list of hex digits sorted by their value corresponding to the comparator
     */
    public static List<HexDigit> getHexNumbers(Comparator<Integer> comparator) {
        return getHexDigits(HexDigit::getValue, comparator);
    }

    /**
     * Static Function returning hexadecimal number with given name (as character).
     * @param name The hex-digit's name
     * @return The hex-digit corresponding to name or null
     */
    public static HexDigit getHexNumber(char name) {
        if (nameToHexDigit.isEmpty()) {
            // Fill valueToHexDigit map using Streams
            List<HexDigit> digits = getHexDigits();
            Map<Character, HexDigit> tempMap = JavaUtils.createMap(digits, HexDigit::getName, digit -> digit);
            nameToHexDigit.putAll(tempMap);
        }

        // Can be null
        return nameToHexDigit.get(name);
    }

    /**
     * @return A new list of hex digits sorted from biggest to smallest digit by value.
     */
    public static List<HexDigit> getHexDigitsDecreasing() {
        if (hexDigitsDecreasing.isEmpty()) {
            // Get the sorted hex digits, by value from biggest to smallest
            // (reverseOrder) and add the whole list to biggestToSmallestHexDigit.
            List<HexDigit> sortedHexDigits = getHexNumbers(Comparator.reverseOrder());
            hexDigitsDecreasing.addAll(sortedHexDigits);
        }

        // Return a new list containing the content of biggestToSmallestHexDigit
        return new ArrayList<>(hexDigitsDecreasing);
    }

    /**
     * @return A new list of all hex-digits (0 to F; Might not be sorted)
     */
    public static List<HexDigit> getHexDigits() {
        return JavaUtils.getAsList(HexDigit.values());
    }

    /**
     * Function for calculating a hexadecimal numbers into its decimal values.
     * @param digits The number's digits (read from left to right)
     * @return The decimal value as BigDecimal
     */
    public static BigDecimal getDecimalValue(HexDigit[] digits) {
        // Starting value 0; power of 16 multiplied with digitValue
        BigDecimal value = new BigDecimal(0);
        BigDecimal sixteen = new BigDecimal(16);
        int power = 0;

        // Go from most right letter to most left letter
        for (int i = digits.length - 1; i >= 0; i--) {
            // Current digit's value in hexadecimal
            int digitValue = digits[i].getValue();

            // Increase value by 16 to the current power times the current digit's value
            value = value.add(sixteen.pow(power).multiply(new BigDecimal(digitValue)));

            power++; // Increasing power
        }

        return value;
    }

    /**
     * Function for getting a hex-number's string representation given it representation as its hex-digits
     * @param digits The hex-number as its digits
     * @return The hex-number as string
     */
    public static String getStringByDigits(HexDigit[] digits) {
        return Arrays.stream(digits).map(HexDigit::getName).map(String::valueOf).collect(Collectors.joining(""));
    }

    /**
     * Function returning a hex-digit array given its representation as string
     * @param str Representation of hex-number as string
     * @return The hex-number as its digits
     */
    public static HexDigit[] getDigits(String str) {
        // Get each letter of str
        String[] elements = str.split("");

        // Convert these letters (of the hex-number) to HexDigits
        List<HexDigit> digits = Arrays.stream(elements).map(e -> e.charAt(0)).map(HexDigitUtils::getHexNumber).collect(Collectors.toList());

        return JavaUtils.getAsArray(digits, new HexDigit[0]);
    }
}