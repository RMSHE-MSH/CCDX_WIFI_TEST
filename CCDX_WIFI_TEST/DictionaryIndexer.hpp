/**
 * @file DictionaryIndexer.hpp
 * @date 20.03.2024
 * @author RMSHE
 *
 * < CCDX_WIFI_TEST >
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

private:
    /**
     * 检查无效账号数据结构中给定用户名下的密码是否存在。
     *
     * @param username 用户名，作为查找的键。
     * @param password 需要检查是否存在的密码。
     * @return bool 返回true如果密码存在于指定用户名下，否则返回false(如果用户名不存在则直接返回false)。
     */
    bool isInvalidAccountsExists(const std::string &username, const std::string &password) {
        // 使用find方法尝试找到用户名
        auto userIt = invalidAccounts.find(username);
        if (userIt != invalidAccounts.end()) {
            // 如果找到用户名，再使用find方法检查密码是否存在
            return userIt->second.find(password) != userIt->second.end();
        }
        // 如果用户名不存在，或密码不在对应的集合中，返回false
        return false;
    }

    // 获取对应用户名的无效密码数目
    size_t invalidPasswordCount(const std::string &username) {
        size_t invalidPasswordCount = 0;
        if (invalidAccounts.find(username) != invalidAccounts.end()) {
            invalidPasswordCount = invalidAccounts[username].size();
        }

        return invalidPasswordCount;
    }

public:
    /**
     * 尝试将给定的用户名和密码添加到无效账户数据结构中。
     * 如果用户名不存在，则新增该用户名及其对应的密码。
     * 如果用户名已存在，但密码不在对应的密码集合中，也将其添加, 若密码已存在，则不会重复添加(用户名和密码都不会重复)。
     *
     * @param username 用户名
     * @param password 密码
     */
    bool addInvalidAccount(const std::string &username, const std::string &password) {
        // 通过insert操作的返回值判断密码是否成功添加
        auto [_, inserted] = invalidAccounts[username].insert(password);

        // 返回密码是否成功添加
        return inserted;
    }

    /**
     * 将无效账户（用户名和密码集合）保存到CSV文件中。
     * 这个函数将invalidAccounts转换成适合writeToCSV函数的格式，并调用它。
     * 文件格式规定为：每行的第一列是用户名，随后的列为对应的密码集合。
     *
     * @param filePath 要追加的CSV文件路径
     */
    void saveInvalidAccounts(const std::string &filePath = "./InvalidAccounts.csv") {
        // 存储转换后的数据
        std::vector<std::vector<std::string>> csvData;

        // 遍历每个账户和对应的密码集合，转换成CSV行格式
        for (const auto &[username, passwords] : invalidAccounts) {
            std::vector<std::string> row; // 存储一行数据，包括用户名和密码
            row.reserve(passwords.size() + 1); // 预留空间，提高效率
            row.push_back(username); // 首列添加用户名
            for (const auto &password : passwords) row.push_back(password);// 添加密码
            csvData.push_back(std::move(row)); // 添加到csvData，使用std::move减少拷贝
        }

        CSV_Operations csv_op; // 创建CSV操作对象

        // 调用writeToCSV函数，将数据覆盖到CSV文件
        csv_op.writeToCSV(csvData, filePath, "w");
    }

    /**
     * 从InvalidAccounts.csv文件中读取无效账户信息。
     * @param filePath CSV文件路径
     */
    void readInvalidAccounts(const std::string &filePath = "./InvalidAccounts.csv") {
        CSV_Operations csv_op; // 创建CSV操作对象

        // 从CSV文件中读取数据
        auto csvData = csv_op.readFromCSV(filePath);

        // 遍历读取到的每一行数据
        for (const auto &row : csvData) {
            if (!row.empty()) { // 确保行数据不为空
                const auto &username = row.front(); // 第一个元素作为用户名
                for (size_t i = 1; i < row.size(); ++i) { // 遍历余下的每个元素（密码）
                    addInvalidAccount(username, row[i]);  // 尝试将给定的用户名和密码添加到无效账户数据结构中。
                }
            }
        }
    }

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

    // 复制密码字典(重置密码字典向量): 将密码字典从unordered_set(无序集合)数据类型复制到vector(向量)数据类型, 因为从vector密码字典抽取密码时速度会更快;
    void copyPasswordDictionary(const std::string &username, const std::string &initialPassword) {
        // 判断初始密码是否有效(true:表示密码有效);
        bool initPwdIsValid = !isInvalidAccountsExists(username, initialPassword);

        // 计算预期的有效密码数量，如果初始密码有效，额外加一(在这里"+1"是因为向量末尾还需要额外容纳一个初始密码)
        size_t expectedValidCount = passwordDictionary.size() - invalidPasswordCount(username) + (initPwdIsValid ? 1 : 0);

        // 清空并释放_passwordDictionary向量空间的内存;
        std::vector<std::string>().swap(_passwordDictionary);

        // 预先分配向量空间，以容纳所有预期的有效密码，减少内存重新分配
        _passwordDictionary.reserve(expectedValidCount);

        // 从passwordDictionary(数据类型为:unordered_set)拷贝到_passwordDictionary(数据类型为:vector)
        for (const auto &password : passwordDictionary) {
            // 使用isInvalidAccountsExists来检查密码是否有效(跳过无效的密码)
            if (!isInvalidAccountsExists(username, password)) {
                _passwordDictionary.push_back(password);
            }
        }

        // 使用C++20范围和算法复制有效密码
        //for (const auto &password : passwordDictionary | std::views::filter([&](const std::string &pwd) {
        //    return !isInvalidAccountsExists(username, pwd);
        //                                                                    })) {
        //    _passwordDictionary.push_back(password);
        //}

        // 随机化_passwordDictionary向量(使密码字典中的密码随机排序)
        static std::mt19937 g(std::random_device{}());
        std::shuffle(_passwordDictionary.begin(), _passwordDictionary.end(), g);

        // 若初始密码有效，则将其添加到向量末尾(密码的抽取是从向量末尾开始的, 这样能保证在第一次抽取密码时取到的是初始密码)
        if (initPwdIsValid) _passwordDictionary.push_back(initialPassword);
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

        return passwordDictionary; // 返回密码字典;
    }

    // 从密码字典抽取并返回一个不重复(多次调用不重复)的字符串, 密码全部抽取完成后返回""空字符串;
    std::string extractUniquePassword() {
        // 如果向量为空，则重新复制密码字典, 并且返回一个空字符串
        if (_passwordDictionary.empty()) return "";

        // 从向量的末尾抽取一个字符串，这样可以保证效率(比循环遍历抽取每个值的速度要快得多,用内存换运行速度)
        std::string extractedPassword = _passwordDictionary.back();
        _passwordDictionary.pop_back();      // 移除已抽取的字符串

        return extractedPassword;           // 返回抽取的字符串
    }

public:
    // 获取密码字典
    std::unordered_set<std::string> getDictionary() { return passwordDictionary; }

    // 获取有效账户的数目
    size_t effectiveAccountCount() { return effectiveAccount.size(); }

    // 获取当前用户名的预期有效密码的数目
    size_t expectedValidPwdCount() { return _passwordDictionary.size(); }

    // 获取指定用户名下的无效密码的数量
    uint32_t invalidPwdCount(const std::string &username) {
        // 尝试在无效账户数据结构中查找指定的用户名
        auto it = invalidAccounts.find(username);
        // 如果找到，则返回对应的无效密码的数量
        if (it != invalidAccounts.end()) return it->second.size();

        // 如果未找到指定的用户名，返回0
        return 0;
    }
};