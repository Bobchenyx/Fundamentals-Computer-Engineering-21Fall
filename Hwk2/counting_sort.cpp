//#include<bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
void print(int a[], int n)  //打印输入序列
{
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << ", ";
    }
    cout << endl;
}
void counting_sort(int a[], int n, int k)
{
    //int *b, *c;
    //b = new int[n];
    //c = new int[k + 1];
    int b[n];
    int c[k+1];
    //for(int i = 0; i <= k; i++)//此处一定注意！课上伪代码数组取值范围1-k; 实际情况数组各项取值范围0-K.
    //{
    //    c[i] = 0;
    //}
    memset(c,0, sizeof(c));
    //cout << "c[] initialization" << endl;
    //print(c, k+1);
    for(int j = 0; j < n; j++)
    {
        c[a[j]] = c[a[j]] + 1;
    }
    //cout << "c[] after give a[]" << endl;
    //print(c, k+1);
    for(int i = 1; i <= k; i++)//数组C计数从指针第二位（1）开始累加.
    {
        c[i] = c[i] + c[i-1];
    }
    //cout << "c[] after counting" << endl;
    //print(c, k+1);
    for(int j = n-1; j >= 0; j--)//此处需要考虑A[0].
    {
        b[c[a[j]]-1] = a[j];
        c[a[j]] = c[a[j]] - 1;
    }
    //cout << "c[] after printing" << endl;
    //print(c, k+1);
    //cout << "--------------------------------------------------" <<endl;
    cout << "====[ Counting_Sort result ]====" << endl;
    print(b, n);
    //delete []b;
    //delete []c;
}
int main()
{
    cout << ">>>>[ Counting_Sort ]<<<<" << endl;
    int a[11] = {20, 18, 5, 7, 16, 10, 9, 3, 12, 14, 0};
    int n = 11;
    int k = 20;
    cout << "----[ Original_Array ]----" << endl;
    print(a, n);
    //cout << "--------------------------------------------------" << endl;
    counting_sort(a, n, k);
    return 0;
}

