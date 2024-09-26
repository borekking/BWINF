package de.flo.hexMax.digit;

import de.flo.hexMax.digit.display.SevenSegmentDisplay;

import java.util.Arrays;

/**
 * Enum of all hexadecimal digits (hex-digits)
 */
public enum HexDigit {

    ZERO('0', 0, true, true, true, false, true, true, true),
    ONE('1', 1, false, false, true, false, false, true, false),
    TWO('2', 2, true, false, true, true, true, false, true),
    THREE('3', 3, true, false, true, true, false, true, true),
    FOUR('4', 4, false, true, true, true, false, true, false),
    FIVE('5', 5, true, true, false, true, false, true, true),
    SIX('6', 6, true, true, false, true, true, true, true),
    SEVEN('7', 7, true, false, true, false, false, true, false),
    EIGHT('8', 8, true, true, true, true, true, true, true),
    NINE('9', 9, true, true, true, true, false, true, true),
    TEN('A', 10, true, true, true, true, true, true, false),
    ELEVEN('B', 11, false, true, false, true, true, true, true),
    TWELVE('C', 12, true, true, false, false, true, false, true),
    THIRTEEN('D', 13, false, false, true, true, true, true, true),
    FOURTEEN('E', 14, true, true, false, true, true, false, true),
    FIFTEEN('F', 15, true, true, false, true, true, false, false);

    /**
     * The hex-digit's name as character (e.g. '0' or 'A')
     */
    private final char name;

    /**
     * The hex-digit's segments
     */
    private final boolean[] segments;

    /**
     * The hex-digit's value in decimal
     */
    private final int value;

    /**
     * Amount of activated segments in SSD representation
     */
    private final int enabledSegments;

    /**
     * @param name The hex-digit's name as char
     * @param value The hex-digit's value in decimal
     * @param segments Boolean array representing a hex-digit's segments using var-args
     */
    HexDigit(char name, int value, boolean... segments) {
        this.name = name;
        this.value = value;
        this.segments = segments;
        this.enabledSegments = this.getEnabledSegments(this.segments);
    }

    /**
     * @return A new SevenSegmentDisplay of the hexadecimal digit
     */
    public SevenSegmentDisplay getDisplay() {
        return new SevenSegmentDisplay(this.getSegments());
    }

    /**
     * Private methode for getting the amount of enabled segments of a digit, used in the constructor.
     * @param segments a digit's segments
     * @return Amount of enabled segments
     */
    private int getEnabledSegments(boolean[] segments) {
        int counter = 0;

        // Go through all segments; If current is enabled, increase counter.
        for (boolean segment : segments)
            if (segment) counter++;

        return counter;
    }

    // Getters
    public char getName() {
        return name;
    }

    public int getValue() {
        return value;
    }

    public int getEnabledSegments() {
        return enabledSegments;
    }

    public boolean[] getSegments() {
        return Arrays.copyOf(this.segments, this.segments.length); // Clone to provide unexpected changing
    }
}