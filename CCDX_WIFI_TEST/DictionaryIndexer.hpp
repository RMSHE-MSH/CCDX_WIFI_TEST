/**
 * @file DictionaryIndexer.hpp
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
#include <unordered_set>
#include "CSV_Operations.hpp"

class DictionaryIndexer {
private:
    // ������Ч�˻����ݽṹ
    std::unordered_map<std::string, std::string> effectiveAccount;

    // ������Ч�˻����ݽṹ
    std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts;

    // �����ֵ䣬���ڴ洢��CSV�ļ��ж�ȡ�����ֵ�(���ֵ���ļ����ص��ڴ��������ٶȻ����);
    std::unordered_set<std::string> passwordDictionary; // (unordered_set���Ա�֤���벻�ظ�)
    std::vector<std::string> _passwordDictionary;       // (vector���Ը���Ч�ĳ�ȡ����)

private:
    /**
     * �����Ч�˺����ݽṹ�и����û����µ������Ƿ���ڡ�
     *
     * @param username �û�������Ϊ���ҵļ���
     * @param password ��Ҫ����Ƿ���ڵ����롣
     * @return bool ����true������������ָ���û����£����򷵻�false(����û�����������ֱ�ӷ���false)��
     */
    bool isInvalidAccountsExists(const std::string &username, const std::string &password) {
        // ʹ��find���������ҵ��û���
        auto userIt = invalidAccounts.find(username);
        if (userIt != invalidAccounts.end()) {
            // ����ҵ��û�������ʹ��find������������Ƿ����
            return userIt->second.find(password) != userIt->second.end();
        }
        // ����û��������ڣ������벻�ڶ�Ӧ�ļ����У�����false
        return false;
    }

    // ��ȡ��Ӧ�û�������Ч������Ŀ
    size_t invalidPasswordCount(const std::string &username) {
        size_t invalidPasswordCount = 0;
        if (invalidAccounts.find(username) != invalidAccounts.end()) {
            invalidPasswordCount = invalidAccounts[username].size();
        }

        return invalidPasswordCount;
    }

public:
    /**
     * ���Խ��������û�����������ӵ���Ч�˻����ݽṹ�С�
     * ����û��������ڣ����������û��������Ӧ�����롣
     * ����û����Ѵ��ڣ������벻�ڶ�Ӧ�����뼯���У�Ҳ�������, �������Ѵ��ڣ��򲻻��ظ����(�û��������붼�����ظ�)��
     *
     * @param username �û���
     * @param password ����
     */
    bool addInvalidAccount(const std::string &username, const std::string &password) {
        // ͨ��insert�����ķ���ֵ�ж������Ƿ�ɹ����
        auto [_, inserted] = invalidAccounts[username].insert(password);

        // ���������Ƿ�ɹ����
        return inserted;
    }

    /**
     * ����Ч�˻����û��������뼯�ϣ����浽CSV�ļ��С�
     * ���������invalidAccountsת�����ʺ�writeToCSV�����ĸ�ʽ������������
     * �ļ���ʽ�涨Ϊ��ÿ�еĵ�һ�����û�����������Ϊ��Ӧ�����뼯�ϡ�
     *
     * @param filePath Ҫ׷�ӵ�CSV�ļ�·��
     */
    void saveInvalidAccounts(const std::string &filePath = "./InvalidAccounts.csv") {
        // �洢ת���������
        std::vector<std::vector<std::string>> csvData;

        // ����ÿ���˻��Ͷ�Ӧ�����뼯�ϣ�ת����CSV�и�ʽ
        for (const auto &[username, passwords] : invalidAccounts) {
            std::vector<std::string> row; // �洢һ�����ݣ������û���������
            row.reserve(passwords.size() + 1); // Ԥ���ռ䣬���Ч��
            row.push_back(username); // ��������û���
            for (const auto &password : passwords) row.push_back(password);// �������
            csvData.push_back(std::move(row)); // ��ӵ�csvData��ʹ��std::move���ٿ���
        }

        CSV_Operations csv_op; // ����CSV��������

        // ����writeToCSV�����������ݸ��ǵ�CSV�ļ�
        csv_op.writeToCSV(csvData, filePath, "w");
    }

    /**
     * ��InvalidAccounts.csv�ļ��ж�ȡ��Ч�˻���Ϣ��
     * @param filePath CSV�ļ�·��
     */
    void readInvalidAccounts(const std::string &filePath = "./InvalidAccounts.csv") {
        CSV_Operations csv_op; // ����CSV��������

        // ��CSV�ļ��ж�ȡ����
        auto csvData = csv_op.readFromCSV(filePath);

        // ������ȡ����ÿһ������
        for (const auto &row : csvData) {
            if (!row.empty()) { // ȷ�������ݲ�Ϊ��
                const auto &username = row.front(); // ��һ��Ԫ����Ϊ�û���
                for (size_t i = 1; i < row.size(); ++i) { // �������µ�ÿ��Ԫ�أ����룩
                    addInvalidAccount(username, row[i]);  // ���Խ��������û�����������ӵ���Ч�˻����ݽṹ�С�
                }
            }
        }
    }

    // ��EffectiveAccount.csv�ļ��ж�ȡ��Ч�˻�(unordered_map���������ܹ������û����ظ�������)��
    std::unordered_map<std::string, std::string> readEffectiveAccount(const std::string &filePath = "./EffectiveAccount.csv") {
        CSV_Operations csv_op; // ����CSV��������

        // ����readFromCSV��ȡCSV�ļ�����
        auto csvData = csv_op.readFromCSV(filePath);

        // ������ȡ����CSV����
        for (const auto &row : csvData) {
            // ȷ��ÿ����������������
            if (row.size() >= 2) effectiveAccount.emplace(row[0], row[1]); // ʹ��emplace����Ч�ʲ��������Ŀ���
        }

        return effectiveAccount;
    }

    // ���������ֵ�(���������ֵ�����): �������ֵ��unordered_set(���򼯺�)�������͸��Ƶ�vector(����)��������, ��Ϊ��vector�����ֵ��ȡ����ʱ�ٶȻ����;
    void copyPasswordDictionary(const std::string &username, const std::string &initialPassword) {
        // �жϳ�ʼ�����Ƿ���Ч(true:��ʾ������Ч);
        bool initPwdIsValid = !isInvalidAccountsExists(username, initialPassword);

        // ����Ԥ�ڵ���Ч���������������ʼ������Ч�������һ(������"+1"����Ϊ����ĩβ����Ҫ��������һ����ʼ����)
        size_t expectedValidCount = passwordDictionary.size() - invalidPasswordCount(username) + (initPwdIsValid ? 1 : 0);

        // ��ղ��ͷ�_passwordDictionary�����ռ���ڴ�;
        std::vector<std::string>().swap(_passwordDictionary);

        // Ԥ�ȷ��������ռ䣬����������Ԥ�ڵ���Ч���룬�����ڴ����·���
        _passwordDictionary.reserve(expectedValidCount);

        // ��passwordDictionary(��������Ϊ:unordered_set)������_passwordDictionary(��������Ϊ:vector)
        for (const auto &password : passwordDictionary) {
            // ʹ��isInvalidAccountsExists����������Ƿ���Ч(������Ч������)
            if (!isInvalidAccountsExists(username, password)) {
                _passwordDictionary.push_back(password);
            }
        }

        // ʹ��C++20��Χ���㷨������Ч����
        //for (const auto &password : passwordDictionary | std::views::filter([&](const std::string &pwd) {
        //    return !isInvalidAccountsExists(username, pwd);
        //                                                                    })) {
        //    _passwordDictionary.push_back(password);
        //}

        // �����_passwordDictionary����(ʹ�����ֵ��е������������)
        static std::mt19937 g(std::random_device{}());
        std::shuffle(_passwordDictionary.begin(), _passwordDictionary.end(), g);

        // ����ʼ������Ч��������ӵ�����ĩβ(����ĳ�ȡ�Ǵ�����ĩβ��ʼ��, �����ܱ�֤�ڵ�һ�γ�ȡ����ʱȡ�����ǳ�ʼ����)
        if (initPwdIsValid) _passwordDictionary.push_back(initialPassword);
    }

    // ��ָ����CSV�ļ�·����ȡ�ֵ�����
    std::unordered_set<std::string> readDictionary(const std::string &filePath = "./Dictionary.csv") {
        CSV_Operations csv_op; // ����CSV��������

        // ����CSV�ļ��е�ÿ������
        for (const auto &row : csv_op.readFromCSV(filePath)) {
            // ������Ƿ�ǿգ�ȷ�����԰�ȫ�����еĵ�һ������
            if (!row.empty()) {
                // ʹ��.front()������ȡÿ�еĵ�һ�����ݣ�������ӵ������ֵ���(�ֵ��ļ�ֻ�е�һ��)
                // .front()�����еĵ�һ��Ԫ�أ������ǹ��ĵĵ�һ������
                passwordDictionary.insert(row.front());
            }
        }

        return passwordDictionary; // ���������ֵ�;
    }

    // �������ֵ��ȡ������һ�����ظ�(��ε��ò��ظ�)���ַ���, ����ȫ����ȡ��ɺ󷵻�""���ַ���;
    std::string extractUniquePassword() {
        // �������Ϊ�գ������¸��������ֵ�, ���ҷ���һ�����ַ���
        if (_passwordDictionary.empty()) return "";

        // ��������ĩβ��ȡһ���ַ������������Ա�֤Ч��(��ѭ��������ȡÿ��ֵ���ٶ�Ҫ��ö�,���ڴ滻�����ٶ�)
        std::string extractedPassword = _passwordDictionary.back();
        _passwordDictionary.pop_back();      // �Ƴ��ѳ�ȡ���ַ���

        return extractedPassword;           // ���س�ȡ���ַ���
    }

public:
    // ��ȡ�����ֵ�
    std::unordered_set<std::string> getDictionary() { return passwordDictionary; }

    // ��ȡ��Ч�˻�����Ŀ
    size_t effectiveAccountCount() { return effectiveAccount.size(); }

    // ��ȡ��ǰ�û�����Ԥ����Ч�������Ŀ
    size_t expectedValidPwdCount() { return _passwordDictionary.size(); }

    // ��ȡָ���û����µ���Ч���������
    uint32_t invalidPwdCount(const std::string &username) {
        // ��������Ч�˻����ݽṹ�в���ָ�����û���
        auto it = invalidAccounts.find(username);
        // ����ҵ����򷵻ض�Ӧ����Ч���������
        if (it != invalidAccounts.end()) return it->second.size();

        // ���δ�ҵ�ָ�����û���������0
        return 0;
    }
};