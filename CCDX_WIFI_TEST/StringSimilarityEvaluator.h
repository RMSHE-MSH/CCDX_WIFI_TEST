#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

class StringSimilarityEvaluator {
private:
    std::string str1, str2;

    // 权重定义，可以根据需求进行调整
    const double weightCosine = 0.40; // 余弦相似度权重
    const double weightNGram = 0.50;  // N-gram相似度权重，因为对密码评估很重要
    const double weightLevenshtein = 0.30; // 莱文斯坦距离（编辑距离）相似度权重
public:
    StringSimilarityEvaluator(const std::string &str1, const std::string &str2);

    // 综合评估两个字符串相似度
    double evaluateStringSimilarity();

    // 计算两个等长字符串的汉明距离: 两个等长字符串之间对应位置的不同字符的个数
    int hammingDistance();

    // 计算两个字符串的莱文斯坦距离（Levenshtein Distance）：也称为编辑距离，衡量通过插入、删除或替换操作将一个字符串转换成另一个字符串所需要的最少操作数。
    int levenshteinDistance();

    /**
    * 计算两个字符串的N-gram相似度
    *
    * @param n N-gram的长度
    * @return 返回两个字符串的相似度
    */
    double nGramSimilarity(uint8_t n);

    // 计算两个字符串的余弦相似度(余弦相似度的范围在-1到1之间，其中1表示完全相同，0表示完全不相关，-1表示完全相反。在文本分析的上下文中，通常情况下，相似度会在0到1之间。)
    double cosineSimilarity();
private:
    /**
     * 根据两个密码的强度评分动态选择N-gram长度。
     *
     * @return 选定的N-gram长度。
     */
    uint8_t selectNGramLengthBasedOnStrength();

    /**
    * 生成一个字符串的N-gram集合
    *
    * @param str 要处理的字符串
    * @param n N-gram的长度
    * @return 返回字符串的N-gram集合
    */
    std::unordered_set<std::string> generateNGrams(const std::string &str, uint8_t n);

    /**
    * 计算两个N-gram集合的相似度
    *
    * @param set1 第一个字符串的N-gram集合
    * @param set2 第二个字符串的N-gram集合
    * @return 返回两个集合的相似度
    */
    double calculateNGram(const std::unordered_set<std::string> &set1, const std::unordered_set<std::string> &set2);

    // 文本预处理：将字符串转换为小写，并计算单词频率
    std::unordered_map<std::string, int> getWordFrequency(const std::string &input);

    // 计算向量的模
    double calculateMagnitude(const std::unordered_map<std::string, int> &freq);
};

/*
int main() {
    // 测试字符串
    std::string str1 = "This is a sample text.";
    std::string str2 = "This text is a sample.566xNZ*J8XLGm";

    StringSimilarityEvaluator sim(str1, str2);

    int distance = sim.levenshteinDistance();
    std::cout << "莱文斯坦距离为: " << distance << std::endl;

    std::cout << "汉明距离: " << sim.hammingDistance() << std::endl;

    // 输出结果
    std::cout << "N-gram 相似度: " << sim.nGramSimilarity(1) << std::endl;

    std::cout << "余弦相似度: " << sim.cosineSimilarity() << std::endl;

    return 0;
}
*/

/*
N-gram的长度在文本处理和相似度计算中扮演着重要的角色，它对结果的影响可以从几个方面来理解：

### 1. 精确性与泛化能力
- **较短的N-gram（如1-gram或2-gram）**：
  - **精确性较低**：短的N-gram可能会捕获到很多常见的、不具有区分度的模式，导致不同文本之间的区分度降低。
  - **泛化能力较强**：由于捕获的是较为普遍的模式，这使得它们在处理非常不同的文本时能够找到一些基本的相似性。

- **较长的N-gram（如4-gram以上）**：
  - **精确性较高**：长的N-gram能够捕获更加具体和独特的文本特征，对于区分性的要求更高的场景（如检测细微的文本变化）更有效。
  - **泛化能力较弱**：由于依赖于更长的字符序列，它可能错过一些因长度较短的相似模式而带来的泛化机会。

### 2. 性能和计算复杂度
- **N值的增加**意味着每个N-gram包含更多的字符，这直接影响到存储和计算N-gram集合的资源消耗。长的N-gram会显著增加计算的复杂度和内存的使用，尤其是在处理大量文本或长文本时。
- 随着N的增加，可能存在的N-gram组合数量呈指数增长，这会导致生成N-gram的过程更加耗时，并且需要更多的内存来存储这些N-gram。

### 3. 对结果的影响
- 在某些应用场景下，选择适当的N值可以显著影响最终的任务性能。例如，在文本相似度计算、语言模型、文本分类等任务中，适当的N值可以平衡模型的泛化能力和对特定模式的敏感度，从而优化任务的效果。

### 4. 应用场景
- 不同的应用场景可能需要不同长度的N-gram。例如，在自然语言处理中，2-gram（双字母组）或3-gram（三字母组）通常用于捕捉词汇之间的关联性；而在密码分析或某些特定的文本分析任务中，可能需要更长的N-gram来识别更加细致的模式或结构。

综上所述，选择N-gram的长度是一个根据具体任务和数据集特性做出的权衡决策，没有一种N值适用于所有情况。通常需要通过实验和评估来确定最适合特定应用需求的N值。
*/
