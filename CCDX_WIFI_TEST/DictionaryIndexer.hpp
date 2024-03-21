/**
 * @file DictionaryIndexer.hpp
 * @date 20.03.2024
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2024 RMSHE. All rights reserved.
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 *
 * Electronic Mail : asdfghjkl851@outlook.com
 */

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
    // 从EffectiveAccount.csv文件中读取有效账户(unordered_map数据类型能够避免用户名重复的问题)。
    std::unordered_map<std::string, std::string> readEffectiveAccount(const std::string &filePath = "./EffectiveAccount.csv") {
        CSV_Operations csv_op; // 创建CSV操作对象

        // 调用readFromCSV读取CSV文件内容
        auto csvData = csv_op.readFromCSV(filePath);

        // 遍历读取到的CSV数据
        for (const auto &row : csvData) {
            // 确保每行数据至少有两列
            if (row.size() >= 2) effectiveAccount.emplace(row[0], row[1]); // 使用emplace提升效率并避免额外的拷贝
        }

        return effectiveAccount;
    }

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
    std::unordered_set<std::string> readDictionary(const std::string &filePath = "./Dictionary.csv") {
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

    // 获取有效账户的数目
    uint32_t getEffectiveAccountAmount() { return effectiveAccount.size(); }
};