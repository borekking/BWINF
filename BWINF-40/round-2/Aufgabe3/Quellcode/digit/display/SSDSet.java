package de.flo.hexMax.digit.display;

import de.flo.hexMax.digit.HexDigit;

import java.util.Arrays;

/**
 * Immutable class for representing one or more Seven Segment Displays
 * by holding them in an array.
 */
public class SSDSet {

    /**
     * Array of all SevenSegmentDisplays
     */
    private final SevenSegmentDisplay[] displays;

    /**
     * @param displays Array of all SevenSegmentDisplays
     */
    public SSDSet(SevenSegmentDisplay[] displays) {
        this.displays = displays;
    }

    /**
     * Constructor taking HexDigits that will be converted to SSDs
     * @param digits Digit that will be converted to SSDs
     */
    public SSDSet(HexDigit[] digits) {
        int length = digits.length;
        this.displays = new SevenSegmentDisplay[length];

        for (int i = 0; i < length; i++) {
            this.displays[i] = digits[i].getDisplay();
        }
    }
    /**
     * Methode for changing a display in this SSDSet to another one
     * @param index The display's index
     * @param display The new display
     * @return A new SSDSet (immutable)
     */
    public SSDSet setDisplay(int index, SevenSegmentDisplay display) {
        SevenSegmentDisplay[] displays = this.getDisplays();
        displays[index] = display;
        return new SSDSet(displays); // SSDSet are immutable
    }

    /**
     * Display getter, returning copy of actual array -> immutable
     * @return A copy of the displays
     */
    public SevenSegmentDisplay[] getDisplays() {
        return Arrays.copyOf(this.displays, this.displays.length);
    }
}