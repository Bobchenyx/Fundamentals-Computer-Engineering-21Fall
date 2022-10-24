#include <iostream>
#include <cmath>
using namespace std;
void print(int a[], int n)  //打印输入序列
{
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << ", ";
    }
    cout << endl;
}
void radix_sort(int a[], int n, int k, int d)
{
    int *b, *c;
    b = new int[n];
    c = new int[k + 1];
    for(int dn = 10; dn <= pow(10,d); dn*=10)
    {
        for(int i = 0; i <= k; i++)//此处一定注意！课上伪代码数组取值范围1-k; 实际情况数组各项取值范围0-K.
        {
            c[i] = 0;
        }
        for(int j = 0; j < n; j++)//数组C各位的指针与数组C各位对应，所以当计算A[j]个数的时候，对应C的位置应该+1.
        {
            c[(a[j]%dn/(dn/10))] = c[(a[j]%dn/(dn/10))] + 1;
        }
        for(int i = 1; i <= k; i++)//数组C计数从指针第二位（1）开始累加.
        {
            c[i] = c[i] + c[i-1];
        }
        for(int j = n - 1; j >= 0; j--)//此处需要考虑A[0].
        {
            b[c[(a[j]%dn/(dn/10))]-1] = a[j];
            c[(a[j]%dn/(dn/10))] = c[(a[j]%dn/(dn/10))] - 1;
        }
        cout << "----[ Counting_Sort result on digit ]---- Round:"<< log10(dn) <<endl;
        print(b, n);
    }
    cout << "====[ Radix_Sort result ]===="<< endl;
    print(b, n);
}
int main()
{
    cout << ">>>>[ Radix_Sort ]<<<<" << endl;
    int a[] = {329, 457, 657, 839, 436, 720, 353};
    int n = 7;//number of elements in array a[]
    int k = 9;//max of each counting sort
    int d = 3;//relate to the number of digits
    cout << "----[ Original_Array ]----" << endl;
    print(a, n);
    radix_sort(a, n, k, d);
    return 0;
}

