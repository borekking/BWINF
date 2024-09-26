package de.flo.zzz.solutionFromVectorSpace;

import de.flo.zzz.util.HammingUtils;

import org.jlinalg.Vector;
import org.jlinalg.f2.F2;

/**
 * Class used to bruteforce a valid solution (by hamming weight) in a given solution set
 */
public class ValidSolutionBruteforcer {

    /**
     * The solution set
     */
    private final Vector<F2>[] span;

    /**
     * Amount of free variables in the solution  set
     */
    private final int freeVariables;

    /**
     * The amount of rows, which the vectors in the solution set have
     */
    private final int rows;

    /**
     * The needed weight of the solution
     */
    private final int weight;

    /**
     * The created solution of the solution space, with given hamming weight
     */
    private final Vector<F2> result;

    /**
     * Constructor taking needed parameters
     * @param span The solution set
     * @param rows The amount of rows, which the vectors in the solution set have
     * @param weight The needed weight of the solution
     * @param maxDepth Maximum Depth of search tree, meaning max. amount of free vars to choose (inclusive)
     */
    public ValidSolutionBruteforcer(Vector<F2>[] span, int rows, int weight, int maxDepth) {
        this.span = span;
        this.freeVariables = this.span.length;
        this.rows = rows;
        this.weight = weight;

        this.result = this.bruteforce(maxDepth);
    }

    /**
     * Methode to bruteforce for the solution with given hamming weight
     * @return The correct solution
     */
    private Vector<F2> bruteforce(int maxDepth) {
        // Try out all any amount of free vectors to choose from 1 to weight (inclusive)
        for (int i = 1; i <= this.weight; i++) {
            // Stop searching if current amount of free variables is bigger then the
            // amount of free variables
            if (i > this.freeVariables) break;

            // Also stop searching if i is bigger then maxDepth
            if (i > maxDepth) break;

            int[] choosenVectors = new int[this.freeVariables];
            Vector<F2> solution = this.bruteforceImp(choosenVectors, 0, -1, i);

            if (solution != null)
                return solution;
        }

        return null;
    }

    /**
     * Actual methode to bruteforce for the solution using a recursive methode
     * @param values Currently chosen free variables as their indexes
     * @param indexInArray Current index in values array
     * @param indexInSpan Current index in span (aka. solution set, aka. free variables)
     * @param k Amount of not chosen free variables
     * @return The resulting vector
     */
    private Vector<F2> bruteforceImp(int[] values, int indexInArray, int indexInSpan, int k) {
        // Base case: No elements left to add in values array
        if (k == 0) {
            // Create solution with current values array and check for needed
            // hamming weight.
            Vector<F2> solution = this.getSolutionVector(values);
            if (HammingUtils.getHammingWeight(solution, this.rows) == this.weight)
                return solution;

            return null;
        }

        int start = indexInSpan + 1; // Inclusive
        int end = this.freeVariables - k; // Inclusive

        for (int i = start; i <= end; i++) {
            values[indexInArray] = i;

            Vector<F2> solution = this.bruteforceImp(values, indexInArray + 1, i, k - 1);
            if (solution != null) return solution;
        }

        return null;
    }

    /**
     * Returns a solution to the system with chosen free variables (by their index)
     * @param values Indexes of free variables chosen
     * @return The resulting solution vector
     */
    private Vector<F2> getSolutionVector(int[] values) {
        F2[] array = new F2[this.rows];

        for (int row = 0; row < this.rows; row++) {
            F2 val = F2.ZERO;

            for (int i : values) {
                Vector<F2> vec = this.span[i];

                F2 cu = vec.getEntry(row + 1);
                val = val.add(cu);
            }

            array[row] = val;
        }

        return new Vector<>(array);
    }

    public Vector<F2> getResult() {
        return result;
    }
}