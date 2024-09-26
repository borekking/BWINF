package de.flo.hexMax;

import de.flo.hexMax.changingTrack.SSDSetChangingRow;
import de.flo.hexMax.digit.HexDigit;
import de.flo.hexMax.digit.display.SSDSet;
import de.flo.hexMax.digit.display.SevenSegmentDisplay;
import de.flo.hexMax.util.JavaUtils;

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * Class to get from created swaps to actual changing row from start number to resulting number as SSDSets
 */
public class ChangingRowCreater {

    /**
     * Created SSDSetChangingRow that will contain all changes to get from given to result number
     */
    private final SSDSetChangingRow changingRow;

    /**
     * Created swaps to get from given to result number
     */
    private final Deque<ResultChangesCreater.Swap> swaps;

    /**
     * Constructor taking created swaps
     *
     * @param swaps Created swaps to get from given to result number
     */
    public ChangingRowCreater(Deque<ResultChangesCreater.Swap> swaps, HexDigit[] givenNumber) {
        SSDSet startNumber = new SSDSet(givenNumber);

        this.swaps = JavaUtils.clone(swaps, ArrayDeque::new);
        this.changingRow = new SSDSetChangingRow();

        this.fillChangingRow(startNumber);
    }

    /**
     * Methode to fill get actual changing row using the swaps
     * @param startNumber First/Start number as SSDSet
     */
    private void fillChangingRow(SSDSet startNumber) {
        SSDSet ssdSet = startNumber;

        while (!this.swaps.isEmpty()) {
            ResultChangesCreater.Swap swap = this.swaps.pop();

            if (swap.getIndexA() == swap.getIndexB()) {
                ssdSet = this.addSwap(ssdSet, swap.getSegmentIndexA(), swap.getSegmentIndexB(), swap.getIndexA());
            } else {
                ssdSet = this.addSwap(ssdSet, swap.getIndexA(), swap.getSegmentIndexA(), swap.getIndexB(), swap.getSegmentIndexB());
            }
        }
    }

    /**
     * Methode for saving swap of two given segments at given (different!) indexes in number into changingRow by changing the SSDSet and returning the new one.
     *
     * @param ssdSet   Current SSDSet
     * @param indexA   Index of first display
     * @param segmentA Index of segment in first display
     * @param indexB   Index of second display
     * @param segmentB Index of segment in second display
     * @return The new SSDSet
     */
    private SSDSet addSwap(SSDSet ssdSet, int indexA, int segmentA, int indexB, int segmentB) {
        // Get displays
        SevenSegmentDisplay displayA = ssdSet.getDisplays()[indexA];
        SevenSegmentDisplay displayB = ssdSet.getDisplays()[indexB];

        // Get the values of the segments in the displays
        boolean valueA = displayA.getSegment(segmentA);
        boolean valueB = displayB.getSegment(segmentB);

        // Update the displays with their negated value at given segment index
        displayA = displayA.setSegment(segmentA, !valueA);
        displayB = displayB.setSegment(segmentB, !valueB);

        // Update ssdSet with changed displays and add it to changingRow
        // (note that SSDs and SSDSets are immutable)
        ssdSet = ssdSet.setDisplay(indexA, displayA);
        ssdSet = ssdSet.setDisplay(indexB, displayB);
        this.changingRow.add(ssdSet);

        // Return new SSDSet
        return ssdSet;
    }

    /**
     * Methode for saving swap of two given segments at given index in number into changingRow;
     * here the segments are in the same digit
     *
     * @param ssdSet   Current SSDSet
     * @param segmentA Index of fist segment
     * @param segmentB Index of second segment
     * @param index    Display's index
     * @return The new SSDSet
     */
    private SSDSet addSwap(SSDSet ssdSet, int segmentA, int segmentB, int index) {
        // Get display
        SevenSegmentDisplay display = ssdSet.getDisplays()[index];

        // Get the values of the segments in the displays
        boolean valueA = display.getSegment(segmentA);
        boolean valueB = display.getSegment(segmentB);

        // Update the display with the values' negated values at given segments
        display = display.setSegment(segmentA, !valueA);
        display = display.setSegment(segmentB, !valueB);

        // Update ssdSet with changed display and add it to changingRow
        // (note that SSDs and SSDSets are immutable)
        ssdSet = ssdSet.setDisplay(index, display);
        this.changingRow.add(ssdSet);

        // Return new SSDSet
        return ssdSet;
    }

    /**
     * @return A copy of created changingRow
     */
    public SSDSetChangingRow getChangingRow() {
        return new SSDSetChangingRow(this.changingRow);
    }
}