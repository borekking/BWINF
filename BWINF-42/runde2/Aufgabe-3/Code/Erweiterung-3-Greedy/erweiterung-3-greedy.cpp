#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> cord;
typedef tuple<cord, cord, cord> ctrip;

const ll R = 85*1e6;
const ll R0 = 2*1e6;
const ll R1 = 10*1e6;
const ll A = 10*1e6;
const ll B = 20*1e6;

// Operations on vectors: Subtraction
cord multiply(cord &A, long double value) {
    return {A.first * value, A.second * value};
}

// Operations on vectors: Subtraction
cord subtract(cord &A, cord &B) {
    return {A.first - B.first, A.second - B.second};
}

// Operations on vectors: Addition
cord add(cord &A, cord &B) {
    return {A.first + B.first, A.second + B.second};
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

// Compute the maximal integer radius r for some point X, such that the circle (X, r)
// is inside of some given Polygon P
ll max_radius_polygon(vector<cord> &P, cord &X) {
    ll radius = -1;

    long double x = X.first;
    long double y = X.second;

    // For each edge of P: Calculate the smallest distance from X to some point on the edge
    for (int i = 0; i < P.size(); i++) {
        cord Y = P[i];
        cord Z = P[(i+1) % P.size()];   

        long double a = Y.first;
        long double b = Y.second;
        long double c = Z.first;
        long double d = Z.second;

        long double t = (x*(c-a) + y*(d-b) - a*(c-a) - b*(d-b)) / ((c-a)*(c-a) + (d-b)*(d-b));
        cord Q;

        if (t <= 0) {
            Q = Y;
        } else if (t >= 1) {    
            Q = Z;
        } else {
            cord v = subtract(Z, Y);
            cord u = multiply(v, t);
            Q = add(Y, u);
        }

        ll cur_radius = dist(X, Q);

        if (radius == -1 || cur_radius < radius) {
            radius = cur_radius;
        }
    }

    return radius;
}   

// Compute the maximal integer radius r for some point X, such that the circle (X, r)
// is inside of some given Circle (Z, R)
ll max_radius_circle(cord &X, cord &Z, ll R) {
    long double x = X.first;
    long double y = X.second;
    long double a = Z.first;
    long double b = Z.second;

    long double t = R / sqrt((x-a)*(x-a) + (y-b)*(y-b));

    cord v = subtract(X, Z);
    cord u = multiply(v, t);
    cord Q = add(Z, u);

    return dist(X, Q);
}   

vector<pair<cord, ll>> generate_greedy( vector<ll> &radius0, vector<ll> &radius1, vector<cord> &P, vector<cord> &T, cord Z) {
    // 1. Part - Generate points in circle (Z, R)
    vector<pair<cord, ll>> result;
    vector<pair<int, ll>> M0; // Contains circles, the coord is saved as index in T

    // Create M0
    for (int i = 0; i < T.size(); i++) {
        if (dist(T[i], Z) <= R) {
            ll r = min(radius0[i], min(radius1[i], R1));

            if (r >= R0) {
                M0.push_back({i, r}); 
            }
        }
    }

    while (M0.size() > 0) {
        // Get circle with biggest possible radius in M0
        pair<cord, ll> K;
        ll radius = -1;

        for (pair<int, ll> circle : M0) {
            ll current = circle.second;

            if (radius == -1 || current > radius) {
                K = {T[circle.first], current};
                radius = current;
            }
        }

        result.push_back(K);

        // Update M0    
        vector<pair<int, ll>> M0_next;

        for (pair<int, ll> circle : M0) {
            // Check distance to circle K
            int i = circle.first;
            ll r = min(circle.second, (ll) dist(T[i], K.first) - A - K.second);
            r = min(r, min(radius0[i], min(radius1[i], R1)));

            if (r >= R0) {
                M0_next.push_back({circle.first, r});
            }
        }

        M0 = M0_next;
    }

    // 2. Part
    vector<pair<int, ll>> M1;

    // Create M1
    for (int i = 0; i < T.size(); i++) {
        if (dist(T[i], Z) > R) {
            ll r = -1;

            for (pair<cord, ll> p : result) {
                ll current;

                if (dist(p.first, Z) <= R) {
                    current = dist(p.first, T[i]) - A - p.second;
                } else {
                    current = dist(p.first, T[i]) - B - p.second;
                }

                if (r == -1 || current < r) {
                    r = current;
                }
            }

            r = min(r, min(radius0[i], R1));

            if (r >= R0) {
                M1.push_back({i, r});
            }
        }
    }

    while (M1.size() > 0) {
        // Get circle with biggest possible radius in M1
        pair<cord, ll> K;
        ll radius = -1;

        for (pair<int, ll> circle : M1) {
            ll current = circle.second;

            if (radius == -1 || current > radius) {
                K = {T[circle.first], current};
                radius = current;
            }
        }

        result.push_back(K);

        // Update M1
        vector<pair<int, ll>> M1_next;

        for (pair<int, ll> circle : M1) {
            // Check distance to circle K
            int i = circle.first;
            ll r = min(circle.second, (ll) dist(T[i], K.first) - B - K.second);
            r = min(r, min(radius0[i], R1));

            if (r >= R0) {
                M1_next.push_back({circle.first, r});
            }
        }

        M1 = M1_next;
    }

    return result;
}

pair<cord, vector<pair<cord, ll>>> solve(vector<cord> &P, vector<cord> &G, vector<cord> &T) {
    // Precompute values of R_Y: Max. radius of points Y to be completly contained in polygon P
    vector<ll> radius0(T.size(), 0);

    for (int i = 0; i < T.size(); i++) {
        cord X = T[i];
        radius0[i] = max_radius_polygon(P, X);
    }

    vector<pair<cord, ll>> result;
    cord best;
    ll best_value = 0;

    for (cord Z : G) {
        // Precompute values of R'_Y: Max. radius for point Y to be completly contained inside of Circle (Z, R)
        vector<ll> radius1(T.size(), 0);

        for (int i = 0; i < T.size(); i++) {
            cord X = T[i];
            if (X.first == Z.first && X.second == Z.second) {
                radius1[i] = R;
            } else {    
                radius1[i] = max_radius_circle(X, Z, R);
            }
        }

        vector<pair<cord, ll>> current = generate_greedy(radius0, radius1, P, T, Z);
        ll current_value = 0;
        for (pair<cord, ll> p : current) {
            current_value += p.second*p.second;
        }

        if (current_value > best_value) {
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

    pair<cord, vector<pair<cord, ll>>> result = solve(P, G, T);

    auto end  = chrono::high_resolution_clock::now();
    cout << endl << "Zeit: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms" << endl;

    // Output
    cord Z = result.first;
    vector<pair<cord, ll>> points = result.second;
    long double area = 0;

    cout << "Gesundheitszentrum: " << ((long double) Z.first / 1000000.0) << " " << ((long double) Z.second / 1000000.0) << endl;
    cout << "Kreise: " << endl;

    for (pair<cord, ll> circle : points) {
        cord c = circle.first;
        ll radius = circle.second;
        
        cout << ((long double) c.first / 1000000.0) << " " << ((long double) c.second / 1000000.0) << " " << ((long double) radius / 1000000.0) << endl;
        area += M_PI * ((long double) radius / 1000000.0) * ((long double) radius / 1000000.0);
    }

    cout << "Flaeche: " << area << endl;
    return 0;
}
