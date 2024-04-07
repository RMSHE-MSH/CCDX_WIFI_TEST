#include "StringStrengthEvaluator.h"

#include <cctype>
#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// 评价字符串的强度
double StringStrengthEvaluator::evaluateString(const std::string &str) {
    // 计算各项评分并乘以相应的权重
    double lengthScore = evaluateLength(str) * lengthWeight;// 长度评价
    double diversityScore = evaluateDiversity(str) * diversityWeight;// 字符多样性评价
    double repetitivenessScore = evaluateRepetitiveness(str) * repetitivenessWeight;// 重复性检查
    double entropyScore = calculateEntropy(str) * entropyWeight;// 熵计算

    // 计算综合评分
    return lengthScore + diversityScore - repetitivenessScore + entropyScore;
}

// 基于字符串长度计算分数，较长的字符串得分更高
double StringStrengthEvaluator::evaluateLength(const std::string &str) {
    const double idealLength = 8; // 理想长度
    return std::min(static_cast<double>(str.length()) / idealLength, 1.0) * 10;
}

// 评价字符多样性，不同类型字符（大写、小写、数字、特殊字符）使用不同权重
double StringStrengthEvaluator::evaluateDiversity(const std::string &str) {
    std::unordered_set<char> uniqueChars(str.begin(), str.end());
    double diversityScore = 0;
    for (char c : uniqueChars) {
        if (isupper(c)) diversityScore += 1.5; // 大写字母
        else if (islower(c)) diversityScore += 2.0; // 小写字母
        else if (isdigit(c)) diversityScore += 1.0; // 数字
        else diversityScore += 3.0; // 特殊字符
    }
    return diversityScore;
}

// 构建KMP算法所需的部分匹配表（Partial Match Table）
// 这个表用于快速在字符串搜索中跳过已匹配的部分
std::vector<int> StringStrengthEvaluator::buildPartialMatchTable(const std::string &pattern) {
    std::vector<int> lps(pattern.length(), 0);
    int len = 0; // 最长相等前后缀的长度
    int i = 1;
    while (i < pattern.length()) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// 检测字符串中重复模式的存在，使用滑动窗口和简化版的KMP算法
double StringStrengthEvaluator::evaluateRepetitiveness(const std::string &str) {
    if (str.length() < 2) return 0.0; // 如果字符串太短，重复性评价无意义

    double repetitivenessScore = 0.0;
    // 定义滑动窗口的最大大小和最小大小，窗口大小决定了我们检测的重复模式的长度
    int maxWindowSize = std::min(10, static_cast<int>(str.length() / 2));
    for (int windowSize = 2; windowSize <= maxWindowSize; ++windowSize) {
        for (size_t start = 0; start <= str.length() - windowSize; ++start) {
            std::string window = str.substr(start, windowSize);
            std::vector<int> lps = buildPartialMatchTable(window);
            int maxLpsValue = *std::max_element(lps.begin(), lps.end());
            // 利用最长相等前后缀的长度评价重复性，越长表示重复性越高
            if (maxLpsValue > 0) {
                repetitivenessScore += static_cast<double>(maxLpsValue) / windowSize;
            }
        }
    }
    return repetitivenessScore;
}

// 精确计算字符串的熵
double StringStrengthEvaluator::calculateEntropy(const std::string &str) {
    std::unordered_map<char, int> charCount;
    for (char c : str) {
        charCount[c]++;
    }
    double entropy = 0.0;
    for (auto &[c, count] : charCount) {
        double p = static_cast<double>(count) / str.length();
        entropy -= p * log2(p);
    }
    return entropy;
}