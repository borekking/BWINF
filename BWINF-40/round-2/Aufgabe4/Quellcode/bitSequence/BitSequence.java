package de.flo.zzz.bitSequence;

import java.util.Arrays;

/**
 * Comparable implementation of a bit sequence containing bits.
 * <p>
 * Example: 1001, 110, 000, 110001
 *
 */
public class BitSequence implements Comparable<BitSequence> {

    /**
     * Amount of bit in the BitSequence
     */
    private final int size;

    /**
     * The BitSequence's actual bits
     */
    private final boolean[] bits;

    /**
     * Contractor creating a BitSequence with given bits
     * @param bits The BitSequence's bits as boolean-array
     */
    public BitSequence(boolean[] bits) {
        this.size = bits.length;
        this.bits = bits;
    }

    /**
     * Constructor taking a binary number as bits
     * @param binaryNumber The binary string
     */
    public BitSequence(String binaryNumber) {
        this.size = binaryNumber.length();
        this.bits = BitSequenceUtils.getBooleanArray(binaryNumber);
    }

    /**
     * XOR operation of two BitSequences, returning a new BitSequence.
     * <p>
     * Note that this operation only works if both BitSequences have the same length.
     * <p>
     * This methode runs in O(n) where n is the BitSequences' size.
     * @param that The second BitSequence to do the XOR on
     * @return The resulting XOR as new BitSequence
     */
    public BitSequence xor(BitSequence that) {
        // Make sure sequences have same length
        if (this.size != that.size) {
            throw new IllegalArgumentException("BitSequences have to have the same amount of bits to use the xor-operation!");
        }

        // Create and fill new boolean-array with XORs
        boolean[] result = new boolean[this.size];

        for (int i = 0; i < this.size; i++) {
            result[i] = this.bits[i] ^ that.bits[i];
        }

        // Return NEW BitSequence
        return new BitSequence(result);
    }

    /**
     * Methode to create readable BS
     * @return Readable BS
     */
    @Override
    public String toString() {
        return BitSequenceUtils.getBinaryString(this.bits);
    }

    /**
     * Equals methode comparing by BS by bits
     * @param o Object to check equality
     * @return If this and o are equal by content
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        BitSequence that = (BitSequence) o;
        return Arrays.equals(this.bits, that.bits);
    }

    /**
     * HashCode methode, creating HashCode by content (bits)
     * @return HashCode created by bits
     */
    @Override
    public int hashCode() {
        return Arrays.hashCode(this.bits);
    }

    /**
     * Comparable implementation to make BS' comparable
     * @param that BS to compare to
     * @return 1 if this is bigger, -1 if that is bigger, 0 for equality
     */
    @Override
    public int compareTo(BitSequence that) {
        if (this.size != that.size) {
            throw new RuntimeException("Can not compare BitSequences, which have not the same size!");
        }

        // Compare bit arrays (this.bits, that.bits) using BitSequence#getBit
        for (int i = 0; i < this.size; i++) {
            boolean bitThis = this.getBit(i);
            boolean bitThat = that.getBit(i);

            if (bitThis && !bitThat) return 1; // Current bit in this is true, current bit in that isn't -> this > that
            if (bitThat && !bitThis) return -1; // Current bit in that is true, current bit in this isn't -> this < that
        }

        // Arrays are equal
        return 0;
    }

    /**
     * Methode to get a bit of BitSequence on given index
     * @param index The bit's index
     * @return The bit at given index
     * @throws ArrayIndexOutOfBoundsException If index >= size
     */
    public boolean getBit(int index) {
        return this.bits[index];
    }

    public int getSize() {
        return size;
    }
}