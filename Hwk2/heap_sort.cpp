#include <iostream>
//#include<cmath>
#include<random>
#include<algorithm>
using namespace std;
void max_heapify(int a[], int i, int n) //heap按max规则调整
{
    int l = 2 * i;
    int r = 2 * i + 1;
    int largest;
    if(l <= n && a[l] > a[i])
    {
        largest = l ;
    }
    else
    {
        largest = i;
    }
    if(r <= n && a[r] > a[largest]){
        largest = r;
    }
    int value;
    if(largest != i){
        value = a[i];
        a[i] = a[largest];
        a[largest] = value;
        max_heapify(a, largest, n);
    }
}
void build_max_heap(int a[], int n) //自身调用 构建max heap
{
    for (int i = n/2; i >= 1 ; i--)
    {
        max_heapify(a, i, n);
    }
}
void print(int a[], int n)  //打印输入序列
{
    for (int i = 1; i <= n; i++)
    {
        cout << a[i] <<" ";
    }
    cout<< endl;
}
/*void print_heap_tree(int a[], int n)
{
    cout << "Max_Heap_Tree:" << endl;
    int total_level = log(n)/log(2);
    for(int l = 0; l <= total_level; l++)
    {
        for(int i = pow(2,l); i < pow(2,l+1) && i <= n ; i++)
        {
            if(a[i] <= 9)
            {
                cout << a[i] <<"    ";
            }
            else
            {
                cout << a[i] <<"   ";
            }
        }
        cout << endl;
    }
}*/
void heap_sort(int a[], int n)
{
    int * H;
    H = new int[n+1];
    for(int i = n; i >= 1; i--)
    {
        H[i] = a[1];
        a[1] = a[i];
        max_heapify(a, 1, i-1);
    }
    cout << "____Heap_sort result:____" << endl;
    print(H, n);
}
int main() {
    cout << ">>>> [ Max_heap & Heap_sort ] <<<<" << endl;
    int n;
    cout <<"____please type your n:____"<< endl;
    cin >> n; //读取目标数列总长度
    int * a;
    a = new int[n+1]; //数组指针从0开始，但heap tree里面key不能为0
    a [0] = 0;//暂且设定数组a[0]=0
    for (int i = 1; i <= n; i++)
    {
        a[i]= i;
    }
    for(int j = n; j >= 1; --j)
    {
        swap(a[j], a[rand()%j+1]);
    }
    cout << "____Input Array:____" << endl;
    print(a, n);
    build_max_heap(a, n);
    cout << "____After Max_heap:____" << endl;
    print(a, n);
    //print_heap_tree(a, n);
    heap_sort(a, n);
    return 0;
}
