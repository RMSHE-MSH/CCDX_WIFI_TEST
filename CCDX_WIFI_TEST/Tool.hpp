/**
 * @file Tool.hpp
 * @date 20.03.2024
 * @author RMSHE
 *
 * < CCDX_WIFI_TEST >
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
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <thread>

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
        std::cout << "CCDX WIFI TEST - Dev.2024.03.26.Mark6" << std::endl;
        std::cout << "[AuthorInfo] Powered by RMSHE" << std::endl;
        std::cout << "[OpenSource] https://github.com/RMSHE-MSH/CCDX_WIFI_TEST" << std::endl;
        std::cout << "[LICENSE] GNU AFFERO GENERAL PUBLIC LICENSE Version 3" << std::endl;

        std::cout << "\n[��ʾ]\n1.�ҵ�����Ч�˻������ڳ����Ŀ¼��\"EffectiveAccount.csv\"�ļ���;" <<
            "\n2.�뽫�ֵ��ļ�\"Dictionary.csv\"�ŵ������Ŀ¼��;" <<
            "\n3.��Ч�˻����ݴ����ڳ����Ŀ¼�� InvalidAccounts.csv �ļ���;"
            << std::endl;

        system("pause");
    }

    // �����ʱ: ������ʱ���ƣ�����������������С��ʱʱ��������ʱʱ�䣨��λ�����룩
    void randomDelay(uint16_t minDelayMs, uint16_t maxDelayMs) {
        // �����С�������ʱֵ�Ƿ����
        if (minDelayMs > maxDelayMs || maxDelayMs == 0) return;

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

    /**
     * @brief ɾ���ַ����е�һ�������Ӵ���
     *
     * @param original ԭʼ�ַ�����
     * @param toRemove Ҫɾ�����Ӵ���
     * @return std::string ɾ��ָ���Ӵ�����ַ�����
     */
    std::string removeSubstring(const std::string &original, const std::string &toRemove) {
        // ��ԭʼ�ַ�����Ҫɾ�����Ӵ�Ϊ�գ���ֱ�ӷ���ԭʼ�ַ���
        if (original.empty() || toRemove.empty()) {
            return original;
        }

        std::string result = original; // ����ԭʼ�ַ����ĸ���
        size_t pos = result.find(toRemove); // �����Ӵ�����ʼλ��

        // ����ҵ��Ӵ�����ɾ��֮
        if (pos != std::string::npos) {
            result.erase(pos, toRemove.length());
        }

        return result;
    }

    // ����һ��������ȵ�����ַ����ĺ���
    // @param minLength �ַ�������С����
    // @param maxLength �ַ�������󳤶�
    // @return ���ɵ�����ַ���
    std::string generateRandomString(int minLength, int maxLength) {
        // ��ʼ�������������
        std::random_device rd;
        std::mt19937 gen(rd());

        // ѡ��һ��������ȣ�λ��ָ������С����󳤶�֮��
        std::uniform_int_distribution<> lengthDistr(minLength, maxLength);
        int stringLength = lengthDistr(gen);

        // ������ܵ��ַ����ϣ�������Сд��ĸ�����ֺ������ַ�
        std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";

        // ��������ַ���
        std::string randomString;
        randomString.reserve(stringLength); // ��ǰΪ�ַ��������㹻�Ŀռ䣬���Ч��
        std::uniform_int_distribution<> charDistr(0, static_cast<int>(chars.length() - 1));
        for (int i = 0; i < stringLength; ++i) {
            randomString += chars[charDistr(gen)];
        }

        return randomString;
    }
};