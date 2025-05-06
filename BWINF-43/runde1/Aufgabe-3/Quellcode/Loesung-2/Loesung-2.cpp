#include <bits/stdc++.h>

using namespace std;

// Function returing the best possible amount of members and the corresponding length
// given a vector of indeces of members to involved
pair<int, int> bar(vector<pair<int, int>> &values, vector<int> &members) {
    // Get all needed values a_i and b_i corresponding to the given members
    vector<pair<int, bool>> lenghts;

    for (int i : members) {
        lenghts.push_back({values[i].first, false});
        lenghts.push_back({values[i].second, true});
    }

    sort(lenghts.begin(), lenghts.end());

    int best_amount = 0; 
    int best_length = -1;
    int current_amount = 0;

    for (int i = 0; i < lenghts.size(); i++) {
        int length;
        bool type;
        tie(length, type) = lenghts[i];

        // Increase the counter if length is a staring point and decrease otherwise
        if (type == false) {
            current_amount++;

            // Update the best length if needed
            if (current_amount > best_amount) {
                best_amount = current_amount;
                best_length = length;
            }
        } else {
            current_amount--;
        }
    }

    return {best_amount, best_length};
}

// Function finding the best triple (left, mid, right) given mid
pair<tuple<int, int, int>, int> foo(vector<pair<int, int>> &values, int mid) {
    int n = values.size();
    vector<int> left;
    vector<int> right;
    int counter = 0;
    
    // Calculate the members completely left and completely right 
    // to mid and count amount of members covered by mid
    for (int i = 0; i < n; i++) {
        int start = values[i].first;
        int end = values[i].second;
        if (end < mid) {
            left.push_back(i);
        } else if (start <= mid && mid <= end) {
            counter++;
        } else if (mid < start) {
            right.push_back(i);
        }
    }

    // Get the best amount to the left and to the right
    // and return correspondingly
    int amount_left, length_left;
    tie(amount_left, length_left) = bar(values, left);

    int amount_right, length_right;
    tie(amount_right, length_right) = bar(values, right);

    int result = amount_left + counter + amount_right;
    return {{length_left, mid, length_right}, result};
}

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

// 2. Solution of BwInf 43, Round 1, Task 3
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
    }

    sort(starting_points.begin(), starting_points.end());

    // Keep track of the currently best triple
    tuple<int, int, int> best_lengths;
    int best_amount = 0;

    // Find the best possible triple by fixing the middle length
    for (int i = 0; i < n; i++) {
        int mid = starting_points[i];
        tuple<int, int, int> current_lengths;
        int current_amount;
        tie(current_lengths, current_amount) = foo(values, mid);

        if (current_amount > best_amount) {
            best_amount = current_amount;
            best_lengths = current_lengths;
        }
    }

    print_results(values, best_amount, best_lengths);
    return 0;
}