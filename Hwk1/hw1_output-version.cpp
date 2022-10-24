#include <iostream>
#include <climits>
#include <ratio>
#include <chrono>

using namespace std;
using namespace std::chrono;

void insertion_sort(int v[], int n)
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
void merge(int a[], int p, int mid, int r)
{
    int n1 = mid - p + 1;
    int n2 = r - mid;
    int* L = new int[n1 + 2];
    int* R = new int[n2 + 2];
    int i, j;
    for (i = 1; i <= n1; i++)
        L[i] = a[p + i - 1];
    for (j = 1; j <= n2; j++)
        R[j] = a[mid + j];
    i = 1;
    j = 1;
    L[n1 + 1] = INT_MAX;
    R[n2 + 1] = INT_MAX;
    int k;
    for (k = p; k <= r; k++)
    {
        if (L[i] <= R[j])
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
void merge_sort(int a[], int p, int r)
{
    if (p < r)
    {
        int mid = (p + r) / 2;
        merge_sort(a, p, mid);
        merge_sort(a, mid + 1, r);
        merge(a, p, mid, r);
    }
}
void print(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}
int main()
{

    cout << "time competition:[insertion_sort/merge_sort]" << endl;
    for (int i = 1; i <= 80; i++)
    {
        int* v, * a, * o;
        v = new int[i];
        a = new int[i];
        o = new int[i];
        for (int j = 0; j < i; j++)
        {
            v[j] = i-j;
            a[j] = v[j];
            o[j] = v[j];
        }
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        insertion_sort(v, i);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::ratio<1, 1000>> duration_IS = duration_cast<duration<double, std::ratio<1, 1000>>>(t2 - t1);
        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        merge_sort(a, 0, i - 1);
        high_resolution_clock::time_point t4 = high_resolution_clock::now();
        duration<double, std::ratio<1, 1000>> duration_MS = duration_cast<duration<double, std::ratio<1, 1000>>>(t4 - t3);
        if (duration_IS > duration_MS)
        {
            cout << "\nInsertion_Sort time:" << duration_IS.count() << " > Merge_Sort time:" << duration_MS.count() << "  Count:" << i << endl;
            cout << "origional_array: " << "";
            print(o, i);
            cout << "Insertion_Sort : " << "";
            print(v, i);
            cout << "  Merge_Sort   : " << "";
            print(a, i);
            cout<< endl;
        }
        else if (duration_IS < duration_MS)
        {
            cout << "\nInsertion_Sort time:" << duration_IS.count() << " < Merge_Sort time:" << duration_MS.count() << "  Count:" << i << endl;
            cout << "origional_array: " << "";
            print(o, i);
            cout << "Insertion_Sort : " << "";
            print(v, i);
            cout << "  Merge_Sort   : " << "";
            print(a, i);
            cout << endl;
        }
        else if (duration_IS == duration_MS)
        {
            cout << "\nInsertion_Sort time:" << duration_IS.count() << " = Merge_Sort time:" << duration_MS.count() << "  Count:" << i << endl;
            cout << "origional_array: " << "";
            print(o, i);
            cout << "Insertion_Sort : " << "";
            print(v, i);
            cout << "  Merge_Sort   : " << "";
            print(a, i);
            cout << endl;
        }
        delete[]v;
    }

}