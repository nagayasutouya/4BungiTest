#ifndef DEFINE_VECTOR_H_ 
#define DEFINE_VECTOR_H_

#include<vector>
#include<string>

using Row = std::vector<std::string>;//一行
using Rows = std::vector<Row>;//行で保存されたデータ群
using VFloat = std::vector<float>;//Float群
using VInt = std::vector<int>;//Int群
using VBool = std::vector<bool>;

#endif