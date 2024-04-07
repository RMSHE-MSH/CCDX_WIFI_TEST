#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

class StringSimilarityEvaluator {
private:
    std::string str1, str2;

    // Ȩ�ض��壬���Ը���������е���
    const double weightCosine = 0.40; // �������ƶ�Ȩ��
    const double weightNGram = 0.50;  // N-gram���ƶ�Ȩ�أ���Ϊ��������������Ҫ
    const double weightLevenshtein = 0.30; // ����˹̹���루�༭���룩���ƶ�Ȩ��
public:
    StringSimilarityEvaluator(const std::string &str1, const std::string &str2);

    // �ۺ����������ַ������ƶ�
    double evaluateStringSimilarity();

    // ���������ȳ��ַ����ĺ�������: �����ȳ��ַ���֮���Ӧλ�õĲ�ͬ�ַ��ĸ���
    int hammingDistance();

    // ���������ַ���������˹̹���루Levenshtein Distance����Ҳ��Ϊ�༭���룬����ͨ�����롢ɾ�����滻������һ���ַ���ת������һ���ַ�������Ҫ�����ٲ�������
    int levenshteinDistance();

    /**
    * ���������ַ�����N-gram���ƶ�
    *
    * @param n N-gram�ĳ���
    * @return ���������ַ��������ƶ�
    */
    double nGramSimilarity(uint8_t n);

    // ���������ַ������������ƶ�(�������ƶȵķ�Χ��-1��1֮�䣬����1��ʾ��ȫ��ͬ��0��ʾ��ȫ����أ�-1��ʾ��ȫ�෴�����ı��������������У�ͨ������£����ƶȻ���0��1֮�䡣)
    double cosineSimilarity();
private:
    /**
     * �������������ǿ�����ֶ�̬ѡ��N-gram���ȡ�
     *
     * @return ѡ����N-gram���ȡ�
     */
    uint8_t selectNGramLengthBasedOnStrength();

    /**
    * ����һ���ַ�����N-gram����
    *
    * @param str Ҫ������ַ���
    * @param n N-gram�ĳ���
    * @return �����ַ�����N-gram����
    */
    std::unordered_set<std::string> generateNGrams(const std::string &str, uint8_t n);

    /**
    * ��������N-gram���ϵ����ƶ�
    *
    * @param set1 ��һ���ַ�����N-gram����
    * @param set2 �ڶ����ַ�����N-gram����
    * @return �����������ϵ����ƶ�
    */
    double calculateNGram(const std::unordered_set<std::string> &set1, const std::unordered_set<std::string> &set2);

    // �ı�Ԥ�������ַ���ת��ΪСд�������㵥��Ƶ��
    std::unordered_map<std::string, int> getWordFrequency(const std::string &input);

    // ����������ģ
    double calculateMagnitude(const std::unordered_map<std::string, int> &freq);
};

/*
int main() {
    // �����ַ���
    std::string str1 = "This is a sample text.";
    std::string str2 = "This text is a sample.566xNZ*J8XLGm";

    StringSimilarityEvaluator sim(str1, str2);

    int distance = sim.levenshteinDistance();
    std::cout << "����˹̹����Ϊ: " << distance << std::endl;

    std::cout << "��������: " << sim.hammingDistance() << std::endl;

    // ������
    std::cout << "N-gram ���ƶ�: " << sim.nGramSimilarity(1) << std::endl;

    std::cout << "�������ƶ�: " << sim.cosineSimilarity() << std::endl;

    return 0;
}
*/

/*
N-gram�ĳ������ı���������ƶȼ����а�������Ҫ�Ľ�ɫ�����Խ����Ӱ����ԴӼ�����������⣺

### 1. ��ȷ���뷺������
- **�϶̵�N-gram����1-gram��2-gram��**��
  - **��ȷ�Խϵ�**���̵�N-gram���ܻᲶ�񵽺ܶೣ���ġ����������ֶȵ�ģʽ�����²�ͬ�ı�֮������ֶȽ��͡�
  - **����������ǿ**�����ڲ�����ǽ�Ϊ�ձ��ģʽ����ʹ�������ڴ���ǳ���ͬ���ı�ʱ�ܹ��ҵ�һЩ�����������ԡ�

- **�ϳ���N-gram����4-gram���ϣ�**��
  - **��ȷ�Խϸ�**������N-gram�ܹ�������Ӿ���Ͷ��ص��ı����������������Ե�Ҫ����ߵĳ���������ϸ΢���ı��仯������Ч��
  - **������������**�����������ڸ������ַ����У������ܴ��һЩ�򳤶Ƚ϶̵�����ģʽ�������ķ������ᡣ

### 2. ���ܺͼ��㸴�Ӷ�
- **Nֵ������**��ζ��ÿ��N-gram����������ַ�����ֱ��Ӱ�쵽�洢�ͼ���N-gram���ϵ���Դ���ġ�����N-gram���������Ӽ���ĸ��ӶȺ��ڴ��ʹ�ã��������ڴ�������ı����ı�ʱ��
- ����N�����ӣ����ܴ��ڵ�N-gram���������ָ����������ᵼ������N-gram�Ĺ��̸��Ӻ�ʱ��������Ҫ������ڴ����洢��ЩN-gram��

### 3. �Խ����Ӱ��
- ��ĳЩӦ�ó����£�ѡ���ʵ���Nֵ��������Ӱ�����յ��������ܡ����磬���ı����ƶȼ��㡢����ģ�͡��ı�����������У��ʵ���Nֵ����ƽ��ģ�͵ķ��������Ͷ��ض�ģʽ�����жȣ��Ӷ��Ż������Ч����

### 4. Ӧ�ó���
- ��ͬ��Ӧ�ó���������Ҫ��ͬ���ȵ�N-gram�����磬����Ȼ���Դ����У�2-gram��˫��ĸ�飩��3-gram������ĸ�飩ͨ�����ڲ�׽�ʻ�֮��Ĺ����ԣ��������������ĳЩ�ض����ı����������У�������Ҫ������N-gram��ʶ�����ϸ�µ�ģʽ��ṹ��

����������ѡ��N-gram�ĳ�����һ�����ݾ�����������ݼ�����������Ȩ����ߣ�û��һ��Nֵ���������������ͨ����Ҫͨ��ʵ���������ȷ�����ʺ��ض�Ӧ�������Nֵ��
*/
