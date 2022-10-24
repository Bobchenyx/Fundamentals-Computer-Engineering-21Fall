#include <iostream>

using namespace std;
const int n = 50;/**此处根据实际情况预设 DP二维数组 大小**/
int Mmg_sum[n][n], Mmg_path[n][n];
int sum, B_min, number;

void Mmg_output_G(int G[], int N, int M) {
    //G[0] = 1;
    number = N;
    for (int i = M; i >= 1; i--) {
        G[i] = number - Mmg_path[i][number];
        number = Mmg_path[i][number];
        if (i == 2) {
            G[i - 1] = number;/**输出G的第一位无需继续循环，可以直接由当前情况得出。**/
            break;
        }
    }
}

void print_partition(int a[], int G[], int M, int N) {
    int count1 = 0, count2 = 0;/**此处采用了两个变量分别划定每组输出的起始和结尾下标；也可以只用一个变量实现。**/
    for (int i = 1; i <= M; i++) {
        count1 = count1 + G[i];
        cout << "( ";
        for (int j = count2 + 1; j <= count1; j++) {
            cout << a[j] << " ";
        }
        count2 = count2 + G[i];
        cout << ")";
    }
    cout << endl;
}

/**此处Max_min_km功能函数通过递归调用实现k自增的比较 算法时间复杂度不会因为递归或者循环而改变**/
/*void Max_min_km(int a[], int k, int i, int j) {
    if (k >= j) {
        Mmg_sum[i][j] = B_min;
        return;
    } else {
        //sum = 0;
        //for (int m = k + 1; m <= j; m++) {
        //    sum = sum + a[m];
        //}
        sum = Mmg_sum[1][j] - Mmg_sum[1][k];//此处求和步骤不需要循环求解，可以利用DP表格钟第一行已经计算好的各长度和相减来得到
        if (sum >= Mmg_sum[i - 1][k] && B_min <= Mmg_sum[i - 1][k]) {
            B_min = Mmg_sum[i - 1][k];
            Mmg_path[i][j] = k;
        } else if (sum < Mmg_sum[i - 1][k] && B_min <= sum) {
            B_min = sum;
            Mmg_path[i][j] = k;
        }
        Max_min_km(a, k + 1, i, j);
    }
}*/

void Max_min_group(int a[], int N, int M) {/**此处可以选择给数组0行0列赋值为0，不进行此赋值也不影响过程和结果。**/
    /*for (int i = 0; i <= M; i++) {
        Mmg_sum[i][0] = 0;
    }
    for (int j = 0; j <= N; j++) {
        Mmg_sum[0][j] = 0;
    }*/
    Mmg_sum[1][0] = 0;
    for (int j = 1; j <= N; j++) {
        Mmg_sum[1][j] = Mmg_sum[1][j - 1] + a[j];/**此处稍微注意是否需要将[1][0]提前赋0**/
        /*sum = 0;
        for (int s = 0; s <= j; s++) {
            sum = sum + a[s];
        }
        Mmg_sum[1][j] = sum;*/
    }
    /**
     * 此处以下代码部分可以通过 Max_min_km功能函数进行递归调用实现
     * 也可以通过三层循环解决 两者算法时间复杂度一样
     * **/
    for (int i = 2; i <= M; i++) {
        /*for (int j = 1; j < i; j++) {
            Mmg_sum[i][j] = 0;
        }*/
        for (int j = i; j <= N; j++) {
            B_min = 0;
            for (int k = i - 1; k < j; k++) {
                sum = Mmg_sum[1][j] - Mmg_sum[1][k];/**此处求和步骤不需要循环求解，可以利用DP表格钟第一行已经计算好的各长度和相减来得到**/
                if (sum >= Mmg_sum[i - 1][k] && B_min <= Mmg_sum[i - 1][k]) {
                    B_min = Mmg_sum[i - 1][k];
                    Mmg_path[i][j] = k;
                } else if (sum < Mmg_sum[i - 1][k] && B_min <= sum) {
                    B_min = sum;
                    Mmg_path[i][j] = k;
                }
            }
            Mmg_sum[i][j] = B_min;
        }
    }
    /*for (int i = 2; i <= M; i++) {
        for (int j = 1; j < i; j++) {
            Mmg_sum[i][j] = 0;
        }
        for (int j = i; j <= N; j++) {
            int k = i - 1;
            B_min = 0;
            Max_min_km(a, k, i, j);
        }
    }*/
}

void print_Mmg(int a[n][n], int N, int M) {
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= N; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**代码编辑初期 调试所用主函数**/
/*int main() {
    cout << ">>>>> Project 1: Max_min_group <<<<<" << endl;
    int N = 12;///输入目标数组的长度
    int a[13] = {0, 3, 9, 7, 8, 2, 6, 5, 10, 1, 7, 6, 4};///int a[N+1];循环cin
    int M = 3;
    int G[4];///int G[M+1]
    Max_min_group(a, N, M);
    cout << "Max_min_group DP_matrix:" << endl;
    print_Mmg(Mmg_sum, N, M);
    cout << "Max_min_group Path_matrix:" << endl;
    print_Mmg(Mmg_path, N, M);
    cout << "Max_min_group Result G[M] = [ ";
    Mmg_output_G(G, N, M);
    for (int i = 1; i <= M; i++) {
        cout << G[i] << ", ";
    }
    cout << "]" << endl;
    cout << "Minimum element of array B is :" << Mmg_sum[M][N] << endl;///输出DP表格C[i][j]的最后一个元素
    cout << "Partition Result of Max_min_group:";
    print_partition(a, G, M, N);
    getchar();
    return 0;
}*/

/**可自定义输入的程序所对应主函数**/
int main() {
    int N, M;
    cout << ">>>>> Project 1: Max_min_group <<<<<" << endl;
    cout << "Please enter the [length] of your [input array]:" << endl;
    cin >> N;
    int a[N + 1];
    a[0] = 0;///为处理后续数组下标以及DP二维数组下标问题 将输入输入改为由0开始
    cout << "Enter input array:" << endl;
    for (int i = 1; i <= N; i++) {
        cin >> a[i];
    }
    cout << "Please enter the [length] of your [Group array]:" << endl;
    cin >> M;
    if(M>N){
        cout<< "invalid cases"<<endl;///处理特殊情况
        return 0;
    }
    int G[M+1];///此处同上 也是为了统一各数组下标
    Max_min_group(a, N, M);
    cout << "Max_min_group DP_matrix:" << endl;
    print_Mmg(Mmg_sum, N, M);
    cout << "Max_min_group Path_matrix:" << endl;
    print_Mmg(Mmg_path, N, M);
    cout << "Max_min_group Result G[M] = [ ";
    Mmg_output_G(G, N, M);
    for (int i = 1; i <= M; i++) {
        cout << G[i] << ", ";
    }
    cout << "]" << endl;
    cout << "Minimum element of array B is :" << Mmg_sum[M][N] << endl;/**输出DP表格C[i][j]的最后一个元素**/
    cout << "Partition Result of Max_min_group:";
    print_partition(a, G, M, N);
    getchar();
    getchar();
    return 0;
}
