package de.flo.hexMax.digit.display;

/**
 * <p>Immutable class for hex-digits in their Seven Segment Display form.
 * <p>Short form for Seven Segment Display: SSD
 */
public class SevenSegmentDisplay {

    // Segments numbered from left to right and top to bottom
    /*
          0)
         ----
       |      |
   1)  |  3)  |  2)
         ----
   4)  |      |  5)
       |      |
         ----
          6)
     */

    public final static int SEGMENT_AMOUNT = 7;

    /**
     * The SSD's segments as booleans (true=activated, false=not activated)
     */
    private final boolean[] segments;

    /**
     * @param segments The SSD's segments
     */
    public SevenSegmentDisplay(boolean[] segments) {
        // Making sure there are exactly seven segments.
        if (segments.length != SEGMENT_AMOUNT) {
            throw new IllegalArgumentException("There have to be " + SEGMENT_AMOUNT + " segments! Provided: " + segments.length);
        }

        this.segments = segments;
    }

    /**
     * Methode for setting a specified segment to a given value (true/false)
     * @param index The Segment's index
     * @param value The segment's new value (true/false)
     * @return A new SSD (SSDs are Immutable)
     */
    public SevenSegmentDisplay setSegment(int index, boolean value) {
        boolean[] segments = this.segments.clone();
        segments[index] = value;
        return new SevenSegmentDisplay(segments); // Note: SSDs are immutable
    }

    /**
     * Methode for getting a segments value
     * @param index The segment's index
     * @return The segments value
     */
    public boolean getSegment(int index) {
        return this.segments[index];
    }

    /**
     * Methode returning if every segment is disabled
     * @return true, if all segments are disabled (false), false otherwise
     */
    public boolean isEmpty() {
        // Go through all segments and return false if the current one is true
        for (boolean seg : this.segments) {
            if (seg) return false;
        }

        // If no segment was true return true, meaning there was no enabled segment, return true;
        return true;
    }
}