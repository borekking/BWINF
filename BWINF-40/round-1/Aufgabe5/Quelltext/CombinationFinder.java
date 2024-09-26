package de.flo.bwinf.task5;

import java.util.*;
import java.util.stream.Collectors;

public class CombinationFinder {

    private final long size;
    private final List<Long> input;
    private final Map<Long, List<Long>> possibleCombinations;

    public CombinationFinder(List<Long> input) {
        this.size = input.size();
        this.input = input;
        this.possibleCombinations = new HashMap<>();

        this.setPossibleAdditions();
    }

    // Start recursively adding every number
    private void setPossibleAdditions() {
        for (int i = 0; i < this.size; i++) {
            List<Integer> list = this.copy(Collections.singletonList(i));
            this.add(list);
            this.getAll(list);
        }
    }

    // Recursively adds possibleCombinations
    private void getAll(List<Integer> list) {
        int lastElement = list.get(list.size() - 1);
        if (lastElement + 1 >= this.size) return;

        for (int i = lastElement + 1; i < this.size; i++) {
            List<Integer> list1 = this.copy(list);
            list1.add(i);
            this.add(list1);
            this.getAll(list1);
        }
    }

    // Adds list of indexes in possibleCombinations
    private void add(List<Integer> list) {
        // Get actual list from indexes
        List<Long> actualList = list.stream().map(this.input::get).collect(Collectors.toList());
        // Get sum of actualList
        long sum = this.sum(actualList);

        // If sum is not a key in possibleCombinations add sum and list to it
        if (!this.possibleCombinations.containsKey(sum))
            this.possibleCombinations.put(sum, actualList);
    }

    private <L> List<L> copy(List<L> list) {
        return new ArrayList<>(list);
    }

    // Returns sum of list
    private long sum(List<Long> list) {
        return list.stream().mapToLong(i -> i).sum();
    }

    public Map<Long, List<Long>> getPossibleCombinations() {
        return possibleCombinations;
    }
}
