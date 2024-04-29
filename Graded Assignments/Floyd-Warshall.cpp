#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

// Function to read the weighted adjacency matrix from file
vector<vector<int>> readGraph(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> graph;
    int n;
    file >> n; // Assuming the first number in the file indicates the number of vertices
    graph.resize(n, vector<int>(n, INT_MAX));

    int i, j, weight;
    while (file >> i >> j >> weight) {
        graph[i][j] = weight;
        // Assuming the graph is undirected, so setting both directions
        graph[j][i] = weight;
    }

    file.close();
    return graph;
}

// Function to compute shortest paths using Floyd-Warshall algorithm
void floydWarshall(const vector<vector<int>>& graph, vector<vector<int>>& dist, vector<vector<int>>& next) {
    int n = graph.size();

    // Initialize dist and next matrices
    dist = graph;
    next.resize(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i != j && dist[i][j] != INT_MAX)
                next[i][j] = j;

    // Floyd-Warshall algorithm
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

// Function to reconstruct the path between vertices i and j
vector<int> reconstructPath(int i, int j, const vector<vector<int>>& next) {
    if (next[i][j] == -1) // No path exists
        return {};

    vector<int> path;
    path.push_back(i);
    while (i != j) {
        i = next[i][j];
        path.push_back(i);
    }
    return path;
}

// Function to output shortest path with intermediate vertices having an index at most k
vector<int> shortestPathWithinK(int i, int j, int k, const vector<vector<int>>& next) {
    vector<int> path = reconstructPath(i, j, next);
    vector<int> pathWithinK;
    for (int v : path) {
        if (v <= k)
            pathWithinK.push_back(v);
    }
    return pathWithinK;
}

int main() {
    string filename = "graph.txt";
    vector<vector<int>> graph = readGraph(filename);

    vector<vector<int>> dist, next;
    floydWarshall(graph, dist, next);

    // Example queries
    vector<pair<int, pair<int, int>>> queries = {{0, {1, 2}}, {1, {2, 1}}};

    for (auto query : queries) {
        int i = query.first;
        int j = query.second.first;
        int k = query.second.second;

        if (dist[i][j] == INT_MAX)
            cout << "No path exists between " << i << " and " << j << endl;
        else {
            vector<int> pathWithinK = shortestPathWithinK(i, j, k, next);
            cout << "Shortest path between " << i << " and " << j << " within k=" << k << ": ";
            for (int v : pathWithinK)
                cout << v << " ";
            cout << endl;
        }
    }

    return 0;
}
