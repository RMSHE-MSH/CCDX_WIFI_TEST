/**
 * @file UsernameGenerator.hpp
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
#include <random>
#include <string>
#include <stdexcept>
#include "Tool.hpp"

#include "DictionaryIndexer.hpp"

Tool tl;
DictionaryIndexer dic;

class UsernameGenerator {
private:

    // 存储已生成的用户名列表
    std::unordered_set<std::string> usernamesList; // (unordered_set可以保证用户名不重复)
    std::vector<std::string> _usernamesList;       // (vector可以更高效的抽取用户名)

    // 用于生成随机数的生成器
    std::mt19937 generator;

    // 用户名种子的最小最大值(最小最大值,运营商注册的账号有数字递增的特点,因此根据已知的两个有效用户名顺序生成中间用户名是一个简洁有效的方法)
    uint64_t minNum, maxNum;

    // 用户名种子(允许多个种子, 在使用多种子模式时程序会以每一个种子为中心点, 并在每个中心点的附近生成新用户名);
    std::set<uint64_t> usernamesSeed;

    //使用顺序生成时记录当前用户名值
    uint64_t currentUsername;

    // 用户名生成数量
    uint32_t amount;

    // 最大无效密码尝试次数(表示一个阈值：程序将不断尝试字典中的密码，同时记录每次尝试的有效性。若某个用户名尝试的密码次数了超过此阈值次数都还没有找到有效密码，则程序将停止向该用户名尝试密码。)
    uint16_t maxInvalidAttempts;

    // 用户名前缀(取决于运营商)
    std::string usernamePrefix;

public:
    // 构造函数，接收最小和最大数字作为用户名范围以及用户名前缀
    explicit UsernameGenerator(uint32_t _amount, uint16_t maxInvalidAttempts, std::set<uint64_t> usernamesSeed, std::string usernamePrefix) :
        amount(_amount), maxInvalidAttempts(maxInvalidAttempts), usernamesSeed(usernamesSeed), usernamePrefix(usernamePrefix) {
        // 找到usernamesSeed中的最小值
        minNum = *usernamesSeed.begin();
        // 找到usernamesSeed中的最大值
        maxNum = *usernamesSeed.rbegin();
        // 将当前用户名初始化为最大值;
        currentUsername = maxNum;

        // 使用当前时间作为种子初始化随机数生成器
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    /**
     * 生成用户名列表;
     * @param gen_mode 生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式; "m":多种子模式)
     * @param maxDecrement 在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
     * @return 返回用户名列表;
     */
    std::vector<std::string> generateUsernamesList(std::string gen_mode, uint64_t maxDecrement = 8) {
        // 判断用户名生成模式;
        if (gen_mode == "r") {
            for (uint32_t i = 0; i < amount; ++i) randomGenerate();// 使用随机模式生成"amount"个用户名;
        } else if (gen_mode == "s") {
            for (uint32_t i = 0; i < amount; ++i) sequenceGenerate();// 使用顺序模式生成"amount"个用户名;
        } else if (gen_mode == "ir") {
            for (uint32_t i = 0; i < amount; ++i) incrementRandomGenerate(maxDecrement);// 使用增量随机模式生成"amount"个用户名;
        } else if (gen_mode == "m") {
            multiSeedsGenerate();// 使用多种子模式生成近"amount"个用户名;
        } else {
            multiSeedsGenerate();// 默认使用多种子模式生成近"amount"个用户名;
        }

        // 过滤用户名列表
        filterUsernames();

        //将用户名列表从unordered_set(无序集合)数据类型复制到vector(向量)数据类型, 因为从vector用户名列表抽取用户名时速度会更快;
        copyUsernamesList();

        //返回用户名列表;
        return getUsernamesList();
    }

    // 获取生成的用户名列表;
    std::vector<std::string> getUsernamesList() { return _usernamesList; }

    // 从用户名列表抽取并返回一个不重复(多次调用不重复)的用户名, 用户名全部抽取完成后返回""空字符串;
    std::string extractUniqueUsernam() {
        // 如果向量为空则返回一个空字符串
        if (_usernamesList.empty()) return "";

        // 从向量的末尾抽取一个用户名，这样可以保证效率(比循环遍历抽取每个值的速度要快得多,用内存换运行速度)
        std::string extractedUsername = _usernamesList.back();
        _usernamesList.pop_back();      // 移除已抽取的字符串

        return extractedUsername;       // 返回抽取的字符串
    }

    // 向用户名列表插入用户名;
    bool insertUsernameList(std::string username) {
        // 尝试将用户名插入到用户名集合中
        if (usernamesList.insert(username).second) {
            return true;// 如果插入成功,返回true;
        } else {
            return false;// 如果插入失败(即用户名列表中已经存在此用户名),返回false;
        }
    }

    // 获取用户名列表向量的用户名个数;
    size_t usernamesListCount() { return _usernamesList.size(); }

public:
    // 打印调试信息;
    void printDebugInfo() {
        std::cout << "为了生成" << amount << "个用户名，"
            << "基于" << usernamesSeed.size() << "个种子，"
            << "需要的生成半径为：" << calculateRadius() << std::endl;

        std::cout << "UsernameSeeds: " << std::endl;
        // 遍历并输出生成的用户名种子
        int i = 0;
        for (const auto &seed : usernamesSeed) {
            std::cout << seed << std::endl;
        }

        std::cout << "Generated Usernames: " << std::endl;
        // 遍历并输出生成的用户名
        i = 0;
        for (const auto &username : _usernamesList) {
            std::cout << tl.removeSubstring(username, usernamePrefix) << std::endl;
        }

        system("pause");
    }

private:
    // [随机模式]: 在范围内生成一个新的随机用户名(随机用户名不会重复)
    std::string randomGenerate() {
        // 如果已生成的用户名数量达到最大可能数量，则抛出异常
        if (usernamesList.size() >= (maxNum - minNum - 1)) {
            throw std::runtime_error("Username space exhausted");
        }

        // 无限循环直到生成一个独一无二的用户名
        while (true) {
            // 在指定范围内生成一个随机数
            std::uniform_int_distribution<uint64_t> distribution(minNum + 1, maxNum - 1);
            std::string newUsername = usernamePrefix + std::to_string((distribution(generator)));

            // 尝试将新用户名加上前缀插入到用户名集合中
            if (usernamesList.insert(newUsername).second) {
                // 如果插入成功（即用户名是独一无二的），则返回该用户名
                return newUsername;
            }
        }
    }

    // [顺序模式]: 在范围内生成一个新的顺序用户名(默认从最大值递减值最小值)
    std::string sequenceGenerate() {
        // 应用递减值(同时确保currentUsername不会低于minNum)
        currentUsername > minNum ? --currentUsername : currentUsername = minNum;

        // 将新用户名加上前缀插入到用户名集合中
        usernamesList.insert(usernamePrefix + std::to_string(currentUsername)).second;

        return usernamePrefix + std::to_string(currentUsername);
    }

    // [增量随机模式]: 在范围内生成一个新的增量随机用户名(数值从maxNum开始，每次调用时都会以一个随机的递减值减少，直到达到minNum。递减值的最大值将通过一个参数`maxDecrement`传递给函数。)
    std::string incrementRandomGenerate(uint64_t maxDecrement) {
        if (currentUsername <= minNum) {
            throw std::runtime_error("All usernames have been generated");
        }

        // 生成一个介于1到maxDecrement之间的随机递减值
        std::uniform_int_distribution<uint64_t> decrementDistribution(1, maxDecrement);
        uint64_t decrement = decrementDistribution(generator);

        // 应用递减值(同时确保currentUsername不会低于minNum)
        currentUsername > minNum + decrement ? currentUsername -= decrement : currentUsername = minNum;

        // 将新用户名加上前缀插入到用户名集合中
        usernamesList.insert(usernamePrefix + std::to_string(currentUsername)).second;

        // 将currentUsername转换为字符串并加上前缀返回
        return usernamePrefix + std::to_string(currentUsername);
    }

    /**
     * [多种子模式]
     * 生成围绕给定种子用户名的用户名集。
     *
     * 本函数接受两个参数：一个包含用户名种子的向量和一个生成半径。基于每个种子用户名，
     * 该函数在考虑避免重叠和重复的情况下，生成一个数值范围内的用户名集合。生成的用户名将
     * 在每个种子的数值前后，根据大小顺序并平等考虑所有种子生成。
     *
     * @param usernamesSeed 用户名种子的向量。
     * @param radius 指定以usernamesSeed中的用户名为中心，生成新用户名的数值半径。
     * @return 返回生成的用户名的向量。
     */
    void multiSeedsGenerate() {
        // 将有效用户名读取到用户名种子中(基于上一次运行的结果优化种子集合)
        for (const auto &accountPair : dic.readEffectiveAccount()) {
            // 删除有效用户名的前缀
            std::string seed = tl.removeSubstring(accountPair.first, usernamePrefix);

            // 将有效用户名从字符串转换为数字后插入到用户名种子集合中;
            usernamesSeed.insert(std::stoll(seed));
        }

        // 计算达到预定用户名数量所需的生成半径;
        uint32_t radius = calculateRadius();

        // 遍历种子用户名列表
        for (const auto &seed : usernamesSeed) {
            // 在指定的数值半径内生成新用户名
            for (uint64_t i = max(0LL, seed - radius); i <= seed + radius; ++i) {
                // 直接将生成的用户名加上前缀插入到用户名列表中
                // unordered_set会自动处理潜在的重复插入，保证用户名的唯一性
                usernamesList.insert(usernamePrefix + std::to_string(i));
            }
        }
    }

    // 复制用户名列表: 将用户名列表从unordered_set(无序集合)数据类型复制到vector(向量)数据类型, 因为从vector用户名列表抽取用户名时速度会更快;
    void copyUsernamesList() {
        // 预先分配足够的空间，以避免在复制过程中的多次内存分配
        _usernamesList.reserve(usernamesList.size());

        // 从usernamesList(数据类型为:unordered_set)拷贝到_usernamesList(数据类型为:vector)
        for (const auto &username : usernamesList) _usernamesList.push_back(username);

        // 对用户名从小到大进行排序
        tl.sortNumericStrings(_usernamesList);
    }

    /**
     * @brief 从生成的用户名列表中过滤掉已验证的有效用户名和尝试无效密码次数过多的用户名
     *
     * 该函数的主要目的是从一个预生成的用户名列表中移除那些已经在有效账户文件 (EffectiveAccount.csv) 中验证过，
     * 以及尝试无效密码次数超过设定阈值（最大无效密码尝试次数）的用户名。这样做可以提高后续操作或测试的效率。
     *
     * 实现方式如下：
     * 1. 首先，通过遍历有效账户映射（effectiveAccount），该映射包含用户名到密码的映射关系，
     *    并检查预生成的用户名列表（usernamesList，一个unordered_set）中是否存在这些用户名。
     *    如果存在，就从usernamesList中移除这些用户名。这一步骤确保了所有已验证的有效用户名被排除。
     * 2. 然后，遍历usernamesList中剩余的用户名，使用一个辅助函数（invalidPwdCount）来获取每个用户名尝试无效密码的次数，
     *    如果某个用户名的无效密码尝试次数超过了最大尝试次数（maxInvalidAttempts），则也将这个用户名从usernamesList中移除。
     *    这一步骤确保了尝试无效密码次数过多的用户名被排除。
     *
     * 该过程充分利用了unordered_set和unordered_map的高效查找能力，平均时间复杂度为O(1)，
     * 这保证了即使面对大量数据，整体操作的执行效率也非常高。
     */
    void filterUsernames() {
        // 遍历有效账户映射，移除usernamesList中已验证的有效用户名
        for (const auto &accountPair : dic.readEffectiveAccount()) {
            usernamesList.erase(accountPair.first); // 尝试移除用户名，如果不存在则无操作
        }

        // 创建一个临时集合来存储需要被移除的用户名，因为在遍历过程中直接修改集合可能会导致问题
        std::unordered_set<std::string> toRemove;

        // 遍历usernamesList，移除无效密码尝试次数过多的用户名
        for (const auto &username : usernamesList) {
            if (dic.invalidPwdCount(username) >= maxInvalidAttempts) {
                toRemove.insert(username); // 将超过尝试次数的用户名添加到toRemove集合中
            }
        }

        // 根据toRemove集合中的内容，从usernamesList中移除对应用户名
        for (const auto &username : toRemove) usernamesList.erase(username);
    }

    /**
     * 计算达到预定用户名数量所需的生成半径。
     * @param amount 预期生成的用户名总数。
     * @param seedCount 种子用户名的数量。
     * @return 计算出的生成半径。
     */
    uint32_t calculateRadius() {
        // 统计种子数;
        size_t seedCount = usernamesSeed.size();

        // 检查种子数量，防止除以0的情况
        if (seedCount <= 0) {
            std::cerr << "种子数量必须大于0。" << std::endl;
            return 0;
        }

        // 计算生成半径，确保结果为正整数
        // 使用ceil来确保总数能覆盖预期的用户名总数
        // 解方程n * (2r + 1) = amount，得到r
        uint32_t radius = static_cast<uint32_t>(std::ceil((amount / static_cast<double>(seedCount) - 1) / 2));

        return radius;
    }
};