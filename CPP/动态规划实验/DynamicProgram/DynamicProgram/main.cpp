//
//  main.cpp
//  DynamicProgram
//
//  Created by 无铭 on 2024/4/10.
//

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <functional>

#define TEST

using namespace std;

const int vertexNum = 6;


vector<vector<vector<int>>> getGreatestDividerFromConvexPolygon(vector<vector<int>>& weightMatrix);

void display(vector<vector<int>> &path);


int minOperatingTime(vector<int> &work_a, vector<int> &work_b, bitset<1024> &vis, int &Index, int &last_b_time, int &time, int &ans);


int main(int argc, const char * argv[]) {
//    FILE * fp = NULL;
//    string file_name = "edgeWeight.txt";
//    fp = fopen(file_name.c_str(), "r");
//    if(!fp){
//        puts("打开文件失败");
//        exit(-1);
//    }
//    char message[256];
//    int vNum;
////    string message;
//    fscanf(fp, "%s %d", message, &vNum);
//    cout << message << vNum << endl;
//    fscanf(fp, "%s", message);
//    cout << message << endl;
//    vector<vector<int>> weightMatrix;
//    weightMatrix.resize(vNum);
//    for(int i = 0; i < vNum; ++i){
//        weightMatrix[i].resize(vNum);
//        for(int j = 0; j < vertexNum; ++j)
//            fscanf(fp, "%d", &weightMatrix[i][j]);
//    }
//    
//#ifndef TEST
//    for(auto &a : weightMatrix){
//        for(auto &i : a)
//            cout << i << " ";
//        cout << endl;
//    }
//#endif
//    getGreatestDividerFromConvexPolygon(weightMatrix);
    
    return 0;
}




vector<vector<vector<int>>> getGreatestDividerFromConvexPolygon(vector<vector<int>>& weightMatrix){
    size_t vNum = weightMatrix.size() + 1;
    vector<vector<int>> dp, path;
    vector<vector<vector<int>>> ans;
    function<int(int, int, int)> getEdgeWeight = [&](int i, int k, int j){
        return weightMatrix[i][k] + weightMatrix[i][j] + weightMatrix[k][j];
    };
    
    dp.resize(vNum);
    path.resize(vNum);
    for(int i = 0; i < vNum; ++i){
        dp[i].resize(vNum, 0);
        path[i].resize(vNum, 0);
    }
    
    
    // dp[i][j] 表示从节点[vi-1,...vj]组成的凸多边形划分为三角形后得到的最大权值
    // 其中节点[vi-1, vi]组成的“凸多边形”的权值为0
    for(int p = 2; p <= vNum; ++p){
        for(int i = 1; i < vNum - p + 1; ++i){
            int j = i + p - 1;
            dp[i][j] = dp[i + 1][j] + getEdgeWeight(i-1, i, j);
            path[i][j] = i;
            for(int k = i + 1; k < j; ++k){
                int weight = dp[i][k] + dp[k + 1][j] + getEdgeWeight(i - 1, k, j);
                if(dp[i][j] < weight){
                    dp[i][j] = weight;
                    path[i-1][j] = k;
                }
            }
        }
    }
    for(auto &a : dp){
        for(auto &i : a)
            cout << i << "\t";
        cout << endl;
    }
    cout << endl;
    for(auto &a : path){
        for(auto &i : a)
            cout << i << "\t";
        cout << endl;
    }
    ans.push_back(dp);
    ans.push_back(path);
    return ans;
}


void display(vector<vector<int>> &path){
    
}


inline int Max(const int &a, const int &b){
    return a > b ? a : b;
}
int minOperatingTime(vector<int> &work_a, vector<int> &work_b, bitset<1024> &vis, int &Index, int &last_time, int &time, int &ans){
    if(vis.count() >= work_a.size()){
        // 还要改
        return min(ans, time);
    }
    for(int i = 0; i < work_a.size(); ++i){
        if (vis[i-1]) {
            vis[i - 1] = 1;
            int time1 = time;
            time += minOperatingTime(work_a, work_b, vis, i, work_b[i-1] + Max(0, last_time-work_a[i-1]), time, ans);
            vis[i - 1] = 0;
            time = time1;
        }
    }
    return ans;
}
