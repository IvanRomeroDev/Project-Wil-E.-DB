/**
* Author: Jorge Raul Tzab Lopez
* Github: https://github.com/SJMA11723
* LinkedIn: https://www.linkedin.com/in/jorgetzab/
*/

#include <bits/stdc++.h>

using namespace std;

long long dp[100];

long long fib(int n){
    if(n < 2) return n;

    if(!dp[n]){
        dp[n] = fib(n - 1) + fib(n - 2);
    }

    return dp[n];
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;
    cout << fib(n);
}
