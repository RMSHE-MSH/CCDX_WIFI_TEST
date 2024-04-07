#include "PasswordOptimizer.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "StringSimilarityEvaluator.h"
#include "StringStrengthEvaluator.h"
#include <cmath>
#include <cstdlib>

// 构造函数，接收无效账户数据和密码字典作为输入
PasswordOptimizer::PasswordOptimizer(std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts,
                                     std::unordered_set<std::string> passwordDictionary) :
    invalidAccounts(invalidAccounts),
    passwordDictionary(passwordDictionary) {
    // 统计无效密码及其出现频率。
    invalidPasswordsCount();
}

// 统计invalidAccounts中的无效密码及其出现频率。
void PasswordOptimizer::invalidPasswordsCount() {
    // 使用范围for循环遍历每个账户及其无效密码集合
    for (const auto &[user, passwords] : invalidAccounts) {
        // 再次遍历每个无效密码
        for (const auto &password : passwords) {
            // 直接使用[]操作符来添加或更新无效密码的频率
            // 如果键不存在，[]操作符会插入一个新元素，并初始化其值为0，然后立即递增
            invalidPassword[password]++;
        }
    }
}

/**
 * 计算给定密码在字符串集合中的适应度得分。
 *
 * password 要评估的密码字符串。
 * stringSet 字符串集合，包括字符串及其出现频率。
 * @return 密码的适应度得分，得分越高表示密码与集合中的字符串越相似且强度越接近。
 */
double PasswordOptimizer::calculateFitness(const std::string &password) {
    //for (auto &i : invalidPassword) {
    //    std::cout << i.first << ":" << i.second << std::endl;
    //}

    // 强度评估器实例，用于评估字符串强度
    StringStrengthEvaluator strengthEvaluator;

    // 计算密码强度
    double password_strength = strengthEvaluator.evaluateString(password);

    // alpha值控制强度得分的衰减速度，可以根据实际情况调整(数值越大对强度接近度的要求就越严格，非常接近时才能得到高分)
    const double alpha = 0.5;

    double totalScore = 0.0; // 总得分
    // double maxScore = 0.0;   // 用于后续得分规范化的最大单个得分

    // 遍历字符串集合，计算每个字符串的得分
    for (const auto &[str, frequency] : invalidPassword) {
        // 计算密码与当前字符串的相似度得分
        StringSimilarityEvaluator similarity(password, str);
        double similarityScore = similarity.evaluateStringSimilarity();

        // 计算当前字符串强度，并计算与密码强度的差异
        double str_strength = strengthEvaluator.evaluateString(str);
        double difference = std::abs(password_strength - str_strength);

        // 使用指数衰减函数计算强度得分，这样强度差异较小的字符串得到较高的分数
        double strengthScore = std::exp(-alpha * difference);

        // 综合考虑相似度得分、强度得分和频率，计算当前字符串的最终得分
        double score = (similarityScore + strengthScore) * frequency;
        totalScore += score;
        // 更新最大得分，用于后续规范化
        // maxScore = std::max(maxScore, score);
    }

    // 规范化总得分，确保得分在一个合理的范围内
    //if (maxScore > 0) {
    //    totalScore /= maxScore;
    //}

    return totalScore;
}