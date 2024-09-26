package de.flo.bwinf.task5;

import java.io.IOException;
import java.text.DecimalFormat;
import java.util.*;

public class Main {

    // 75,52s


    public static void main(String[] args) {
        System.out.println("HEY!");
        Scanner sc = new Scanner(System.in);

        while (true) {     // Returns input from Console
            String fileName = getInput("Bitte geben Sie den Dateinamen Ihrer Gewichte-Datei an: ", sc);
            Writer writer = new Writer("Loesungen.txt");

            boolean print = ask("Sollen die Ergebnisse in die Konsole ausgegeben werden? (y/n)", sc),
                    write = ask("Sollen die Ergebnisse in eine Datei (Loesungen.txt) geschrieben werden? (y/n)", sc);

            GewichteChecker gewichteChecker;
            long nano;
            try {
                nano = System.nanoTime();
                gewichteChecker = new GewichteChecker(fileName);
                nano = System.nanoTime() - nano;
            } catch (IOException e) {
                System.out.println("Die Datei \"" + fileName + "\" konnte nicht gefunden Werden." + System.lineSeparator());
                boolean repeat = ask("Wollen Sie dies wiederholen? (y/n)", sc);
                if (!repeat) break;
                continue;
            }

            // Print/Write (if question was answered with "y") the result or closest result
            // for each number between 10 and 10_000
            for (long l = 10L; l <= 10_000; l += 10) {
                ListResult result = gewichteChecker.getResult(l);
                String str = serializeSolution(l, result);

                if (print)
                    System.out.print(str);

                if (write)
                    writer.write(str);
            }

            int directResults = gewichteChecker.getDirectResultsSize(), closestResults = gewichteChecker.getClosestResultsSize();
            System.out.println();
            System.out.println("Nanosekunden: " + nano + " (" + new DecimalFormat("0.00").format(nano / 1_000_000_000D) + "s)");
            System.out.println("Genaue Ergebnisse: " + directResults + " / 1000");
            System.out.println("Naechste Ergebnisse: " + closestResults + " / 1000");
            System.out.println("Gesammt Ergebnisse: " + (directResults + closestResults) + " / 1000");

            writer.close();

            boolean repeat = ask("Wollen Sie dies wiederholen? (y/n)", sc);
            if (!repeat) break;
        }

        sc.close();
    }

    private static String serializeSolution(long weight, ListResult result) {
        return weight + "g: " + result.serializeSolution();
    }

    private static String getInput(Scanner sc) {
        return sc.nextLine();
    }

    private static String getInput(String msg, Scanner sc) {
        System.out.println(msg);
        return getInput(sc);
    }

    private static boolean ask(String q, Scanner sc) {
        String input;
        do {
            System.out.println(q);
            input = getInput(sc);
        } while (!Arrays.asList("y", "n").contains(input));

        return input.equals("y");
    }
}
