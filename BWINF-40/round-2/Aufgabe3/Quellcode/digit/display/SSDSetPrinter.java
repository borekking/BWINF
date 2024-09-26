package de.flo.hexMax.digit.display;

import de.flo.hexMax.digit.HexDigit;

import java.util.Arrays;
import java.util.StringJoiner;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * Class for creating readable SSDSets for printing them to console
 */
public class SSDSetPrinter {

    /**
     * Spaces between each number (in every line)
     */
    private final String delimiter;

    public SSDSetPrinter() {
        this.delimiter = "    ";
    }

    /**
     * Methode to create a human-readable String of a hex-number as its digits
     *
     * @param digits The number's digits
     * @return The readable representation of the digits
     */
    public String createSSDSetString(HexDigit[] digits) {
        return this.createSSDSetString(new SSDSet(digits));
    }

    /**
     * Methode to create a human-readable String of a hex-number as SSDSet
     *
     * @param displays The SSDSet that will be printing
     * @return The readable representation of the SSDSet
     */
    public String createSSDSetString(SSDSet displays) {
        SevenSegmentDisplay[] displaysArray = displays.getDisplays();

        // Make sure text is readable by splitting into groups of max 15 if there
        // are more than 15 displays.
        if (displaysArray.length > 15) {
            StringBuilder builder = new StringBuilder();
            int i = 0, max = displaysArray.length - 15;

            while (i < max) {
                SevenSegmentDisplay[] subArray = Arrays.copyOfRange(displaysArray, i, i + 15);
                builder.append(this.createSSDSetString(new SSDSet(subArray)));
                i += 15;
            }

            if (i < displaysArray.length - 1) {
                SevenSegmentDisplay[] subArray = Arrays.copyOfRange(displaysArray, i, displaysArray.length);
                builder.append(this.createSSDSetString(new SSDSet(subArray)));
            }

            return builder.toString();
        }

        // Function taking segment index (0-6), returning the displays' values of that segment
        Function<Integer, Boolean[]> segmentGetter = index -> Arrays.stream(displays.getDisplays()).map(d -> d.getSegment(index)).toArray(Boolean[]::new);

        // first horizontal line (first segment)
        return createHorizontal(segmentGetter.apply(0)) + System.lineSeparator() +

                // 2nd and 3rd segments
                createVertical(segmentGetter.apply(1), segmentGetter.apply(2)) + System.lineSeparator() +

                // second horizontal line (4th segments)
                createHorizontal(segmentGetter.apply(3)) + System.lineSeparator() +

                // 5th and 6th segments
                createVertical(segmentGetter.apply(4), segmentGetter.apply(5)) + System.lineSeparator() +

                // last horizontal line (7th segments)
                createHorizontal(segmentGetter.apply(6)) + System.lineSeparator();
    }

    /**
     * Methode for printing SevenSegmentDisplay's parallel, vertical segments
     *
     * @param segmentsLeft  All digits' left segment, horizontal (true/false)
     * @param segmentsRight ALL digits' right segment, horizontal (true/values)
     * @return Two lines with the segments' values
     */
    private String createVertical(Boolean[] segmentsLeft, Boolean[] segmentsRight) {
        String innerDelimiter = "      "; // Spaces for in the number
        String activated = "|"; // An activated horizontal segments
        String disabled = " "; // Spaces filling disabled horizontal segments

        // StringJoiner used for creating the horizontal line
        StringJoiner line = new StringJoiner(this.delimiter);

        for (int i = 0; i < segmentsLeft.length; i++) {
            // Get left and right segment
            boolean left = segmentsLeft[i], right = segmentsRight[i];
            // Append line
            line.add((left ? activated : disabled) + innerDelimiter + (right ? activated : disabled));
        }

        return line + System.lineSeparator() + line; // line twice; always to segments per vertical line
    }

    /**
     * Methode for printing SevenSegmentDisplay's parallel, vertical segments
     *
     * @param segments The digits' segment, vertical
     * @return One lines with the segments' values
     */
    private String createHorizontal(Boolean[] segments) {
        String activated = "  ----  "; // An activated horizontal segments
        String disabled = "        "; // Spaces filling disabled horizontal segments

        // Map to activated/disabled String; Collect using Joiner with delimiter as delimiter
        return Arrays.stream(segments).map(s -> s ? activated : disabled).collect(Collectors.joining(this.delimiter));
    }
}