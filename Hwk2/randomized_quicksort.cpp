#include <iostream>
#include <ratio>
#include <chrono>
#include <random>
using namespace std;
using namespace chrono;
int partition(int a[], int l, int r)
{
    int R = rand()%(r - l + 1) + l;//随机生成[l,r]之间整数
    cout<< "[" << R << "] ";
    int pivot = a[R];
    a[R] = a[r];
    a[r] = pivot; //将随机认定的pivot与数组最后一位数调换
    int e;//定义一个用于下列循环交换数组内容的变量
    int i = l - 1;
    for(int j = l; j < r; j++)
    {
        if(a[j] <= pivot)
        {
            i = i + 1;
            e = a[i];
            a[i] = a[j];
            a[j] = e;
        }
    }
    a[r] = a[i + 1];
    a[i + 1] = pivot;
    return i+1;
}
void quicksort(int a[], int l, int r)
{
    int p;
    if(l <= r)
    {
        p = partition(a, l, r);
        quicksort(a, l, p-1);
        quicksort(a, p+1, r);
    }
}
void print_array(int a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        cout << a[i] << ", ";
    }
    cout << endl;
}
int main() {
    cout << ">>>> [Quick_Sort] <<<<" << endl;
    int n;
    cout << "---- please give your n: ----" << endl;
    cin >> n;
    int * a, * v;
    a = new int[n];
    v = new int[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = i + 1;
        v[i] = a[i];
    }
    cout << "____Original Array:____" << endl;
    print_array(v, n);
    for(int j = 1; j <= 5; j++)
    {
        cout << "____Random Pivot Position a[R]:____" << endl;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        quicksort(a, 0, n-1);
        cout << endl;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, ratio<1, 1000>> duration_QS = duration_cast<duration<double, ratio<1, 1000>>>(t2 - t1);
        cout << "____QuickSort Result:____" << endl;
        print_array(a, n);
        cout << "____QuickSort Running Time:____"<< endl;
        cout << duration_QS.count() << "ms" << "  Count:" << j << endl;
    }
    return 0;
}
