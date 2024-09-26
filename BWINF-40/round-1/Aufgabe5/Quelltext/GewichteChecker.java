package de.flo.bwinf.task5;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.Paths;
import java.util.*;

public class GewichteChecker {

    // Biggest creatable addable
    private long biggestAddable;

    // Amount of total weights (including doubles)
    private int maxWeights;

    // Map of weights: weight -> amount
    private Map<Long, Integer> weights;

    // List of ListResult with doubled elements
    private List<ListResult> doubles;

    // Map of Numbers which can be created with a List of weights:
    // Number -> List
    private Map<Long, List<Long>> addables;

    // List of weights which can not be created by the weights
    private List<Long> notPossibleWeights,

    // List with all addables in sorted order
    addablesKeys;

    // Indirect Results Map: (Weight -> IndirectResult: (closestWeight (long), List Left, List Right) )
    private Map<Long, IndirectResult> closestResults;

    // Direct Results Map: (Weight -> ListResult: (List Left, List Right) )
    private Map<Long, ListResult> directResults,

    // Cache with working weights: Map: (Weight -> ListResult: (List Left, List Right) )
    cache;

    public GewichteChecker(String fileName) throws IOException {
        if (!init(fileName))
            throw new IOException();

        if (this.weights.size() == 0) {
            for (long w = 10; w <= 10_000; w+=10)
                this.closestResults.put(w, new IndirectResult(0, ListResult.emptyListResult()));
            return;
        }

        this.setAddables();

        this.setSortedKeys();

        this.biggestAddable = this.addablesKeys.get(this.addablesKeys.size() - 1);

        this.getDirectResults();

        this.getClosestsResults();
    }

    public ListResult getResult(long weight) {
        ListResult result = null;

        if (this.directResults.containsKey(weight))
            result = this.directResults.get(weight);

        else if (this.closestResults.containsKey(weight))
            result = this.closestResults.get(weight);

        return result;
    }

    // Returns false on failing to read file, else true
    private boolean init(String fileName) {
        if (fileName == null) return false;

        this.notPossibleWeights = new ArrayList<>();
        this.doubles = new ArrayList<>();

        this.directResults = new HashMap<>();
        this.cache = new HashMap<>();
        this.weights = new HashMap<>();
        this.closestResults = new HashMap<>();

        // Get file´s lines
        List<String> lines;
        try {
            lines = Files.readAllLines(Paths.get(fileName));
        } catch (IOException | InvalidPathException e) {
            return false;
        }

        // Fill weights
        for (int i = 1; i < lines.size(); i++) {
            String[] line = lines.get(i).split(" ");
            long weight = Long.parseLong(line[0]);
            int amount = Integer.parseInt(line[1]);

            this.weights.put(weight, amount);
        }

        return true;
    }

    // Sets addables to List of all possible Combinations of weights (weight to List)
    private void setAddables() {
        // Create List of all weights (including doubles)
        List<Long> totalWeights = new ArrayList<>();
        for (long key : this.weights.keySet())
            for (int i = 0; i < this.weights.get(key); i++)
                totalWeights.add(key);

        // Set maxWeights to size of created list
        this.maxWeights = totalWeights.size();

        // Set addables
        CombinationFinder combinationFinder = new CombinationFinder(totalWeights);
        this.addables = combinationFinder.getPossibleCombinations();
    }

    // Adds ListResult for all weights (10, 20, ..., 10_000) using methode
    // "getDirectResult". (ListResult: Left List and Right List) If there is no
    // result nothing gets added
    private void getDirectResults() {
        for (long weight = 10; weight <= 10_000; weight += 10) {
            ListResult result = this.getDirectResult(weight);

            if (result != null)
                this.directResults.put(weight, result);
        }
    }

    // Adds the closest result for all weights (10, 20, ..., 10_000)
    // if there is no "best"/normal result in "directResults" using the
    // methode "getClosestResult".
    // (IndirectResult: Closest Weight, Left List, Right List)
    private void getClosestsResults() {
        if (this.directResults.size() == 1000) return;

        for (long weight = 10L; weight <= 10_000; weight += 10) {
            // Skip if there already is a ListResult for current weight
            if (this.directResults.containsKey(weight)) continue;

            IndirectResult result = this.getClosestResult(weight);
            long closestWeight = result.getClosestWeight();

            // Put new result in cache
            if (!this.cache.containsKey(closestWeight))
                this.cache.put(closestWeight, result);

            this.closestResults.put(weight, result);
        }
    }

    // Return the "ListResult" of a weight:
    // (ListResult = Left List and Right List which form the weight)
    private ListResult getDirectResult(long weight) {
        // Check if you can form the weight from the input-weights
        if (this.isAddable(weight)) {
            return new ListResult(new ArrayList<>(), this.addables.get(weight));
        } else {
            // Gets ListResult (might be null)
            return this.getListResult(weight);
        }
    }

    // Returns the closest result possible to weight
    private IndirectResult getClosestResult(long weight) {
        // Get closest normal weight below weight
        long closestBelow = 0L, closestAhead = -1L;
        for (long l = weight - 10; l >= 10; l -= 10) {
            if (this.directResults.containsKey(l)) {
                closestBelow = l;
                break;
            }
        }

        // Get closest normal weight ahead of weight
        for (long l = weight + 10; l <= 10_000; l += 10) {
            if (this.directResults.containsKey(l)) {
                closestAhead = l;
                break;
            }
        }

        // Make sure closestAhead is not -1
        boolean check = false;
        if (closestAhead == -1) {
            closestAhead = this.getClosestAddableWeight(weight);
            check = true;
        }

        // Difference between weight and closestBelow
        long difBelow = weight - closestBelow;
        // Difference between weight and numAhead
        // (Make sure difAhead is not -1 because closestAhead might be smaller than weight)
        long difAhead = Math.abs(closestAhead - weight);

        // Check if closestAhead is equal to sum of all weights (biggestAddable)
        // and the difference to closestAhead is smaller (or equal) to difBelow
        if (closestAhead == this.biggestAddable && difAhead <= difBelow)
           return new IndirectResult(closestAhead, new ArrayList<>(), this.addables.get(closestAhead));

        // Gets smaller difference                    
        long smallerDif = Math.min(difBelow, difAhead);

        // Test all weight from (weight-smallerDif) to (weight+smallerDif), going away from weight
        for (long i = 1; i < smallerDif; i++) {
            // Skip weights dividable by 10 because it would be in directResults (-> already checked)
            if (i % 10 == 0) continue;

            // Initialize weight i steps left and right from weight
            long w1 = weight - i, w2 = weight + i;

            // Ckeck already existing weights
            if (this.cache.containsKey(w1))
                return new IndirectResult(w1, this.cache.get(w1));

            if (this.cache.containsKey(w2))
                return new IndirectResult(w2, this.cache.get(w2));

            // Get IndirectResults usning "getIndirectResult" and return them if they are not null
            IndirectResult indirectResult1 = this.getIndirectResult(w1);
            if (indirectResult1 != null)
                return indirectResult1;

            IndirectResult indirectResult2 = this.getIndirectResult(w2);
            if (indirectResult2 != null)
                return indirectResult2;
        }

        // Set closest if there is nothing addable between closestBelow and closestAhead
        long closest = difAhead < difBelow ? closestAhead : closestBelow;

        // Set result
        ListResult result;
        if (closest == 0) {
            result = ListResult.emptyListResult();
        } else if (!check) {
            result = this.directResults.get(closest);
        } else {
            result = this.directResults.containsKey(closest) ? this.directResults.get(closest) : new ListResult(new ArrayList<>(), this.addables.get(closest));
        }

        // Return map of closest and result
        return new IndirectResult(closest, result);
    }

    // Getting result of w if notPossibleWeights does not contain w1
    private IndirectResult getIndirectResult(long w) {
        if (!this.notPossibleWeights.contains(w)) {
            // Get result of w
            ListResult result = this.getDirectResult(w);

            if (result != null) {
                return new IndirectResult(w, result);
            } else {
                this.notPossibleWeights.add(w);
            }
        }
    
        return null;
    }

    // Return ListResult for weight if existing
    private ListResult getListResult(long weight) {
        // Goes through all possible addable numbers
        for (long current : this.addablesKeys) {
            // Get difference from weight to current
            long dif = current - weight;

            if (dif < 0) continue; // Prevent negative differences

            // Check if the difference is also addable
            if (this.isAddable(dif)) {
                List<Long> left = this.addables.get(dif), right = this.addables.get(current);

                // Check if there are one or more weights in both lists, which are used to often
                if (this.hasNoDoubles(left, right))
                    return new ListResult(left, right);
            }
        }

        return null;
    }

    // Returns if there are weights more often than possible
    private boolean hasNoDoubles(List<Long> left, List<Long> right) {
        if (left.size() + right.size() > this.maxWeights) return false;

        // Check if left and right are already marked as double in "doubles"
        if (this.doubles.stream().anyMatch(result -> result.getLeft().equals(left)
                && result.getRight().equals(right))) return false;

        // Actually checking if there are elements too often, return true if not so
        if (this.noDoubles(left, right)) return true;

        // Else putting lists in "doubles" and return false
        this.doubles.add(new ListResult(left, right));
        return false;
    }

    // Returns if there are weights more often than possible
    private boolean noDoubles(List<Long> list, List<Long> list1) {
        Map<Long, Integer> existingTimes = new HashMap<>();
        list.forEach(i -> existingTimes.put(i, existingTimes.getOrDefault(i, 0) + 1));
        list1.forEach(i -> existingTimes.put(i, existingTimes.getOrDefault(i, 0) + 1));

        for (long key : existingTimes.keySet()) {
            long actualTimes = this.getAmount(key), realTimes = existingTimes.get(key);
            if (realTimes > actualTimes) return false;
        }

        return true;
    }

    // Returns the closest addable (from "addables") to w
    private long getClosestAddableWeight(long w) {
        long closest = 0, dif = w;
        for (long l : this.addablesKeys) {
            long lDif = Math.abs(w - l);

            if (l > w) {
                if (lDif < dif)
                    return l;
                return closest;
            }

            if (lDif < dif) {
                closest = l;
                dif = lDif;
            }
        }

        return this.biggestAddable;
    }

    public void setSortedKeys() {
        this.addablesKeys = new ArrayList<>(this.addables.keySet());
        Collections.sort(this.addablesKeys);
    }

    // Returns amount of weight
    private long getAmount(long weight) {
        return this.weights.get(weight);
    }

    // Returns if weight is addable
    private boolean isAddable(long weight) {
        return this.addables.containsKey(weight);
    }

    public int getDirectResultsSize() {
        return this.directResults.size();
    }

    public int getClosestResultsSize() {
        return this.closestResults.size();
    }
}
