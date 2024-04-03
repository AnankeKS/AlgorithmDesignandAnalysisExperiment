//
//  test.cpp
//  DevideAndConquer
//
//  Created by 无铭 on 2024/4/3.
//

#include "test.hpp"

void test(const char * lable, function<void(vector<int>)> codeblock, vector<int> & array) {
    cout << "函数 " << lable << " 正在测试中\n";
    codeblock(array);
    cout << "函数 " << lable << " 测试完毕\n";
}
