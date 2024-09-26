package de.flo.hexMax.digitChanges;

import de.flo.hexMax.digit.HexDigit;
import de.flo.hexMax.digit.display.SevenSegmentDisplay;
import de.flo.hexMax.util.JavaUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.function.Function;

/**
 * Class for finding out which segments have to be changed to
 * get from one HexDigit to another.
 */
public class DigitChanges {

    /**
     * Lists of the indexes of segments which have to be removed or have to be enabled, to get goal.
     * (Those which can stay can be ignored); protected to make handling in subclasses easier.
     */
    private final List<Integer> add, remove;

    /**
     * Int-Array containing the size of add and remove list in this order [add.size, remove.size]
     */
    private final int[] values;

    /**
     * @param digit Start digit
     * @param goal Goal digit
     */
    public DigitChanges(HexDigit digit, HexDigit goal) {
        this.add = new ArrayList<>();
        this.remove = new ArrayList<>();

        this.findChanges(digit, goal);

        this.values = new int[] {this.add.size(), this.remove.size()};
    }

    /**
     * Add indexes of segment that have to be removed or added, to get from digit to goal, into their lists
     * @param digit Start digit
     * @param goal Goal digit
     */
    private void findChanges(HexDigit digit, HexDigit goal) {
        // Numbers' segments
        boolean[] digitSegments = digit.getSegments(), goalSegments = goal.getSegments();

        // Get segments indexes: stay/remove/add
        for (int i = 0; i < SevenSegmentDisplay.SEGMENT_AMOUNT; i++) {
            boolean numberSegment = digitSegments[i], goalSegment = goalSegments[i];

            // Segment has to be removed because it's enabled in number but not in goal
            if (numberSegment && !goalSegment) {
                this.remove.add(i);
            }
            // Segment has to be enabled/added because it's disabled in number but enabled in goal
            else if (!numberSegment && goalSegment) {
                this.add.add(i);
            }

            // If segment already is on right position, it can be ignored
        }
    }

    /**
     * @return The value array containing the amount of segments to add and remove
     */
    public int[] getValues() {
        return Arrays.copyOf(this.values, this.values.length);
    }

    /**
     * Methode to get a copy of add list
     * @param func A function to convert an integer collection to another Collection<Integer>
     * @param <V> Type of created Collection<Integer> (e.g. ArrayList<Integer>)
     * @return A copy of the add list converted by given function func
     */
    public <V extends Collection<Integer>> V getAdd(Function<Collection<Integer>, V> func) {
        return JavaUtils.clone(this.add, func);
    }

    /**
     * Methode to get a copy of remove list
     * @param func A function to convert an integer collection to another Collection<Integer>
     * @param <V> Type of created Collection<Integer> (e.g. ArrayList<Integer>)
     * @return A copy of the remove list converted by given function func
     */
    public <V extends Collection<Integer>> V getRemove(Function<Collection<Integer>, V> func) {
        return JavaUtils.clone(this.remove, func);
    }
}