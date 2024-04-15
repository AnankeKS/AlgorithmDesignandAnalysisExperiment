//
//  main.cpp
//  DevideAndConquer
//
//  Created by 无铭 on 2024/4/3.
//

#include "test.hpp"



void findMaxAndSecond(vector<int>& array);

void mergeSort(vector<int>& array, int L, int R);

int * returnArr(){
    int max = 10, secLargest = 9;
    static int a[] = {2, max, secLargest};
    return a;
}


#ifdef _cplusplus
extern "C" void display();
#else
void display();
#endif

void getHex(int n){
   if(n > 0){
       getHex(n / 16);
       cout << n % 16 << " ";
   }
}

int main(int argc, const char * argv[]) {
    test("测试 const", [&](){
        // C++ 中的 const 类似于 #define，区别在于宏定义是在预处理阶段的替换，而 const 是在编译阶段的替换
        getHex(1234);
        cout << endl;
    });
    return 0;
}



// MARK: 查找最大值和次大值
void findMaxAndSecond(vector<int>& array){
    int low = 0, high = (int)array.size();
    vector<int> ans;
    int num_1, num_2, max = 0;
    int index;
    
    
    for(int index = low; index < high; ++index){
        int mid = (low + high) / 2;
        num_1 = array[low];
        num_2 = array[mid];
        // find the maximum in [low, mid)
        for(int i = low; i < mid; ++i)
            if(array[i] > num_1)
                num_1 = array[i];
        // find the maximum in [mid, high)
        for(int i = mid; i < high; ++i)
            if(array[i] > num_2)
                num_2 = array[i];
        
        // find the second largest number between num_1 and num_2
        if(num_1 >= num_2){
            max = max > num_1 ? max : num_1;
            ans.push_back(num_2);
            high = mid;
            index = low;
        } else {
            max = max > num_2 ? max : num_2;
            ans.push_back(num_1);
            low = mid;
            index = low;
        }
    }
    
    int secLargest = ans[0];
    for(auto &i : ans)
        if(secLargest < i)
            secLargest = i;
    
    cout << "最大值为: " << max << endl;
    cout << "次大值为: " << secLargest << endl;
}


// MARK: 归并排序
void merge_(vector<int>& array, int L, int M, int R);
void mergeSort(vector<int>& array, int L, int R){
    if(L == R)
        return;
    else {
        int mid = (L + R) / 2;
        mergeSort(array, L, mid);
        mergeSort(array, mid + 1, R);
        merge_(array, L, mid, R);
    }
}

void merge_(vector<int>& array, int L, int M, int R){
    vector<int> left, right;
    for(int i = L; i <= M; ++i)
        left.push_back(array[i]);
    for(int i = M + 1; i <= R; ++i)
        right.push_back(array[i]);
    
    
    int i = 0, j = 0, k = L;
    while(i < left.size() && j < right.size()){
        if(left[i] < right[j])
            array[k] = left[i++];
        else
            array[k] = right[j++];
        k++;
    }
    while(i < left.size())
        array[k++] = left[i++];
    while(j < right.size())
        array[k++] = right[j++];
}
