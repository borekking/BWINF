#include <bits/stdc++.h>

using namespace std;

// Always assuming the amount of exams to be <= 26 because they are given as chars in A-Z
const int MAXN = 26;

// Global variables used in Targan to find all SCCs
vector<set<int>> adjacency(MAXN);
vector<int> num(MAXN, -1);
vector<int> low(MAXN, -1);
stack<int> sta;
vector<bool> in(MAXN, 0);
vector<vector<int>> components;

int counter = 0;

// Find all strongly connected components using Tarjan
void tarjan(int root) {
    num[root] = counter++;
    low[root] = num[root];
    sta.push(root);
    in[root] = 1;

    for (int next : adjacency[root]) {
        if (num[next] == -1) {
            tarjan(next);
        }
        if (in[next]) {
            low[root] = min(low[root], low[next]);
        }
    }

    if (num[root] == low[root]) {
        vector<int> c;
        int node;
        do {
            node = sta.top();
            sta.pop();
            c.push_back(node);
            in[node] = 0;
        } while (node != root);

        components.push_back(c);
    }
}

// Topolocial Sort using Kahn's Algorithm
vector<int> toposort(vector<set<int>> &graph) {
    int n = graph.size();
    vector<int> in(n, 0);
    vector<int> nodes; // Nodes sorted in topological order
    queue<int> zero_nodes; // All nodes currently having indegree 0
    
    // Initialize every node's indegree
    for (int i = 0; i < n; i++) {
        for (int next : graph[i]) {
            in[next]++;
        }
    }

    // Find all nodes which has indeegree 0
    for (int i = 0; i < n; i++) {
        if (in[i] == 0) {
            zero_nodes.push(i);
        }
    }
    
    // Iteratively create the topolical order by removing the nodes with indegree 0 from
    // the graph and adding them into nodes. Then update adjacent node's indegree.
    while (zero_nodes.size()) {
        int node = zero_nodes.front();
        zero_nodes.pop();
        nodes.push_back(node);

        for (int next : graph[node]) {
            in[next]--;
            if (in[next] == 0) {
                zero_nodes.push(next);
            }
        }
    }

    return nodes;
}

int main() {
    // Get input and outpout file from standart input
    cout << "Enter file name: " << endl;
    string input_file;
    cin >> input_file;
    cout << "Enter output name: " << endl;
    string output_file;
    cin >> output_file;

    // Get the file corresponding to input_file 
    ifstream file(input_file.c_str());

    // Send outputs to output_file
    freopen(output_file.c_str(), "w", stdout);

    // Get n, m and k from the file's first line
    string first_line;
    getline(file, first_line);
    stringstream first_line_stream(first_line);
    int n, m, k;
    first_line_stream >> n >> m >> k;

    // Read in the next n lines from the file containing the exams
    for (int i = 0; i < n; i++) {
        // Get the current line
        vector<int> exams;
        string line; 
        getline(file, line);
        
        // Add every exam to the vector using offset -'A' (a symbol is an exam if its not a ' ' or '<')
        int length = line.size();
        for (int j = 0; j < length; j++) {
            if (line.at(j) == ' ' || line.at(j) == '<') {
                continue;
            }    

            int value = (int) (line.at(j) - 'A');
            exams.push_back(value);
        }

        // Insert the list of exams into the graph represented by the adjacency list
        for (int j = 0; j < exams.size()-1; j++) {
            adjacency[exams[j]].insert(exams[j+1]);
        }
    }

    // Read the last line of input containing the k exams we are interesed in
    string last_line;
    getline(file, last_line);
    stringstream last_line_stream(last_line);
    set<int> interesting_exams;
    
    for (int i = 0; i < k; i++) {
        string exam;
        last_line_stream >> exam;
        int value = (int) (exam.at(0) - 'A');
        interesting_exams.insert(value);
    }

    // Get all SCCs of the graph represented by adjacency
    for (int i = 0; i < m; i++) {
        if (num[i] == -1) {
            tarjan(i);
        }
    }

    // Map nodes (exams) to their components by index the component's index 
    vector<int> components_map(m, -1);

    for (int i = 0; i < components.size(); i++) {
        for (int v : components[i]) {
            components_map[v] = i;
        }
    }

    // Construct new graph using SCCs 
    // -> The components form the nodes (using their index) 
    // -> component i has a directed edge to component j if any node in component i has an directed edge to any node in component j
    vector<set<int>> graph2(components.size());

    for (int i = 0; i < components.size(); i++) {
        // Go trough all nodes of current componets
        for (int node : components[i]) {
            // Go through all the nodes adjacent to node
            for (int next : adjacency[node]) {
                // Add an edge from component i to next's component (if its not the same component)
                int component_next = components_map[next];
                if (component_next != i) {
                    graph2[i].insert(component_next);
                }
            }
        }
    }

    // Create a topological sort of the new graph
    vector<int> order = toposort(graph2);

    // Output the result by printing each component's nodes in the given order
    for (int i = 0; i < order.size(); i++) {
        int current = order[i];
        sort(components[current].begin(), components[current].end());
        cout << "-> ";
        for (int x : components[current]) {
            cout << (char) (x+'A') << " ";
        }
        cout << endl;
    }

    // Filtered version to only show wanted exams:
    cout << "Resulting order for the given k exams:" << endl;
    for (int i = 0; i < order.size(); i++) {
        int current = order[i];
        for (int x : components[current]) {
            if (interesting_exams.find(x) == interesting_exams.end()) {
                continue;
            }
            cout << (char) (x+'A') << " ";
        }
    }
    cout << endl;
    return 0;
}