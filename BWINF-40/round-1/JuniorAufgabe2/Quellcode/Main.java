package de.flo.bwinf.juniorAufgabe2;

import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while(true) {
            String fileName = getInput("Bitte geben Sie den Namen der Datei mit der Praeferenztabelle ein: ", sc);

            JuniorAufgabe2 juniorAufgabe2;
            try {
                juniorAufgabe2 = new JuniorAufgabe2(fileName);
            } catch (IOException ignored) {
                System.out.println("Die Datei " + fileName + " konnte nicht gefunden werde.");
                boolean repeat = ask("Moechten Sie ihre Eingabe wiederholen? (y/n)",sc);
                if (!repeat) break;
                continue;
            }

            int[] result = juniorAufgabe2.getResult();
            System.out.println("Bestes Datum: " + result[0] + ", noetige Veraenderungen: " + result[1] + System.lineSeparator());

            boolean repeat = ask("Moechten Sie dies wiederholen? (y/n)",sc);
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
