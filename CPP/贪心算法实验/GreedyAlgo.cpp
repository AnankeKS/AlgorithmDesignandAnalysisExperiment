//
//  main.cpp
//  greedyAlgo
//
//  Created by 无铭 on 2024/3/29.
//

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <fstream>
#include <string>

#define MAX ((1 << 30) - 1 + (1 << 30))
#define TEST

using namespace std;

struct edge {
    int start;
    int end;
    int weight;
    
    edge(): start(0), end(0), weight(MAX){}
    edge(int start, int end, int weight): start(start), end(end), weight(weight) {}
    edge(const edge & r): start(r.start), end(r.end), weight(r.weight){}
    edge(edge & r): start(r.start), end(r.end), weight(r.weight){}
    
    inline bool operator>(const edge& r){
        return this->weight > r.weight;
    }
    inline bool operator>=(const edge& r){
        return this->weight >= r.weight;
    }
    edge& operator= (const edge& r){
        this->start = r.start;
        this->end = r.end;
        this->weight = r.weight;
        return *this;
    }
};


void test(const char* lable, function<void(void)> codeblock);
vector<edge> Kruskal(vector<vector<int>>& weightMatrix);
vector<edge> Prim(vector<vector<int>>& weightMatrix);

int main(){
    vector<vector<int>> weightMatrix;
    cout << "文件操作" << endl;
    // 从文件中获取信息
    string file_name = "图.txt";
    fstream fs;
    fs.open(file_name, ios::in | ios::out);
    if(!fs.good()){
        puts("打开文件出错");
        exit(-2);
    }
    
    string buffer;
    int vertexNum;
    int edgeNum;
    
    fs >> buffer >> vertexNum;
    cout << buffer << vertexNum << endl;
    fs >> buffer >> edgeNum;
    cout << buffer << edgeNum << endl;
    fs >> buffer;
    cout << buffer << endl;
    
    int start, end, weight;
    weightMatrix.resize(vertexNum);
    for(int i = 0; i < vertexNum; ++i){
        weightMatrix[i].resize(vertexNum);
        for(int j = 0; j < vertexNum; ++j)
            weightMatrix[i][j] = MAX;
    }
    while(!fs.eof()){
        fs >> start >> end >> weight;
        weightMatrix[start - 1][end - 1] = weightMatrix[end - 1][start - 1] = weight;
    }
    fs.close();
    
#ifdef TEST
    test("my_function", [&](){
        Kruskal(weightMatrix);
//        Prim(weightMatrix);
        
    });
#endif
    
    return 0;
}


// MARK: -function test
void test(const char* lable, function<void(void)> codeblock){
    cout << "函数" << lable << "正在测试中" << endl;
    cout << " ---------- \n" << endl;
    codeblock();
    cout << "\n ---------- " << endl;
}


// MARK: -Algorism Kruskal
vector<edge> Kruskal(vector<vector<int>>& weightMatrix){
    vector<edge> ans;
    size_t vertexNum = weightMatrix.size();
    vector<int> collection(vertexNum - 1);
    // 构造小根堆
    priority_queue<edge, vector<edge>, greater<>> minHeap;
    for(int i = 0; i < vertexNum; ++i)
        collection[i] = i + 1;
    for(int i = 0; i < vertexNum; ++i)
        for(int j = i; j < vertexNum; ++j)
            if(weightMatrix[i][j] < MAX)
                minHeap.push(edge(i + 1, j + 1, weightMatrix[i][j]));
    
    // 开始
    while(minHeap.size()){
        edge tmp = minHeap.top();
        minHeap.pop();
        if(collection[tmp.start - 1] == collection[tmp.end - 1])
            continue;
        else {
            ans.push_back(tmp);
            int start = collection[tmp.start - 1];
            for(int i = 0; i < vertexNum; ++i)
                if(start == collection[i])
                    collection[i] = collection[tmp.end - 1];
        }
    }
    for(auto &i : ans)
        cout << i.start << " --> " << i.end << "   " << i.weight << endl;
    return ans;
}


// MARK: -Algorism
vector<edge> Prim(vector<vector<int>>& weightMatrix){
    vector<bool> vis;
    vector<edge> edge_, ans;
    size_t vertexNum = weightMatrix.size();
    vis.resize(vertexNum - 1);
    for(int i = 0; i < vertexNum; ++i)
        edge_.push_back(edge(0, i, weightMatrix[0][i]));
    vis[0] = 1;
    
    function<edge(void)> findMin = [&](){
        edge tmp;
        for(int i = 0; i < vertexNum; ++i)
            if(!vis[edge_[i].end] && tmp.weight > edge_[i].weight)
                tmp = edge_[i];
        return tmp;
    };

    while(ans.size() < vertexNum - 1){
        edge min = findMin();
        vis[min.end] = 1;
        ans.push_back(min);
        int start = min.end;
        for(int i = 0; i < vertexNum; ++i)
            if(!vis[i] && edge_[i].weight > weightMatrix[start][i]){
                edge_[i].start = start;
                edge_[i].weight = weightMatrix[start][i];
            }
    }
    
    for(auto &i : ans)
        cout << i.start << " --> " << i.end << "   " << i.weight << endl;
    return ans;
}
