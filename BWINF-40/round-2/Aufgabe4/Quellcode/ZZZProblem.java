package de.flo.zzz;

import de.flo.zzz.bitSequence.BitSequence;
import de.flo.zzz.util.JLinAlgUtils;
import de.flo.zzz.util.JavaUtils;

import org.jlinalg.Matrix;
import org.jlinalg.f2.F2;

import java.util.List;

/**
 * Class implementing a ZZZ-Problem
 */
public class ZZZProblem {

    /**
     * Amount of bit sequences
     */
    public final int n;

    /**
     * Amount of added  bit sequences
     */
    public final int k;

    /**
     * Amount if bits per bit sequence/bit sequences' size
     */
    public final int m;

    /**
     * Amount of bit sequences to find (which create the zero-bit-sequences)
     */
    public final int size;

    /**
     * The given bit sequences
     */
    private final List<BitSequence> bitSequences;

    /**
     * Constructor taking the needed attributes
     * @param n Amount of bit sequences
     * @param k Amount of added  bit sequences
     * @param m Amount if bits per bit sequence/bit sequences' size
     * @param bitSequences The bit sequences
     */
    public ZZZProblem(int n, int k, int m, List<BitSequence> bitSequences) {
        this.n = n;
        this.k = k;
        this.m = m;
        this.size = this.k + 1;
        this.bitSequences = bitSequences;
    }

    /**
     * Methode to create the matrix corresponding to the problem, where each bit sequences is a column
     * @return The matrix
     */
    public Matrix<F2> createMatrix() {
        // Define amount of rows and columns
        int rows = this.m;
        int columns = this.n;

        // Create matrix
        F2[][] matrixArray = new F2[rows][columns];

        // Iterate through rows and columns
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                // Get current BS' bit, transform to F2, set in matrix-array
                boolean val = this.bitSequences.get(col).getBit(row);
                F2 f2 = JLinAlgUtils.boolToF2Function.apply(val);

                matrixArray[row][col] = f2;
            }
        }

        return new Matrix<>(matrixArray);
    }

    /**
     * @return A clone of the given bit sequences
     */
    public List<BitSequence> getBitSequences() {
        return JavaUtils.clone(this.bitSequences);
    }
}