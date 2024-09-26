package de.flo.hexMax.changingTrack;

import java.util.Deque;
import java.util.Iterator;
import java.util.LinkedList;

/**
 * <p>Class for keeping track of any Datatype T in a specific order.
 *
 * <p>Because only adding and iterating is needed (no instant access), using
 * a LinkedList for holding the elements.
 *
 * <p>This class implements the Iterable interface s.t. you can iterate
 * through the elements.
 */
public class ChangingRow<T> implements Iterable<T> {

    /**
     * Deque holding elements; protected to make handling in child classes easier.
     * No indexed access to elements.
     */
    protected final Deque<T> elements;

    /**
     * Default constructor
     */
    public ChangingRow() {
        this.elements = new LinkedList<>();
    }

    /**
     * Constructor taking another ChangingRow of same type and adding it to itself
     * @param changes The ChangingRow to add
     */
    public ChangingRow(ChangingRow<T> changes) {
        this();
        this.addAll(changes);
    }

    /**
     * Methode for adding a new element
     * @param t Element to add
     */
    public void add(T t) {
        this.elements.add(t);
    }

    /**
     * Methode to add a whole ChangingRow
     * @param changes The ChangingRow to add
     */
    public void addAll(ChangingRow<T> changes) {
        this.elements.addAll(changes.elements);
    }

    public int getSize() {
        return this.elements.size();
    }

    @Override
    public Iterator<T> iterator() {
        return this.elements.iterator();
    }
}