//
//  main.cpp
//  HeuristicSearch
//
//  Created by 无铭 on 2024/4/23.
//

#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <time.h>

#define pi (3.14)

using namespace std;

class GSA {
private:
    const int dim;                  // 维数
    const int N;                    // 粒子个数
    const double G;                 // 万有引力常数，后续的常数都会根据这个计算，默认为 100
    const double a;                 // 计算万有引力时的常数 a, 默认为 20
    const int T;                    // 最大迭代次数
    const double itervalLength;     // 界限被设置为 [-itervalLength / 2, itervalLength / 2]
    double minFit;                  // 全局最小适应度，这个值就是最终要找的
    double maxFit;                  // 全局最大适应度
    int minFitIndex;                // 最小适应度对应的粒子下标
    
    vector<double> fitness;         // 适应值，里面存放了每个粒子的适应值
    vector<double> q;               // 每个粒子归一化后的值
    vector<double> M;               // 每个粒子的质量
    vector<vector<double>> F;       // 在迭代中某一维度的粒子所受到的的万有引力的合外力，行数为维度数，列数为粒子个数
    vector<vector<double>> X;       // 在迭代中某一维度的粒子所处的位置
    vector<vector<double>> V;       // 在迭代中某一维度的粒子的速度
    vector<vector<double>> A;       // 在迭代中某一维度的粒子的加速度
    
    void updateFitness();                       // 更新适应度
    void worst();                               // 获取最小适应度  返回值为最小适应度对应的粒子下表
    void best();                                // 获取最大适应度
    void updateM();                             // 获取所有粒子的质量
    double updateG(const int &iter) const;      // 更新第 iter 次迭代时的万有引力常数
    void updateA(const int &iter);              // 更新加速度
    double getR(const int &i, const int &j);    // 计算欧式距离
    void updateVaX(const int &iter);            // 更新速度和位置
    
public:
    GSA(const int dim = 10, const int N = 50, const double G = 100, const double a = 20, const int T = 800, const double itervalLength = 10, double minFit = 10000, double maxFit = 0);
    
    // MARK: get the answer
    void solve();                               // 获取结果的函数
    double print();                             // 输出最小适应度及其组成，返回最小适应度
};

// MARK: 构造器
GSA :: GSA(const int dim, const int N, const double G, const double a, const int T, const double itervalLength, double minFit, double maxFit): dim(dim), N(N), G(G), a(a), T(T), itervalLength(itervalLength), minFit(minFit), maxFit(maxFit){
    // 设置随机数种子
    srand((unsigned int)time(0));
    // 第一个随机数总是非常假的假随机
    // 过滤掉第一个随机数
    rand();
    
    fitness.resize(this->N);
    q.resize(this->N);
    M.resize(this->N);
    F.resize(this->N);
    X.resize(this->N);
    V.resize(this->N);
    A.resize(this->N);
    // 对列进行初始化
    for(int i = 0; i < this->N; ++i){
        F[i].resize(this->dim, 0);
        X[i].resize(this->dim, 0);
        V[i].resize(this->dim, 0);
        A[i].resize(this->dim, 0);
        for(int j = 0; j < this->dim; ++j){
            X[i][j] = rand() / double(RAND_MAX) * itervalLength - itervalLength / 2;           // 范围设置为 [-Bound/2, Bound/2]
        }
    }
}

// MARK: 更新适应度
void GSA :: updateFitness() {
    for(int i = 0; i < this->N; ++i){
        double fitness1 = 0, fitness2 = 1;
        for(int j = 0; j < this->dim; ++j){
            double Xij = X[i][j];
            fitness1 += Xij * Xij / 4000;
            fitness2 *= cos(Xij/sqrt(j + 1));
        }
        fitness[i] = fitness1 - fitness2 + 1;
    }
}

// MARK: 获取最小适应度
void GSA :: worst() {
    for(int i = 0; i < this->N; ++i)
        if(this->fitness[i] < this->minFit){
            this->minFit = this->fitness[i];
            this->minFitIndex = i;
        }
}

// MARK: 获取最大适应度
void GSA :: best() {
    this->maxFit = fitness[0];
    for(auto &i : fitness)
        if(i > this->maxFit)
            this->maxFit = i;
}

// MARK: 更新第 iter 次迭代时的万有引力常数
double GSA :: updateG(const int &iter) const {
    double _G = G*exp((-(this->a)*iter)/(this->T));
    return _G;
}

// MARK: 获取所有粒子的质量
void GSA :: updateM() {
    // 更新适应度
    updateFitness();
    worst();
    best();
    int worstValue = this->minFit;
    int bestValue = this->maxFit;
    
    // 更新归一化后的每个粒子的值
    double sumQ = 0;
    for(int i = 0; i < N; ++i) {
        q[i] = (fitness[i] - worstValue) / (bestValue - worstValue);
        sumQ += q[i];
    }
    // 更新质量
    for(int i = 0; i < N; ++i)
        if(q[i] == 0)
            M[i] = q[i] / sumQ;
        else
            M[i] = 0.000025;
}

// MARK: 计算欧式距离
double GSA :: getR(const int &i, const int &j){
    double sumR = 0;
    for(int d = 0; d < this->dim; ++d){
        sumR += abs(X[i][d] - X[j][d]);
    }
    return sumR / this->dim;
}

// MARK: 更新加速度
void GSA :: updateA(const int &iter){
    // 粒子之间的力
    vector<vector<vector<double>>> f;
    // 当前迭代层次的 G
    double G = this->updateG(iter);
    // 更新此次迭代的质量
    this->updateM();
    double r = 0.0001;
    f.resize(this->N);
    // 获取粒子i、j之间的力
    for(int i = 0; i < this->N; ++i){
        f[i].resize(this->N);
        for(int j = 0; j < this->N; ++j){
            f[i][j].resize(this->dim);
            
            int R = getR(i, j);
            for(int k = 0; k < dim; ++k){
                if(i != j){
                    f[i][j][k] = G * (this->M[i] - this->M[j]) / (R + r) * (X[i][dim] - X[j][dim]);
                } else {
                    f[i][j][k] = 0;
                }
            }
        }
    }
    
    // 计算每个维度的粒子所受到的合力
    srand((unsigned int)time(0));
    for(int i = 0; i < this->N; ++i){
        for(int d = 0; d < this->dim; ++d){
            this->F[i][d] = 0;
            for(int j = 0; j < this->N; ++j){
                this->F[i][d] += rand() / (double)RAND_MAX * f[i][j][d];
            }
            // 更新加速度
            this->A[i][d] = this->F[i][d] / this->M[i];
        }
    }
}

void GSA :: updateVaX(const int &iter){
    // 更新每个维度的每个粒子的加速度
    this->updateA(iter);
    const double Bound = this->itervalLength / 2;
    for(int i = 0; i < this->N; ++i){
        for(int d = 0; d < this->dim; ++d){
            // 先更新速度
            V[i][d] = rand()/(double)RAND_MAX * V[i][d] + A[i][d];
            // 防止越界
            if(V[i][d] > Bound || V[i][d] < -Bound)
                V[i][d] = 0;
            
            X[i][d] = X[i][d] + V[i][d];
            // 同上
            if(X[i][d] > Bound || X[i][d] < -Bound)
                X[i][d] = Bound;
        }
    }
}

double GSA :: print() {
    // 找到最小适应度
    int index = this->minFitIndex;
    cout << this->fitness[index] << endl;
    cout << "最优解为：" << this->minFit << endl;
    cout << "其组成为: ";
    for(int i = 0; i < this->dim; ++i)
        cout << this->X[index][i] << " ";
    cout << endl;
    return this->minFit;
}

void GSA :: solve() {
    for(int iter = 0; iter < this->T; ++iter){
        this->updateVaX(iter);
    }
    this->print();
}


int main(){
    srand((unsigned int)time(0));
    GSA ans;
    clock_t start, end;
    start = clock();
    ans.solve();
    end = clock();
    cout << "程序耗时：" << double(end - start)/CLOCKS_PER_SEC << "秒" << endl;
    return 0;
}
