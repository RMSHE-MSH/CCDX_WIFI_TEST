/**
 * @file Tool.hpp
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
#include <chrono>
#include <algorithm>
#include <ranges>
#include "DictionaryIndexer.hpp"

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
        std::cout << "CCDX WIFI TEST - Dev.2024.03.26.Mark4" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/CCDX_WIFI_TEST" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[提示]\n1.找到的有效账户储存在程序根目录的\"EffectiveAccount.csv\"文件中;\n2.请将字典文件\"Dictionary.csv\"放到程序根目录中;\n" << std::endl;

        system("pause");
    }

    // 显示字典内容
    void displayDictionary() {
        system("CLS");
        DictionaryIndexer dic;

        uint64_t num = 0;
        for (auto &i : dic.readDictionary("./Dictionary.csv")) std::cout << ++num << "\t" << i << std::endl;

        std::cout << "\n以上是读取到的字典文件内容.\n" << std::endl;
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
};