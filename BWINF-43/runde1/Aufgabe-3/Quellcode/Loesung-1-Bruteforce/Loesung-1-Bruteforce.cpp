#include <bits/stdc++.h>

using namespace std;

// Print the resulting best triple and which members could participate
void print_results(vector<pair<int, int>> &values, int best_amount, tuple<int, int, int> &best_lengths) {
    int l1 = get<0>(best_lengths);
    int l2 = get<1>(best_lengths);
    int l3 = get<2>(best_lengths);
    int n = values.size();

    cout << "Max. amount of members: " << best_amount << " out of " << n << endl;
    cout << "Used lenghts: " << l1 << ", " << l2 << ", " << l3 << endl;

    cout << "Members participating: ";
    for (int c = 0; c < n; c++) {
        int start = values[c].first;
        int end = values[c].second; 

        if ((start <= l1 && l1 <= end) || (start <= l2 && l2 <= end) || (start <= l3 && l3 <= end)) {
            cout << c << ", ";
        }
    }
    cout << endl;

    cout << "Members not participating: ";
    for (int c = 0; c < n; c++) {
        int start = values[c].first;
        int end = values[c].second; 

        if (!((start <= l1 && l1 <= end) || (start <= l2 && l2 <= end) || (start <= l3 && l3 <= end))) {
            cout << c << ", ";
        }
    }
    cout << endl;

    cout << "Length l1 = " << l1 << ": ";
    int counter1 = 0;
    for (int c = 0; c < n; c++) {
        int start = values[c].first;
        int end = values[c].second; 

        if (start <= l1 && l1 <= end) {
            cout << c << ", ";
            counter1++;
        }
    }
    cout << endl << "(total of " << counter1 << " for " << l1 << ")" << endl;

    cout << "Length l2 = " << l2 << ": ";
    int counter2 = 0;
    for (int c = 0; c < n; c++) {
        int start = values[c].first;
        int end = values[c].second; 

        if (start <= l2 && l2 <= end) {
            cout << c << ", ";
            counter2++;
        } 
    }
    cout << endl << "(total of " << counter2 << " for " << l2 << ")" << endl;

    cout << "Length l3 = " << l3 << ": ";
    int counter3 = 0;
    for (int c = 0; c < n; c++) {
        int start = values[c].first;
        int end = values[c].second; 

        if (start <= l3 && l3 <= end) {
            cout << c << ", ";
            counter3++;
        }
    }
    cout << endl << "(total of " << counter3 << " for " << l3 << ")" << endl;
}

// 1. Solution (Bruteforce) of BwInf 43, Round 1, Task 3
int main() {
    // Get input and output files from standart input
    string input_file, output_file;
    cout << "Input file: " << endl;
    cin >> input_file;
    cout << "Output file: " << endl;
    cin >> output_file;
    freopen(input_file.c_str(), "r", stdin);
    freopen(output_file.c_str(), "w", stdout);

    // Read amount of members and their a_i, b_i values from the file
    int n; 
    cin >> n;
    vector<pair<int, int>> values;

    for (int i = 0; i < n; i++) {
        int s, e;
        cin >> s >> e;
        values.push_back({s, e});
    }

    // Create vector only containing the starting point of each member and sort
    vector<int> starting_points;
    for (int i = 0; i < n; i++) {
        starting_points.push_back(values[i].first);
        // starting_points.push_back(e); // Not necessary
    }

    sort(starting_points.begin(), starting_points.end());

    // Bruteforce all triples of starting points
    // Keep track of currently best triple
    int best_amount = 0;
    tuple<int, int, int> best_lengths;

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            for (int k = j+1; k < n; k++) {
                // Current triple (l1, l2, l3)
                int l1 = starting_points[i];
                int l2 = starting_points[j];
                int l3 = starting_points[k];

                // Count the amount of members that would participate when using the current lengths
                int counter = 0;

                for (int c = 0; c < n; c++) {
                    int start = values[c].first;
                    int end = values[c].second;
                    // A member participates if one of the three lengths matches his preferences
                    if ((start <= l1 && l1 <= end) || (start <= l2 && l2 <= end) || (start <= l3 && l3 <= end)) {
                        counter++;
                    }
                }

                // Update the best triple if necessary
                if (counter > best_amount) {
                    best_lengths = {starting_points[i], starting_points[j], starting_points[k]};
                    best_amount = counter;
                }
            }
        }   
    }

    print_results(values, best_amount, best_lengths);
    return 0;
}