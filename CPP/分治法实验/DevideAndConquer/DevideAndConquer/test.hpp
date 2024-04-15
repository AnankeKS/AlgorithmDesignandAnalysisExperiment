//
//  test.hpp
//  DevideAndConquer
//
//  Created by 无铭 on 2024/4/3.
//

#ifndef test_hpp
#define test_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

void test(const char * label, function<void(vector<int>)> codeblock, vector<int> & array);

void test(const char * label, function<void(void)> codeblock);

#endif /* test_hpp */
