package de.flo.bwinf.task5;

import java.util.List;

public class IndirectResult extends ListResult {

    /*
     * Saves Two Lists (ListResult) and a "closest" weight
     *
     * @Overrides the serializeSolution methode from ListResult
     *
     */

    private final long closestWeight;

    public IndirectResult(long weight, ListResult lr) {
        super(lr.getLeft(), lr.getRight());

        this.closestWeight = weight;
    }

    public IndirectResult(long weight, List<Long> left, List<Long> right) {
        super(left, right);

        this.closestWeight = weight;
    }

    @Override
    public String serializeSolution() {
        return String.format("naechtes Ergebnis: %dg: " + super.serializeSolution(), this.closestWeight);
    }

    public long getClosestWeight() {
        return closestWeight;
    }
}
