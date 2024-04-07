#include "StringStrengthEvaluator.h"

#include <cctype>
#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// �����ַ�����ǿ��
double StringStrengthEvaluator::evaluateString(const std::string &str) {
    // ����������ֲ�������Ӧ��Ȩ��
    double lengthScore = evaluateLength(str) * lengthWeight;// ��������
    double diversityScore = evaluateDiversity(str) * diversityWeight;// �ַ�����������
    double repetitivenessScore = evaluateRepetitiveness(str) * repetitivenessWeight;// �ظ��Լ��
    double entropyScore = calculateEntropy(str) * entropyWeight;// �ؼ���

    // �����ۺ�����
    return lengthScore + diversityScore - repetitivenessScore + entropyScore;
}

// �����ַ������ȼ���������ϳ����ַ����÷ָ���
double StringStrengthEvaluator::evaluateLength(const std::string &str) {
    const double idealLength = 8; // ���볤��
    return std::min(static_cast<double>(str.length()) / idealLength, 1.0) * 10;
}

// �����ַ������ԣ���ͬ�����ַ�����д��Сд�����֡������ַ���ʹ�ò�ͬȨ��
double StringStrengthEvaluator::evaluateDiversity(const std::string &str) {
    std::unordered_set<char> uniqueChars(str.begin(), str.end());
    double diversityScore = 0;
    for (char c : uniqueChars) {
        if (isupper(c)) diversityScore += 1.5; // ��д��ĸ
        else if (islower(c)) diversityScore += 2.0; // Сд��ĸ
        else if (isdigit(c)) diversityScore += 1.0; // ����
        else diversityScore += 3.0; // �����ַ�
    }
    return diversityScore;
}

// ����KMP�㷨����Ĳ���ƥ���Partial Match Table��
// ��������ڿ������ַ���������������ƥ��Ĳ���
std::vector<int> StringStrengthEvaluator::buildPartialMatchTable(const std::string &pattern) {
    std::vector<int> lps(pattern.length(), 0);
    int len = 0; // ����ǰ��׺�ĳ���
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

// ����ַ������ظ�ģʽ�Ĵ��ڣ�ʹ�û������ںͼ򻯰��KMP�㷨
double StringStrengthEvaluator::evaluateRepetitiveness(const std::string &str) {
    if (str.length() < 2) return 0.0; // ����ַ���̫�̣��ظ�������������

    double repetitivenessScore = 0.0;
    // ���廬�����ڵ�����С����С��С�����ڴ�С���������Ǽ����ظ�ģʽ�ĳ���
    int maxWindowSize = std::min(10, static_cast<int>(str.length() / 2));
    for (int windowSize = 2; windowSize <= maxWindowSize; ++windowSize) {
        for (size_t start = 0; start <= str.length() - windowSize; ++start) {
            std::string window = str.substr(start, windowSize);
            std::vector<int> lps = buildPartialMatchTable(window);
            int maxLpsValue = *std::max_element(lps.begin(), lps.end());
            // ��������ǰ��׺�ĳ��������ظ��ԣ�Խ����ʾ�ظ���Խ��
            if (maxLpsValue > 0) {
                repetitivenessScore += static_cast<double>(maxLpsValue) / windowSize;
            }
        }
    }
    return repetitivenessScore;
}

// ��ȷ�����ַ�������
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