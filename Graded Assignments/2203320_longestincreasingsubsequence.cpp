/*
Created By: Samrat Acharya Moon
Roll no.: 2203320
*/



#include <iostream>
#include <vector>

using namespace std;

// Function to perform binary search
int bin(vector<int> vec, int k, vector<int> arr){
    int low=0;
    int high = vec.size()-1;
    int ans =-1;
    while(low<=high){
        int mid = low + (high - low)/2;
        if(arr[vec[mid]] <= k){
            low = mid+1;
        }else{
            ans = mid;
            high = mid-1;
        }
    }
    return ans;
}

// Function to compute Longest Increasing Subsequence (LIS) recursively
void LIS(vector<int> List, int i, vector<int> &tail, vector<int> &pred, vector<int> &mem){
    if(i == 0){
        tail.push_back(0);
        pred.push_back(-1);
        mem.push_back(0);
        return;
    }
    LIS(List,i-1,tail,pred,mem);
    if(List[mem.back()] <= List[i]){
        mem.push_back(i);
        tail.push_back(mem.back());
        if(mem.size()== 1){
            pred.push_back(-1);
        }else{
            pred.push_back(mem[mem.size()-2]);
        }
    }else{
        int c = bin(mem,List[i],List);
        mem.at(c)= i;
        tail.push_back(mem.back());
        if(mem.size()== 1){
            pred.push_back(-1);
        }
        else{
            pred.push_back(mem[c-1]);
        }
    }
}

// Main function
int main(){
    int n;
    cout<<"enter value of i: ";
    cin>>n;
    vector<int> mem;
    vector<int> List = {-2,7,4,0,1,5,3,8,6,5,6,4,6,6,6};
    vector<int> tail;
    vector<int> pred;

    // Computing LIS non-recursively for subarray A[1....n]
    for(int i = 0; i<=n; i++){
        if(i == List.size()) break;
        if(i == 0){
            tail.push_back(0);
            pred.push_back(-1);
            mem.push_back(0);
        }
        else{
            if(List[mem.back()] <= List[i]){
                mem.push_back(i);
                tail.push_back(mem.back());
                if(mem.size()== 1){
                    pred.push_back(-1);
                }else{
                    pred.push_back(mem[mem.size()-2]);
                }
            }else{
                int c = bin(mem,List[i],List);
                mem.at(c)= i;
                tail.push_back(mem.back());
                if(mem.size()== 1){
                    pred.push_back(-1);
                }
                else{
                    pred.push_back(mem[c-1]);
                }
            }
        }
    }

    // Printing the result for LIS non-recursively in subarray A[1....n]
    int j = n;
    vector<int> res;
    while(j>=0){
        res.push_back(List[tail[j]]);
        j = pred[j];
    }
    cout<<"Longest non decreasing sequence in subarray A[1...."<<n<<"](non-recursively) is: ";
    for(int i = res.size()-1; i>= 0;i--){
        cout<<res[i]<<" ";
    }
    res.clear();
    cout<<endl;

    // Computing LIS non-recursively ending at A[n]
    int i = n;
    while(i!= -1){
        res.push_back(List[i]);
        i = pred[i];
    }
    cout<<"Longest non decreasing sequence ending in A["<<n<<"th] element (non-recursively) is: ";
    for(int i = res.size()-1; i>= 0;i--){
        cout<<res[i]<<" ";
    }
    cout<<endl;

    // Clearing vectors for recursion
    mem.clear();
    tail.clear();
    pred.clear();

    // Computing LIS recursively for subarray A[1....n]
    LIS(List,n,tail,pred,mem);

    // Printing the result for LIS recursively in subarray A[1....n]
    j = n;
    res.clear();
    while(j>=0){
        res.push_back(List[tail[j]]);
        j = pred[j];
    }
    cout<<"Longest non decreasing sequence in subarray A[1...."<<n<<"](recursively) is: ";
    for(int i = res.size()-1; i>= 0;i--){
        cout<<res[i]<<" ";
    }
    res.clear();
    cout<<endl;

    // Computing LIS recursively ending at A[n]
    i = n;
    while(i!= -1){
        res.push_back(List[i]);
        i = pred[i];
    }
    cout<<"Longest non decreasing sequence ending in A["<<n<<"th] element (non-recursively) is: ";
    for(int i = res.size()-1; i>= 0;i--){
        cout<<res[i]<<" ";
    }
    cout<<endl;
}
