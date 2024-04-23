//
//  main.cpp
//  Search
//
//  Created by 无铭 on 2024/4/16.
//

#include <iostream>
#include <functional>
#include <queue>

#define INF (1<<30)
using namespace std;

struct Thing {
    int weight;
    int value;
    
    Thing(int weight = 0, int value = 0): weight(weight), value(value) {}
    Thing& operator=(const Thing& r){
        this->weight = r.weight;
        this->value = r.value;
        return *this;
    }
};

void packge(vector<Thing>& Things, int cp, int rp, int rw, vector<int> &ans, vector<int> &x, int n, int &bestv){
    if(n >= Things.size()){
        if(bestv < cp){
            bestv = cp;
            ans = x;
        }
        return;
    }
    if(cp + rp > bestv){
        cp += Things[n].value;
        rw -= Things[n].weight;
        rp -= Things[n].value;
        x[n] = 1;
        if(Things[n+1].weight <= rw)
            packge(Things, cp, rp, rw, ans, x, n + 1, bestv);
        cp -= Things[n].value;
        rw += Things[n].weight;
        x[n] = 0;
        if(cp + rp > bestv)
            packge(Things, cp, rp, rw, ans, x, n + 1, bestv);
        rp += Things[n].value;
    }
}

struct node {
    int cl;
    int id;
    vector<int> x;
    // 默认四个顶点
    node(int cl = 0, int id = 0): cl(cl), id(id) { x = {0, 1, 2, 3};}
    node& operator=(const node& r) {
        this->cl = r.cl;
        this->id = r.id;
        this->x = r.x;
        return *this;
    }
    bool operator<(const node &r)const {
        return this->cl > r.cl;
    }
};

void tsp(vector<vector<int>>& weightMatrix, int vertexNum){
    // 设置优先队列（优先队列是反过来的）
    priority_queue<node, vector<node>, less<node>> q;
    vector<int> ans(4);
    int bestl = INF;
    // id 表示层数
    node newNode(0, 0);
    q.push(newNode);
    while(q.size()){
        node cur = q.top();
        q.pop();
        int n = cur.id;
        // 到了倒数第二个节点时可以直接得出结果了
        if(n == vertexNum - 2){
            int maybeTheBestCL = cur.cl + weightMatrix[cur.x[n]][cur.x[n + 1]] + weightMatrix[cur.x[n]][cur.x[0]];
            if(bestl > maybeTheBestCL){
                bestl = maybeTheBestCL;
                for(int i = 0; i < vertexNum; ++i)
                    ans[i] = cur.x[i] + 1;
                // 在最后一次交换时会把 ans[n] 和 ans[n + 1] 的值互换，这是在下面的那个嵌套中的操作
                // 因此，要得到正确的顺序，这里需要交换回来
                swap(ans[n], ans[n + 1]);
            }
        }
        if(cur.cl < bestl){
            for(int i = n + 1; i < vertexNum; ++i){
                if(weightMatrix[cur.x[n]][cur.x[i]]){
                    swap(cur.x[n], cur.x[i]);
                    node newNode = node(cur.cl + weightMatrix[cur.x[n]][cur.x[i]], n + 1);
                    newNode.x = cur.x;
                    q.push(newNode);
                }
            }
        }
    }
    
    cout << bestl << endl;
    cout << 1 << "  ";
    for(auto &i : ans)
        cout << i << "  ";
    cout << endl;
}

void test1(){
    vector<Thing> Things;
    int w[] = {2, 5, 4, 2}, v[] = {6, 3, 5, 4};
    int rp = 0;
    for(int i = 0; i < 4; ++i) {
        Things.push_back(Thing(w[i], v[i]));
        rp += v[i];
    }
    vector<int> ans(4), x(4);
    int bestv = 0;
    
    packge(Things, 0, rp, 10, ans, x, 0, bestv);
    cout << bestv << endl;
    for(auto &i : ans)
        cout << i << "\t";
    cout << "\n";
}

void test2(){
    FILE *fp = nullptr;
    int vertexNum;
    vector<vector<int>> weightMatrix;
    fp = fopen("weight.txt", "r");
    if(!fp) {
        puts("打开文件失败");
        exit(-1);
    }
    char str[64];
    fscanf(fp, "%s", str);
    fscanf(fp, "%s", str);
    fscanf(fp, "%s %d", str, &vertexNum);
    
    weightMatrix.resize(vertexNum);
    for(int i = 0; i < vertexNum; ++i) {
        weightMatrix[i].resize(vertexNum);
        for (int j = 0; j < vertexNum; ++j)
            weightMatrix[i][j] = INF;
    }
    int start, end, weight;
    while(fscanf(fp, "%d %d %d", &start, &end, &weight) > 0)
        weightMatrix[start - 1][end - 1] = weightMatrix[end - 1][start - 1] = weight;
    
    tsp(weightMatrix, vertexNum);
}

int main(int argc, const char * argv[]) {
    
//    test1();
    test2();
    
    return 0;
}
