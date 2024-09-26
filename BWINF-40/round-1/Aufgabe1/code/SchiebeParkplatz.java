package de.flo.bwinf.schiebeParkplatz;

import java.io.IOException;
import java.nio.file.Files;

import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SchiebeParkplatz {

    private int carAmount;
    private CarRow carRow;
    private List<String> results;

    // Throws IOException on failing to read file with name fileName
    public SchiebeParkplatz(String fileName) throws IOException {
        // Initialize
        if (!init(fileName))
            throw new IOException();

        // Add every car´s solution to results (List<String>)
        for (int index = 0; index < this.carAmount; index++)
            this.results.add(this.getCarsSolution(index));
    }

    public void print() {
        List<String> main = IntStream.range(0, this.carAmount).boxed().map(i -> String.valueOf((char) (65 + i))).collect(Collectors.toList());
        String lower = this.carRow.getAsString();
        System.out.printf("[ %s ]%n[ %s ]%n", String.join(" | ", main), lower);
    }

    // Returns false on failing to read file, else true
    private boolean init(String fileName) {
        if (fileName == null) return false;

        // Initialize results List
        this.results = new ArrayList<>();

        // Get file´s lines
        List<String> lines;
        try {
            lines = Files.readAllLines(Paths.get(fileName));
        } catch (IOException e) {
            return false;
        }

        // Get carAmounts by getting min and max from file
        String[] lineOne = lines.get(0).split(" ");
        char min = lineOne[0].charAt(0), max = lineOne[1].charAt(0);
        this.carAmount = max - min + 1;

        // Initialize carRow
        this.carRow = new CarRow(this.carAmount);
        // Fill carRow by getting name and index of each car from file
        for (int i = 2; i < lines.size(); i++) {
            String[] line = lines.get(i).split(" ");
            char name = line[0].charAt(0);
            int index = Integer.parseInt(line[1]);
            this.carRow.addCar(index, name);
        }

        return true;
    }

    // Returns Solution of car at index as String
    private String getCarsSolution(int index) {
        // Initialize String which starts with the normal cars name
        String prefix = (char) (65 + index) + ": ";

        // Check if car is not blocked, if so only returns prefix
        if (this.carRow.isEmpty(index)) {
            return prefix;
        } else {
            // Gets a result with information about direction, moves per blocking car
            // and if the Direction was successful
            Result resultLeft = this.getResult(index, Direction.LEFT), resultRight = this.getResult(index, Direction.RIGHT);
            // Return better result (not unSuccessfull or fewer moves) as String
            return resultLeft.compare(resultRight).getResult(prefix);
        }
    }

    // Counts every blocking car´s moves by simulating cars movement
    // in a copy of the carRow until the given index is not blocked
    // anymore and returns it in a Result.
    private Result getResult(int index, Direction direction) {
        // Create result and clone of carRow
        Result result = new Result(direction);
        CarRow carRowClone = this.carRow.clone();

        // Initialize integer subIndex (real index/left index)
        int subIndex = carRowClone.getRealIndex(index);

        // Loop as long as carRowClone is not empty at the index:
        while (!carRowClone.isEmpty(index)) {
            try {
                // Set subIndex to new left index of car
                subIndex = carRowClone.moveRecursively(subIndex, direction, result::add);

                // Catches exception which gets thrown on too big/too small index
            } catch (IllegalArgumentException e) {
                // Setting result unSuccessful and breaking loop
                result.setUnSuccessful(true);
                break;
            }
        }
        return result;
    }

    public CarRow getCarRow() {
        return carRow;
    }

    public List<String> getResults() {
        return results;
    }
}
