//
//  test.cpp
//  DevideAndConquer
//
//  Created by 无铭 on 2024/4/3.
//

#include "test.hpp"

void test(const char * label, function<void(vector<int>)> codeblock, vector<int> & array) {
    cout << "函数 " << label << " 正在测试中\n";
    codeblock(array);
    cout << "函数 " << label << " 测试完毕\n";
}


void test(const char * label, function<void(void)> codeblock){
    cout << "函数 " << label << " 正在测试中\n";
    codeblock();
    cout << "函数 " << label << " 测试完毕\n";
}

