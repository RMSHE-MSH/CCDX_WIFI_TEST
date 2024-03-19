#pragma once
#include <unordered_set>
#include <random>
#include <string>
#include <stdexcept>
#include "Tool.hpp"

Tool tl;

class UsernameGenerator {
private:
    // 存储已生成的用户名列表
    std::unordered_set<std::string> usernamesList; // (unordered_set可以保证用户名不重复)
    std::vector<std::string> _usernamesList;       // (vector可以更高效的抽取用户名)

    // 用于生成随机数的生成器
    std::mt19937 generator;

    // 用户名数字的最小最大值
    uint64_t minNum, maxNum;

    //使用顺序生成时记录当前用户名值
    uint64_t currentUsername;

    // 用户名生成数量
    uint16_t amount;

public:
    // 构造函数，接收最小和最大数字作为用户名范围
    explicit UsernameGenerator(uint16_t _amount = 1000, uint64_t min = 8143086109, uint64_t max = 9390026092) : amount(_amount), minNum(min), maxNum(max), currentUsername(max) {
        // 使用当前时间作为种子初始化随机数生成器
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    /**
     * 生成用户名列表;
     * @param gen_mode 生成模式("r":随机模式; "s":顺序模式; "ir":增量随机模式;)
     * @param maxDecrement 在增量随机模式下时可用, 随机递减值的最大值通过这个参数传递
     * @return 返回用户名列表;
     */
    std::vector<std::string> generateUsernamesList(std::string gen_mode, uint64_t maxDecrement = 8) {
        // 判断用户名生成模式;
        if (gen_mode == "r") {
            for (uint16_t i = 0; i < amount; ++i) randomGenerate();// 使用随机模式生成"amount"个用户名;
        } else if (gen_mode == "s") {
            for (uint16_t i = 0; i < amount; ++i) sequenceGenerate();// 使用顺序模式生成"amount"个用户名;
        } else if (gen_mode == "ir") {
            for (uint16_t i = 0; i < amount; ++i) incrementRandomGenerate(maxDecrement);// 使用增量随机模式生成"amount"个用户名;
        } else {
            for (uint16_t i = 0; i < amount; ++i) sequenceGenerate();// 默认使用顺序模式生成"amount"个用户名;
        }

        //将用户名列表从unordered_set(无序集合)数据类型复制到vector(向量)数据类型, 因为从vector用户名列表抽取密码时速度会更快;
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

private:
    // 随机模式: 在范围内生成一个新的随机用户名(随机用户名不会重复)
    std::string randomGenerate() {
        // 如果已生成的用户名数量达到最大可能数量，则抛出异常
        if (usernamesList.size() >= (maxNum - minNum - 1)) {
            throw std::runtime_error("Username space exhausted");
        }

        // 无限循环直到生成一个独一无二的用户名
        while (true) {
            // 在指定范围内生成一个随机数
            std::uniform_int_distribution<uint64_t> distribution(minNum + 1, maxNum - 1);
            std::string newUsername = std::to_string((distribution(generator)));

            // 尝试将新用户名插入到用户名集合中
            if (usernamesList.insert(newUsername).second) {
                // 如果插入成功（即用户名是独一无二的），则返回该用户名
                return newUsername;
            }
        }
    }

    // 顺序模式: 在范围内生成一个新的顺序用户名(默认从最大值递减值最小值)
    std::string sequenceGenerate() {
        // 应用递减值(同时确保currentUsername不会低于minNum)
        currentUsername > minNum ? --currentUsername : currentUsername = minNum;

        // 将新用户名插入到用户名集合中
        usernamesList.insert(std::to_string(currentUsername)).second;

        return std::to_string(currentUsername);
    }

    // 增量随机模式: 在范围内生成一个新的增量随机用户名(数值从maxNum开始，每次调用时都会以一个随机的递减值减少，直到达到minNum。递减值的最大值将通过一个参数`maxDecrement`传递给函数。)
    std::string incrementRandomGenerate(uint64_t maxDecrement) {
        if (currentUsername <= minNum) {
            throw std::runtime_error("All usernames have been generated");
        }

        // 生成一个介于1到maxDecrement之间的随机递减值
        std::uniform_int_distribution<uint64_t> decrementDistribution(1, maxDecrement);
        uint64_t decrement = decrementDistribution(generator);

        // 应用递减值(同时确保currentUsername不会低于minNum)
        currentUsername > minNum + decrement ? currentUsername -= decrement : currentUsername = minNum;

        // 将新用户名插入到用户名集合中
        usernamesList.insert(std::to_string(currentUsername)).second;

        // 将currentUsername转换为字符串并返回
        return std::to_string(currentUsername);
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
};