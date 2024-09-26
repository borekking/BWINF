package de.flo.bwinf.schiebeParkplatz;

import java.util.function.Function;

public enum Direction {

	//         name    newLeftIndex   checkIndex
	LEFT("links", i -> --i,     i -> i-1),
	RIGHT("rechts", i -> ++i,   i -> i+2);

	private final String name;
	private final Function<Integer, Integer> newLeftIndex, checkIndex;

	Direction(String name, Function<Integer, Integer> newLeftIndex, Function<Integer, Integer> checkIndex) {
		this.name = name;
		// index which will be the new left index after a move
		this.newLeftIndex = newLeftIndex;
		// index which will be new and has to be checked after a move
		this.checkIndex = checkIndex;
	}

	// Get index which will be the new left index
	public int getNewLeftIndex(int i) {
		return this.newLeftIndex.apply(i);
	}

	// Get index which will be new and has to be checked
	public int 	getCheckIndex(int i) {
		return this.checkIndex.apply(i);
	}

	public String getName() {
		return name;
	}
}

