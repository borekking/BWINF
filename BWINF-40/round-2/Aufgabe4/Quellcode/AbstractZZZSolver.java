package de.flo.zzz;

import de.flo.zzz.bitSequence.BitSequence;

import org.jlinalg.Matrix;
import org.jlinalg.f2.F2;

import java.util.List;

/**
 * Abstract class for providing an abstraction for classes solving the ZZZ-Problem
 */
public abstract class AbstractZZZSolver {

    /**
     * ZZZ-Problem instance
     */
    protected final ZZZProblem problem;

    /**
     * The problems linear system
     */
    protected final Matrix<F2> linearSystem;

    /**
     * The problems bit sequences
     */
    protected final List<BitSequence> bitSequences;

    /**
     * Constructor taking the ZZZ-Problem
     * @param problem The ZZZ-Problem
     */
    public AbstractZZZSolver(ZZZProblem problem) {
        this.problem = problem;
        this.linearSystem = this.problem.createMatrix();
        this.bitSequences = this.problem.getBitSequences();
    }

    /**
     * Methode for solving the ZZZ-Problem provided as field
     * @return The solution to the ZZZ-Problem
     */
    public abstract int[] getResult();
}