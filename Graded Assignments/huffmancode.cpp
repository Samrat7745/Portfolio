/*
Samrat Acharya Moon, 2203320
Reference: GeekforGeeks
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#define MAX_TREE_HT 100 

using namespace std;
 

struct MinHeapNode { 
  
    // One of the input characters 
    char data; 
  
    // Frequency of the character 
    unsigned freq; 
  
    // Left and right child of this node 
    struct MinHeapNode *left, *right; 
}; 
  

struct MinHeap { 
  
    // Current size of min heap 
    unsigned size; 
  
    // capacity of min heap 
    unsigned capacity; 
  
    // Array of minheap node pointers 
    struct MinHeapNode** array; 
}; 
  
 
struct MinHeapNode* newNode(char data, unsigned freq){ 
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc( 
        sizeof(struct MinHeapNode)); 
  
    temp->left = temp->right = NULL; 
    temp->data = data; 
    temp->freq = freq; 
  
    return temp; 
} 
  

struct MinHeap* createMinHeap(unsigned capacity){ 
  
    struct MinHeap* minHeap 
        = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
  
    // current size is 0 
    minHeap->size = 0; 
  
    minHeap->capacity = capacity; 
  
    minHeap->array = (struct MinHeapNode**)malloc( 
        minHeap->capacity * sizeof(struct MinHeapNode*)); 
    return minHeap; 
}

vector<pair<char,int>> createFrequencyTable(string filename){
    vector<pair<char, int>> table;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return table; // Return empty vector if file cannot be opened
    }
    map<char,int> mp;
    string line;
    while(getline(file,line)){
        for(auto c: line){
            if(c !=' '){
                mp[c]++;
            }
        }
    }
    for (const auto& entry : mp) {
            table.push_back(entry);
        }
    file.close();
    return table;
} 
  

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){ 
  
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 

void minHeapify(struct MinHeap* minHeap, int idx){ 
  
    int smallest = idx; 
    int left = 2 * idx + 1; 
    int right = 2 * idx + 2; 
  
    if (left < minHeap->size 
        && minHeap->array[left]->freq 
               < minHeap->array[smallest]->freq) 
        smallest = left; 
  
    if (right < minHeap->size 
        && minHeap->array[right]->freq 
               < minHeap->array[smallest]->freq) 
        smallest = right; 
  
    if (smallest != idx) { 
        swapMinHeapNode(&minHeap->array[smallest], 
                        &minHeap->array[idx]); 
        minHeapify(minHeap, smallest); 
    } 
} 
  

int UnitSize(struct MinHeap* minHeap){ 
  
    return (minHeap->size == 1); 
} 
  

struct MinHeapNode* extractMin(struct MinHeap* minHeap){ 
  
    struct MinHeapNode* temp = minHeap->array[0]; 
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; 
  
    minHeap->size--; 
    minHeapify(minHeap, 0); 
  
    return temp; 
} 
  

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){ 
  
    ++minHeap->size; 
    int i = minHeap->size - 1; 
  
    while (i 
           && minHeapNode->freq 
                  < minHeap->array[(i - 1) / 2]->freq) { 
  
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    minHeap->array[i] = minHeapNode; 
} 
  

void buildMinHeap(struct MinHeap* minHeap){ 
  
    int n = minHeap->size - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        minHeapify(minHeap, i); 
} 
  
void printArr(int arr[], int n){ 
    int i; 
    for (i = 0; i < n; ++i) 
        printf("%d", arr[i]); 
  
    printf("\n"); 
} 
  

int isLeaf(struct MinHeapNode* root){ 
  
    return !(root->left) && !(root->right); 
} 
  

struct MinHeap* createAndBuildMinHeap(vector<pair<char,int>> table){ 
    struct MinHeap* minHeap = createMinHeap(table.size());
    for (int i = 0; i < table.size(); ++i)
        minHeap->array[i] = newNode(table[i].first, table[i].second);
    minHeap->size = table.size(); 
    buildMinHeap(minHeap); 
    return minHeap; 
} 

struct MinHeapNode* buildHuffmanTree(vector<pair<char,int>> table){ 
    struct MinHeapNode *left, *right, *top; 
    struct MinHeap* minHeap = createAndBuildMinHeap(table); 
    while (!UnitSize(minHeap)) { 
        left = extractMin(minHeap); 
        right = extractMin(minHeap); 
        top = newNode('$', left->freq + right->freq); 
        top->left = left; 
        top->right = right; 
        insertMinHeap(minHeap, top); 
    }
    return extractMin(minHeap); 
} 
string encode(string text, map<char, string>& codes) {
    string encodedText;
    for (char c : text) {
        encodedText += codes[c];
    }
    return encodedText;
}

string decode(string encodedText, struct MinHeapNode* root) {
    string decodedText;
    struct MinHeapNode* curr = root;
    for (char bit : encodedText) {
        if (bit == '0') {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
        if (curr->left == nullptr && curr->right == nullptr) {
            decodedText += curr->data;
            curr = root;
        }
    }
    return decodedText;
}
void generateCodes(struct MinHeapNode* root, string code, map<char, string>& codeTable) {
    if (root == nullptr) return;
    if (isLeaf(root)) {
        codeTable[root->data] = code;
    }
    generateCodes(root->left, code + "0", codeTable);
    generateCodes(root->right, code + "1", codeTable);
}
map<char, string> generateCodeTable(struct MinHeapNode* root) {
    map<char, string> codeTable;
    string code;
    generateCodes(root, code, codeTable);
    return codeTable;
}



void printCodes(struct MinHeapNode* root, int arr[], int top){ 
    if (root->left) {
        arr[top] = 0; 
        printCodes(root->left, arr, top + 1); 
    } 
    // Assign 1 to right edge and recur 
    if (root->right) { 
        arr[top] = 1; 
        printCodes(root->right, arr, top + 1); 
    }
    if (isLeaf(root)) {
        printf("%c: ", root->data); 
        printArr(arr, top); 
    } 
} 
  

struct MinHeapNode* HuffmanCodes(vector<pair<char,int>> table)
{
    struct MinHeapNode* root = buildHuffmanTree(table); 
    int arr[MAX_TREE_HT], top = 0; 
    printCodes(root, arr, top); 
    return root;
}
void printCodeTable(const map<char, string>& codeTable) {
    cout << "Huffman Code Table:" << endl;
    for (const auto& entry : codeTable) {
        cout << "'" << entry.first << "': " << entry.second << endl;
    }
}
  

int main() 
{
    string file = "text.txt";
    vector<pair<char,int>> freqt = createFrequencyTable(file);
    struct MinHeapNode* root = HuffmanCodes(freqt);
    map<char, string> codeTable = generateCodeTable(root);
    printCodeTable(codeTable);
    ifstream message(file);
    string ms; 
    getline(message,ms);
    string encodedText = encode(ms, codeTable);
    message.close();
    string outputFilename = "encoded_output.txt";
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file " << outputFilename << endl;
        return 1;
    }
    outputFile << encodedText;
    string deco = decode(encodedText, root);
    outputFile.close();
    string outputFilename2 = "decoded_output.txt";
    ofstream outputFile2(outputFilename2);
    if (!outputFile2.is_open()) {
        cerr << "Error: Unable to open output file " << outputFilename2 << endl;
        return 1;
    }
    outputFile2 << deco;

    cout << "Text encoded successfully. Encoded text saved to " << outputFilename2 << endl;
    outputFile2.close();

    return 0; 
}