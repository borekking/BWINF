package de.flo.bwinf.juniorAufgabe2;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class JuniorAufgabe2 {

	private int members, dates;
	private int[][] table;
	private int[] changesByDate, bestDate;

	public JuniorAufgabe2(String fileName) throws IOException {
		if (!this.init(fileName))
			throw new IOException();

		this.getNeededChangesPerDate();

		this.bestDate = this.getBestDate();
	}

	public int[] getResult() {
		return this.bestDate;
	}

	// Only returns true on success
	private boolean init(String fileName) {
		if (fileName == null)
			return false;

		// Get file's lines
		List<String> lines;
		try {
			lines = Files.readAllLines(Paths.get(fileName));
		} catch (IOException e) {
			return false;
		}

		// Get members (members) and dates (dates) amount
		String[] firstLineSplitted = lines.get(0).split(" ");
		this.members = Integer.parseInt(firstLineSplitted[0]);
		this.dates = Integer.parseInt(firstLineSplitted[1]);

		// Initialize table
		this.table = new int[this.members][this.dates];

		// Fill table with values
		for (int i = 0; i < this.members; i++) {
			// Get the current row's values
			String[] values = lines.get(i + 1).split(" ");
			// Fill current row with values
			for (int j = 0; j < this.dates; j++)
				this.table[i][j] = Integer.parseInt(values[j]);
		}

		return true;
	}

	// Fill changesByDate with count of members who do not find this date perfect
	private void getNeededChangesPerDate() {
		// Initialize changesByDate
		this.changesByDate = new int[this.dates];
		// Go through all members
		for (int i = 0; i < this.members; i++) {
			// Get best value for current member
			int lowest = -1;
			for (int j = 0; j < this.dates; j++) {
				int current = this.table[i][j];
				if (current < lowest || lowest == -1)
					lowest = current;
			}

			// Increases changesByDate[j] by one if you would have to make a change for
			// current date for current member
			for (int j = 0; j < this.dates; j++)
				if (this.table[i][j] > lowest)
					this.changesByDate[j]++;
		}
	}

	// Returns integer array with (index 0) the best date (index of date +1) and
	// (index 1) the changes you have to make for this date
	private int[] getBestDate() {
		// Get best date by changes to make
		int bestDate = 0, changes = this.changesByDate[0];
		for (int i = 1; i < this.changesByDate.length; i++) {
			int current = this.changesByDate[i];
			if (current < changes) {
				bestDate = i;
				changes = current;
			}
		}

		return new int[] { bestDate + 1, changes };
	}
}
