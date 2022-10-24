#include <iostream>
#include <vector>
#include <cmath>
//#include<queue>
#include <algorithm>
#include <cstring>

using namespace std;

struct Vertex {
    int label = 0;//后续打乱顺序后再输出可能会用到编号
    double estimate = INT_MAX;
    //vector<Vertex>vertex_come;
    //vector<Vertex>vertex_from;
    vector<Vertex *> vertex_come;
    vector<Vertex *> vertex_from;
    Vertex *parent = NULL;
};

/*struct cmp {
    bool operator()(Vertex *a, Vertex *b) {
        return a->estimate > b->estimate;
    }
};*/

bool cmp(Vertex *a, Vertex *b){
    return a->estimate > b->estimate;
}

int main() {
    cout << ">>>> [Dijkstra's algorithm] <<<<" << endl;

    int vertex_number, edge_number;
    cout << "please give the number of vertex & edges:" << endl;
    cin >> vertex_number >> edge_number;
    int consumption[vertex_number + 1][vertex_number + 1];
    memset(consumption, 0, sizeof(consumption));
    Vertex vertex_array[vertex_number + 1];

    int from, come, cost;
    cout << "please give the cost of each path: ex. [from][come][cost]" << endl;
    for (int i = 1; i <= edge_number; i++) {
        cin >> from >> come >> cost;
        //vertex_array[come].vertex_from.push_back(vertex_array[from]);
        //vertex_array[from].vertex_come.push_back(vertex_array[come]);
        vertex_array[from].vertex_come.push_back(&vertex_array[come]);
        vertex_array[come].vertex_from.push_back(&vertex_array[from]);
        consumption[from][come] = cost;
    }

    //consumption 测试代码
    /*for(int i = 1;i<=vertex_number;i++){
        for(int j =1; j<=vertex_number;j++){
            cout << consumption[i][j]<<" ";
        }
        cout <<endl;
    }*/

    int start_point;
    cout << "please give the starting point" << endl;
    cin >> start_point;
    //start_point = 1;
    vertex_array[start_point].label = start_point;
    vertex_array[start_point].estimate = 0;
    vertex_array[start_point].parent = NULL;
    //Vertex *source = &vertex_array[start_point];

    //priority_queue<Vertex *, vector<Vertex *>, cmp> Q;
    vector<Vertex *>vertex_queue;
    for (int i = 1; i <= vertex_number; i++) {
        vertex_array[i].label = i;
        //Q.push(&vertex_array[i]);
        vertex_queue.push_back(&vertex_array[i]);
    }

    sort(vertex_queue.begin(),vertex_queue.end(),cmp);
    //cout<<"top_label: "<<Q.top()->label<<endl;//测试优先级队列
    //节点间关系测试函数
    /*for (int i = 1; i <= vertex_number; i++) {
        cout << "vertex:" << vertex_array[i].label << " vertex_come: ";
        for (int j = 0; j < vertex_array[i].vertex_come.size(); j++) {
            cout << vertex_array[i].vertex_come[j]->label << " ";
        }
        cout << endl;
    }*/

    vector<Vertex *>q_operate;
    Vertex *tar;
    while (vertex_queue.size()!=0){
    //while (Q.empty() != 1) {
        //tar = Q.top();
        //pop测试函数
        tar = vertex_queue[vertex_queue.size()-1];
        //cout << "pop_label: "<<Q.top()->label<<endl;
        cout << "pop_label: "<<vertex_queue[vertex_queue.size()-1]->label<<endl;
        //Q.pop();
        for (int i = 0; i < tar->vertex_come.size(); i++) {
            if (tar->vertex_come[i]->estimate > tar->estimate + consumption[tar->label][tar->vertex_come[i]->label]) {
                tar->vertex_come[i]->estimate = tar->estimate + consumption[tar->label][tar->vertex_come[i]->label];
                tar->vertex_come[i]->parent = tar;
            }
        }
        //测试函数
        for(int j = 1;j<=vertex_number;j++){
            cout<<"| node:"<<j<<" est: "<<vertex_array[j].estimate<<" ";
        }
        cout<< endl;
        vertex_queue.pop_back();
        sort(vertex_queue.begin(),vertex_queue.end(),cmp);
        //cyx独家笨方法
        /*for(int i = 0;i<Q.size();i++){
            q_operate.push_back(Q.top());
            Q.pop();
        }
        for(int i = 0;i<q_operate.size();i++){
            Q.push(q_operate[i]);
        }
        q_operate.clear();*/
    }

    cout << "\n>>>> [Shortest Path result]: <<<<" << endl;
    for (int i = 1; i <= vertex_number; i++) {
        if (i == start_point) {
            continue;
        }
        cout << "vertex no." << i << " path_from: " << vertex_array[i].parent->label <<" Cost: "<<vertex_array[i].estimate <<endl;
    }

    return 0;
}
//程序输入样例
/*5 9
1 2 10
1 3 3
2 3 1
2 4 2
3 2 4
3 4 8
3 5 2
4 5 17
5 4 9
1*/