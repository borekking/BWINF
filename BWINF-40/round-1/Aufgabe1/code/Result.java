package de.flo.bwinf.schiebeParkplatz;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Result {

	private final Direction direction; // for message
	private final Map<Character, Integer> moves; // move amount per car (by name)
	private final List<Character> keys; // List to save order of moved cars
	private boolean unSuccessful; // If the Direction was unsuccessful

	public Result(Direction direction) {
		this.direction = direction;

		this.moves = new HashMap<>();
		this.keys = new ArrayList<>();
	}

	// Returns Result which was not unSuccessful. If both were successful:
	// returns result with less absolut moves
	public Result compare(Result that) {
		return this.isUnSuccessful() ? that
				: that.isUnSuccessful() ? this : this.getAbsMoves() < that.getAbsMoves() ? this : that;
	}

	// Add car by increasing it´s value in Map (moves) and
	// (Re-) Putting it into List (keys)
	public void add(char car) {
		this.moves.put(car, this.moves.getOrDefault(car, 0) + 1);

		if (this.keys.contains(car))
			this.keys.remove((Character) car);
		this.keys.add(car);
	}

	// Returns the Result as String
	public String getResult(String prefix) {
		List<String> res = new ArrayList<>();

		for (char key : this.keys)
			res.add(key + " " + this.moves.get(key) + " " + this.direction.getName());

		return prefix + String.join(", ", res);
	}

	// Returns all moves in moves added up to a sum
	private int getAbsMoves() {
		return this.moves.values().stream().mapToInt(i -> i).sum();
	}

	public void setUnSuccessful(boolean unSuccessful) {
		this.unSuccessful = unSuccessful;
	}

	public boolean isUnSuccessful() {
		return unSuccessful;
	}
}
