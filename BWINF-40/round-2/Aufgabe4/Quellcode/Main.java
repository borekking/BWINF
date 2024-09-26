package de.flo.zzz;

import de.flo.zzz.bitSequence.BitSequence;
import de.flo.zzz.bitSequence.BitSequenceUtils;
import de.flo.zzz.util.JavaUtils;

import java.io.File;
import java.io.FileNotFoundException;

import java.util.ArrayList;
import java.util.Arrays;
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
    public static void main(String[] args) throws FileNotFoundException {
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
        ZZZProblem problem = null;

        while (problem == null) {
            String fileName = getInput(sc, "Please enter file name/path:");
            try {
                problem = readFile(fileName);
            } catch (FileNotFoundException e) {
                System.out.println("Could not find file name/path \"" + fileName + "\"! Please try again:");
                continue;
            }

            if (problem == null) {
                System.out.println("Given file seems to be unvalid!");
                continue;
            }

            runTask(problem);
        }
    }

    /**
     * Functio for running an actual problem instance
     * @param task The ZZZ-Problem to run
     */
    private static void runTask(ZZZProblem task) {
        List<BitSequence> bitSequences = task.getBitSequences();

        long millis = System.currentTimeMillis();
        int[] indexes = new ZZZSolver(task).getResult();
        millis = System.currentTimeMillis() - millis;

        System.out.println("Bit Sequences:");
        if (indexes.length == 0) System.out.println("/");
        for (int index : indexes) {
            System.out.println(bitSequences.get(index));
        }

        System.out.println();
        System.out.println("millis: " + millis);
        System.out.println("indexes: " + Arrays.toString(indexes));
        System.out.println();
    }

    /**
     * Function to create ZZZProblem instance given a file; making sure that problem will be zero if any input format is wrong
     * @param fileName The file's name
     * @return The ZZZProblem instance
     * @throws FileNotFoundException If the file could not be found
     */
    private static ZZZProblem readFile(String fileName) throws FileNotFoundException {
        File file = new File(fileName);

        List<String> lines;
        try {
            lines = JavaUtils.readFile(file);
        } catch (FileNotFoundException e) {
            throw new FileNotFoundException();
        }

        if (lines.size() < 1) return null;

        String firstLine = lines.get(0);

        String[] splitted = firstLine.split(" ");

        int n, k, m;
        try {
            n = Integer.parseInt(splitted[0]);
            k = Integer.parseInt(splitted[1]);
            m = Integer.parseInt(splitted[2]);
        } catch(NumberFormatException e) {
            return null;
        }

        if (lines.size() <= n) return null;

        List<BitSequence> bits = new ArrayList<>();
        for (int i = 1; i <= n; i++) {
            if (!BitSequenceUtils.isBinary(lines.get(i))) {
                return null;
            }

            bits.add(new BitSequence(lines.get(i)));
        }

        return new ZZZProblem(n, k, m, bits);
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