#include<iostream>
#include <climits>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

void merge( int a[] , int p , int mid , int r )
{
    int n1 = mid - p + 1;
    int n2 = r - mid;
    int* L = new int[n1 + 2];
    int* R = new int[n2 + 2];
    int i,j;
    for (i = 1; i <= n1; i++)
        L[i] = a [p + i -1];
    for (j = 1; j <= n2; j++)
        R[j] = a [mid + j];
    i=1;
    j=1;
    L[n1 + 1] = INT_MAX;
    R[n2 + 1] = INT_MAX;
    int k;
    for (k = p ; k <= r; k++)
    {
        if ( L[i] <= R[j] )
        {
            a[k] = L[i];
            i = i + 1;
        }
        else
        {
            a[k] = R[j];
            j = j + 1;
        }
    }
    delete[]L;
    delete[]R;
}
void merge_sort( int a[] , int p , int r )
{
    if ( p < r)
    {
        int mid = ( p + r ) / 2 ;
        merge_sort(a , p , mid );
        merge_sort( a , mid+1 , r );
        merge( a , p , mid , r );
    }
}
void print(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        cout << a[i] <<",";
    }
    cout <<  endl;
}
int main()
{
    for (int i = 1; i <= 40; i++) {
        int *v, *a;
        v = new int[i];
        a = new int[i];
        for (int j = 0; j < i; j++) {
            v[j] = i - j;
            a[j] = v[j];
        }
    cout << "\noriginal sequence:" << endl;
    print( v ,i );
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    merge_sort( a , 0 , i-1 );
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::ratio<1, 1000>> duration_MS = duration_cast<duration<double, std::ratio<1, 1000>>>(t2 - t1);
    cout << "merge_sort result:" << endl;
    print( a ,i );
    cout << "Merge_Sort time:" << duration_MS.count() << "  Count:" << i << endl;
    }
}