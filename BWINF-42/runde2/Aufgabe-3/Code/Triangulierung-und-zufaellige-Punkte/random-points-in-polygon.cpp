#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> cord;
typedef tuple<cord, cord, cord> ctrip;

// Operations on vectors: Subtraction
cord multiply(cord &A, ll value) {
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

// Operations on vectors: Cross Product 
ll cross_product(cord &A, cord &B) {
    return (A.first * B.second) - (A.second * B.first);
}

// Check if X lies left (1) or right (-1) when going from P to Q; 
// returning 0 if P, Q, X are co linear
int get_left_right(cord &P, cord &Q, cord &X) {
    cord v = subtract(Q, P);
    cord u = subtract(X, P);
    ll s = cross_product(u, v);

    if (s > 0) {
        return 1;
    } else if (s < 0) {
        return -1;
    } else {
        return 0;
    }
}

// Check if B there is a convex angle at B when going from A to B to C.
bool is_convex(cord &A, cord &B, cord &C) {
    int s = get_left_right(A, C, B);

    if (s == 1) {
        return true;  
    } else {
        return false;
    }
}

// Check weather a given point X is on line AB
bool point_on_line(cord &A, cord &B, cord &X) {
    ll a = A.first, b = A.second;
    ll c = B.first, d = B.second;
    ll x = X.first, y = X.second;

    // 1. Case: c == a
    if (c == a) {
        ll mi = min(b, d);
        ll ma = max(b, d);
        return x == a && mi <= y && ma >= y;
    }

    // 2. Case: b == d
    if (b == d) {
        ll mi = min(a, c);
        ll ma = max(a, c);
        return y == b && mi <= x && ma >= x;
    }

    // 3. Case c != a and b != d
    long double t = ((long double) (x-a)) / (((long double) (c-a)));
    return (d-b) * (x-a) == (y-b) * (c-a) && t >= 0 && t <= 1;

}

// Return wheater X lies in or on the triangle ABC
bool point_in_or_on_triangle(cord &A, cord &B, cord &C, cord &X) {
    int s0 = get_left_right(A, B, X);
    int s1 = get_left_right(B, C, X);
    int s2 = get_left_right(C, A, X);

    if (s0 == s1 && s1 == s2) {
        return true;
    }

    if (point_on_line(A, B, C) || point_on_line(B, C, X) || point_on_line(C, A, X)) {
        return true;
    }

    return false;
}

// Given a polygon with 3 or more vertices, return a ear
ctrip find_ear(vector<cord> &vertices) {
    int n = vertices.size();
    ctrip ear;

    // Find ear (v1, v2, v3) <-> v2 convex && no other point of vertices lies in or on the triangle
    for (int i = 0; i < n; i++) {
        int a = i, b = (i + 1) % n, c = (i+2) % n;
        bool valid = true;

        if (!is_convex(vertices[a], vertices[b], vertices[c])) {
            continue;
        }

        for (int j = 0; j < n; j++) {
            if (j == a || j == b || j == c) {
                continue;
            }

            // Check if X lies in the current triangle or on one of its sides
            cord X = vertices[j];
            if (point_in_or_on_triangle(vertices[a], vertices[b], vertices[c], X)) {
                valid = false;
                break;
            }
        }

        if (valid == true) {
            ear = {vertices[a], vertices[b], vertices[c]};
            break;
        }
    }   

    return ear;
}

// Triangulate a given polygon by naive ear-clipping
vector<ctrip> triangulation(vector<cord> vertices) {
    int n = vertices.size();
    vector<ctrip> triangles;

    for (int i = 0; i < n-2; i++) {
        // Find ear
        ctrip ear = find_ear(vertices);
        triangles.push_back(ear);

        // Remove the ear tip (middle vertex)
        cord tip = get<1>(ear);
        auto tip_it = find(vertices.begin(), vertices.end(), tip);
        vertices.erase(tip_it);
    }

    return triangles;
}

long double triangle_area(cord &A, cord &B, cord &C) {
    // Side Lenghts
    long double a = dist(A, B);
    long double b = dist(B, C);
    long double c = dist(A, C);

    // Heron
    long double s = (a+b+c)/2;
    return sqrt(s*(s-a)*(s-b)*(s-c));
}

// Given a triangle and some amount of points, return a vector with 
// that amount of points inside of the triangle
vector<cord> create_points(ctrip &triangle, int amount) {
    vector<cord> points;    

    cord A = get<0>(triangle);
    cord B = get<1>(triangle);
    cord C = get<2>(triangle);

    ll a1 = A.first, a2 = A.second;
    ll b1 = B.first, b2 = B.second;
    ll c1 = C.first, c2 = C.second;

    for (int i = 0; i < amount; i++) {
        // Create pseudo random point contained in the triangle
        ll MY_MIN = 0.005 * RAND_MAX;
        ll MY_MAX = 0.995 * RAND_MAX;

        ll rand1 = (MY_MIN + rand()) % MY_MAX;
        ll rand2 = (MY_MIN + rand()) % MY_MAX;

        long double alpha = (long double) rand1 / (long double) RAND_MAX;
        long double beta = (long double) rand2 / (long double) RAND_MAX;

        if (alpha + beta > 1) {
            alpha = 1 - alpha;
            beta = 1 - beta;
        }

        ll x = a1 + alpha*(b1 - a1) + beta*(c1 - a1);
        ll y = a2 + alpha*(b2 - a2) + beta*(c2 - a2);

        points.push_back({x, y});
    }

    return points;
}

int main() {
    // Seed random number generator
    srand((unsigned)time(NULL));

    int N;
    string file0, file1;

    cout << "Please enter the file containing the polygon: " << endl;
    cin >> file0;
    cout << "Please enter the file the points will be writen to: " << endl;
    cin >> file1;
    cout << "Please enter the amount of points to create: " << endl;
    cin >> N;

    // Cin and Cout into file
    freopen(file0.c_str(), "r", stdin);
    freopen(file1.c_str(), "w", stdout);

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

    // Make sure polygon points are given counter clockwise

    vector<ctrip> ears = triangulation(vertices);

    // For each triangle calculate the area and the total area
    vector<long double> areas(ears.size());
    long double total = 0;

    for (int i = 0; i < ears.size(); i++) {
        ctrip triangle = ears[i];
        cord A = get<0>(triangle);
        cord B = get<1>(triangle);
        cord C = get<2>(triangle);
        long double area = triangle_area(A, B, C);
        areas[i] = area;
        total += area;
    }

    // For each triangle create some amount of random points inside the triangle
    vector<cord> points;

    for (int i = 0; i < ears.size(); i++) {
        int amount = ceil((long double) N * (long double) (areas[i] / total));

        ctrip triangle = ears[i];
        vector<cord> trig_points = create_points(triangle, amount);

        for (cord p : trig_points) {
            points.push_back(p);
        }
    }

    cout << points.size() << endl;

    for (cord p : points) {
        cout << ((long double) p.first / 1000000.0) << " " << ((long double) p.second / 1000000.0) << endl;
    }

    return 0;
}
