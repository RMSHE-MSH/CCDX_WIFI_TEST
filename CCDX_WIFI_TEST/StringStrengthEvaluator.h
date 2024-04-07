#pragma once

#include <string>
#include <vector>

/**
* [�ַ���ǿ������] ��������һ�������ǿ��, ���е÷�Խ��������Խ��ȫ;
*
* 1. �������ۣ��ַ���������������ĸ��Ӷ�ָ��֮һ��
* 2. �ַ������ԣ������ַ����в�ͬ�ַ������࣬����Сд��ĸ����д��ĸ�����֡������ַ��ȡ�
* 3. �ظ��Լ�飺����ַ�������ͬ�ַ����Ӵ��ظ����ֵ�Ƶ�ʣ�Ƶ���ظ����ܽ�������ĸ��Ӷȡ�
* 4. �صļ��㣺�ؿ��������ַ����Ĳ���Ԥ���Ժ�����ԡ�
*/
class StringStrengthEvaluator {
private:
    // ����������ֵ�Ȩ�أ����Ը������밲ȫ��Ҫ�����
    const double lengthWeight;            // ����Ȩ��
    const double diversityWeight;         // �ַ�������Ȩ��
    const double repetitivenessWeight;    // �ظ���Ȩ��
    const double entropyWeight;           // ��Ȩ��
public:
    // ���캯������ʼ���������ֵ�Ȩ�أ����Ը������밲ȫ��Ҫ�����
    StringStrengthEvaluator(const double lengthWeight = 1.5,
                            const double diversityWeight = 1.2,
                            const double repetitivenessWeight = 1.7,
                            const double entropyWeight = 1.3) :
        lengthWeight(lengthWeight),
        diversityWeight(diversityWeight),
        repetitivenessWeight(repetitivenessWeight),
        entropyWeight(entropyWeight) {}

    // �����ַ�����ǿ��
    double evaluateString(const std::string &str);

private:
    // �����ַ������ȼ���������ϳ����ַ����÷ָ���
    double evaluateLength(const std::string &str);
    // �����ַ������ԣ���ͬ�����ַ�����д��Сд�����֡������ַ���ʹ�ò�ͬȨ��
    double evaluateDiversity(const std::string &str);

    // ����KMP�㷨����Ĳ���ƥ���Partial Match Table��
    // ��������ڿ������ַ���������������ƥ��Ĳ���
    std::vector<int> buildPartialMatchTable(const std::string &pattern);

    // ����ַ������ظ�ģʽ�Ĵ��ڣ�ʹ�û������ںͼ򻯰��KMP�㷨
    double evaluateRepetitiveness(const std::string &str);

    // ��ȷ�����ַ�������
    double calculateEntropy(const std::string &str);
};

/*
int main() {
    std::string testString = "000000";
    double score = StringStrengthEvaluator::evaluateString(testString);
    std::cout << "�ۺϵ÷�: " << score << std::endl;
    return 0;
}
*/