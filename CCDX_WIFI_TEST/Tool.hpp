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
        std::cout << "CCDX WIFI TEST - Beta.2024.03.20.Mark0" << std::endl;
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
};