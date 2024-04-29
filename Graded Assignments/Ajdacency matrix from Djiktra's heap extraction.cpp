/*
Samrat Acharya Moon, 2203320

All the time complexities are commented with the codeand Final complexity is mentioned below
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#define INTMAX 9999

using namespace std;

char source;

// Structure to represent a vertex
struct Node {
    char vertex; // present vertex
    char predecessor; // Predecessor vertex
    int key; // Shortest path distance
    Node(char d, char p, int k) : vertex(d), predecessor(p), key(k) {}
};

struct listnode {
    char node;
    int key;
    struct listnode* next;
    listnode(char c , int k, struct listnode* n): node(c), key(k), next(n) {}
};
// O(1)
void addnode(struct listnode* list, int i, char c){
    list[i].node = c;
    list[i].next = nullptr;
}
// O(1)
struct listnode* createnode(char c, int k){
    struct listnode* temp =(struct listnode*) malloc(sizeof(struct listnode));
    temp->next = nullptr;
    temp->node = c;
    temp->key = k;
    return temp;
}

// Time Complexity: O(n * m), where n is the number of lines in the file and n is the average length of each line.
vector<vector<Node>> readInputFromFile(const string& filename) {
    vector<vector<Node>> sequence;
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        // Read each line from the file
        while (getline(file, line)) {
            // Parse each line to create Node objects
            vector<Node> heap;
            char vertex, predecessor;
            int key;
            string temp;
            // Remove unnecessary characters and separate values
            for(auto s: line){
                if (s == '(' || s == ')'){
                    continue;
                } else if ( s == ',' ){
                    temp.append(" ");
                    continue;
                }
                string a(1,s);
                temp.append(a);
            }
            stringstream ss(temp);
            // Create Node objects and push them into the vector
            while (ss >> vertex >> predecessor >> key) {
                heap.push_back(Node(vertex, predecessor, key));
            }
            // Add the vector of Node objects to the sequence
            sequence.push_back(heap);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return sequence;
}

int dist(char pred, vector<vector<Node>> seq , int id){
    if(pred == source){
        return 0;
    }
    for (int i = seq.size() - id ; i>= 0 ; i--){
        for(int j = 0; j < seq[i].size() ; j--){
            if(seq[i][j].vertex == pred) return seq[i][j].key;
        }
    }
    return INTMAX;
}
// Time Complexity: O(m * m * m), m : no. of elements of heap 
void updatelist(struct listnode* list , vector<Node> heap, int m, vector<vector<Node>> sequence){
    for(auto e: heap){ // Iterate over each element in the heap
        if (e.key == INTMAX){ // Constant time operation
            continue;
        }
        int alreadyexist = 0; // Constant time operation
        // checking list for adding or ignoring
        for(int i =0 ; i<m; i++){ // Iterate over each element in the list, m is the size of the list
            if(e.predecessor == list[i].node){ // Constant time operation
                struct listnode* temp = list[i].next; // Constant time operation
                struct listnode* prev = &list[i]; // Constant time operation
                while(temp != nullptr){ // Iterate over the linked list, potentially up to the size of the list
                    prev= temp; // Constant time operation
                    temp = temp->next; // Constant time operation
                    if(prev->node == e.vertex){ // Constant time operation
                        alreadyexist ++; // Constant time operation
                        break;
                    }
                }
                if(alreadyexist == 0){ // Constant time operation
                    prev->next = createnode(e.vertex,e.key - dist(e.predecessor,sequence , heap.size())); // createnode, dist, and heap.size() are assumed to be constant time operations
                    break;
                }else {
                    alreadyexist = 0; // Constant time operation
                    break;
                }
            }
        }  
    }
}
// Time Complexity: O(m ), m : no. of elements of heap note*: this is the worsdt time complexity 
void printtraversal(struct listnode l){
    cout<<l.node<<"";
    struct listnode* ln = l.next;
    while(ln != nullptr){
        cout<<"->("<<ln->node<<","<<ln->key<<")";
        ln = ln->next;
    }
}

int main() {
  // Sample data (replace with your actual data reading logic)
    string filename = "input.txt"; // Update with your file name
    vector<vector<Node>> sequence = readInputFromFile(filename);
    int m = sequence[0].size() + 1; // this is the same m for complexity consideration
    source = sequence[0][0].predecessor;
    struct listnode* list = (struct listnode*) malloc(m*sizeof(struct listnode));
    int i = 0;
    if(sequence[0][0].predecessor != '_'){addnode(list,i,sequence[0][0].predecessor);}
    i++;
    
    
    // Creating adjacency list template  O(m*m)
    for(auto h: sequence[0]){
        addnode(list,i,h.vertex);
        i++;
    }
    // adding elements to the list  // O(m^4)
    for(auto h: sequence){
        int n = h.size();
        updatelist(list, h , m, sequence);
    }
    // printing output     O(m*m) 
    for(int i = 0; i<m;i++){
        if(list[i].next != nullptr){
        printtraversal(list[i]);
        cout<<endl;
        }
    }
  return 0;
}

// This final complexity turns out to be O(m^4)


