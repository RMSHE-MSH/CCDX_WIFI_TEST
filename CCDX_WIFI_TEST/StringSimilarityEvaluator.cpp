#include "StringSimilarityEvaluator.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "StringStrengthEvaluator.h"

StringSimilarityEvaluator::StringSimilarityEvaluator(const std::string &str1, const std::string &str2) :str1(str1), str2(str2) {}

// 综合评估两个字符串相似度
double StringSimilarityEvaluator::evaluateStringSimilarity() {
    // 计算原始相似度和距离
    double cosine = cosineSimilarity(); // 余弦相似度
    double nGram = nGramSimilarity(selectNGramLengthBasedOnStrength());  // 计算N-gram相似度(基于密码强度动态选择N-gram长度)
    int levenshtein = levenshteinDistance(); // 莱文斯坦距离

    // 将莱文斯坦距离转换为相似度得分
    // 这里距离越小，相似度越高
    double levenshteinSimilarity = 1.0 / (1.0 + levenshtein);

    // 将余弦相似度转换为[0, 1]范围的得分，以便与其他得分统一处理
    double cosineScore = (cosine + 1) / 2;

    // 计算综合评估得分
    // 使用加权平均方法，将不同相似度和距离度量综合在一起
    double totalScore = weightCosine * cosineScore
        + weightNGram * nGram
        + weightLevenshtein * levenshteinSimilarity;

    return totalScore;
}

// 计算两个等长字符串的汉明距离: 两个等长字符串之间对应位置的不同字符的个数
int StringSimilarityEvaluator::hammingDistance() {
    if (str1.size() != str2.size()) {
        // 如果字符串长度不相等，抛出无效参数异常
        return -1;
    }

    // 使用std::mismatch算法找到不匹配的元素对，并计算它们的数量
    // 这是一种更高效的方法，避免了手动遍历字符串并比较每个字符
    auto pair = std::mismatch(str1.begin(), str1.end(), str2.begin());
    int distance = 0;
    while (pair.first != str1.end()) {
        ++distance; // 对不匹配的字符计数
        // 移动到下一个不匹配的字符
        pair = std::mismatch(pair.first + 1, str1.end(), pair.second + 1);
    }

    return distance;
}

// 计算两个字符串的莱文斯坦距离（Levenshtein Distance）：也称为编辑距离，衡量通过插入、删除或替换操作将一个字符串转换成另一个字符串所需要的最少操作数。
int StringSimilarityEvaluator::levenshteinDistance() {
    // 确保str1是较短的字符串，减小空间复杂度
    if (str1.size() > str2.size()) {
        // 交换str1与str2的值;
        std::swap(str1, str2);
    }

    size_t m = str1.size();
    size_t n = str2.size();
    std::vector<int> dp(n + 1, 0);

    // 初始化dp数组，对应于将str1转换为空字符串的操作
    for (int j = 0; j <= n; j++) {
        dp[j] = j;
    }

    // 用于存储dp[j-1]（左上角的值）在更新前的值
    int prev, temp;

    // 动态规划计算莱文斯坦距离
    for (int i = 1; i <= m; i++) {
        prev = dp[0]; // 存储dp[0]，即当前行的第一个元素，在迭代前的值
        dp[0] = i;    // 更新dp[0]，对应于将空字符串转换为str1.substr(0, i)
        for (int j = 1; j <= n; j++) {
            temp = dp[j]; // 临时存储当前dp[j]，稍后用于计算下一列
            // 如果当前字符相同，则不需要操作；否则，选择插入、删除、替换中的最小操作次数
            dp[j] = (str1[i - 1] == str2[j - 1]) ? prev : std::min({ dp[j - 1], dp[j], prev }) + 1;
            prev = temp; // 更新prev为当前列的原始值，为计算下一列做准备
        }
    }

    // dp[n]存储的是将s1完全转换为str2的最小操作次数
    return dp[n];
}

/**
* 计算两个字符串的N-gram相似度
*
* @param n N-gram的长度
* @return 返回两个字符串的相似度
*/
double StringSimilarityEvaluator::nGramSimilarity(uint8_t n) {
    // 生成N-gram集合
    auto nGrams1 = generateNGrams(str1, n);
    auto nGrams2 = generateNGrams(str2, n);

    // 计算相似度
    return calculateNGram(nGrams1, nGrams2);
}

// 计算两个字符串的余弦相似度(余弦相似度的范围在-1到1之间，其中1表示完全相同，0表示完全不相关，-1表示完全相反。在文本分析的上下文中，通常情况下，相似度会在0到1之间。)
double StringSimilarityEvaluator::cosineSimilarity() {
    auto freq1 = getWordFrequency(str1);
    auto freq2 = getWordFrequency(str2);

    // 计算点积
    double dotProduct = 0.0;
    for (const auto &word : freq1) {
        auto it = freq2.find(word.first);
        if (it != freq2.end()) {
            dotProduct += word.second * it->second;
        }
    }

    // 计算模并应用余弦相似度公式
    double magnitude1 = calculateMagnitude(freq1);
    double magnitude2 = calculateMagnitude(freq2);

    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0; // 防止除以零
    }
    return dotProduct / (magnitude1 * magnitude2);
}

/**
 * 根据两个密码的强度评分动态选择N-gram长度。
 *
 * @return 选定的N-gram长度。
 */
uint8_t StringSimilarityEvaluator::selectNGramLengthBasedOnStrength() {
    StringStrengthEvaluator Strength(8);

    // 计算两个密码的强度评分并求平均值
    double avgScore = (Strength.evaluateString(str1) + Strength.evaluateString(str2)) / 2.0;

    // 通过细化的阈值区间来决定N-gram的长度，以更准确地反映密码的实际安全性
    if (avgScore < 50) {
        // 如果密码强度评分非常低，意味着密码复杂度较低，使用1-gram可以捕获基本的重复模式
        return 1;
    } else if (avgScore < 80) {
        // 对于中等强度的密码，使用2-gram，因为它能够更好地分析密码中的局部结构
        return 2;
    } else if (avgScore < 120) {
        // 高强度的密码使用3-gram，这样可以分析更复杂的模式和结构
        return 3;
    } else {
        // 极高强度的密码选择4-gram，假设这样的密码包含非常复杂的结构，需要更细致的分析
        return 4;
    }
}

/**
* 生成一个字符串的N-gram集合
*
* @param str 要处理的字符串
* @param n N-gram的长度
* @return 返回字符串的N-gram集合
*/
std::unordered_set<std::string> StringSimilarityEvaluator::generateNGrams(const std::string &str, uint8_t n) {
    std::unordered_set<std::string> nGrams;
    // 遍历字符串，提取所有长度为n的子串
    for (size_t i = 0; i + n <= str.size(); ++i) {
        nGrams.insert(str.substr(i, n));
    }
    return nGrams;
}

/**
* 计算两个N-gram集合的相似度
*
* @param set1 第一个字符串的N-gram集合
* @param set2 第二个字符串的N-gram集合
* @return 返回两个集合的相似度
*/
double StringSimilarityEvaluator::calculateNGram(const std::unordered_set<std::string> &set1, const std::unordered_set<std::string> &set2) {
    std::unordered_set<std::string> intersection;

    // 计算交集
    std::for_each(set1.begin(), set1.end(), [&](const std::string &s) {
        if (set2.find(s) != set2.end()) {
            intersection.insert(s);
        }
                  });

    // 计算并集的大小，这里并集大小等于两个集合的大小减去交集的大小
    size_t unionSize = set1.size() + set2.size() - intersection.size();

    // 计算并返回相似度
    return static_cast<double>(intersection.size()) / unionSize;
}

// 文本预处理：将字符串转换为小写，并计算单词频率
std::unordered_map<std::string, int> StringSimilarityEvaluator::getWordFrequency(const std::string &input) {
    std::unordered_map<std::string, int> freq;
    std::string word;
    for (auto ch : input) {
        if (std::isalpha(ch)) {
            // 转换为小写并构建单词
            word += std::tolower(ch);
        } else if (!word.empty()) {
            // 单词结束，增加计数
            ++freq[word];
            word.clear(); // 重置单词
        }
    }
    if (!word.empty()) {
        ++freq[word]; // 处理最后一个单词
    }
    return freq;
}

// 计算向量的模
double StringSimilarityEvaluator::calculateMagnitude(const std::unordered_map<std::string, int> &freq) {
    double magnitude = 0.0;
    for (const auto &item : freq) {
        magnitude += item.second * item.second;
    }
    return std::sqrt(magnitude);
}