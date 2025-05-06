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

// 3. Solution of BwInf 43, Round 1, Task 3
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
    // and create vector containing all points a_i and b_i 
    vector<int> starting_points;
    vector<tuple<int, bool, int>> list;
    for (int i = 0; i < n; i++) {
        starting_points.push_back(values[i].first);
        list.push_back({values[i].first, false, i});
        list.push_back({values[i].second, true, i});
    }

    sort(starting_points.begin(), starting_points.end());
    sort(list.begin(), list.end());

    // Keep track of the currently best triple
    tuple<int, int, int> best_lengths;
    int best_amount = 0;

    // Find the best possible triple by fixing the middle length
    for (int i = 0; i < n; i++) {
        int mid = starting_points[i];

        // Get current amount for mid
        int best_mid = 0;

        for (int j = 0; j < n; j++) {
            if (values[j].first <= mid && mid <= values[j].second) {
                best_mid++;
            }
        }

        // Get best possible amount to the left of mid
        int best_amount_left = 0;
        int best_length_left = -1;
        int current_amount_left = 0;

        for (int j = 0; j < 2*n; j++) {
            int length, index;
            bool type;
            tie(length, type, index) = list[j];

            // Dont consider member if its range is not completely to the left of mid
            if (values[index].second >= mid) {
                continue;
            }

            if (type == false) {
                current_amount_left++;

                if (current_amount_left > best_amount_left) {
                    best_amount_left = current_amount_left;
                    best_length_left = length;
                }
            } else {
                current_amount_left--;
            }
        }

        // Get best possible amount to the right of mid#
        int best_amount_right = 0;
        int best_length_right = -1;
        int current_amount_right = 0;

        for (int j = 0; j < 2*n; j++) {
            int length, index;
            bool type;
            tie(length, type, index) = list[j];

            // Dont consider member if its range is not completely to the right of mid
            if (values[index].first <= mid) {
                continue;
            }

            if (type == false) {
                current_amount_right++;

                if (current_amount_right > best_amount_right) {
                    best_amount_right = current_amount_right;
                    best_length_right = length;
                }
            } else {
                current_amount_right--;
            }
        }

        // Update the overall best amount if needed
        int amount = best_amount_left + best_mid + best_amount_right;

        if (amount > best_amount) {
            best_amount = amount;
            best_lengths = {best_length_left, mid, best_length_right};
        }
    }

    print_results(values, best_amount, best_lengths);
    return 0;
}