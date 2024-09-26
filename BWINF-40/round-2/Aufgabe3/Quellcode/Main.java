package de.flo.hexMax;

import de.flo.hexMax.changingTrack.SSDSetChangingRow;
import de.flo.hexMax.digit.HexDigit;
import de.flo.hexMax.digit.HexDigitUtils;
import de.flo.hexMax.digit.display.SSDSetPrinter;
import de.flo.hexMax.util.JavaUtils;

import java.io.File;
import java.io.FileNotFoundException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Deque;
import java.util.List;
import java.util.Scanner;

/**
 * Main class of whole Program
 */
public class Main {

    /**
     * Private constructor, s.t. this class can not have objects
     */
    private Main() {
    }

    /**
     * main Methode of the program
     * @param args Program's args (unused)
     */
    public static void main(String... args) {
        // Declare one Scanner for console to use for the whole program
        Scanner sc = new Scanner(System.in);

        boolean repeat = true;

        // Run the procedure, ask the user if he wants to repeat,
        // if so, repeat.
        while (repeat) {
            run(sc);

            repeat = getBoolInput(sc, "Do you want to repeat this program (y/n)?");
            if (repeat) System.out.println("---------------" + System.lineSeparator());
        }
    }

    /**
     * Methode asking the user for inputs (file name/path, print-changes) and running the problem-solving
     * @param sc A console Scanner
     */
    private static void run(Scanner sc) {
        // Get input file's name from user
        List<String> fileLines = null;

        while (fileLines == null) {
            String fileName = getInput(sc, "Please enter file name/path:");
            fileLines = getFileContent(fileName);

            if (fileLines == null) {
                System.out.println("Could not find file name/path \"" + fileName + "\"! Please try again:");
            }
        }

        // Get hex-digits and max-changes (m) from file
        HexDigit[] digits = HexDigitUtils.getDigits(fileLines.get(0));

        if (!validDigits(digits)) {
            System.out.println("Given digits could not be loaded. Given file seems to be invalid! " + System.lineSeparator() + "Rerunning program.");
            System.out.println("---------------" + System.lineSeparator());
            run(sc);
            return;
        }

        int maxChanges;
        try {
            maxChanges = Integer.parseInt(fileLines.get(1));
        } catch (NumberFormatException e) {
            System.out.println("Given max-changes (m) could not obtained. Given file seems to be invalid! " + System.lineSeparator() + "Rerunning program.");
            System.out.println("---------------" + System.lineSeparator());
            run(sc);
            return;
        }

        // Get if the changes should be printed from user
        boolean printChanges = getBoolInput(sc, "Should the needed changes be printed? (y/n)");

        // Actually run the given task
        runHexMax(digits, maxChanges, printChanges);
    }

    /**
     * Returns if given HexDigits are valid meaning non is null
     * @param digits The HexDigits to check
     * @return If the HexDigits are valid
     */
    private static boolean validDigits(HexDigit[] digits) {
        for (HexDigit digit : digits) {
            if (digit == null) return false;
        }

        return true;
    }

    /**
     * Methode to return a File's content as List of Strings by filename
     * @param fileName The file's name
     * @return The file's content
     */
    private static List<String> getFileContent(String fileName) {
        File file = new File(fileName);

        List<String> lines;
        try {
            lines = JavaUtils.readFile(file);
        } catch (FileNotFoundException e) {
            lines = null;
        }

        return lines;
    }

    /**
     * Run a HexMax Problem and print results and stats
     * @param digits HexMax number's digits (HexDigits)
     * @param maxChanges Max amount of changes, m
     * @param printChanges If the Changing Row should be printed
     */
    private static void runHexMax(HexDigit[] digits, int maxChanges, boolean printChanges) {
        long millis = System.currentTimeMillis();

        // Init HexMaxSolver
        long hexMaxSolverConstructorMillis = System.currentTimeMillis();
        AbstractHexMaxSolver solver = new HexMaxSolver(digits, maxChanges);
        hexMaxSolverConstructorMillis = System.currentTimeMillis() - hexMaxSolverConstructorMillis;

        // Solving
        long hexMaxSolverSolvingMillis = System.currentTimeMillis();
        HexDigit[] result = solver.solve();
        hexMaxSolverSolvingMillis = System.currentTimeMillis() - hexMaxSolverSolvingMillis;

        // Get swaps
        long changesCreationMillis = System.currentTimeMillis();
        Deque<ResultChangesCreater.Swap> swaps = new ResultChangesCreater(digits, result).getSwaps();
        changesCreationMillis = System.currentTimeMillis() - changesCreationMillis;

        // Printing results (input, result, amount of changes, increasing of result number to input number)
        // and needed time (pre-loading, solving, changes) in milliseconds
        System.out.println("Input number:");
        System.out.println(new SSDSetPrinter().createSSDSetString(digits));

        long changesSeraMillis = -1;

        if (printChanges) {
            System.out.println("Changes:");

            changesSeraMillis = System.currentTimeMillis();
            SSDSetChangingRow changingRow = new ChangingRowCreater(swaps, digits).getChangingRow();
            String text = changingRow.getText();
            changesSeraMillis = System.currentTimeMillis() - changesSeraMillis;

            // Print "/%n" if text is empty, text otherwise.
            System.out.println(text.isEmpty() ? "/" + System.lineSeparator(): text);
        }

        millis = System.currentTimeMillis() - millis;

        System.out.println("---");

        System.out.println("Input:  " + HexDigitUtils.getStringByDigits(digits));
        System.out.println("Result: " + HexDigitUtils.getStringByDigits(result));
        System.out.println("Needed Changes: " + swaps.size() + " / " + maxChanges);
        System.out.println("Increased in %: " + getIncreasingPercent(digits, result));

        System.out.println("---");

        System.out.println("HexMaxSolver Constructor (pre-loading): " + hexMaxSolverConstructorMillis + "ms");
        System.out.println("HexMaxSolver solving: " + hexMaxSolverSolvingMillis + "ms");
        System.out.println("Result Changes Creater: " + changesCreationMillis + "ms");
        System.out.println("Result Changes Serialization: " + changesSeraMillis + "ms");
        System.out.println("Total (without this stats): " + millis + "ms");

        System.out.println("---");
        System.out.println();
    }

    /**
     * Methode for calculating increase from a to b, where a and b are hex-numbers as their digits
     * @param digits Number a
     * @param result Number b
     * @return Increase from a to b in percent
     */
    private static double getIncreasingPercent(HexDigit[] digits, HexDigit[] result) {
        /*
         * Note:
         * if b = a + xa, x = (b - a) / a
         */

        // Get numbers' values as BigDecimal
        BigDecimal digitsValue = HexDigitUtils.getDecimalValue(digits); // a value
        BigDecimal resultValue = HexDigitUtils.getDecimalValue(result); // b value

        // Avoid dividing by zero
        if (digitsValue.doubleValue() == 0D) {
            return 0D;
        }

        // Difference of b and a: b - a
        BigDecimal difference = resultValue.subtract(digitsValue);


        // Get fraction (b - a) / a with scale 10, rounding down
        BigDecimal factor = difference.divide(digitsValue, 10, RoundingMode.DOWN);

        // Get from decimal to percentage, multiply with 100.
        BigDecimal percent = factor.multiply(new BigDecimal(100));

        return percent.doubleValue();
    }

    /**
     * Methode to get a user's input
     * @param sc Scanner for console
     * @param msg Message to send before getting input
     * @return The user input
     */
    private static String getInput(Scanner sc, String msg) {
        System.out.println(msg);

        if (sc.hasNext()) {
            return sc.nextLine();
        }

        return null; // Unreachable
    }

    /**
     * Methode to get a boolean input from user (true=y, false=n)
     * @param sc Scanner for console
     * @param msg Message to send before getting input
     * @return The boolean input from the user
     */
    private static boolean getBoolInput(Scanner sc, String msg) {
        System.out.println(msg);

        while (sc.hasNext()) {
            String str = sc.nextLine().toLowerCase();

            if (str.equals("y")) return true;
            else if (str.equals("n")) return false;
            else System.out.println("Input did not match 'y' or 'n'! Please try again!" + System.lineSeparator() + msg);
        }

        return false; // Unreachable
    }
}