package de.flo.bwinf.task5;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class ListResult {

    public static ListResult emptyListResult() {
        return new ListResult(new ArrayList<>(), new ArrayList<>());
    }

    /*
     * Contains a left and a right List
     */

    protected final List<Long> left, right;

    public ListResult(List<Long> left, List<Long> right) {
        this.left = left;
        this.right = right;
    }

    public List<Long> getLeft() {
        return left;
    }

    public List<Long> getRight() {
        return right;
    }

    public String serializeSolution() {
        if (this.left.isEmpty() && this.right.isEmpty())
            return String.format("links: /; rechts: /%n");
        else if (this.left.isEmpty())
            return String.format("links: /; rechts: %s%n", listToString(this.right, ", ", "g"));
        else
            return String.format("links: %s; rechts: %s%n", listToString(this.left, ", ", "g"), listToString(this.right, ", ", "g"));
    }

    private String listToString(List<Long> list, String delimiter, String after) {
        return list.stream().map(l -> l + after).collect(Collectors.joining(delimiter));
    }
}
