package de.flo.hexMax.digitChanges;

import de.flo.hexMax.digit.HexDigit;

import java.util.HashMap;
import java.util.Map;

/**
 *  <p>Class for holding how to change all hex-digits to
 *  all other hex-digits
 *
 *  <p>Using a singleton because this class is only needed ones but is nice to have non-static
 */
public class HexDigitChanges {

    /**
     * Singleton
      */
    private static HexDigitChanges singleton;

    /**
     * Getter for singleton
     * @return The singleton of this class
     */
    public static HexDigitChanges getSingleton() {
        if (singleton == null) {
            singleton = new HexDigitChanges();
        }

        return singleton;
    }

    /**
     * Map pointing from values of hex-digits to a map pointing from values of hex-digits to the associated DigitChanges object:
     * <p>{hex-digit d (as decimal value) -> {hex-digit g (as decimal value) -> changes from d to g}}
     */
    private final Map<Integer, Map<Integer, DigitChanges>> changesMap;

    private HexDigitChanges() {
        this.changesMap = new HashMap<>();

        this.fill(this.changesMap);
    }

    /**
     * Methode returning the DigitChanges object for two hex-digits
     * @param digit Start hex-digit
     * @param goal Goal hex-digit
     * @return The associated DigitChanges object from changesMap
     */
    public DigitChanges getChanges(HexDigit digit, HexDigit goal) {
        int digitValue = digit.getValue(), goalValue = goal.getValue();

        Map<Integer, DigitChanges> inner = this.changesMap.get(digitValue);
        return inner.get(goalValue);
    }

    /**
     * Methoe for filling the Map with all hex-digit combinations
     * @param changesMap The HashMap with all hex-digit combinations by value
     */
    private void fill(Map<Integer, Map<Integer, DigitChanges>> changesMap) {
        for (HexDigit digit : HexDigit.values()) {
            Map<Integer, DigitChanges> innerChanges = new HashMap<>();

            for (HexDigit goal : HexDigit.values()) {
                DigitChanges currentChanges = new DigitChanges(digit, goal);

                innerChanges.put(goal.getValue(), currentChanges);
            }

            changesMap.put(digit.getValue(), innerChanges);
        }
    }
}