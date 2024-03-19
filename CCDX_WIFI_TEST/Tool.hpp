#pragma once
#include <chrono>
#include <algorithm>
#include <ranges>

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

    static void displayInfo() {
        std::cout << "CCDX WIFI TEST - Beta.2024.03.20.Mark0" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/GearDesignGuide" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[提示]\n1.找到的有效账户储存在程序根目录的\"EffectiveAccount.csv\"文件中;\n2.请将字典文件\"Dictionary.csv\"放到程序根目录中;\n" << std::endl;

        system("pause");
    }
};