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

    freopen(filename.c_str(), "r", stdin);
    freopen(output.c_str(), "w", stdout);

    // Read ppm file 
    string magic; 
    cin >> magic;

    int rows, columns, max; 
    cin >> columns >> rows >> max;

    vector<vector<tuple<int, int, int>>> image(rows, vector<tuple<int, int, int>>(columns));

    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int r, g, b;
            cin >> r >> g >> b;
            image[i][j] = make_tuple(r, g, b);
        }
    }

    // Track the message
    vector<int> message;

    // Current coordinates (x, y) in the image
    int x = 0, y = 0;
    int r, g, b;

    do {
        // Get current pixel
        tuple<int, int, int> pixel = image[y][x];
        r = get<0>(pixel);
        g = get<1>(pixel);
        b = get<2>(pixel);

        // Get current character for the message
        int character_int = r;
        message.push_back(character_int);

        // Get new coordinates
        x = (x + g) % columns;
        y = (y + b) % rows;
    } while (g != 0 || b != 0);

    // Print the message
    for (int i = 0; i < (int) message.size(); i++) {
        int character_int = message[i];
        char character = (char) character_int;
        cout << character;
    }

    cout << endl;
    return 0;
}
