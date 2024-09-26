package de.flo.hexMax;

import de.flo.hexMax.digit.HexDigit;

/**
 * Abstract class for the HexMax Solver providing an
 * abstraction for the constructor and an abstract
 * methode for actually solving the hex max problem.
 */
public abstract class AbstractHexMaxSolver {

    /**
     * Protected array of the number's digits
     */
    protected final HexDigit[] digits;

    /**
     * Size of digits
     */
    protected final int n;

    /**
     * Maximum amount of changes (m)
     */
    protected final int maxChanges;

    /**
     * @param digits the number's digits
     * @param maxChanges the max. amount of changes (m)
     */
    public AbstractHexMaxSolver(HexDigit[] digits, int maxChanges) {
        this.digits = digits;
        this.maxChanges = maxChanges;
        this.n = this.digits.length;
    }

    /**
     * Abstract methode for actually solving the hex max problems.
     *
     * @return Array of HexDigits representing the resulting hex-number.
     */
    public abstract HexDigit[] solve();
}