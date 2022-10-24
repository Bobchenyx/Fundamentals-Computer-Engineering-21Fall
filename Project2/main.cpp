//
// Created by Yixiao Chen on 2021/12/3.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

struct Task {
    int label;//后续打标签为local[0]/remote[1]
    vector<int> pred;
    vector<int> succ;
    int No;//任务编号属性 暂时用数组index替代
    int core_run; //当前任务被规划核心
    int ready_time_local = 0;
    int ready_time_ws = 0;
    int ready_time_c = 0;
    int ready_time_wr = 0;
    int finish_time_local = 0;
    int finish_time_ws = 0;
    int finish_time_c = 0;
    int finish_time_wr = 0;
    int priority;//任务优先级属性 暂时用结构体之外的数组解决
    Task() {}
};

struct Core {
    int ready_time = 0; // 核心最早空闲时间
    int start_time = 0; // 该变量为当前正在规划的任务的实际开始时间
    double unit_e_consumption = 0;
    double energy_consumption = 0;
    vector<int> task_list;
    Core() {}
};

int task_number, core_number, edge_number;
int T_s, T_c, T_r;
//int T_s = 3, T_c = 1, T_r = 1;//测试代码 [参照论文原文数据]
int consumption[21][6];//任务数量暂设上限20.本地核心数上限5 [参考论文中任务数10.核心数3]
/*int consumption[11][4] = {0, 0, 0, 0,
                          0, 9, 7, 5,
                          0, 8, 6, 5,
                          0, 6, 5, 4,
                          0, 7, 5, 3,
                          0, 5, 4, 2,
                          0, 7, 6, 4,
                          0, 8, 5, 3,
                          0, 6, 4, 2,
                          0, 5, 3, 2,
                          0, 7, 4, 2,};*///consumption matrix 测试代码[参考论文数据]

int Sorted_priority[20];//暂时将优先级数组设置为全局变量 方便函数间调用
double Energy_consumption;
int Time_consumption;
double TIME_LIMIT;

void Primary_assignment(Task task_array[]) {
    int remote_consumption = T_s + T_c + T_r;
    int local_min_consumption; // min函数无法比较两个类型不同的变量
    for (int i = 1; i <= task_number; i++) {
        local_min_consumption = INT_MAX;
        for (int j = 1; j <= core_number; j++) {
            local_min_consumption = min(local_min_consumption, consumption[i][j]);
        }
        if (local_min_consumption > remote_consumption) {
            task_array[i].label = 1;//标记 cloud task
        } else {
            task_array[i].label = 0;//标记 local 后续决定具体执行位置
        }
    }

    //label测试代码
    cout<<"label array"<<endl;
    for(int i=1;i<=task_number;i++){
        cout << task_array[i].label <<" ";
    }
    cout<<endl;
}

void Task_prioritizing(Task task_array[]) {
    int remote_consumption = T_s + T_c + T_r;
    double w[task_number + 1];///次数需要思考具体数据类型 int/double 后去取平均操作int会取整
    memset(w, 0, sizeof(w));

    /** 遍历任务 计算每个任务的cost **/
    for (int i = 1; i <= task_number; i++) {
        if (task_array[i].label == 1) {
            w[i] = remote_consumption;
        } else {
            for (int j = 1; j <= core_number; j++) {
                w[i] = w[i] + consumption[i][j];
                cout << consumption[i][j]<<" ";
            }
            w[i] = w[i] / core_number;
        }
    }

    //cost 测试代码;
    cout<<"Cost array"<<endl;
    for(int i=1;i<=task_number;i++){
        cout << w[i]<<" ";
    }
    cout << endl;

    /** 利用任务间依赖关系 由下至上计算优先级对应数值 **/
    double priority[task_number + 1];
    priority[task_number] = w[task_number];//将exit任务的优先级设置为其自身的cost
    ///此处日后仍需考虑 exit task 不止一个的情况

    ///此处存疑 如何recursively compute
    double max_succ;
    for (int i = task_number - 1; i >= 1; i--) {
        max_succ = 0;
        for (int j = 0; j < task_array[i].succ.size(); j++) {
            max_succ = max(max_succ, priority[task_array[i].succ[j]]);
        }
        priority[i] = w[i] + max_succ;
    }

    //优先级测试代码
    cout<< "priority queue" <<endl;
    for(int i=1;i<=task_number;i++){
        cout << priority[i]<<" ";
    }
    cout<< endl;

    /** 按priority数值大小 暂用insertion_sort进行重新排序 **/
    //新创建一个数组 用于保存排序后任务编号
    //int Sorted_priority[task_number + 1]; 暂时设置为全局变量
    memset(Sorted_priority, 0, sizeof(Sorted_priority));
    for (int i = 1; i <= task_number; i++) {
        Sorted_priority[i] = i;
    }

    //利用priority数组个任务优先级的大小关系 同时排序任务编号对应的数组 得到下一步所需规划顺序
    double value1;
    int value2;
    for (int i = 2; i <= task_number; i++) {
        value1 = priority[i];
        value2 = Sorted_priority[i];
        int j = i - 1;
        while (j >= 1 && priority[j] < value1) {
            priority[j + 1] = priority[j];
            Sorted_priority[j + 1] = Sorted_priority[j];
            j--;
        }
        priority[j + 1] = value1;
        Sorted_priority[j + 1] = value2;
    }

    //优先级序列排序后测试代码
    cout << "Sorted priority" << endl;
    for (int i = 1; i <= task_number; i++) {
        cout << Sorted_priority[i] << " ";
    }
    cout << endl;
}

void Execution_unit_select(Task task_array[], Core core_array[]) {
    /** 按前一步排好的优先级进行任务的初步规划 **/
    int max_ft, finish_time, min_ft, temp_ft, temp_rt, temp_st, best_ft_core, temp_ws, temp_c;
    for (int i = 1; i <= task_number; i++) {
        /** 如果该任务标记为remote[1]，则直接对其传输、执行、接收三步进行时间的规划 **/
        if (task_array[Sorted_priority[i]].label == 1) {

            //分别更新该task的属性 和 云端core的task列表
            task_array[Sorted_priority[i]].core_run = 0;
            core_array[0].task_list.push_back(Sorted_priority[i]);

            //计算数据传输的最早开始时间 RT_WS
            //此处需要比较 所有子任务的结束时间[本地任务-结束；云端任务-传输结束]
            //比较云端任务传输的最早开始时间 云端传输线程的最早空闲时间
            max_ft = 0;
            for (int j = 0; j < task_array[Sorted_priority[i]].pred.size(); j++) {
                finish_time = max(task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_local,
                                  task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_ws);
                max_ft = max(max_ft, finish_time);
            }
            task_array[Sorted_priority[i]].ready_time_ws = max_ft;

            // 计算出当前云端任务最早传输时间后，可以通过比较 传输核心最早空闲时间 决定最终传输开始时间
            core_array[0].start_time = max(core_array[0].ready_time, task_array[Sorted_priority[i]].ready_time_ws);
            // 当前任务传输结束时间 = 开始时间+传输耗时
            task_array[Sorted_priority[i]].finish_time_ws = core_array[0].start_time + T_s;
            // 更新当前传输核心的最早空闲时间
            core_array[0].ready_time = task_array[Sorted_priority[i]].finish_time_ws;

            //云端任务计算的最早开始时间 RT_c [任务自身传输结束，子云端任务计算结束]取最大值
            max_ft = 0;
            for (int j = 0; j < task_array[Sorted_priority[i]].pred.size(); j++) {
                //前置任务云端执行结束时间 FT_c
                max_ft = max(max_ft, task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_c);
            }
            task_array[Sorted_priority[i]].ready_time_c = max(task_array[Sorted_priority[i]].finish_time_ws, max_ft);
            //论文中计算的ready_time即为运算计算的开始时间 因此，此处更新当前任务云计算结束时间
            task_array[Sorted_priority[i]].finish_time_c = task_array[Sorted_priority[i]].ready_time_c + T_c;

            //云端任务回传数据ready time
            task_array[Sorted_priority[i]].ready_time_wr = task_array[Sorted_priority[i]].finish_time_c;
            //云端回传数据结束时间
            task_array[Sorted_priority[i]].finish_time_wr = task_array[Sorted_priority[i]].ready_time_wr + T_r;

        }
            /** 若该任务被标记为本地任务[0]，则需要分别考虑其在每个核心 以及在云端运行的结束时间，最后选择最优 **/
        else {
            min_ft = INT_MAX;//min_ft变量记录当前最早结束时间 用于比较 并选择最终任务部署的核心
            //遍历该任务在每个核心上运行的结束时间，更新最小值
            //首先需要计算当前任务如果在本地端部署的ready_time —— temp_rt
            temp_rt = 0;//初始化变量
            for (int j = 0; j < task_array[Sorted_priority[i]].pred.size(); j++) {
                max_ft = max(task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_wr,
                             task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_local);
                temp_rt = max(temp_rt, max_ft);
            }
            //cout << "task " << Sorted_priority[i] << " temp_rt:" << temp_rt;//测试代码
            //接下来需要遍历每个本地核心
            for (int j = 1; j <= core_number; j++) {
                //通过比较该任务的最早开始时间和当前核心的最早空闲时间得到该任务最早开始时间
                temp_st = max(temp_rt, core_array[j].ready_time);
                temp_ft = temp_st + consumption[Sorted_priority[i]][j];
                if (temp_ft < min_ft) {
                    min_ft = temp_ft;
                    best_ft_core = j;
                    //暂时更新一下当前task对应的结束时间和对应核心
                    task_array[Sorted_priority[i]].finish_time_local = temp_ft;
                    task_array[Sorted_priority[i]].core_run = j;
                    //此处不更新当前核心相关属性因为日后需要通过比较得到最终需要更新的核心编号
                } else {
                    continue;
                }
            }
            //cout << " FT:" << temp_ft << endl;//测试代码

            //接下来需要考虑该任务如果部署在云端情况下的finish_time
            //首先考虑当前云端传输的最早开始时间
            temp_ws = 0;
            for (int j = 0; j < task_array[Sorted_priority[i]].pred.size(); j++) {
                max_ft = max(task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_local,
                             task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_ws);
                temp_ws = max(temp_ws, max_ft);
            }
            //计算当前任务云端数据上传的开始时间
            temp_st = max(core_array[0].ready_time, temp_ws);

            //计算当前任务在云端计算开始时间temp_c 以及当前任务最早回传结束时间
            max_ft = 0;
            for (int j = 0; j < task_array[Sorted_priority[i]].pred.size(); j++) {
                //前置任务云端执行结束时间 FT_c
                max_ft = max(max_ft, task_array[task_array[Sorted_priority[i]].pred[j]].finish_time_c);
            }
            temp_c = max(temp_st + T_s, max_ft);
            temp_ft = temp_c + T_c + T_r;
            //cout << " cloud_ft: " << temp_ft << endl;//测试代码
            //此时进行最终判断 按判断结果更新核心相关属性
            if (temp_ft < min_ft) {
                /** 判断结果为该任务最终部署在云端 **/
                best_ft_core = 0;
                //更新任务所属核心和核心对应任务列表
                task_array[Sorted_priority[i]].core_run = 0;
                core_array[0].task_list.push_back(Sorted_priority[i]);
                core_array[0].ready_time = temp_st + T_s;
                //更新任务的云端部署有关结束时间
                task_array[Sorted_priority[i]].finish_time_wr = temp_ft;
                task_array[Sorted_priority[i]].finish_time_ws = temp_st + T_s;
                task_array[Sorted_priority[i]].finish_time_c = temp_c + T_c;
                //重置该任务本地端相关信息
                task_array[Sorted_priority[i]].finish_time_local = 0;
            } else {
                /** 判断结果为该任务最终部署在本地 **/
                //更新最优解对应核心的任务序列和最早空闲时间
                core_array[best_ft_core].task_list.push_back(Sorted_priority[i]);
                core_array[best_ft_core].ready_time = task_array[Sorted_priority[i]].finish_time_local;
            }
            //cout << "task:" << Sorted_priority[i] << " best_ft_core:" << best_ft_core << endl;
        }
    }
}

void Initial_scheduling_test(Task task_array[], Core core_array[]) {
    /** 初步规划测试函数 **/
    int m, n, h;
    //分别输出每个核心上规划的任务编号
    for (n = 1; n <= core_number; n++) {
        cout << "core:" << n << " | ";
        for (m = 0; m < core_array[n].task_list.size(); m++) {
            cout << core_array[n].task_list[m] << " ";
        }
        cout << endl;
    }
    //输出云端数据传输线路上规划的任务编号
    cout << "core:0" << " | ";
    for (m = 0; m < core_array[0].task_list.size(); m++) {
        cout << core_array[0].task_list[m] << " ";
    }
    cout << endl;
    //分别输出每个任务的结束时间
    for (h = 1; h <= task_number; h++) {
        cout << "task" << h;
        cout << " local_ft: " << task_array[h].finish_time_local;
        cout << " could_ft: " << task_array[h].finish_time_wr;
        cout << " ws_ft: " << task_array[h].finish_time_ws;
        cout << " c_ft: " << task_array[h].finish_time_c;
        cout << endl;
    }
}

void Compute_Time_Consumption(Task task_array[]) {
    /**遍历任务 找到最晚结束时间**/
    int finish_time = 0;
    int m;
    vector<int> exit_task;
    Time_consumption = 0; //变量初始化
    for (m = 1; m <= task_number; m++) {
        finish_time = max(task_array[m].finish_time_local, task_array[m].finish_time_wr);
        Time_consumption = max(Time_consumption, finish_time);
    }
    cout << "TIME_CONSUMPTION = " << Time_consumption << endl;
}

void Initial_Scheduling(Task task_array[], Core core_array[]) {
    cout << ">>> INITIAL SCHEDULING <<<" << endl;
    Primary_assignment(task_array); //给每个任务打初始标签：本地任务0/云端1
    Task_prioritizing(task_array); //计算任务优先级 指定后续任务的规划顺序
    Execution_unit_select(task_array, core_array); //按优先级对每个任务进行运算位置的选择 并完成初步规划
    Initial_scheduling_test(task_array, core_array); //测试函数 Initial scheduling 结果输出
    Compute_Time_Consumption(task_array); //初步规划结束以后 计算总耗时
}

void Compute_Energy_Consumption(Task task_array[], Core core_array[], double energy_array[]) {
    Energy_consumption = 0;//变量初始化
    /** 给所有核心单位能量赋值 **/
    int m, n;
    for (n = 0; n <= core_number; n++) {
        core_array[n].unit_e_consumption = energy_array[n];
    }
    // 本地核心和云端核心计算方式有所区别
    core_array[0].energy_consumption = T_s * core_array[0].unit_e_consumption * core_array[0].task_list.size();
    Energy_consumption = core_array[0].energy_consumption;
    cout << "core:0 energy: " << core_array[0].energy_consumption << endl;//输出测试代码
    for (n = 1; n <= core_number; n++) {
        for (m = 0; m < core_array[n].task_list.size(); m++) {
            core_array[n].energy_consumption = core_array[n].energy_consumption +
                                               consumption[core_array[n].task_list[m]][task_array[core_array[n].task_list[m]].core_run] *
                                               core_array[n].unit_e_consumption;
        }
        cout << "core:" << n << " energy: " << core_array[n].energy_consumption << endl;//输出测试代码
        Energy_consumption = Energy_consumption + core_array[n].energy_consumption;
    }
    cout << "ENERGY_CONSUMPTION = " << Energy_consumption << endl;//输出测试代码
}

double
Rescheduling_Algorithm(Task temp_task[], Core temp_core[], int target_task, int original_core, double energy_array[]) {
    int n;
    for (n = 0; n <= core_number; n++) {
        temp_core[n].unit_e_consumption = energy_array[n];
    }
    /** 根据每个任务既定的运行核心 计算出运行耗时 求出当前情况下的priority序列 **/
    //计算每个任务的cost
    int t, c;
    int w[task_number + 1];
    for (t = 1; t <= task_number; t++) {
        if (temp_task[t].core_run == 0) {
            w[t] = T_s + T_c + T_r;//云端任务单独计算
        } else {
            w[t] = consumption[t][temp_task[t].core_run];//本地核心既定.直接计算
        }
    }
    //cost输出测试函数
    /*cout<<"Cost_array: ";
    for(t = 1;t<=task_number;t++ ){
        cout<<w[t]<<" ";
    }
    cout<<endl;*/

    //根据cost值计算出每个任务priority的值
    int temp_p[task_number + 1];
    memset(temp_p, 0, sizeof(temp_p));
    int max_succ;
    for (t = task_number; t >= 1; t--) {
        max_succ = 0;
        for (c = 0; c < temp_task[t].succ.size(); c++) {
            max_succ = max(max_succ, temp_p[temp_task[t].succ[c]]);
        }
        temp_p[t] = w[t] + max_succ;
    }
    //temp_p输出测试函数
    /*cout<<"temp_p_array: ";
    for(t = 1;t<=task_number;t++ ){
        cout<<temp_p[t]<<" ";
    }
    cout<<endl;*/

    //根据priority值计算当前的优先级序列
    int priority[task_number + 1];
    memset(priority, 0, sizeof(priority));
    for (t = 1; t <= task_number; t++) {
        priority[t] = t;
    }
    int value1, value2;
    for (t = 2; t <= task_number; t++) {
        value1 = temp_p[t];
        value2 = priority[t];
        c = t - 1;
        while (c >= 1 && temp_p[c] < value1) {
            temp_p[c + 1] = temp_p[c];
            priority[c + 1] = priority[c];
            c--;
        }
        temp_p[c + 1] = value1;
        priority[c + 1] = value2;
    }
    /*cout<<"priority_array: ";
    for(t = 1;t<=task_number;t++ ){
        cout<<priority[t]<<" ";
    }
    cout<<endl;*/

    /** 按当前优先级完成任务的规划 —— 根据core_run属性 直接计算结束时间有关属性 **/
    int max_ft, finish_time, temp_ft;
    for (t = 1; t <= task_number; t++) {
        //云端任务规划
        if (temp_task[priority[t]].core_run == 0) {
            /** 首先应该重置该任务本地结束时间信息 因为该任务有可能是被调整的任务 **/
            temp_task[priority[t]].finish_time_local = 0;

            //更新当前核心对应任务序列
            temp_core[0].task_list.push_back(priority[t]);
            //计算最早传输时间RT_WS
            max_ft = 0;
            for (c = 0; c < temp_task[priority[t]].pred.size(); c++) {
                finish_time = max(temp_task[temp_task[priority[t]].pred[c]].finish_time_local,
                                  temp_task[temp_task[priority[t]].pred[c]].finish_time_ws);
                max_ft = max(max_ft, finish_time);
            }
            temp_task[priority[t]].ready_time_ws = max_ft;
            //计算该任务真正传输开始时间
            temp_core[0].start_time = max(temp_core[0].ready_time, temp_task[priority[t]].ready_time_ws);
            //计算当前任务传输结束时间
            temp_task[priority[t]].finish_time_ws = temp_core[0].start_time + T_s;
            //更新传输核心最早空闲时间
            temp_core[0].ready_time = temp_task[priority[t]].finish_time_ws;
            //计算云端任务计算的最早开始时间 RT_C
            max_ft = 0;
            for (c = 0; c < temp_task[priority[t]].pred.size(); c++) {
                //前置任务云端执行结束时间 FT_c
                max_ft = max(max_ft, temp_task[temp_task[priority[t]].pred[c]].finish_time_c);
            }
            temp_task[priority[t]].ready_time_c = max(temp_task[priority[t]].finish_time_ws, max_ft);
            //更新当前任务云计算结束时间
            temp_task[priority[t]].finish_time_c = temp_task[priority[t]].ready_time_c + T_c;
            //云端任务回传结束时间
            temp_task[priority[t]].finish_time_wr = temp_task[priority[t]].finish_time_c + T_r;
        }
            //本地任务规划
        else {
            /** 首先重置该任务云端部署相关信息 **/
            temp_task[priority[t]].finish_time_ws = 0;
            temp_task[priority[t]].finish_time_c = 0;
            temp_task[priority[t]].finish_time_wr = 0;

            //更新当前核心任务序列
            temp_core[temp_task[priority[t]].core_run].task_list.push_back(priority[t]);
            //计算该任务ready_time
            max_ft = 0;
            for (c = 0; c < temp_task[priority[t]].pred.size(); c++) {
                temp_ft = max(temp_task[temp_task[priority[t]].pred[c]].finish_time_wr,
                              temp_task[temp_task[priority[t]].pred[c]].finish_time_local);
                max_ft = max(max_ft, temp_ft);
            }
            temp_task[priority[t]].ready_time_local = max_ft;
            //计算当前任务真正开始时间
            temp_core[temp_task[priority[t]].core_run].start_time = max(
                    temp_core[temp_task[priority[t]].core_run].ready_time,
                    temp_task[priority[t]].ready_time_local);
            //计算当前任务结束时间
            temp_task[priority[t]].finish_time_local = temp_core[temp_task[priority[t]].core_run].start_time +
                                                       consumption[priority[t]][temp_task[priority[t]].core_run];
            //更新当前核心的最早空闲时间
            temp_core[temp_task[priority[t]].core_run].ready_time = temp_task[priority[t]].finish_time_local;
        }
    }
    //规划完成测试函数
    //Initial_scheduling_test(temp_task,temp_core);

    /** 计算所有任务最终用时 **/
    max_ft = 0;
    for (t = 1; t <= task_number; t++) {
        temp_ft = max(temp_task[t].finish_time_wr, temp_task[t].finish_time_local);
        max_ft = max(max_ft, temp_ft);
    }
    //cout << "finish_time: "<< max_ft << endl;

    /** 返回值判断 超时跳出当前循环 **/
    if (max_ft > TIME_LIMIT) {
        return 0;
    } else if (max_ft == Time_consumption) {
        return 886;
    }
    int delta_time = max_ft - Time_consumption;
    //cout << "delta_time: " << delta_time << endl;

    /** 计算当前任务迁移后能耗变化 **/
    double current_energy, original_energy;
    if (temp_task[target_task].core_run == 0) {
        current_energy = T_s * temp_core[0].unit_e_consumption;
    } else {
        current_energy = temp_core[temp_task[target_task].core_run].unit_e_consumption *
                         consumption[target_task][temp_task[target_task].core_run];
    }
    original_energy = temp_core[original_core].unit_e_consumption * consumption[target_task][original_core];
    //cout <<"original_energy: "<< original_energy<<endl;
    //cout <<"current_energy: "<< current_energy<<endl;
    double delta_energy = original_energy - current_energy;

    //cout << "delta_energy: " << delta_energy << endl;
    /** 返回当前能耗时间比 **/
    return delta_energy / delta_time;
}

void Print_Solution(Task task_array[], Core core_array[], double energy_array[]) {
    Initial_scheduling_test(task_array, core_array);
    Compute_Time_Consumption(task_array);//过程中会更新 全局变量任务总耗时
    Compute_Energy_Consumption(task_array, core_array, energy_array);//过程中会更新 全局变量任务总功耗
}

void Task_Migration(double time_max_rate, Task task_array[], Core core_array[], double energy_array[]) {
    cout << "\n>>> ENERGY-AWARE SCHEDULING <<<" << endl;
    /** 首先需要创造两个副本 对应所有任务和所有核心 **/
    Task temp_task[task_number + 1];//新建一个任务序列副本 用于task_reschedule
    Task temp_task_best[task_number + 1];// 用于保存 循环中的最优解

    Core temp_core[core_number + 1];//新建核心副本 用于task_reschedule
    Core temp_core_best[core_number + 1];// 用于保存循环中的最优解
    Core empty_core[core_number + 1];//用于循环中对temp_core进行重置操作

    TIME_LIMIT = Time_consumption * time_max_rate;
    cout << ">>> TIME_LIMIT = " << TIME_LIMIT << " <<<" << endl;
    double max_ET_rate, ET_rate;
    int original_core;
    int cont_d = 0;
    int m, n;

    /** 最外层循环 最多每个任务都调整一次位置 中途设有break判断 **/
    for (int k = 1; k <= task_number; k++) {
        cout << "-----round: " << k << " -----" << endl;//适当记录循环次数 以便后续调试
        max_ET_rate = 0;//外层循环跳出判断变量重置为0
        cont_d = 0;//外层循环继续变量重置为0
        /** 需要先将当前最优序列进行拷贝 以进行后续优化步骤 **/
        for (int i = 1; i <= task_number; i++) {
            temp_task[i] = task_array[i];
        }
        for (int i = 1; i <= task_number; i++) {
            //cout << "TRY_task: "<< i << endl;
            if (task_array[i].core_run == 0) {
                /** 如果任务已经被规划到云端 则无法通过调整任务位置以降低能耗**/
                //直接进入下一个任务的循环
                //cout <<"skipped"<<endl;//测试代码
                continue;
            } else {
                /** 遍历其他核心 找到最优的**/
                for (int j = 0; j <= core_number; j++) {
                    //cout << "try_core: " << j <<" temp_core:"<<temp_task[i].core_run<<endl;//测试代码
                    if (j >= temp_task[i].core_run) {
                        /** 如果该任务转移到当前核心.能量并没有减少.则直接跳过该层循环 **/
                        //cout << "skipped"<<endl;//测试代码
                        break;
                        /** 此处默认输入consumption矩阵时 核心能耗.随编号增加 **/
                    } else {
                        /** 需要清零覆盖临时核心的属性 **/
                        for (n = 0; n <= core_number; n++) {
                            temp_core[n] = empty_core[n];
                        }
                        original_core = temp_task[i].core_run;
                        //cout << "original core: "<< temp_task[i].core_run;//测试代码
                        temp_task[i].core_run = j;///更新当前被迁移任务的目标核心
                        ET_rate = Rescheduling_Algorithm(temp_task, temp_core, i, original_core, energy_array);
                        //cout << "ET_rate: " <<ET_rate <<endl;
                        //cout <<"original_core: "<< original_core<<endl;//测试代码
                        ///此处如果reschedule过程中计算出T_total>TIME_LIMIT则直接返回E/T=0
                        if (ET_rate == 886) {
                            ///该情况表明时间没变且能量减少 保存当前结果 还原改动 跳出循环
                            for (m = 1; m <= task_number; m++) {
                                temp_task_best[m] = temp_task[m];
                            }
                            for (n = 0; n <= core_number; n++) {
                                temp_core_best[n] = temp_core[n];
                            }
                            temp_task[i].core_run = original_core;//还原改动
                            cont_d = 1;//外层循环继续变量激活
                            break;
                        } else if (ET_rate > max_ET_rate) {
                            ///记录当前最优情况
                            max_ET_rate = ET_rate;
                            for (m = 1; m <= task_number; m++) {
                                temp_task_best[m] = temp_task[m];
                            }
                            for (n = 0; n <= core_number; n++) {
                                temp_core_best[n] = temp_core[n];
                            }
                        }
                        temp_task[i].core_run = original_core;///经过该层循环需要改回原状
                    }
                }
                if (ET_rate == 886) {
                    break;
                }
                //cout << "Best_core: " << temp_task_best[i].core_run << endl;
            }
        }
        if (max_ET_rate == 0 && cont_d == 0) {
            /** 循环跳出判断 如果一整轮遍历过后 该值并没有得到更新 则可理解为无更优解 **/
            break;
        } else {
            /** 此时为找出新一组最优解情况 需要将新一组temp最优解赋值给原有序列 以进行下一轮选择 **/
            for (m = 1; m <= task_number; m++) {
                task_array[m] = temp_task_best[m];
            }
            for (n = 0; n <= core_number; n++) {
                core_array[n] = temp_core_best[n];
            }
            Print_Solution(task_array, core_array, energy_array);//测试函数 结果输出函数
        }
        /** 输出当前循环最优解 同时计算当前最优解总能耗 总耗时**/
    }
    cout << "We couldn't find any better solution within the TIME_LIMIT" << endl;
}

int main() {
    cout << ">>>>> [Energy-and-Performance-Aware-Task-Scheduling] <<<<<\n" << endl;

    /** 读取总任务量 **/
    //int task_number, core_number, edge_number, i, j;
    cout << "Please type the total Number of Tasks" << endl;
    cin >> task_number;
    //task_number = 10; //测试代码[参照论文数据]
    Task task_array[task_number + 1]; //创建一个task类型数组 每个位置存储对应任务的结构体 结构体内包含：前置节点数组、后继节点数组

    /** 读取本地能够进行计算核心数量 **/
    cout << "Please type the total number of Local Cores" << endl;
    cin >> core_number;
    //core_number = 3; //测试代码[参照论文数据]
    Core core_array[core_number + 1];//创建一个Core类型的数组 每个位置对应结构体存储 核心相关信息

    /** 读取各个任务在不同核心上执行耗时 **/
    cout << "Please give the Consumption matrix:" << endl;
    //int consumption[task_number + 1][core_number + 1];
    memset(consumption, 0, sizeof(consumption));
    for (int i = 1; i <= task_number; i++) {
        //task_array[i].No = i; 任务编号属性 后续代码优化改进时添加
        for (int j = 1; j <= core_number; j++) {
            cin >> consumption[i][j]; // time of task i on core j
        }
    }
    /** 读取云端各环节耗时 **/
    //int T_s, T_c, T_r;
    cout << "Please input [Sending time T_s][Process Time T_c][Receive Time T_r] for Remote Computing" << endl;
    cin >> T_s >> T_c >> T_r;

    cout << "Please type the total number of Edges" << endl;
    cin >> edge_number;
    //edge_number = 15;//测试代码 [参照论文数据]
    cout << "Please give the order graph" << endl;
    int start, end;
    for (int i = 0; i < edge_number; i++) {
        cin >> start >> end;
        task_array[start].succ.push_back(end);
        task_array[end].pred.push_back(start);
    }
    /*int order[15][2] = {1, 2,
                        1, 3,
                        1, 4,
                        1, 5,
                        1, 6,
                        2, 8,
                        2, 9,
                        3, 7,
                        4, 8,
                        4, 9,
                        5, 9,
                        6, 8,
                        7, 10,
                        8, 10,
                        9, 10};*///测试代码 [参照论文数据]
    /*for (i = 0; i < edge_number; i++) {
        task_array[order[i][0]].succ.push_back(order[i][1]);
        task_array[order[i][1]].pred.push_back(order[i][0]);
    }*/
    //前置任务序列 后级任务序列 测试代码
    cout<< "succ_pred test"<<endl;
    for(int i=1;i<=task_number;i++){
        cout<<"task "<< i << " " <<endl;
        for(int j=0;j<task_array[i].pred.size();j++){
            cout << task_array[i].pred[j]<<" ";
        }
        cout << " || ";
        for(int j=0;j<task_array[i].succ.size();j++){
            cout << task_array[i].succ[j]<<" ";
        }
        cout << endl;
    }

    /** 在不考虑各任务功耗的条件下，按时间最短的标准对任务进行分配和规划 **/
    Initial_Scheduling(task_array, core_array);

    /** 结束初步规划后 需要读取每个核心单位时间能耗 进行当前规划的能耗计算 **/
    double energy_array[core_number + 1];
    cout << "pleas give the power of sending:"<<endl;
    cin >> energy_array[0];
    //energy_array[0] = 0.5;////测试代码[参照论文数据]
    cout << "pleas give the power of each local core:"<<endl;
    for(int i=1;i<=core_number;i++){
        cin >> energy_array[i];
    }
    /*energy_array[1] = 1;
    energy_array[2] = 2;
    energy_array[3] = 4;*///测试代码[参照论文数据]

    Compute_Energy_Consumption(task_array, core_array, energy_array);

    /** 设置时间上限 随后用循环进行尝试的调整 在时间允许的范围内选择最优解 **/
    double time_max_rate = 1.5;

    Task_Migration(time_max_rate, task_array, core_array, energy_array);

    return 0;
}
