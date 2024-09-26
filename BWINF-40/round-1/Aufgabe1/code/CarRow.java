package de.flo.bwinf.schiebeParkplatz;

import java.util.StringJoiner;
import java.util.function.Consumer;

public class CarRow implements Cloneable {

    private final char[] cars;
    private final int size;

    // Initializes size and new char-array
    public CarRow(int size) {
        this.size = size;
        this.cars = new char[this.size];
    }

    // Initializes size and char-array
    private CarRow(char[] cars) {
        this.cars = cars;
        this.size = cars.length;
    }

    // Set car´s name into index and index+1
    public void addCar(int index, char carName) {
        this.cars[index] = carName;
        this.cars[index + 1] = carName;
    }

    public CarRow clone() {
        return new CarRow(this.cars.clone());
    }

    // Check if there is a car at index
    public boolean isEmpty(int index) {
        return this.isNull(this.cars[index]);
    }

    public String getAsString() {
        StringJoiner lower = new StringJoiner(" | ");
        for (char car : this.cars)
            lower.add(String.valueOf(this.isNull(car) ? ' ' : car));

        return lower.toString();
    }

    // Returns left/real index of car after move (if moved)
    // Throws IllegalArgumentException on wrong index
    public int moveRecursively(int index, Direction direction, Consumer<Character> onSuccess)
            throws IllegalArgumentException {
        // Index where car will get moved to
        int checkIndex = direction.getCheckIndex(index);
        // Check ArrayOutOfBounds
        if (!this.validIndex(checkIndex))
            throw new IllegalArgumentException();

        // Check value of move methode
        if (this.move(index, checkIndex, direction)) {
            // Use onSuccess (task)
            onSuccess.accept(this.cars[checkIndex]);
            return direction.getNewLeftIndex(index);
        } else {
            // Calls itself if there is a blocking car blocking the current blocking car. In this case the cars
            // which are blocking the current car are getting moved recursive.
            this.moveRecursively(this.getRealIndex(checkIndex), direction, onSuccess);
            return index;
        }
    }

    // Moves car at index in direction
    // Returns if methode was able to move the car
    public boolean move(int index, int to, Direction direction) {
        // Check if move is possible
        if (!this.isEmpty(to)) return false;

        // Go in given direction
        switch (direction) {
            case LEFT:
                this.goLeft(index);
                break;
            case RIGHT:
                this.goRight(index);
                break;
        }

        return true;
    }

    // Returns left part of car
    public int getRealIndex(int index) {
        if (index + 1 >= this.size) return index - 1;

        char car1 = this.cars[index], car2 = this.cars[index + 1];
        if (car1 == car2 && !this.isNull(car1)) return index;

        return index - 1;
    }

    private boolean validIndex(int index) {
        return index >= 0 && index < this.size;
    }

    private void goLeft(int index) {
        this.cars[index + 1] = Character.MIN_VALUE;
        this.cars[index - 1] = this.cars[index];
    }

    private void goRight(int index) {
        this.cars[index + 2] = this.cars[index];
        this.cars[index] = Character.MIN_VALUE;
    }

    private boolean isNull(char c) {
        return c == Character.MIN_VALUE;
    }

    public char[] getCars() {
        return cars;
    }

    public int getSize() {
        return size;
    }
}
