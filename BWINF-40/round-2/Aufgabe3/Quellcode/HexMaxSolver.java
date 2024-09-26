package de.flo.hexMax;

import de.flo.hexMax.digit.HexDigit;
import de.flo.hexMax.digit.HexDigitUtils;
import de.flo.hexMax.digitChanges.HexDigitChanges;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class HexMaxSolver extends AbstractHexMaxSolver {

    /**
     * <p>Memo object for memorizing methode calls of solveImp (DP):
     * <p> {index -> {A -> {R -> result of index, A, R}}}
     */
    private final Map<Integer, Map<Integer, Map<Integer, HexDigit[]>>> memo;

    /**
     * HexDigitChanges object from singleton
     */
    private final HexDigitChanges hexDigitChanges;

    /**
     * maxChanges doubled - Max. amount of changes (m) doubled: 2m;
     * Needed in solveImp-methode checks
     */
    private final int doubledMaxChanges;

    /**
     * @param digits     the number's digits
     * @param maxChanges the max. amount of changes (m)
     */
    public HexMaxSolver(HexDigit[] digits, int maxChanges) {
        super(digits, maxChanges);

        this.doubledMaxChanges = 2 * maxChanges;

        // Loading changes
        this.hexDigitChanges = HexDigitChanges.getSingleton();

        this.memo = new HashMap<>();

        // Prefill memo on indexes
        for (int index = 0; index <= this.n; index++) {
            this.memo.put(index, new HashMap<>());
        }
    }

    /**
     * Abstract methode for actually solving the hex max problems.
     *
     * @return List of HexDigits representing the resulting hex-number.
     */
    @Override
    public HexDigit[] solve() {
        return this.solveImp(0, 0, 0);
    }

    /**
     * Implementation of solving.
     *
     * @param index The current index in digits
     * @param A     The currently used amount of adds (segment which will be activated)
     * @param R     The currently used amount of removes (segment which will be deactivated)
     * @return An array of the biggest hex-number as hex-digits from given list (digits; constructor)
     * starting at index or null if there is no
     */
    private HexDigit[] solveImp(int index, int A, int R) {
        // Check in memo
        if (this.isInMemo(index, A, R)) {
            return this.getFromMemo(index, A, R);
        }

        int AR = A + R; // Used twice

        // A + R > 2*m
        // Max amount of changes is overstepped, so returning null
        if (AR > this.doubledMaxChanges) {
            return null;
        }

        // index >= n
        // Index is bigger than or equal size of digits;
        // index was out of range
        if (index >= this.n) {
            // Check A = R, meaning a's can be swapped with r's
            // s.t. solution is valid
            if (A != R) return null;
            // A == R given and A <= m and R <= m given by previous condition

            // Return empty array
            return new HexDigit[0];
        }

        // A+R = 2*m
        // Amount of changes exactly reached
        if (AR == this.doubledMaxChanges) {
            // If dif is not zero, meaning no solution, return null
            if (A != R) return null; // Check A == R

            // Return digits from index (inclusive) to end (n, exclusive)
            return Arrays.copyOfRange(this.digits, index, this.n);
        }

        HexDigit current = this.digits[index];
        HexDigit[] result = null; // Default result is null

        int nextIndex = index + 1;

        // Go through all hex-digits in decreasing order
        for (HexDigit digit : HexDigitUtils.getHexDigitsDecreasing()) {
            // a and r in int array obtained HexDigitChanges object
            int[] values = this.hexDigitChanges.getChanges(current, digit).getValues();
            int a = values[0], r = values[1]; // Amount of segments to add (a) / remove (r) to get from current to digit

            // Recursive part to get the best solution for next index with new A and R
            HexDigit[] subResult = this.solveImp(nextIndex, A + a, R + r);

            if (subResult != null) {
                int newSize = this.n - index;
                result = new HexDigit[newSize]; // Set result to new HexDigit Array

                // Fill result with digit on first index and subResult on the others
                result[0] = digit;
                System.arraycopy(subResult, 0, result, 1, newSize - 1);

                // Break loop because best result was found
                break;
            }
        }

        // Put result in memo (might be null)
        this.putInMemo(index, A, R, result);

        // Return the found result (might be null)
        return result;
    }

    // ---------<DP>---------

    /**
     * Methode returning if given solveImp input was already saved/is contained in memo
     *
     * @return If given solveImp input was already saved/is contained in memo
     */
    private boolean isInMemo(int index, int A, int R) {
        Map<Integer, Map<Integer, HexDigit[]>> subMap = this.memo.get(index);
        if (!subMap.containsKey(A)) return false;

        Map<Integer, HexDigit[]> subSubMap = subMap.get(A);
        return subSubMap.containsKey(R);
    }

    /**
     * Methode returning result to given solveImp input from memo
     *
     * @return Result to given solveImp input
     */
    private HexDigit[] getFromMemo(int index, int A, int R) {
        Map<Integer, Map<Integer, HexDigit[]>> subMap = this.memo.get(index);
        Map<Integer, HexDigit[]> subSubMap = subMap.get(A);
        return subSubMap.get(R);
    }

    /**
     * Methode for putting given solveImp input in memo with given result
     */
    private void putInMemo(int index, int A, int R, HexDigit[] result) {
        // Get subMap (will exist because of prefilling in constructor) by index
        Map<Integer, Map<Integer, HexDigit[]>> subMap = this.memo.get(index);

        // Get subSubMap by A:
        // If subMap already contains A, get it from subMap.
        // Otherwise, create new HashMap and put it into subMap
        Map<Integer, HexDigit[]> subSubMap;

        if (subMap.containsKey(A)) {
            subSubMap = subMap.get(A);
        } else {
            subSubMap = new HashMap<>();
            subMap.put(A, subSubMap);
        }

        // Put result into subSubMap
        subSubMap.put(R, result);
    }
    // ---------</DP>---------
}