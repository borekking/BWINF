package de.flo.zzz;

import de.flo.zzz.bruteforceAlgorithm.BruteforceZZZSolver;
import de.flo.zzz.solutionFromVectorSpace.ValidSolutionBruteforcer;
import de.flo.zzz.util.JLinAlgUtils;

import org.jlinalg.LinSysSolver;
import org.jlinalg.Matrix;
import org.jlinalg.Vector;
import org.jlinalg.f2.F2;

/**
 * Actual class for solving the ZZZ-Problem
 */
public class ZZZSolver extends AbstractZZZSolver {

    /**
     * Max. depth of trying out combinations of free variables in a solution set
     */
    private final static int maxDimension = 3;

    /**
     * Max. k to use the ruteforce approach (combining bit sequences)
     */
    private final static int maxK = 4;

    public ZZZSolver(ZZZProblem problem) {
        super(problem);
    }

    /**
     * Implementation of solving the ZZZ-Problem using different algorithms.
     * <p> 1. Use the bruteforce approach (combining bit sequences) if k <= 4
     * <p> 2. Create solution space of linear system associated to the given ZZZ-Problem
     * <p> 3. Bruteforce correct solution in created solution space, with max depth of 3
     * <p> 4. Otherwise, return empty solution
     * @return The solution to the given ZZZ-Problem, or an empty solution
     */
    @Override
    public int[] getResult() {
        // 1. Solve the problem by bruteforcing BS combinations
        //    if k <= 4 (maxK)
        if (this.problem.k <= maxK) {
            return new BruteforceZZZSolver(this.problem).getResult();
        }

        // 2. Create solutionSpace
        Vector<F2>[] solutionSpace = this.solveHomogenousSystem(this.linearSystem);

        // 3. Try to bruteforce the correct solution in the solution set, with max
        //    depth of 3 (maxDimension)
        Vector<F2> solution = new ValidSolutionBruteforcer(solutionSpace, this.problem.n,
                this.problem.size, maxDimension).getResult();

        if (solution != null) {
            return this.convertVectorIndexes(solution);
        }

        // Otherwise, return empty int-array
        return new int[0];

    }

    /**
     * Methode to create the actual solution (bit sequences' indexes) given the solution as binary vector
     * @param solution The binary vector
     * @return The corresponding indexes as int-array
     */
    private int[] convertVectorIndexes(Vector<F2> solution) {
        int[] result = new int[this.problem.size];
        int i = 0;

        for (int j = 1; j <= this.problem.n; j++) {
            if (solution.getEntry(j) == F2.ONE) {
                result[i] = j - 1;
                i++;
            }
        }

        return result;
    }

    private Vector<F2>[] solveHomogenousSystem(Matrix<F2> matrix) {
        // Create zero vector
        Vector<F2> bVec = JLinAlgUtils.getZeroVector(matrix.getRows());

        // Solve Ax = 0 for solution space of x
        return LinSysSolver.solutionSpace(matrix, bVec).getGeneratingSystem();
    }
}