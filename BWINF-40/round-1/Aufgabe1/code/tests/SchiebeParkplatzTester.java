package de.flo.bwinf.schiebeParkplatz.tests;

import de.flo.bwinf.schiebeParkplatz.CarRow;
import de.flo.bwinf.schiebeParkplatz.Direction;
import de.flo.bwinf.schiebeParkplatz.SchiebeParkplatz;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.util.List;

public class SchiebeParkplatzTester {

    @Test
    public void parkplatz0() {
        assertTrue(this.test("parkplatz0.txt"), "parkplatz0.txt failed");
    }

    @Test
    public void parkplatz1() {
        assertTrue(this.test("parkplatz1.txt"), "parkplatz1.txt failed");
    }

    @Test
    public void parkplatz2() {
        assertTrue(this.test("parkplatz2.txt"), "parkplatz2.txt failed");
    }

    @Test
    public void parkplatz3() {
        assertTrue(this.test("parkplatz3.txt"), "parkplatz3.txt failed");
    }

    @Test
    public void parkplatz4() {
        assertTrue(this.test("parkplatz4.txt"), "parkplatz4.txt failed");
    }

    @Test
    public void parkplatz5() {
        assertTrue(this.test("parkplatz5.txt"), "parkplatz5.txt failed");
    }

    private boolean test(String fileName) {
        SchiebeParkplatz schiebeParkplatz;
        try {
            schiebeParkplatz = new SchiebeParkplatz(fileName);
        } catch (IOException ignored) {
            return true;
        }

        List<String> list = schiebeParkplatz.getResults();
        CarRow carRow = schiebeParkplatz.getCarRow();

        if (list.size() < carRow.getSize()) return false;

        // Test all moves
        for (int i = 0; i < list.size(); i++)
            if (!this.testMoves(i, carRow, list.get(i)))
                return false;

        return true;
    }

    private boolean testMoves(int index, CarRow carRow, String str) {
        String[] arr = str.split(": ");
        // No result: Test if no result is needed
        if (arr.length < 2) return carRow.isEmpty(index);

        String[] moves = arr[1].split(", ");
        CarRow carRowCopy = carRow.clone();

        for (String move : moves) {
            String[] split = move.split(" ");
            char car = split[0].charAt(0);
            int lowerIndex = carRow.getRealIndex(this.getIndex(carRowCopy.getCars(), car));

            Direction direction = this.getDirection(split[2]);
            if (direction == null) return false;

            int amount = Integer.parseInt(split[1]);

            for (int i = 0; i < amount; i++) {
                int moveIndex = direction.getCheckIndex(lowerIndex);
                if (moveIndex < 0 && moveIndex >= carRow.getSize())
                    return false;

                if (!carRowCopy.move(lowerIndex, moveIndex, direction))
                    return false;

                lowerIndex = direction == Direction.LEFT ? lowerIndex-1 : lowerIndex+1;
            }
        }

        return true;
    }

    private Direction getDirection(String d) {
        if (d.equalsIgnoreCase("links"))
            return Direction.LEFT;
        else if (d.equalsIgnoreCase("rechts"))
            return Direction.RIGHT;
        else return null;
    }

    private int getIndex(char[] cars, char name) {
        for (int index = 0; index < cars.length; index++)
            if (cars[index] == name)
                return index;
        return -1;
    }
}
