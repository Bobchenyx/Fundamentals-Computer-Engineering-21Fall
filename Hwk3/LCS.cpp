#include <iostream>
using namespace std;
const int n = 100;
int lcs_length[n][n];
int lcs_path[n][n];
void print_lcs(string x, int i, int j)
{
    if(i==0 || j==0){
        return;
    }
    if(lcs_path[i][j]==1){
        print_lcs(x, i-1, j-1);
        cout << x[j-1] <<" ";
    }else if(lcs_path[i][j]==2){
        print_lcs(x, i-1, j);
    }else{
        print_lcs(x, i, j-1);
    }
}
int length_lcs(string x, string y)
{
    for(int i=1; i<=y.length(); i++){
        lcs_length[i][0] = 0;
    }
    for(int j=0; j<=x.length(); j++){
        lcs_length[0][j] = 0;
    }
    for(int i=1; i<=y.length(); i++){
        for(int j=1; j<=x.length(); j++){
            if(y[i-1]==x[j-1]){
                lcs_length[i][j] = lcs_length[i-1][j-1] + 1;
                lcs_path[i][j] = 1;
            }else if(lcs_length[i-1][j] >= lcs_length[i][j-1]){
                lcs_length[i][j] = lcs_length[i-1][j];
                lcs_path[i][j] = 2;
            }else{
                lcs_length[i][j] = lcs_length[i][j-1];
                lcs_path[i][j] = 3;
            }
        }
    }
    return lcs_length[y.length()][x.length()];
}

void print_matrix_lcs(string x, string y)
{
    for(int i=0; i<=y.length(); i++){
        for(int j=0; j<=x.length(); j++){
            cout << lcs_length[i][j] << " ";
        }
        cout << endl;
    }

}

void print(string a)
{
    cout << "Sequence [ ";
    for (int i=0; i<a.length(); i++){
        cout << a[i] << " " ;
    }
    cout << "] " << "length of the input sequence: " << a.length() << endl;
}
int main() {
    string a, b;
    int length;
    cout << ">>>>>> DP_LCS <<<<<<" << endl;
    cout << "____please enter your sequence X:____" << endl;
    cin >> a;
    print(a);
    cout << "____please enter your sequence Y:____" << endl;
    cin >> b;
    print(b);
    length = length_lcs(a, b);
    cout << "\n>>>>[ LCS(X,Y) = " << length << " ]<<<<\n" << endl;
    cout << "____LCS_MATRIX____" << endl;
    print_matrix_lcs(a,b);
    cout << "\n>>>>[The Longest subsequence of X & Y is : ";
    print_lcs(a, b.length(), a.length());
    cout << " ]<<<<" << endl;
    return 0;
}
