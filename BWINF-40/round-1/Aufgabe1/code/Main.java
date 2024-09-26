package de.flo.bwinf.schiebeParkplatz;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Main {

    // Main Methode
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while (true) {     // Returns input from Console
            String fileName = getInput("Bitte geben Sie den Dateinamen Ihrer Parkplatz-Datei an: ", sc);
                        // Ask Methode prints question. Returns true if answer (from Console) is "y" and false if answer is "n"
            boolean save = ask(System.lineSeparator() + "Soll das ergebnis in eine Datei gespeichert werden? (y/n)", sc);
            boolean print = ask(System.lineSeparator() + "Soll der Parkplatz ausgegeben werden? (y/n)", sc);

            SchiebeParkplatz schiebeParkplatz;
            try {
                schiebeParkplatz = new SchiebeParkplatz(fileName);
            } catch (IOException e) {
                System.out.println("Die Datei \"" + fileName + "\" konnte nicht gefunden Werden." + System.lineSeparator());

                boolean repeat = ask("Wollen Sie dies wiederholen? (y/n)", sc);
                if (!repeat) break;

                continue;
            }

            if (print) {
                System.out.println("Parkplatz:");
                schiebeParkplatz.print();
                System.out.println();
            }

            List<String> results = schiebeParkplatz.getResults();
            System.out.println(System.lineSeparator() + "Loesungen: ");
            results.forEach(System.out::println);

            if (save) {
                Writer writer = new Writer("Loesung.txt");
                writer.write(String.join("\n", results));
                writer.close();
            }

            boolean repeat = ask("Wollen Sie dies wiederholen? (y/n)", sc);
            if (!repeat) break;
        }

        sc.close();
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
