/**
 * @file Tool.hpp
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
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <thread>

class Tool {
public:
    // 对包含全数字字符串的vector进行排序
    void sortNumericStrings(std::vector<std::string> &vector) {
        // 使用范围库中的sort函数进行排序
        // 自定义比较函数作为lambda表达式，将字符串转换为长整型后比较
        std::ranges::sort(vector, [](const std::string &a, const std::string &b) {
            // 将字符串转换为long long类型后比较
            return std::stoll(a) < std::stoll(b); });
    }

    // 显示版本信息
    static void displayInfo() {
        std::cout << "CCDX WIFI TEST - Dev.2024.03.26.Mark6" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/CCDX_WIFI_TEST" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[提示]\n1.找到的有效账户储存在程序根目录的\"EffectiveAccount.csv\"文件中;" <<
            "\n2.请将字典文件\"Dictionary.csv\"放到程序根目录中;" <<
            "\n3.无效账户数据储存在程序根目录的 InvalidAccounts.csv 文件中;"
            << std::endl;

        system("pause");
    }

    // 随机延时: 爬虫延时机制，接受两个参数：最小延时时间和最大延时时间（单位：毫秒）
    void randomDelay(uint16_t minDelayMs, uint16_t maxDelayMs) {
        // 检查最小和最大延时值是否合理
        if (minDelayMs > maxDelayMs || maxDelayMs == 0) return;

        // 创建随机数生成器，使用随机设备来提供种子
        std::random_device rd;
        std::mt19937 gen(rd());
        // 定义一个范围在minDelayMs到maxDelayMs之间的均匀分布
        std::uniform_int_distribution<> distrib(minDelayMs, maxDelayMs);

        // 生成一个随机延时值
        uint16_t delay = distrib(gen);

        // 实现延时
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    /**
     * @brief 删除字符串中的一个连续子串。
     *
     * @param original 原始字符串。
     * @param toRemove 要删除的子串。
     * @return std::string 删除指定子串后的字符串。
     */
    std::string removeSubstring(const std::string &original, const std::string &toRemove) {
        // 若原始字符串或要删除的子串为空，则直接返回原始字符串
        if (original.empty() || toRemove.empty()) {
            return original;
        }

        std::string result = original; // 创建原始字符串的副本
        size_t pos = result.find(toRemove); // 查找子串的起始位置

        // 如果找到子串，则删除之
        if (pos != std::string::npos) {
            result.erase(pos, toRemove.length());
        }

        return result;
    }

    // 生成一个随机长度的随机字符串的函数
    // @param minLength 字符串的最小长度
    // @param maxLength 字符串的最大长度
    // @return 生成的随机字符串
    std::string generateRandomString(int minLength, int maxLength) {
        // 初始化随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());

        // 选择一个随机长度，位于指定的最小和最大长度之间
        std::uniform_int_distribution<> lengthDistr(minLength, maxLength);
        int stringLength = lengthDistr(gen);

        // 定义可能的字符集合，包括大小写字母、数字和特殊字符
        std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";

        // 生成随机字符串
        std::string randomString;
        randomString.reserve(stringLength); // 提前为字符串分配足够的空间，提高效率
        std::uniform_int_distribution<> charDistr(0, static_cast<int>(chars.length() - 1));
        for (int i = 0; i < stringLength; ++i) {
            randomString += chars[charDistr(gen)];
        }

        return randomString;
    }
};