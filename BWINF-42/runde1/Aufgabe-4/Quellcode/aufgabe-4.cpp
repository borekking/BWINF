#include <bits/stdc++.h>

using namespace std;

// Declare constants for the different components / building blocks
const string X = "X";

const string B = "B";
const string R = "R";
const string r = "r";
const string W = "W";

// Function computing the resulting boolean vector created by inputing
// a given boolean vector input into the building-blocks construct given.
vector<bool> create_result(vector<vector<string>> &blocks, int columns, int rows, vector<bool> &start) {
    // Track the current input row using the current_row boolean vector
    // of length (columns) and iterativle construct the next one.
    vector<bool> current_row = start;

    // Iterate through all rows of the building blocks 
    // and compute the next input row using the current one (current_row)
    for (int row = 0; row < rows - 2; row++) {
        // Store the input row in next_row
        vector<bool> next_row(columns, 0);

        // Iterate through the columns of the current row
        // and construct the next input row
        int column = 0;

        // Loop as long as the current column is smaller then columns - 1,
        // because the program will try to access the symbol the the right of column,
        // since the building blocks only come in pairs of two symbols.
        while (column < columns - 1) {
            // If the current column does not contains a building block, 
            // skip to the next column.
            if (blocks[row][column] == X) {
                column++;
                continue;
            } 
            
            // Receive the current building block consisting of two symbols
            string s1 = blocks[row][column];
            string s2 = blocks[row][column + 1];

            // Check for the different types of building blocks:
            // blue, white, and red 
            // For each one: Construct the next input row.

            // 1. Case: Blue building block
            if (s1 == B) {
                // A blue block correspond to passing the current state down to the next one
                next_row[column] = current_row[column];
                next_row[column + 1] = current_row[column + 1];
            // 2. Case: White building block
            } else if (s1 == W) {
                // A white block enables both columns iff (if and only if) 
                // less then two columns are "enabled" in the current rows.
                // That is iff not both columns are "enabled".
                next_row[column] = !(current_row[column] && current_row[column + 1]);
                next_row[column + 1] = !(current_row[column] && current_row[column + 1]);
            // 3. Case: Red building block
            } else if (s1 == r || s1 == R) {
                // 3.1: The first symbol is R (upper case) and the second is r (lower case)
                // In that case both columns of the next row are enabled iff the first 
                // of the two colums is "disabled".
                if (s1 == R) {
                    next_row[column] = !current_row[column];
                    next_row[column + 1] = !current_row[column];
                // 3.2: the first symbol is r (lower case) and the second is R (upper case)
                // In that case the both columns of the next row are enabled iff the second 
                // of the two colums is "disabled".
                } else {
                    next_row[column] = !current_row[column + 1];
                    next_row[column + 1] = !current_row[column + 1];
                }
            }

            // Increase the column by 2 to check for the next building block
            column += 2;
        }

        // Set the next input row to be the one just constructed
        current_row = next_row;
    }

    // Return the next input row computes last,
    // such that the enabled/disabled LEDs can be computed.
    return current_row;
}

int main() {
    // Get filename for input and output files
    cout << "Please enter a filename:" << endl;    
    
    string filename;
    cin >> filename;
    
    cout << "Please enter an output file:" << endl;
    
    string output;
    cin >> output;
    
    cout << "Output will be written to: " << output << endl;

    freopen(filename.c_str(), "r", stdin);
    freopen(output.c_str(), "w", stdout);
    

    int columns, rows;
    cin >> columns >> rows;

    // Read 1st row: Inputs / Lamps.
    // inputs stores the indexes of the lamps (0-indexes)
    vector<int> inputs; 

    for (int i = 0; i < columns; i++) {
        string symbol;
        cin >> symbol;

        // Check weather the current symbol is not an captial X
        // i.e. a Q*.
        if (symbol != X) {
            inputs.push_back(i);
        }
    }

    // Read the rows-2 rows in the middle: Building blocks.
    // blocks stores the building blocks in the rows-2 rows; 2D string-vector of size (m-2)*columns
    vector<vector<string>> blocks(rows-2, vector<string>(columns));

    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < columns; j++) {
            string symbol;
            cin >> symbol;
            blocks[i][j] = symbol;
        }
    }

    // Read last row: Outputs / LEDs
    vector<int> outputs;

    for(int i = 0; i < columns; i++) {
        string symbol;
        cin >> symbol;

        // Check weather the current symbol is not an captial X,
        // i.e. a L*.
        if (symbol != X) {
            outputs.push_back(i);
        }
    }
    
    // Test all 2^n possible variations of inputs for the lamps (0 or 1)
    // where n is the amount of lamps in the 1st row
    int amount_inputs = (int) inputs.size();
    int amount_outputs = (int) outputs.size();
    int amount_total = 1 << amount_inputs; // 2^amount_inputs for amount_inputs <= 30

    for(int i = 0; i < amount_total; i++) {
        // The current variation is given by the binary representation of i.
        // start will store an input for the building blocks stores in blocks;
        // e.i., every column will start with 0 or 1. Ones can only occour if
        // there is a lamp in that column and that one is enabled in the current variation.
        vector<bool> start(columns, 0);

        // Calculate the start vector (i.e. the input for the builduing blocks)
        // using the binary representation of i.
        for (int j = 0; j < amount_inputs; j++) {
            // Check if the j-th bit of i is 1 from the right
            if (i & (1 << (amount_inputs - j - 1))) {
                // If so, set the j-th lamp to 1.
                // That is, set the position of the j-th lamp to 1 in the start vector
                start[inputs[j]] = 1;
            }
        }

        // Calculate the resulting output using the current input (start).
        vector<bool> result = create_result(blocks, columns, rows, start);

        // Write the current variation of inputs for the lamps to the output file.
        for (int j = 0; j < amount_inputs; j++) {
            bool enabled = start[inputs[j]];
            cout << enabled << " ";
        }
        
        cout << "=> ";

        // Write the resulting outputs of the LEDs to the output file
        for(int j = 0; j < amount_outputs; j++) {
            bool enabled = result[outputs[j]];
            cout << enabled << " ";
        }

        cout << endl;
    }

    return 0;
}