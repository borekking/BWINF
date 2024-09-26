#include <bits/stdc++.h>

using namespace std;

int main() {
    // Get filename and use in/out to given file and an output file "output.txt"
    cout << "Please enter a filename:" << endl;    
    
    string filename;
    cin >> filename;
    
    cout << "Please enter an output file:" << endl;
    
    string output;
    cin >> output;
    
    cout << "Output will be written to: " << output << endl;

    freopen(filename.c_str(), "r", stdin);
    freopen(output.c_str(), "w", stdout);

    // Read from file: number of bags, numbert of types, amount of games per type
    int bags, amount_types;
    cin >> bags >> amount_types;
    vector<int> types(amount_types);

    for (int i = 0; i < amount_types; i++) {
        cin >> types[i];
    }

    // Calculate the result - Per bag: Number of games per type 
    vector<vector<int>> result(bags, vector<int>(amount_types, 0));
    int index = 0; // Keep track of the current bag 

    // For each type: Calculate number of games per bag + rest
    for (int i = 0; i < amount_types; i++) {
        // Calculate the number of games each bag can get of current type using
        // the total number of games of current type. + Add that to each bag
        int number_games = types[i];
        int games_per_bag = number_games / bags; // Integer division!

        for (int j = 0; j < bags; j++) {
            result[j][i] += games_per_bag;
        }

        // Now, add the left over games of current type
        int rest = number_games % bags;
        for (int j = 0; j < rest; j++) {
            result[index][i] += 1;
            index = (index + 1) % bags;
        }
    } 

    // Output result by bag
    for (int i = 0; i < bags; i++) {
        cout << "Bag " << (i+1) << ": ";

        for (int j = 0; j < amount_types; j++) {
            cout << result[i][j] << " ";
        }

        cout << endl;
    }

    return 0;
}
