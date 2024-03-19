#pragma once
#include <unordered_set>
#include <random>
#include <string>
#include <stdexcept>
#include "Tool.hpp"

Tool tl;

class UsernameGenerator {
private:
    // �洢�����ɵ��û����б�
    std::unordered_set<std::string> usernamesList; // (unordered_set���Ա�֤�û������ظ�)
    std::vector<std::string> _usernamesList;       // (vector���Ը���Ч�ĳ�ȡ�û���)

    // ���������������������
    std::mt19937 generator;

    // �û������ֵ���С���ֵ
    uint64_t minNum, maxNum;

    //ʹ��˳������ʱ��¼��ǰ�û���ֵ
    uint64_t currentUsername;

    // �û�����������
    uint16_t amount;

public:
    // ���캯����������С�����������Ϊ�û�����Χ
    explicit UsernameGenerator(uint16_t _amount = 1000, uint64_t min = 8143086109, uint64_t max = 9390026092) : amount(_amount), minNum(min), maxNum(max), currentUsername(max) {
        // ʹ�õ�ǰʱ����Ϊ���ӳ�ʼ�������������
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    /**
     * �����û����б�;
     * @param gen_mode ����ģʽ("r":���ģʽ; "s":˳��ģʽ; "ir":�������ģʽ;)
     * @param maxDecrement ���������ģʽ��ʱ����, ����ݼ�ֵ�����ֵͨ�������������
     * @return �����û����б�;
     */
    std::vector<std::string> generateUsernamesList(std::string gen_mode, uint64_t maxDecrement = 8) {
        // �ж��û�������ģʽ;
        if (gen_mode == "r") {
            for (uint16_t i = 0; i < amount; ++i) randomGenerate();// ʹ�����ģʽ����"amount"���û���;
        } else if (gen_mode == "s") {
            for (uint16_t i = 0; i < amount; ++i) sequenceGenerate();// ʹ��˳��ģʽ����"amount"���û���;
        } else if (gen_mode == "ir") {
            for (uint16_t i = 0; i < amount; ++i) incrementRandomGenerate(maxDecrement);// ʹ���������ģʽ����"amount"���û���;
        } else {
            for (uint16_t i = 0; i < amount; ++i) sequenceGenerate();// Ĭ��ʹ��˳��ģʽ����"amount"���û���;
        }

        //���û����б��unordered_set(���򼯺�)�������͸��Ƶ�vector(����)��������, ��Ϊ��vector�û����б��ȡ����ʱ�ٶȻ����;
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

private:
    // ���ģʽ: �ڷ�Χ������һ���µ�����û���(����û��������ظ�)
    std::string randomGenerate() {
        // ��������ɵ��û��������ﵽ���������������׳��쳣
        if (usernamesList.size() >= (maxNum - minNum - 1)) {
            throw std::runtime_error("Username space exhausted");
        }

        // ����ѭ��ֱ������һ����һ�޶����û���
        while (true) {
            // ��ָ����Χ������һ�������
            std::uniform_int_distribution<uint64_t> distribution(minNum + 1, maxNum - 1);
            std::string newUsername = std::to_string((distribution(generator)));

            // ���Խ����û������뵽�û���������
            if (usernamesList.insert(newUsername).second) {
                // �������ɹ������û����Ƕ�һ�޶��ģ����򷵻ظ��û���
                return newUsername;
            }
        }
    }

    // ˳��ģʽ: �ڷ�Χ������һ���µ�˳���û���(Ĭ�ϴ����ֵ�ݼ�ֵ��Сֵ)
    std::string sequenceGenerate() {
        // Ӧ�õݼ�ֵ(ͬʱȷ��currentUsername�������minNum)
        currentUsername > minNum ? --currentUsername : currentUsername = minNum;

        // �����û������뵽�û���������
        usernamesList.insert(std::to_string(currentUsername)).second;

        return std::to_string(currentUsername);
    }

    // �������ģʽ: �ڷ�Χ������һ���µ���������û���(��ֵ��maxNum��ʼ��ÿ�ε���ʱ������һ������ĵݼ�ֵ���٣�ֱ���ﵽminNum���ݼ�ֵ�����ֵ��ͨ��һ������`maxDecrement`���ݸ�������)
    std::string incrementRandomGenerate(uint64_t maxDecrement) {
        if (currentUsername <= minNum) {
            throw std::runtime_error("All usernames have been generated");
        }

        // ����һ������1��maxDecrement֮�������ݼ�ֵ
        std::uniform_int_distribution<uint64_t> decrementDistribution(1, maxDecrement);
        uint64_t decrement = decrementDistribution(generator);

        // Ӧ�õݼ�ֵ(ͬʱȷ��currentUsername�������minNum)
        currentUsername > minNum + decrement ? currentUsername -= decrement : currentUsername = minNum;

        // �����û������뵽�û���������
        usernamesList.insert(std::to_string(currentUsername)).second;

        // ��currentUsernameת��Ϊ�ַ���������
        return std::to_string(currentUsername);
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
};