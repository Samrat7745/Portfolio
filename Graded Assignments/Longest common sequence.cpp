#include <iostream>
#include <vector>

using namespace std;

// void LCS(vector<vector<pair<int,vector<int>>>> &S, int i, int j, vector<int> arr1, vector<int> arr2){
    
//     if(i==0 && j==0){
//         if(arr1[0]==arr2[0]){
//             S[0][0].first = 1;
//             S[0][0].second.push_back(i);
//         }else{
//             S[0][0].first = 0;
//         }
//         return;
//     }
//     if(j == -1){
//         j = arr2.size() -1;
//         i = i-1;
//     }
//     LCS(S,i,j-1,arr1,arr2);
//     if(i==0){
//         if(arr1[i]==arr2[j]){
//             S[i][j].first = 1 + S[i][j-1].first;
//             S[i][j].second =  S[i][j-1].second;
//             S[i][j].second.push_back(i);
//         }else{
//             S[i][j].first = S[i][j-1].first;
//             S[i][j].second = S[i][j-1].second;
//         }
//         return;
//     }
//     if(j==0){
//         if(arr1[i]==arr2[j]){
//             S[i][j].first = 1 + S[i-1][j].first;
//             S[i][j].second = S[i-1][j].second;
//             S[i][j].second.push_back(i);
//         }else{
//             S[i][j].first = S[i-1][j].first;
//             S[i][j].second = S[i-1][j].second;
//         }
//         return;
//     }
//     if(arr1[i]==arr2[j]){
//         S[i][j].first = 1 + S[i-1][j-1].first;
//         S[i][j].second = S[i-1][j-1].second;
//         S[i][j].second.push_back(i);
//     }else{
//         int c = max(S[i][j-1].first,S[i-1][j].first);
//         S[i][j].first = c;
//         if(c == S[i][j-1].first){
//             S[i][j].second = S[i][j-1].second;
//         }else{
//              S[i][j].second = S[i-1][j].second;
//         }
//     }
//     return;
// }

int LCS(vector<vector<pair<int,vector<int>>>> &S, int m, int n, vector<int> arr1, vector<int> arr2) 
{ 
    if (m == 0 || n == 0) 
        return 0; 
    if (arr1[m - 1] == arr2[n - 1]){ 
        
        return S[m][n] = 1 + LCS(S,m-1,n-1,arr1,arr2);
    }
  
    if (S[m][n] != -1) { 
        return S[m][n]; 
    } 
    return S[m][n] = max(LCS(S,m,n-1,arr1,arr2),LCS(S,m,n-1,arr1,arr2)); 
} 


int main() {
    vector<int> arr1 = {-1,2,4,13,-5,3,6,11,-15};
    vector<int> arr2 = {1,2,3,4,5,6,-15};
    int m = arr1.size();
    int n = arr2.size();
    vector<vector<pair<int,vector<int>>>> S(m+1,vector<pair<int,vector<int>>>(n+1));
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            S[i][j].first = -1;
        }
    }
    
    LCS(S,m,n,arr1,arr2);
    cout<<"size of the longest common sequence: "<<S[m-1][n-1].first<<endl;
    for(auto ele : S[m-1][n-1].second){
        cout<<arr1[ele] <<" ";
    }
    return 0;
}