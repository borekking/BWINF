package de.flo.zzz.util;

import org.jlinalg.Matrix;
import org.jlinalg.RingElement;
import org.jlinalg.Vector;
import org.jlinalg.f2.F2;

import java.util.function.Function;

/**
 * Utility class containing functions related to JLinAlg library
 */
public class JLinAlgUtils {

    /**
     * Private constructor, s.t. this class can not have objects
     */
    private JLinAlgUtils() {
    }

    /**
     * Function (as public static attribute) converting from F2 object to integer
     */
    public static final Function<F2, Integer> f2ToIntFunction = f2 -> f2.isOne() ? 1 : 0;

    /**
     * Function (as public static attribute) converting from F2 object to boolean
     */
    public static final Function<F2, Boolean> f2ToBoolFunction = RingElement::isOne;

    /**
     * Function (as public static attribute) converting from boolean to F2 object
     */
    public static Function<Boolean, F2> boolToF2Function = b -> b ? F2.ONE : F2.ZERO;

    /**
     * Function to create the zero vector of given length
     * @param rows Zero vector's length
     * @return The zero vector of given length
     */
    public static Vector<F2> getZeroVector(int rows) {
        F2[] bVecArray = new F2[rows];
        for (int i = 0; i < rows; i++) {
            bVecArray[i] = F2.ZERO;
        }
        return new Vector<>(bVecArray);
    }

    /**
     * Function to create Matrix given array of vectors.
     * <p>
     * Here the vectors will be the vectors columns
     * @param vectors The vectors
     * @param rows The amount of rows for all vectors
     * @return The resulting matrix
     */
    public static Matrix<F2> createMatrix(Vector<F2>[] vectors, int rows) {
        F2[][] matrixArr = new F2[rows][vectors.length];

        for (int row = 0; row < vectors.length; row++) {
            for (int col = 0; col < rows; col++) {
                matrixArr[col][row] = vectors[row].getEntry(col + 1);
            }
        }

        return new Matrix<>(matrixArr);
    }
}