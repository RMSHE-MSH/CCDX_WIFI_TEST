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

// �ۺ����������ַ������ƶ�
double StringSimilarityEvaluator::evaluateStringSimilarity() {
    // ����ԭʼ���ƶȺ;���
    double cosine = cosineSimilarity(); // �������ƶ�
    double nGram = nGramSimilarity(selectNGramLengthBasedOnStrength());  // ����N-gram���ƶ�(��������ǿ�ȶ�̬ѡ��N-gram����)
    int levenshtein = levenshteinDistance(); // ����˹̹����

    // ������˹̹����ת��Ϊ���ƶȵ÷�
    // �������ԽС�����ƶ�Խ��
    double levenshteinSimilarity = 1.0 / (1.0 + levenshtein);

    // ���������ƶ�ת��Ϊ[0, 1]��Χ�ĵ÷֣��Ա��������÷�ͳһ����
    double cosineScore = (cosine + 1) / 2;

    // �����ۺ������÷�
    // ʹ�ü�Ȩƽ������������ͬ���ƶȺ;�������ۺ���һ��
    double totalScore = weightCosine * cosineScore
        + weightNGram * nGram
        + weightLevenshtein * levenshteinSimilarity;

    return totalScore;
}

// ���������ȳ��ַ����ĺ�������: �����ȳ��ַ���֮���Ӧλ�õĲ�ͬ�ַ��ĸ���
int StringSimilarityEvaluator::hammingDistance() {
    if (str1.size() != str2.size()) {
        // ����ַ������Ȳ���ȣ��׳���Ч�����쳣
        return -1;
    }

    // ʹ��std::mismatch�㷨�ҵ���ƥ���Ԫ�ضԣ����������ǵ�����
    // ����һ�ָ���Ч�ķ������������ֶ������ַ������Ƚ�ÿ���ַ�
    auto pair = std::mismatch(str1.begin(), str1.end(), str2.begin());
    int distance = 0;
    while (pair.first != str1.end()) {
        ++distance; // �Բ�ƥ����ַ�����
        // �ƶ�����һ����ƥ����ַ�
        pair = std::mismatch(pair.first + 1, str1.end(), pair.second + 1);
    }

    return distance;
}

// ���������ַ���������˹̹���루Levenshtein Distance����Ҳ��Ϊ�༭���룬����ͨ�����롢ɾ�����滻������һ���ַ���ת������һ���ַ�������Ҫ�����ٲ�������
int StringSimilarityEvaluator::levenshteinDistance() {
    // ȷ��str1�ǽ϶̵��ַ�������С�ռ临�Ӷ�
    if (str1.size() > str2.size()) {
        // ����str1��str2��ֵ;
        std::swap(str1, str2);
    }

    size_t m = str1.size();
    size_t n = str2.size();
    std::vector<int> dp(n + 1, 0);

    // ��ʼ��dp���飬��Ӧ�ڽ�str1ת��Ϊ���ַ����Ĳ���
    for (int j = 0; j <= n; j++) {
        dp[j] = j;
    }

    // ���ڴ洢dp[j-1]�����Ͻǵ�ֵ���ڸ���ǰ��ֵ
    int prev, temp;

    // ��̬�滮��������˹̹����
    for (int i = 1; i <= m; i++) {
        prev = dp[0]; // �洢dp[0]������ǰ�еĵ�һ��Ԫ�أ��ڵ���ǰ��ֵ
        dp[0] = i;    // ����dp[0]����Ӧ�ڽ����ַ���ת��Ϊstr1.substr(0, i)
        for (int j = 1; j <= n; j++) {
            temp = dp[j]; // ��ʱ�洢��ǰdp[j]���Ժ����ڼ�����һ��
            // �����ǰ�ַ���ͬ������Ҫ����������ѡ����롢ɾ�����滻�е���С��������
            dp[j] = (str1[i - 1] == str2[j - 1]) ? prev : std::min({ dp[j - 1], dp[j], prev }) + 1;
            prev = temp; // ����prevΪ��ǰ�е�ԭʼֵ��Ϊ������һ����׼��
        }
    }

    // dp[n]�洢���ǽ�s1��ȫת��Ϊstr2����С��������
    return dp[n];
}

/**
* ���������ַ�����N-gram���ƶ�
*
* @param n N-gram�ĳ���
* @return ���������ַ��������ƶ�
*/
double StringSimilarityEvaluator::nGramSimilarity(uint8_t n) {
    // ����N-gram����
    auto nGrams1 = generateNGrams(str1, n);
    auto nGrams2 = generateNGrams(str2, n);

    // �������ƶ�
    return calculateNGram(nGrams1, nGrams2);
}

// ���������ַ������������ƶ�(�������ƶȵķ�Χ��-1��1֮�䣬����1��ʾ��ȫ��ͬ��0��ʾ��ȫ����أ�-1��ʾ��ȫ�෴�����ı��������������У�ͨ������£����ƶȻ���0��1֮�䡣)
double StringSimilarityEvaluator::cosineSimilarity() {
    auto freq1 = getWordFrequency(str1);
    auto freq2 = getWordFrequency(str2);

    // ������
    double dotProduct = 0.0;
    for (const auto &word : freq1) {
        auto it = freq2.find(word.first);
        if (it != freq2.end()) {
            dotProduct += word.second * it->second;
        }
    }

    // ����ģ��Ӧ���������ƶȹ�ʽ
    double magnitude1 = calculateMagnitude(freq1);
    double magnitude2 = calculateMagnitude(freq2);

    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0; // ��ֹ������
    }
    return dotProduct / (magnitude1 * magnitude2);
}

/**
 * �������������ǿ�����ֶ�̬ѡ��N-gram���ȡ�
 *
 * @return ѡ����N-gram���ȡ�
 */
uint8_t StringSimilarityEvaluator::selectNGramLengthBasedOnStrength() {
    StringStrengthEvaluator Strength(8);

    // �������������ǿ�����ֲ���ƽ��ֵ
    double avgScore = (Strength.evaluateString(str1) + Strength.evaluateString(str2)) / 2.0;

    // ͨ��ϸ������ֵ����������N-gram�ĳ��ȣ��Ը�׼ȷ�ط�ӳ�����ʵ�ʰ�ȫ��
    if (avgScore < 50) {
        // �������ǿ�����ַǳ��ͣ���ζ�����븴�ӶȽϵͣ�ʹ��1-gram���Բ���������ظ�ģʽ
        return 1;
    } else if (avgScore < 80) {
        // �����е�ǿ�ȵ����룬ʹ��2-gram����Ϊ���ܹ����õط��������еľֲ��ṹ
        return 2;
    } else if (avgScore < 120) {
        // ��ǿ�ȵ�����ʹ��3-gram���������Է��������ӵ�ģʽ�ͽṹ
        return 3;
    } else {
        // ����ǿ�ȵ�����ѡ��4-gram��������������������ǳ����ӵĽṹ����Ҫ��ϸ�µķ���
        return 4;
    }
}

/**
* ����һ���ַ�����N-gram����
*
* @param str Ҫ������ַ���
* @param n N-gram�ĳ���
* @return �����ַ�����N-gram����
*/
std::unordered_set<std::string> StringSimilarityEvaluator::generateNGrams(const std::string &str, uint8_t n) {
    std::unordered_set<std::string> nGrams;
    // �����ַ�������ȡ���г���Ϊn���Ӵ�
    for (size_t i = 0; i + n <= str.size(); ++i) {
        nGrams.insert(str.substr(i, n));
    }
    return nGrams;
}

/**
* ��������N-gram���ϵ����ƶ�
*
* @param set1 ��һ���ַ�����N-gram����
* @param set2 �ڶ����ַ�����N-gram����
* @return �����������ϵ����ƶ�
*/
double StringSimilarityEvaluator::calculateNGram(const std::unordered_set<std::string> &set1, const std::unordered_set<std::string> &set2) {
    std::unordered_set<std::string> intersection;

    // ���㽻��
    std::for_each(set1.begin(), set1.end(), [&](const std::string &s) {
        if (set2.find(s) != set2.end()) {
            intersection.insert(s);
        }
                  });

    // ���㲢���Ĵ�С�����ﲢ����С�����������ϵĴ�С��ȥ�����Ĵ�С
    size_t unionSize = set1.size() + set2.size() - intersection.size();

    // ���㲢�������ƶ�
    return static_cast<double>(intersection.size()) / unionSize;
}

// �ı�Ԥ�������ַ���ת��ΪСд�������㵥��Ƶ��
std::unordered_map<std::string, int> StringSimilarityEvaluator::getWordFrequency(const std::string &input) {
    std::unordered_map<std::string, int> freq;
    std::string word;
    for (auto ch : input) {
        if (std::isalpha(ch)) {
            // ת��ΪСд����������
            word += std::tolower(ch);
        } else if (!word.empty()) {
            // ���ʽ��������Ӽ���
            ++freq[word];
            word.clear(); // ���õ���
        }
    }
    if (!word.empty()) {
        ++freq[word]; // �������һ������
    }
    return freq;
}

// ����������ģ
double StringSimilarityEvaluator::calculateMagnitude(const std::unordered_map<std::string, int> &freq) {
    double magnitude = 0.0;
    for (const auto &item : freq) {
        magnitude += item.second * item.second;
    }
    return std::sqrt(magnitude);
}