#include<iostream>
#include<random>
#include<algorithm>
#include<ctime>
using namespace std;
void Insertion_sort(int a[], int p, int r)///insertion算法从之前代码复制过来时需要注意变量及使用问题
{
    if(p < r)
    {
        int value;
        int i, j;
        for (i = p + 1; i <= r; i++)///i的终点是数列的终点，需要小于等于才能遍历整个数列
        {
            value = a[i];
            j = i - 1;
            while (j >= p && a[j] >= value)///此处需要特别注意j指针往回循环的终点是数列的起点
            {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = value;
        }
    }
}
int partition(int a[], int p, int r, int med)
{
    int pivot = med;///将之前选取的中位数座位partition函数的pivot值
    int x = a[r];///由于不知道中位数对应数组中的具体位置，所以暂时进行直接替换并且暂存被替换的元素
    a[r] = med;
    int i = p - 1;///按照参考教材上pivot再最右侧的partition算法，i和j分别从p-1和p开始遍历数列
    for(int j = p; j < r; j++)
    {
        if(a[j] == pivot)///由于先前不知道中位数在原数组中的位置，所以此处单独与设立条件（把被替换的a[r]放回数组）
        {
            a[j] = x;
        }
        if(a[j] <= pivot)
        {
            i = i + 1;
            swap(a[i], a[j]);
        }
    }
    a[r] = a[i + 1];
    a[i + 1] = pivot;
    return i+1;
}
int Select(int a[], int p, int r, int i, int count)///需要循环调用Select以完成寻找中位数，count变量是select进行的次数
{
    int q, k, med;
    if(p == r)
        return a[p];
    if(r-p+1 <= 5)///考虑循环过程中数组长度小于等5的情况——base case
    {
        Insertion_sort(a,p,r);
        if(count > 1)///返回中位数 数组 的中位数
            return med = a[(r+p)/2];
        else///找到第i小的值了，直接return到主函数
            return a[p+i-1];
    }
    else{
        for (int j = p; j <= (r-5); j = j+5){
            Insertion_sort(a, j, j+4);///每5个数一组进行insert排序
        }
        int b = (r-p+1)/5;///新数组的长度就是原数组长度除以5取整
        int m[b];
        for (int j = 0; j < b; j++){
            m[j] = a[p + 2 + j*5];///按指针寻找将所有中位数写入新的数组，以进行下一轮Select
        }
        count++;///此处需要循环调用Select，所以循环计数变量+1
        med = Select(m, 0, b-1, b/2, count);///此处循环调用Select  >>>注意“i"对应的值<<<
    }
    if(count > 1){
        count--;///需要回溯到最初状态 寻找最初状态的a[]和对应的左右边界指针p,r
        return med;///在回溯过程中一直返回先前找到的中位数，以进行接下来的partition工作
    }
    q = partition(a, p, r, med);
    k = q - p + 1;///此处以下部分逻辑与 rand——select算法基本没有区别
    if(i == k){
        return a[q];
    }else if(i < k){
        return Select(a, p, q-1, i, 0);
    }else{
        return Select(a, q+1, r, i-k, 0);
    }
}
void print(int a[], int n)
{
    for (int i = 0; i < n; i++){
        cout << a[i] <<" ";
    }
    cout<< endl;
}
int main()
{
    srand((unsigned)time(NULL));///需要和rand一起使用，保证每次运行时候rand出来的值有区别
    cout << ">>>> [ Rand_select & Select ] <<<<" << endl;
    int n = 100;
    int * a;
    a = new int[n];
    for (int i = 0; i < n; i++){
        a[i]= i+1;
    }
    cout << "____[ Generate Array ]:____" << endl;
    print(a, n);
    for(int j = n - 1 ; j >= 1; --j){
        swap(a[j], a[rand()%j]);///通过随机生成指针位置并交换 以达成打乱原有序数组的目的
    }
    cout << "____[ Input Array ]:____" << endl;
    print(a, n);
    int num, S, count = 0;
    for(int i=0; i < 5; i++){
        num= rand()%100+1;
        S = Select(a, 0, n-1, num, count);
        cout << "\n____[ Select Result ]:____" << endl;
        cout <<"number"<< num <<"-th element:  " << S << endl;
    }
    return 0;
}
