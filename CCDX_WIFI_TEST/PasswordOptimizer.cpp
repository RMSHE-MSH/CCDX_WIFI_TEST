#include "PasswordOptimizer.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "StringSimilarityEvaluator.h"
#include "StringStrengthEvaluator.h"
#include <cmath>
#include <cstdlib>

// ���캯����������Ч�˻����ݺ������ֵ���Ϊ����
PasswordOptimizer::PasswordOptimizer(std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts,
                                     std::unordered_set<std::string> passwordDictionary) :
    invalidAccounts(invalidAccounts),
    passwordDictionary(passwordDictionary) {
    // ͳ����Ч���뼰�����Ƶ�ʡ�
    invalidPasswordsCount();
}

// ͳ��invalidAccounts�е���Ч���뼰�����Ƶ�ʡ�
void PasswordOptimizer::invalidPasswordsCount() {
    // ʹ�÷�Χforѭ������ÿ���˻�������Ч���뼯��
    for (const auto &[user, passwords] : invalidAccounts) {
        // �ٴα���ÿ����Ч����
        for (const auto &password : passwords) {
            // ֱ��ʹ��[]����������ӻ������Ч�����Ƶ��
            // ����������ڣ�[]�����������һ����Ԫ�أ�����ʼ����ֵΪ0��Ȼ����������
            invalidPassword[password]++;
        }
    }
}

/**
 * ��������������ַ��������е���Ӧ�ȵ÷֡�
 *
 * password Ҫ�����������ַ�����
 * stringSet �ַ������ϣ������ַ����������Ƶ�ʡ�
 * @return �������Ӧ�ȵ÷֣��÷�Խ�߱�ʾ�����뼯���е��ַ���Խ������ǿ��Խ�ӽ���
 */
double PasswordOptimizer::calculateFitness(const std::string &password) {
    //for (auto &i : invalidPassword) {
    //    std::cout << i.first << ":" << i.second << std::endl;
    //}

    // ǿ��������ʵ�������������ַ���ǿ��
    StringStrengthEvaluator strengthEvaluator;

    // ��������ǿ��
    double password_strength = strengthEvaluator.evaluateString(password);

    // alphaֵ����ǿ�ȵ÷ֵ�˥���ٶȣ����Ը���ʵ���������(��ֵԽ���ǿ�Ƚӽ��ȵ�Ҫ���Խ�ϸ񣬷ǳ��ӽ�ʱ���ܵõ��߷�)
    const double alpha = 0.5;

    double totalScore = 0.0; // �ܵ÷�
    // double maxScore = 0.0;   // ���ں����÷ֹ淶������󵥸��÷�

    // �����ַ������ϣ�����ÿ���ַ����ĵ÷�
    for (const auto &[str, frequency] : invalidPassword) {
        // ���������뵱ǰ�ַ��������ƶȵ÷�
        StringSimilarityEvaluator similarity(password, str);
        double similarityScore = similarity.evaluateStringSimilarity();

        // ���㵱ǰ�ַ���ǿ�ȣ�������������ǿ�ȵĲ���
        double str_strength = strengthEvaluator.evaluateString(str);
        double difference = std::abs(password_strength - str_strength);

        // ʹ��ָ��˥����������ǿ�ȵ÷֣�����ǿ�Ȳ����С���ַ����õ��ϸߵķ���
        double strengthScore = std::exp(-alpha * difference);

        // �ۺϿ������ƶȵ÷֡�ǿ�ȵ÷ֺ�Ƶ�ʣ����㵱ǰ�ַ��������յ÷�
        double score = (similarityScore + strengthScore) * frequency;
        totalScore += score;
        // �������÷֣����ں����淶��
        // maxScore = std::max(maxScore, score);
    }

    // �淶���ܵ÷֣�ȷ���÷���һ������ķ�Χ��
    //if (maxScore > 0) {
    //    totalScore /= maxScore;
    //}

    return totalScore;
}