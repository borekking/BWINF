package de.flo.hexMax;

import de.flo.hexMax.digit.HexDigit;
import de.flo.hexMax.digitChanges.DigitChanges;
import de.flo.hexMax.digitChanges.HexDigitChanges;
import de.flo.hexMax.util.JavaUtils;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/**
 * Class for creating a SSDSet-ChangingRow given the given
 * hex-number and the resulting hex-number
 */
public class ResultChangesCreater {

    /**
     * Class representing a swap of to segments
     */
    public static class Swap {

        /**
         * Index of first and second digits
         */
        private final int indexA, indexB;

        /**
         * First and second index of segment in digit
         */
        private final int segmentIndexA, segmentIndexB;

        /**
         * Constructor taking the needed information
         * @param indexA Index of first digits
         * @param segmentIndexA First index of segment in digit
         * @param indexB Index of second digits
         * @param segmentIndexB Second index of segment in digit
         */
        public Swap(int indexA, int segmentIndexA, int indexB, int segmentIndexB) {
            this.indexA = indexA;
            this.segmentIndexA = segmentIndexA;
            this.indexB = indexB;
            this.segmentIndexB = segmentIndexB;
        }

        // Getters
        public int getIndexA() {
            return indexA;
        }

        public int getIndexB() {
            return indexB;
        }

        public int getSegmentIndexA() {
            return segmentIndexA;
        }

        public int getSegmentIndexB() {
            return segmentIndexB;
        }
    }

    /**
     * Size of given (and result) number
     */
    private final int size;

    /**
     * Given and resulting hex-numbers of hex-max problem
     */
    private final HexDigit[] given, result;

    /**
     * Maps of index of digit in number to List of segment that have to be...:
     * <ul>
     *     <li> enable-list: enabled (because they are disabled in given but enabled in result)
     *     </li>
     *     <li> disable-list: disabled (because they are enabled in given but disabled in result)
     *     </li>
     * </ul>
     */
    private final Map<Integer, Deque<Integer>> enable, disable;

    /**
     * List of needed swaps to get from given to result number
     */
    private final LinkedList<Swap> swaps;

    public ResultChangesCreater(HexDigit[] digits, HexDigit[] result) {
        this.given = digits;
        this.result = result;

        this.swaps = new LinkedList<>();

        this.size = this.result.length;

        // Init and fill Maps
        this.enable = new HashMap<>();
        this.disable = new HashMap<>();
        this.loadEnableDisable();

        // Actually fill the swaps list
        this.fillSwaps();
    }

    /**
     * Methode for filling the swaps list containing the swaps from given to result number
     */
    private void fillSwaps() {
        // 1. Only swap inside digits
        this.swapInside();

        // 2. Swap each enable with next disable (they are same size)
        this.swapOutside();
    }

    /**
     * Swap adds with removes in all numbers (not just in one at a time)
     */
    private void swapOutside() {
        Deque<Integer> enableDeque = new ArrayDeque<>(), disableDeque = new ArrayDeque<>();
        int enableIndex = -1, disableIndex = -1;

        boolean isDone = false;
        while (!isDone) {
            // Set enable Deque to next non-empty Deque in enable map if needed and possible (by index)
            while (enableDeque.isEmpty() && enableIndex < this.size - 1) {
                enableIndex++;
                enableDeque = this.enable.get(enableIndex);
            }

            // Set disable Deque to next non-empty Deque in disable map if needed and possible (by index)
            while (disableDeque.isEmpty() && disableIndex < this.size - 1) {
                disableIndex++;
                disableDeque = this.disable.get(disableIndex);
            }

            // Check Deques not being updated, meaning no changes left
            // They should have the same amount of elements, so just testing one
            if (enableDeque.isEmpty()) {
                isDone = true;
            } else {
                int enableSegment = enableDeque.pop();
                int disabledSegment = disableDeque.pop();

                this.addSwap(enableIndex, enableSegment, disableIndex, disabledSegment);
            }
        }
    }

    /**
     * Swap adds with removes until one list is empty inside of all digits
     */
    private void swapInside() {
        for (int i = 0; i < this.size; i++)
            this.swapInsideDigit(i);
    }

    /**
     * Swap adds with removes until one list is empty at index
     * @param index Current index
     */
    private void swapInsideDigit(int index) {
        // Get which segments have to be enabled/disabled
        Deque<Integer> enables = this.enable.get(index);
        Deque<Integer> disables = this.disable.get(index);

        // Swap inside the digit until one list is empty
        while (!enables.isEmpty() && !disables.isEmpty()) {
            int enableSegment = enables.pop();
            int disableSegment = disables.pop();

            this.addSwap(enableSegment, disableSegment, index);
        }
    }

    /**
     * Methode for saving swap of two given segments at given (different!) indexes in number into changingRow
     *
     * @param indexA   Index of first display
     * @param segmentA Index of segment in first display
     * @param indexB   Index of second display
     * @param segmentB Index of segment in second display
     */
    private void addSwap(int indexA, int segmentA, int indexB, int segmentB) {
        Swap swap = new Swap(indexA, segmentA, indexB, segmentB);
        this.swaps.add(swap);
    }

    /**
     * Methode for saving swap of two given segments at given index in number into changingRow;
     * here the segments are in the same digit
     *
     * @param segmentA Index of fist segment
     * @param segmentB Index of second segment
     * @param index    Display's index
     */
    private void addSwap(int segmentA, int segmentB, int index) {
        Swap swap = new Swap(index, segmentA, index, segmentB);
        this.swaps.add(swap);
    }

    /**
     * Methode for filling of enable and disable map
     */
    private void loadEnableDisable() {
        // Get all segments that have to be enabled/activated and deactivated/disabled
        // by looping through all digits
        for (int i = 0; i < this.size; i++) {
            // Get changes needed to get from given at i to result at i
            DigitChanges changes = HexDigitChanges.getSingleton().getChanges(this.given[i], this.result[i]);

            // Lists of segments that have to be enabled/disabled by index
            Deque<Integer> adds = changes.getAdd(ArrayDeque::new), removes = changes.getRemove(ArrayDeque::new);

            // Add arrays to their maps
            this.enable.put(i, adds);
            this.disable.put(i, removes);
        }
    }

    /**
     * Methode for getting a copy of the created swaps list
     * @return A copy of the swaps list
     */
    public LinkedList<Swap> getSwaps() {
        return JavaUtils.clone(this.swaps, LinkedList::new);
    }
}