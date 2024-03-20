/**
 * @file Tool.hpp
 * @date 20.03.2024
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2024 RMSHE. All rights reserved.
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 *
 * Electronic Mail : asdfghjkl851@outlook.com
 */

#pragma once
#include <chrono>
#include <algorithm>
#include <ranges>
#include "DictionaryIndexer.hpp"

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

    // ��ʾ�汾��Ϣ
    static void displayInfo() {
        std::cout << "CCDX WIFI TEST - Beta.2024.03.20.Mark0" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/CCDX_WIFI_TEST" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[��ʾ]\n1.�ҵ�����Ч�˻������ڳ����Ŀ¼��\"EffectiveAccount.csv\"�ļ���;\n2.�뽫�ֵ��ļ�\"Dictionary.csv\"�ŵ������Ŀ¼��;\n" << std::endl;

        system("pause");
    }

    // ��ʾ�ֵ�����
    void displayDictionary() {
        system("CLS");
        DictionaryIndexer dic;

        uint64_t num = 0;
        for (auto &i : dic.readDictionary("./Dictionary.csv")) std::cout << ++num << "\t" << i << std::endl;

        std::cout << "\n�����Ƕ�ȡ�����ֵ��ļ�����.\n" << std::endl;
        system("pause");
    }

    // �����ʱ: ������ʱ���ƣ�����������������С��ʱʱ��������ʱʱ�䣨��λ�����룩
    void randomDelay(uint16_t minDelayMs, uint16_t maxDelayMs) {
        // �����С�������ʱֵ�Ƿ����
        if (minDelayMs > maxDelayMs) {
            std::cerr << "��С��ʱֵ���ܴ��������ʱֵ��" << std::endl;
            return;
        }

        // �����������������ʹ������豸���ṩ����
        std::random_device rd;
        std::mt19937 gen(rd());
        // ����һ����Χ��minDelayMs��maxDelayMs֮��ľ��ȷֲ�
        std::uniform_int_distribution<> distrib(minDelayMs, maxDelayMs);

        // ����һ�������ʱֵ
        uint16_t delay = distrib(gen);

        // ʵ����ʱ
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
};