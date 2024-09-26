package de.flo.hexMax.changingTrack;

import de.flo.hexMax.digit.display.SSDSet;
import de.flo.hexMax.digit.display.SSDSetPrinter;

/**
 *  Class used for tracking a hex-number's changing process (represented by SSDSets).
 */
public class SSDSetChangingRow extends ChangingRow<SSDSet> {

    /**
     * Default constructor
     */
    public SSDSetChangingRow() {
        super();
    }

    /**
     * Constructor taking another ChangingRow of same type and adding it to itself
     * @param changes The ChangingRow to add
     */
    public SSDSetChangingRow(SSDSetChangingRow changes) {
        super(changes);
    }

    /**
     * @return All entries as String
     */
    public String getText() {
        SSDSetPrinter printer = new SSDSetPrinter();
        StringBuilder builder = new StringBuilder();

        int i = 0;
        for (SSDSet ssdSet : this.elements) {
            builder.append("Step ").append(i++).append(System.lineSeparator()); // Increasing i

            builder.append(printer.createSSDSetString(ssdSet)); // Actual displays

            builder.append(System.lineSeparator()); // Empty line
        }

        return builder.toString();
    }
}