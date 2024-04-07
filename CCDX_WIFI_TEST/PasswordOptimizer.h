/**
 * @file PasswordOptimizer.hpp
 * @date 27.03.2024
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
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

 // 密码优化器: 使用遗传算法来优化密码字典中的密码，减少无效密码的比例
class PasswordOptimizer {
public:
    // 构造函数，接收无效账户数据和密码字典作为输入
    PasswordOptimizer(std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts,
                      std::unordered_set<std::string> passwordDictionary);

    double calculateFitness(const std::string &password);
private:
    // 存储所有无效密码及其出现频率
    std::unordered_map<std::string, uint32_t> invalidPassword;

    // 引用传递的无效账户数据
    std::unordered_map<std::string, std::unordered_set<std::string>> &invalidAccounts;

    // 引用传递的密码字典
    std::unordered_set<std::string> &passwordDictionary;

private:
    // 统计invalidAccounts中的无效密码及其出现频率。
    void invalidPasswordsCount();
};