#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> cord;
typedef tuple<cord, cord, cord> ctrip;

const ll R = 85*1e6;
const ll A = 10*1e6;
const ll B = 20*1e6;

// Operations on vectors: Subtraction
cord subtract(cord &A, cord &B) {
    return {A.first - B.first, A.second - B.second};
}

// Operations on vectors: Length
long double length(cord &A) {
    return sqrt((A.first)*(A.first) + (A.second)*(A.second));
}

// Operations on vectors: Distance
long double dist(cord &A, cord &B) {
    cord v = subtract(B, A);
    return length(v);
}

vector<cord> generate_greedy(vector<cord> &P, vector<cord> &T, cord Z) {
    cord Y = T.back();
    vector<cord> result = {Y};

    // Create Set M
    vector<cord> M;

    for (cord C : T) {
        if (
            ((dist(Z, Y) <= R || dist(Z, C) <= R) && dist(C, Y) >= A) ||
            (dist(Z, Y) > R && dist(Z, C) > R && dist(C, Y) >= B)
        ) {
            M.push_back(C);
        }
    }

    while (M.size() > 0) {
        cord X = M.back();
        result.push_back(X);

        // Update M
        vector<cord> M_next;

        for (cord C : M) {
            if (
                ((dist(Z, X) <= R || dist(Z, C) <= R) && dist(C, X) >= A) ||
                (dist(Z, X) > R && dist(Z, C) > R && dist(C, X) >= B)
            ) {
                M_next.push_back(C);
            }
        }

        M = M_next;
    }

    return result;
}

pair<cord, vector<cord>> solve(vector<cord> &P, vector<cord> &G, vector<cord> &T) {
    vector<cord> result;
    cord best;

    for (cord Z : G) {
        vector<cord> current = generate_greedy(P, T, Z);

        if (current.size() > result.size()) {
            result = current;
            best = Z;
        }
    }

    return {best, result};
}

vector<cord> read(string &file) {
    // Cin from file
    freopen(file.c_str(), "r", stdin);

    int n = 0;
    long double x, y;
    cin >> n;

    vector<cord> vertices(n);

    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        ll value_x = 1e6 * x;
        ll value_y = 1e6 * y;
        vertices[i] = make_pair(value_x, value_y);
    }

    return vertices;
}

int main() {
    string file0;
    cout << "Please enter file containing the polygon: " << endl;
    cin >> file0;
    
    string file1;
    cout << "Please enter file containing the points of set G: " << endl;
    cin >> file1;
    
    string file2;
    cout << "Please enter file containing the points of set T: " << endl;
    cin >> file1;

    vector<cord> P = read(file0);
    vector<cord> G = read(file1);
    vector<cord> T = read(file2);

    auto start = chrono::high_resolution_clock::now();

    pair<cord, vector<cord>> result = solve(P, G, T);

    auto end  = chrono::high_resolution_clock::now();
    cout << endl << "Zeit: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;

    // Output
    cord Z = result.first;
    vector<cord> points = result.second;

    cout << "Gesundheitszentrum: " << ((long double) Z.first / 1000000.0) << " " << ((long double) Z.second / 1000000.0) << endl;
    cout << "Points: " << endl;
    for (cord c : points) {
        cout << ((long double) c.first / 1000000.0) << " " << ((long double) c.second / 1000000.0) << endl;
    }

    return 0;
}
