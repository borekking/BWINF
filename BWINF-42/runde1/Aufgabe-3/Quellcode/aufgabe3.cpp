#include <bits/stdc++.h>

using namespace std;

// Types
typedef long long ll;
typedef string str;

typedef vector<ll> vll;
template <class T> using V = vector<T>;

typedef pair<ll, ll> pll;

typedef priority_queue<pll, V<pll>, greater<pll>> pqll;

// Firsts
#define FAST cin.tie(0); cout.tie(0); ios::sync_with_stdio(0)
#define READ_FILE(i, o) freopen(i, "r", stdin); freopen(o, "w", stdout)
#define PRECISION(n) cout << fixed << setprecision(n)

// Pair
#define MP make_pair

// For
#define rep(i,n) for (ll i = 0; i < (n); i++)
#define each(e,v) for(auto &e : v)

// Input
#define gets(val) cin >> val
template <class T> void get(T &t) {
    cin >> t;
}
template <class T, class... U> void get(T &t, U &... u) {
    get(t); get(u...);
}
template <class T> void get(V<T> &v) {
    each(e, v) get(e);
}

// Output
#define puts(val) cout << val << "\n"
template <class T> void put(T &t) {
    cout << t;
}
template <class T, class... U> void put(T &t, U &... u) {
    put(t); put(u...);
}

#define nxt() cout << "\n"
#define sp() cout << " "

#define PB push_back
#define P push
#define I insert

const double eps = 1e-9;
const ll INF = 1e16;

void solve() {
    ll n,m; get(n,m);

    V<str> map1(0);
    
    V<str> map2(0);

    string a;
    rep(i,n){
        get(a);
        map1.PB(a);
    }
    rep(i,n){
        get(a);
        map2.PB(a);
    }
    
    // find coordinates of start and ending point

    ll startingPoint, endingPoint;

    rep(i,n){
        rep(j,m){
            if(map1[i][j] == 'A'){
                startingPoint = (m-2)*(i-1) + (j-1);
            }else if(map1[i][j] == 'B'){
                endingPoint = (m-2)*(i-1) + (j-1);
            }
        }
    }

    rep(i,n){
        rep(j,m){
            if(map2[i][j] == 'A'){
                startingPoint = (m-2)*(i-1) + (j-1) + (n-2)*(m-2);
            }else if(map2[i][j] == 'B'){
                endingPoint = (m-2)*(i-1) + (j-1) + (n-2)*(m-2);
            }
        }
    }

    V<V<pll>> adj(0);

    ll cur = 0;

    n -= 2;
    m -= 2;

    V<pll> tempAdj = {};

    // build all connections

    // map1 -> map1/map2
    for(int i = 1; i<n+1; i++){
        for(int j = 1; j<m+1; j++){
            tempAdj = {};
            // oben
            if(map1[i-1][j] != '#' && cur > m){
                tempAdj.PB(MP(cur-m, 1));
            }
            // unten
            if(map1[i+1][j] != '#' && cur + m < n*m){
                tempAdj.PB(MP(cur+m, 1));
            }
            // links
            if(map1[i][j-1] != '#' && cur%m != 1){
                tempAdj.PB(MP(cur-1, 1));
            }
            // rechts
            if(map1[i][j+1] != '#' && cur%m != 0){
                tempAdj.PB(MP(cur+1, 1));
            }
            // stockwerk
            if(map2[i][j] != '#'){
                tempAdj.PB(MP(cur+n*m,3));
            }
            adj.PB(tempAdj);
            cur += 1;
        }
    }


    // map2 -> map2/map1
    for(int i = 1; i<n+1; i++){
        for(int j = 1; j<m+1; j++){
            tempAdj = {};
            // oben
            if(map2[i-1][j] != '#' && cur > m){
                tempAdj.PB(MP(cur-m, 1));
            }
            // unten
            if(map2[i+1][j] != '#' && cur + m < 2*n*m){
                tempAdj.PB(MP(cur+m, 1));
            }
            // links
            if(map2[i][j-1] != '#' && cur%m != 1){
                tempAdj.PB(MP(cur-1, 1));
            }
            // rechts
            if(map2[i][j+1] != '#' && cur%m != 0){
                tempAdj.PB(MP(cur+1, 1));
            }
            // stockwerk
            if(map1[i][j] != '#'){
                tempAdj.PB(MP(cur-n*m,3));
            }
            adj.PB(tempAdj);
            cur += 1;
        }
    }
    
    // now just run plain dijikstra on it


    pqll pq;

    pq.P(MP(0,startingPoint));

    vll dist(n*m*2, INF);
    vll pre(n*m*2, -1);

    ll node, distance;
    while(!pq.empty()){
        tie(distance, node) = pq.top();
        pq.pop();

        each(e, adj[node]){
            if(distance+e.second < dist[e.first]){
                dist[e.first] = distance+e.second;
                pre[e.first] = node;
                pq.P(MP(distance+e.second, e.first));
            }
        }
    }

    // puts(dist[endingPoint]);

    // backtrack the nodes

    ll backtracking = endingPoint;
    ll temp = endingPoint;
    while(backtracking != startingPoint){
        temp = backtracking;
        backtracking = pre[backtracking];
        
        // find out where the new index is
        ll row = backtracking % m;
        ll column = (backtracking - row)/m;
        // find out which direction we are going (invert it)
        char sign = '0';
        if(temp + 1 == backtracking){
            sign = '<';
        }else if(temp - 1 == backtracking){
            sign = '>';
        }else if(temp + m == backtracking){
            sign = '^';
        }else if(temp - m == backtracking){
            sign = 'v';
        }else{
            sign = '!';
        }
        if(backtracking < n*m){
            // map 1
            map1[column+1][row+1] = sign;
        }else{
            // map 2
            map2[column-n+1][row+1] = sign;
        }
    }  

    // output the result

    puts("OUTPUT:");

    rep(i,n+2){
        put(map1[i]);
        nxt();
    }
    nxt();
    rep(i,n+2){
        put(map2[i]);
        nxt();
    }
}

int main() {
    READ_FILE("aufgabe3Input.txt", "aufgabe3Output.txt"); // comment this line if input/output should be done using console

    solve();

    return 0;
}