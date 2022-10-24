#include <ctime>
#include <iostream>
#include <climits>

using namespace std;

void insertion_sort( int v[] , int n )
{
    int value;
    int i, j;
    for (i = 1; i < n; i++)
    {
        value = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > value) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = value;
    }
}
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
int main()
{
    cout << "time competition:[insertion_sort/merge_sort]" << endl;
    for (int i = 1; i <=70; i++)
    {
        int * v, * a;
        v = new int[i];
        a = new int[i];
        for (int j = 0; j < i; j++)
        {
            v[j] = i - j;
            a[j] = v[j];
        }
        clock_t start_IS, end_IS;
        start_IS = 0;
        end_IS = 0;
        double time_IS;
        start_IS = clock();
        insertion_sort(v, i);
        end_IS = clock();
        time_IS = (double)(end_IS - start_IS) / CLOCKS_PER_SEC;
        clock_t start_MS, end_MS;
        start_MS = 0;
        end_MS = 0;
        double time_MS;
        start_MS = clock();
        merge_sort(a, 0, i-1);
        end_MS = clock();
        time_MS = (double)(end_MS - start_MS) / CLOCKS_PER_SEC;
        if (time_IS > time_MS)
        {
            cout << "Insertion_Sort time:" << time_IS << " > Merge_Sort time:" << time_MS << "  Count:" << i << endl;
        }
        else if (time_IS < time_MS)
        {
            cout << "Insertion_Sort time:" << time_IS << " < Merge_Sort time:" << time_MS << "  Count:" << i << endl;
        }
        else if (time_IS == time_MS)
        {
            cout << "Insertion_Sort time:" << time_IS << " = Merge_Sort time:" << time_MS << "  Count:" << i << endl;
        }
        delete []v;
    }

}