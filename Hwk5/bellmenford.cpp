#include <iostream>
#include <cstring>

using namespace std;

struct Vertex{
    int label;
    int estimation = INT_MAX;
    int path_label = 0;
};
struct Edge{
    Vertex *edge_from;
    Vertex *edge_come;
};

int main() {
    cout << ">>>> [Bellman-Ford] <<<<" << endl;

    int vertex_number, edge_number;
    cout << "please give the number of vertex & edges:" << endl;
    cin >> vertex_number >> edge_number;
    int consumption[vertex_number + 1][vertex_number + 1];
    memset(consumption, 0, sizeof(consumption));
    Vertex vertex_array[vertex_number + 1];
    for(int i=1;i<=vertex_number;i++){
        vertex_array[i].label = i;
    }
    Edge edge_array[edge_number+1];

    int from, come, cost;
    cout << "please give the cost of each path: ex. [from][come][cost]" << endl;
    for (int i = 1; i <= edge_number; i++) {
        cin >> from >> come >> cost;
        edge_array[i].edge_come = &vertex_array[come];
        edge_array[i].edge_from = &vertex_array[from];
        consumption[from][come] = cost;
    }

    int start_point;
    cout << "please give the starting point" << endl;
    cin >> start_point;
    //start_point = 1;
    vertex_array[start_point].estimation = 0;

    for (int i = 1; i <= vertex_number - 1; i++) {
        for (int j = 1; j <= edge_number; j++) {
            if (edge_array[j].edge_come->estimation > edge_array[j].edge_from->estimation +
                                                      consumption[edge_array[j].edge_from->label][edge_array[j].edge_come->label]) {
                edge_array[j].edge_come->estimation = edge_array[j].edge_from->estimation +
                                                      consumption[edge_array[j].edge_from->label][edge_array[j].edge_come->label];
                edge_array[j].edge_come->path_label = edge_array[j].edge_from->label;
            }
        }
    }

    for(int i =1;i<=edge_number;i++){
        if(edge_array[i].edge_come->estimation > edge_array[i].edge_from->estimation +
                                                 consumption[edge_array[i].edge_from->label][edge_array[i].edge_come->label]){
            cout <<"Sorry, there seems to be a negative loop in the graph, please check. END"<<endl;
            return 0;
        }
    }

    for(int i=1;i<=vertex_number;i++){
        cout<< "vertex no." <<i<<" ";
        if(i == start_point){
            cout << "[Source]"<<endl;
        }
        else{
            cout <<"path_from: " << vertex_array[i].path_label<<" ";
            cout <<"cost: "<< vertex_array[i].estimation<<endl;
        }
    }

    return 0;
}
