#pragma once
#include <unordered_set>
#include "CSV_Operations.hpp"

class DictionaryIndexer {
private:
    // 定义有效账户数据结构
    std::unordered_map<std::string, std::string> effectiveAccount;

    // 定义无效账户数据结构
    std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts;

    // 密码字典，用于存储从CSV文件中读取到的字典(将字典从文件加载到内存中运行速度会更快);
    std::unordered_set<std::string> passwordDictionary; // (unordered_set可以保证密码不重复)
    std::vector<std::string> _passwordDictionary;       // (vector可以更高效的抽取密码)

public:
    // 复制密码字典: 将密码字典从unordered_set(无序集合)数据类型复制到vector(向量)数据类型, 因为从vector密码字典抽取密码时速度会更快;
    void copyPasswordDictionary() {
        // 预先分配足够的空间，以避免在复制过程中的多次内存分配
        _passwordDictionary.reserve(passwordDictionary.size());

        // 从passwordDictionary(数据类型为:unordered_set)拷贝到_passwordDictionary(数据类型为:vector)
        for (const auto &password : passwordDictionary) _passwordDictionary.push_back(password);

        // 随机化_passwordDictionary向量(使密码字典中的密码随机排序)
        std::random_device rd;                                                       // 随机数生成器
        std::mt19937 g(rd());                                                        // 用随机数生成器初始化Mersenne Twister引擎
        std::shuffle(_passwordDictionary.begin(), _passwordDictionary.end(), g);     // 随机化_passwordDictionary向量
    }

    // 从指定的CSV文件路径读取字典数据
    std::unordered_set<std::string> readDictionary(const std::string &filePath) {
        CSV_Operations csv_op; // 创建CSV操作对象

        // 遍历CSV文件中的每行数据
        for (const auto &row : csv_op.readFromCSV(filePath)) {
            // 检查行是否非空，确保可以安全访问行的第一列数据
            if (!row.empty()) {
                // 使用.front()方法获取每行的第一列数据，将其添加到密码字典中(字典文件只有第一列)
                // .front()返回行的第一个元素，即我们关心的第一列数据
                passwordDictionary.insert(row.front());
            }
        }

        // 复制密码字典:
        copyPasswordDictionary();

        return passwordDictionary; // 返回密码字典;
    }

    // 从密码字典抽取并返回一个不重复(多次调用不重复)的字符串, 密码全部抽取完成后返回""空字符串;
    std::string extractUniquePassword() {
        // 如果向量为空，则重新复制密码字典, 并且返回一个空字符串
        if (_passwordDictionary.empty()) {
            copyPasswordDictionary();   // 复制密码字典:
            return "";
        }

        // 从向量的末尾抽取一个字符串，这样可以保证效率(比循环遍历抽取每个值的速度要快得多,用内存换运行速度)
        std::string extractedPassword = _passwordDictionary.back();
        _passwordDictionary.pop_back();      // 移除已抽取的字符串

        return extractedPassword;           // 返回抽取的字符串
    }

public:
    // 获取密码字典
    std::unordered_set<std::string> getDictionary() { return passwordDictionary; }
};