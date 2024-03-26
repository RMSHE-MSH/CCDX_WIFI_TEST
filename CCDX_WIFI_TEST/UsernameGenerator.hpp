/**
 * @file UsernameGenerator.hpp
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
#include <unordered_set>
#include <random>
#include <string>
#include <stdexcept>
#include "Tool.hpp"

Tool tl;
DictionaryIndexer dic;

class UsernameGenerator {
private:
    // �洢�����ɵ��û����б�
    std::unordered_set<std::string> usernamesList; // (unordered_set���Ա�֤�û������ظ�)
    std::vector<std::string> _usernamesList;       // (vector���Ը���Ч�ĳ�ȡ�û���)

    // ���������������������
    std::mt19937 generator;

    // �û������ӵ���С���ֵ(��С���ֵ,��Ӫ��ע����˺������ֵ������ص�,��˸�����֪��������Ч�û���˳�������м��û�����һ�������Ч�ķ���)
    uint64_t minNum, maxNum;

    // �û�������(����������, ��ʹ�ö�����ģʽʱ�������ÿһ������Ϊ���ĵ�, ����ÿ�����ĵ�ĸ����������û���);
    std::set<uint64_t> usernamesSeed;

    //ʹ��˳������ʱ��¼��ǰ�û���ֵ
    uint64_t currentUsername;

    // �û�����������
    uint32_t amount;

    // �û���ǰ׺(ȡ������Ӫ��)
    std::string usernamePrefix;

public:
    // ���캯����������С�����������Ϊ�û�����Χ�Լ��û���ǰ׺
    explicit UsernameGenerator(uint32_t _amount, std::set<uint64_t> usernamesSeed, std::string usernamePrefix) :
        amount(_amount), usernamesSeed(usernamesSeed), usernamePrefix(usernamePrefix) {
        // �ҵ�usernamesSeed�е���Сֵ
        minNum = *usernamesSeed.begin();
        // �ҵ�usernamesSeed�е����ֵ
        maxNum = *usernamesSeed.rbegin();
        // ����ǰ�û�����ʼ��Ϊ���ֵ;
        currentUsername = maxNum;

        // ʹ�õ�ǰʱ����Ϊ���ӳ�ʼ�������������
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    /**
     * �����û����б�;
     * @param gen_mode ����ģʽ("r":���ģʽ; "s":˳��ģʽ; "ir":�������ģʽ; "m":������ģʽ)
     * @param maxDecrement ���������ģʽ��ʱ����, ����ݼ�ֵ�����ֵͨ�������������
     * @return �����û����б�;
     */
    std::vector<std::string> generateUsernamesList(std::string gen_mode, uint64_t maxDecrement = 8) {
        // �ж��û�������ģʽ;
        if (gen_mode == "r") {
            for (uint32_t i = 0; i < amount; ++i) randomGenerate();// ʹ�����ģʽ����"amount"���û���;
        } else if (gen_mode == "s") {
            for (uint32_t i = 0; i < amount; ++i) sequenceGenerate();// ʹ��˳��ģʽ����"amount"���û���;
        } else if (gen_mode == "ir") {
            for (uint32_t i = 0; i < amount; ++i) incrementRandomGenerate(maxDecrement);// ʹ���������ģʽ����"amount"���û���;
        } else if (gen_mode == "m") {
            multiSeedsGenerate();// ʹ�ö�����ģʽ���ɽ�"amount"���û���;
        } else {
            multiSeedsGenerate();// Ĭ��ʹ�ö�����ģʽ���ɽ�"amount"���û���;
        }

        // �����ɵ��û����б��й��˵�����֤����Ч�û���
        filterEffectiveUsernames();

        //���û����б��unordered_set(���򼯺�)�������͸��Ƶ�vector(����)��������, ��Ϊ��vector�û����б��ȡ�û���ʱ�ٶȻ����;
        copyUsernamesList();

        //�����û����б�;
        return getUsernamesList();
    }

    // ��ȡ���ɵ��û����б�;
    std::vector<std::string> getUsernamesList() { return _usernamesList; }

    // ���û����б��ȡ������һ�����ظ�(��ε��ò��ظ�)���û���, �û���ȫ����ȡ��ɺ󷵻�""���ַ���;
    std::string extractUniqueUsernam() {
        // �������Ϊ���򷵻�һ�����ַ���
        if (_usernamesList.empty()) return "";

        // ��������ĩβ��ȡһ���û������������Ա�֤Ч��(��ѭ��������ȡÿ��ֵ���ٶ�Ҫ��ö�,���ڴ滻�����ٶ�)
        std::string extractedUsername = _usernamesList.back();
        _usernamesList.pop_back();      // �Ƴ��ѳ�ȡ���ַ���

        return extractedUsername;       // ���س�ȡ���ַ���
    }

    // ���û����б�����û���;
    bool insertUsernameList(std::string username) {
        // ���Խ��û������뵽�û���������
        if (usernamesList.insert(username).second) {
            return true;// �������ɹ�,����true;
        } else {
            return false;// �������ʧ��(���û����б����Ѿ����ڴ��û���),����false;
        }
    }

    // ��ȡ�û����б��������û�������;
    size_t usernamesListCount() { return _usernamesList.size(); }

public:
    // ��ӡ������Ϣ;
    void printDebugInfo() {
        std::cout << "Ϊ������" << amount << "���û�����"
            << "����" << usernamesSeed.size() << "�����ӣ�"
            << "��Ҫ�����ɰ뾶Ϊ��" << calculateRadius() << std::endl;

        std::cout << "UsernameSeeds: " << std::endl;
        // ������������ɵ��û�������
        int i = 0;
        for (const auto &seed : usernamesSeed) {
            std::cout << seed << std::endl;
        }

        std::cout << "Generated Usernames: " << std::endl;
        // ������������ɵ��û���
        i = 0;
        for (const auto &username : _usernamesList) {
            std::cout << tl.removeSubstring(username, usernamePrefix) << std::endl;
        }

        system("pause");
    }

private:
    // [���ģʽ]: �ڷ�Χ������һ���µ�����û���(����û��������ظ�)
    std::string randomGenerate() {
        // ��������ɵ��û��������ﵽ���������������׳��쳣
        if (usernamesList.size() >= (maxNum - minNum - 1)) {
            throw std::runtime_error("Username space exhausted");
        }

        // ����ѭ��ֱ������һ����һ�޶����û���
        while (true) {
            // ��ָ����Χ������һ�������
            std::uniform_int_distribution<uint64_t> distribution(minNum + 1, maxNum - 1);
            std::string newUsername = usernamePrefix + std::to_string((distribution(generator)));

            // ���Խ����û�������ǰ׺���뵽�û���������
            if (usernamesList.insert(newUsername).second) {
                // �������ɹ������û����Ƕ�һ�޶��ģ����򷵻ظ��û���
                return newUsername;
            }
        }
    }

    // [˳��ģʽ]: �ڷ�Χ������һ���µ�˳���û���(Ĭ�ϴ����ֵ�ݼ�ֵ��Сֵ)
    std::string sequenceGenerate() {
        // Ӧ�õݼ�ֵ(ͬʱȷ��currentUsername�������minNum)
        currentUsername > minNum ? --currentUsername : currentUsername = minNum;

        // �����û�������ǰ׺���뵽�û���������
        usernamesList.insert(usernamePrefix + std::to_string(currentUsername)).second;

        return usernamePrefix + std::to_string(currentUsername);
    }

    // [�������ģʽ]: �ڷ�Χ������һ���µ���������û���(��ֵ��maxNum��ʼ��ÿ�ε���ʱ������һ������ĵݼ�ֵ���٣�ֱ���ﵽminNum���ݼ�ֵ�����ֵ��ͨ��һ������`maxDecrement`���ݸ�������)
    std::string incrementRandomGenerate(uint64_t maxDecrement) {
        if (currentUsername <= minNum) {
            throw std::runtime_error("All usernames have been generated");
        }

        // ����һ������1��maxDecrement֮�������ݼ�ֵ
        std::uniform_int_distribution<uint64_t> decrementDistribution(1, maxDecrement);
        uint64_t decrement = decrementDistribution(generator);

        // Ӧ�õݼ�ֵ(ͬʱȷ��currentUsername�������minNum)
        currentUsername > minNum + decrement ? currentUsername -= decrement : currentUsername = minNum;

        // �����û�������ǰ׺���뵽�û���������
        usernamesList.insert(usernamePrefix + std::to_string(currentUsername)).second;

        // ��currentUsernameת��Ϊ�ַ���������ǰ׺����
        return usernamePrefix + std::to_string(currentUsername);
    }

    /**
     * [������ģʽ]
     * ����Χ�Ƹ��������û������û�������
     *
     * ��������������������һ�������û������ӵ�������һ�����ɰ뾶������ÿ�������û�����
     * �ú����ڿ��Ǳ����ص����ظ�������£�����һ����ֵ��Χ�ڵ��û������ϡ����ɵ��û�����
     * ��ÿ�����ӵ���ֵǰ�󣬸��ݴ�С˳��ƽ�ȿ��������������ɡ�
     *
     * @param usernamesSeed �û������ӵ�������
     * @param radius ָ����usernamesSeed�е��û���Ϊ���ģ��������û�������ֵ�뾶��
     * @return �������ɵ��û�����������
     */
    void multiSeedsGenerate() {
        // ����Ч�û�����ȡ���û���������(������һ�����еĽ���Ż����Ӽ���)
        for (const auto &accountPair : dic.readEffectiveAccount()) {
            // ɾ����Ч�û�����ǰ׺
            std::string seed = tl.removeSubstring(accountPair.first, usernamePrefix);

            // ����Ч�û������ַ���ת��Ϊ���ֺ���뵽�û������Ӽ�����;
            usernamesSeed.insert(std::stoll(seed));
        }

        // ����ﵽԤ���û���������������ɰ뾶;
        uint32_t radius = calculateRadius();

        // ���������û����б�
        for (const auto &seed : usernamesSeed) {
            // ��ָ������ֵ�뾶���������û���
            for (uint64_t i = max(0LL, seed - radius); i <= seed + radius; ++i) {
                // ֱ�ӽ����ɵ��û�������ǰ׺���뵽�û����б���
                // unordered_set���Զ�����Ǳ�ڵ��ظ����룬��֤�û�����Ψһ��
                usernamesList.insert(usernamePrefix + std::to_string(i));
            }
        }
    }

    // �����û����б�: ���û����б��unordered_set(���򼯺�)�������͸��Ƶ�vector(����)��������, ��Ϊ��vector�û����б��ȡ�û���ʱ�ٶȻ����;
    void copyUsernamesList() {
        // Ԥ�ȷ����㹻�Ŀռ䣬�Ա����ڸ��ƹ����еĶ���ڴ����
        _usernamesList.reserve(usernamesList.size());

        // ��usernamesList(��������Ϊ:unordered_set)������_usernamesList(��������Ϊ:vector)
        for (const auto &username : usernamesList) _usernamesList.push_back(username);

        // ���û�����С�����������
        tl.sortNumericStrings(_usernamesList);
    }

    /**
     * @brief �����ɵ��û����б��й��˵�����֤����Ч�û���
     *
     * �ú����������Ǵ����ɵ��û����б����ų��Ѿ�����Ч�˻��ļ� (EffectiveAccount.csv) ����֤�����û���������߲��Ե�Ч�ʡ�
     * ����ͨ������ unordered_map (effectiveAccount) �е�ÿ��Ԫ�أ��û���-����ԣ���
     * ����� unordered_set (usernamesList) ���Ƿ������Щ�û����������������� usernamesList ���Ƴ���Ӧ���û�����
     * �ù��������� unordered_set �� unordered_map �ĸ�Ч����������ƽ��ʱ�临�Ӷ�Ϊ O(1)����֤����������ĸ�Ч�ԡ�
     */
    void filterEffectiveUsernames() {
        // ����Ч�û������б���
        for (const auto &accountPair : dic.readEffectiveAccount()) {
            // ֱ�ӳ��Դ�usernamesList��ɾ����ǰ���������û���
            // ����û�����������usernamesList�У�erase�������������κ�Ч��
            usernamesList.erase(accountPair.first);
        }
    }

    /**
     * ����ﵽԤ���û���������������ɰ뾶��
     * @param amount Ԥ�����ɵ��û���������
     * @param seedCount �����û�����������
     * @return ����������ɰ뾶��
     */
    uint32_t calculateRadius() {
        // ͳ��������;
        uint32_t seedCount = usernamesSeed.size();

        // ���������������ֹ����0�����
        if (seedCount <= 0) {
            std::cerr << "���������������0��" << std::endl;
            return 0;
        }

        // �������ɰ뾶��ȷ�����Ϊ������
        // ʹ��ceil��ȷ�������ܸ���Ԥ�ڵ��û�������
        // �ⷽ��n * (2r + 1) = amount���õ�r
        uint32_t radius = static_cast<uint32_t>(std::ceil((amount / static_cast<double>(seedCount) - 1) / 2));

        return radius;
    }
};