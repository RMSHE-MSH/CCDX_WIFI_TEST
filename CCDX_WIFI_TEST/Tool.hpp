#pragma once
#include <chrono>
#include <algorithm>
#include <ranges>

class Tool {
public:
    // �԰���ȫ�����ַ�����vector��������
    void sortNumericStrings(std::vector<std::string> &vector) {
        // ʹ�÷�Χ���е�sort������������
        // �Զ���ȽϺ�����Ϊlambda���ʽ�����ַ���ת��Ϊ�����ͺ�Ƚ�
        std::ranges::sort(vector, [](const std::string &a, const std::string &b) {
            // ���ַ���ת��Ϊlong long���ͺ�Ƚ�
            return std::stoll(a) < std::stoll(b); });
    }

    static void displayInfo() {
        std::cout << "CCDX WIFI TEST - Beta.2024.03.20.Mark0" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/GearDesignGuide" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[��ʾ]\n1.�ҵ�����Ч�˻������ڳ����Ŀ¼��\"EffectiveAccount.csv\"�ļ���;\n2.�뽫�ֵ��ļ�\"Dictionary.csv\"�ŵ������Ŀ¼��;\n" << std::endl;

        system("pause");
    }
};