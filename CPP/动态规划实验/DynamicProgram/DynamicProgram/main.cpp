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
#include <math.h>

#define TEST

using namespace std;

const int vertexNum = 6;


vector<vector<vector<int>>> getGreatestDividerFromConvexPolygon(vector<vector<int>>& weightMatrix);

void display(vector<vector<int>> &path);


int minOperatingTime(vector<int> &work_a, vector<int> &work_b, bitset<1024> &vis, int &Index, int &last_b_time, int &time, int &ans);

struct Point {
    double x;
    int y;
    
    Point(double x, double y): x(x), y(y){}
    Point(): x(0), y(0){}
    Point& operator=(const Point& r){
        this->x = r.x;
        this->y = r.y;
        return *this;
    }
};


void f(vector<vector<double>>& v, bitset<8> &vis, int start, double cl, double &bestl, int n){
    if(n >= v.size()){
        if(cl < bestl)
            bestl = cl;
        return;
    }
    for(int i = 0; i < v.size(); ++i){
        if(!vis[i] && v[start][i] + cl < bestl){
            vis[start] = 1;
            f(v, vis, i, cl + v[start][i], bestl, n + 1);
            vis[start] = 0;
        }
    }
}

void find(int arr[], int root, int len, bool &ans){
    if(arr[root] != -1){
        int r = 2*(root + 1);
        if(r - 1 < len) {
            if(arr[r-1] < arr[root]) {
                cout << "左" << endl;
                find(arr, r - 1, len, ans);
            }
            else {
                cout << root << endl;
                ans = false;
                return ;
            }
        }
        if(r < len){
            if(arr[r] == -1 || arr[r] >= arr[root]) {
                cout << "右" << endl;
                find(arr, r, len, ans);
            }
            else {
                //            cout << root << endl;
                ans = false;
                return ;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    FILE * fp = NULL;
    string file_name = "edgeWeight.txt";
    fp = fopen(file_name.c_str(), "r");
    if(!fp){
        puts("打开文件失败");
        exit(-1);
    }
    
    Point arr[8];
    int a, b, c;
    for(int i = 0; i < 8; ++i){
        fscanf(fp, "%d %d %d", &a, &b, &c);
        arr[i] = Point(b, c);
    }
    
//    int arr[] = {5, 1, 6, -1, -1, 3, 7};
//    bool ans = true;
//    find(arr, 0, 7, ans);
//    cout << ans << endl;
    
//    vector<vector<double>> weightM;
//    weightM.resize(8);
//    for(int i = 0; i < 8; ++i){
//        weightM[i].reserve(8);
//        for(int j = 0; j < 8; ++j){
//            weightM[i][j] = sqrt(abs(pow(arr[i].x, 2) - pow(arr[j].x, 2) + abs(pow(arr[i].y, 2) - pow(arr[j].y, 2))));
//        }
//    }
//    
//    for(int i = 0; i < 8; ++i){
//        for(int j = 0; j < 8; ++j){
//            cout << weightM[i][j] << '\t';
//        }
//        cout << endl;
//    }
//    double bestW = 0;
//    for(int i = 0; i < 8; ++i)
//        bestW += weightM[0][i];
//    bitset<8> vis = 0;
////    cout << bestW << endl;
//    f(weightM, vis, 0, 0, bestW, 0);
//    cout << bestW << endl;
    
    char message[256];
    int vNum;
//    string message;
    fscanf(fp, "%s %d", message, &vNum);
    cout << message << vNum << endl;
    fscanf(fp, "%s", message);
    cout << message << endl;
    vector<vector<int>> weightMatrix;
    weightMatrix.resize(vNum);
    for(int i = 0; i < vNum; ++i){
        weightMatrix[i].resize(vNum);
        for(int j = 0; j < vertexNum; ++j)
            fscanf(fp, "%d", &weightMatrix[i][j]);
    }
    
#ifndef TEST
    for(auto &a : weightMatrix){
        for(auto &i : a)
            cout << i << " ";
        cout << endl;
    }
#endif
    getGreatestDividerFromConvexPolygon(weightMatrix);
    
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
    for(int p = 2; p < vNum; ++p){
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
