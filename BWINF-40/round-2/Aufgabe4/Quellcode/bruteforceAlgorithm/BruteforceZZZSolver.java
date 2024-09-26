package de.flo.zzz.bruteforceAlgorithm;

import de.flo.zzz.AbstractZZZSolver;
import de.flo.zzz.ZZZProblem;
import de.flo.zzz.bitSequence.BitSequence;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * ZZZSolver implementation where the ZZZ-problem is solved by a bruteforce solution.
 */
public class BruteforceZZZSolver extends AbstractZZZSolver {

    public BruteforceZZZSolver(ZZZProblem problem) {
        super(problem);
    }

    @Override
    public int[] getResult() {
        // Get all combinations of floor(k/2) BitSequences
        int halfK = this.problem.k / 2; // Note floor because if int-divisiom

        // All combinations of halfK BS from bitSequences list in a HashMap
        // The HashMap points from the resulting BS to all indexes (as int-arrays
        // in a set) creating the BS.
        Map<BitSequence, Set<int[]>> combinations = this.getCombinations(halfK);

        // Using different methods depending if size (=k+1) is odd
        if (this.problem.size % 2 == 0) {
            return this.findEvenSolution(combinations);
        } else {
            return this.findOddSolution(combinations);
        }
    }

    /**
     * Implementation of bruteforcing if k+1 is even.
     * @param combinations All combinations of the BitSequences
     * @return The result to the ZZZ-Problem
     */
    private int[] findEvenSolution(Map<BitSequence, Set<int[]>> combinations) {
        // Go through all different combinations and the associated set of< indexes.
        // For each set, search for two disjoint arrays in the set and return
        // both (merged) if they could be found.
        for (Set<int[]> set : combinations.values()) {
            int[] disjointArray = this.findDisjointArrays(set);
            if (disjointArray != null) return disjointArray;
        }

        return null;
    }

    /**
     * Methode to find two disjoint arrays in a set of int-arrays
     * @param set The set with int-arrays
     * @return Any two disjoint array merged into one array, or null
     * if there is non
     */
    private int[] findDisjointArrays(Set<int[]> set) {
        // Go through all combinations of two int-arrays using
        // double-loop. Check if both current int-arrays are
        // disjoint, if so return them merged.
        for (int[] arr1 : set) {
            for (int[] arr2 : set) {
                if (this.isDisjoint(arr1, arr2)) {
                    return this.mergeArrays(arr1, arr2);
                }
            }
        }

        return null;
    }

    /**
     * Implementation of bruteforcing if k+1 is odd.
     * @param combinations All combinations of the BitSequences
     * @return The result to the ZZZ-Problem
     */
    private int[] findOddSolution(Map<BitSequence, Set<int[]>> combinations) {
        // Go through all BS, for each go through the combinations.
        // XOR current BS with current combination and check for combinations
        // creating the XOR. Now check for any disjoint solutions of all BS.
        for (int i = 0; i < this.problem.n; i++) {
            BitSequence bs1 = this.bitSequences.get(i);
            int[] arrI = new int[]{i};

            for (BitSequence bs2 : combinations.keySet()) {
                Set<int[]> set = combinations.get(bs2);
                BitSequence bs3 = bs1.xor(bs2);

                Set<int[]> set2 = combinations.get(bs3);
                if (set2 == null) continue;

                int[] disjointArray = this.findDisjointArrays(set, set2, arrI);
                if (disjointArray != null) return disjointArray;
            }
        }
        return null;
    }

    /**
     * Methode to find two int-arrays int two sets of int-arrays which are
     * disjoint and don't contain any element of a third given int-array
     * @param set1 First set of int-arrays
     * @param set2 Second set of int-arrays
     * @param arr The third int-array
     * @return Any two matching arrays merged into one array with the third one,
     * or null if there is non
     */
    private int[] findDisjointArrays(Set<int[]> set1, Set<int[]> set2, int[] arr) {
        // Go through all combinations of two int-arrays using
        // double-loop. Check if both current int-arrays and the third array
        // are disjoint, if so return them merged.
        for (int[] arr1 : set1) {
            for (int[] arr2 : set2) {
                if (this.isDisjoint(arr1, arr2, arr)) {
                    return this.mergeArrays(arr1, arr2, arr);
                }
            }
        }

        return null;
    }

    /**
     * Methode to merge given int-arrays
     * @param arrays The int-arrays
     * @return The merged int-arrays
     */
    private int[] mergeArrays(int[]... arrays) {
        int n = arrays.length;
        if (n == 0) return new int[0]; // Special case, where arrays is empty

        int newLength = 0; // Get the length of the merge-array
        for (int[] arr : arrays) newLength += arr.length;

        // Copy first array of arrays into new array result
        int[] result = Arrays.copyOf(arrays[0], newLength);
        int cu = arrays[0].length; // Variable to keep track of next index in result

        // Go through left arrays and copy them into result
        for (int i = 1; i < n; i++) {
            int[] arr = arrays[i];
            System.arraycopy(arr, 0, result, cu, arr.length);
            cu += arr.length;
        }

        return result;
    }

    /**
     * Methode to check if int-arrays are disjoint
     * @param arrays The arrays to check as var-args
     * @return If all int-arrays are disjointed
     */
    private boolean isDisjoint(int[]... arrays) {
        // Go through the elements of each array and check if it was
        // already saved in set, then add it to the set.
        // If any element was already saved, return false.
        Set<Integer> set = new HashSet<>();

        for (int[] arr : arrays) {
            for (int e : arr) {
                if (set.contains(e)) return false;

                set.add(e);
            }
        }

        return true;
    }

    /**
     * Methode to create all combinations of given size subsets of BitSequences
     * in bitSequences list.
     * @param size The size of the subsets
     * @return A HashMap of BitSequences b pointing to set of all indexes (int-arrays
     * sized size) creating b.
     */
    private Map<BitSequence, Set<int[]>> getCombinations(int size) {
        Map<BitSequence, Set<int[]>> combinations = new HashMap<>();

        BitSequence zeroBS = new BitSequence(new boolean[this.problem.m]);
        int[] indexes = new int[size];
        this.getCombinationsImp(combinations, zeroBS, indexes, 0, -1, size);

        return combinations;
    }

    /**
     * Recursive implementation of getting all combinations
     * @param combinations The HashMap which the results will be added to
     * @param currentBS The current BitSequences created by the indexes
     * @param indexes The indexes used
     * @param indexInArray The current index in indexes
     * @param indexInList The current index in bitSequences list
     * @param k The amount of indexes not choosen yet
     */
    public void getCombinationsImp(Map<BitSequence, Set<int[]>> combinations,
                                   BitSequence currentBS, int[] indexes, int indexInArray,
                                   int indexInList, int k) {
        // Base case: no indexes left to add
        if (k == 0) {
            Set<int[]> combination = combinations.computeIfAbsent(currentBS, k1 -> new HashSet<>());
            combination.add(Arrays.copyOf(indexes, indexes.length));
            return;
        }

        int start = indexInList + 1;
        int end = this.problem.n - k;
        int nextK = k - 1;
        int nextIndexInArray = indexInArray + 1;

        for (int i = start; i < end; i++) {
            indexes[indexInArray] = i;
            BitSequence nexBS = currentBS.xor(this.bitSequences.get(i));
            getCombinationsImp(combinations, nexBS, indexes, nextIndexInArray, i, nextK);
        }
    }
}