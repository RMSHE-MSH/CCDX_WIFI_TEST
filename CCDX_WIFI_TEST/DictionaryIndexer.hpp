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

public:
    // ���������ֵ�: �������ֵ��unordered_set(���򼯺�)�������͸��Ƶ�vector(����)��������, ��Ϊ��vector�����ֵ��ȡ����ʱ�ٶȻ����;
    void copyPasswordDictionary() {
        // Ԥ�ȷ����㹻�Ŀռ䣬�Ա����ڸ��ƹ����еĶ���ڴ����
        _passwordDictionary.reserve(passwordDictionary.size());

        // ��passwordDictionary(��������Ϊ:unordered_set)������_passwordDictionary(��������Ϊ:vector)
        for (const auto &password : passwordDictionary) _passwordDictionary.push_back(password);

        // �����_passwordDictionary����(ʹ�����ֵ��е������������)
        std::random_device rd;                                                       // �����������
        std::mt19937 g(rd());                                                        // ���������������ʼ��Mersenne Twister����
        std::shuffle(_passwordDictionary.begin(), _passwordDictionary.end(), g);     // �����_passwordDictionary����
    }

    // ��ָ����CSV�ļ�·����ȡ�ֵ�����
    std::unordered_set<std::string> readDictionary(const std::string &filePath) {
        CSV_Operations csv_op; // ����CSV��������

        // ����CSV�ļ��е�ÿ������
        for (const auto &row : csv_op.readFromCSV(filePath)) {
            // ������Ƿ�ǿգ�ȷ�����԰�ȫ�����еĵ�һ������
            if (!row.empty()) {
                // ʹ��.front()������ȡÿ�еĵ�һ�����ݣ��������ӵ������ֵ���(�ֵ��ļ�ֻ�е�һ��)
                // .front()�����еĵ�һ��Ԫ�أ������ǹ��ĵĵ�һ������
                passwordDictionary.insert(row.front());
            }
        }

        // ���������ֵ�:
        copyPasswordDictionary();

        return passwordDictionary; // ���������ֵ�;
    }

    // �������ֵ��ȡ������һ�����ظ�(��ε��ò��ظ�)���ַ���, ����ȫ����ȡ��ɺ󷵻�""���ַ���;
    std::string extractUniquePassword() {
        // �������Ϊ�գ������¸��������ֵ�, ���ҷ���һ�����ַ���
        if (_passwordDictionary.empty()) {
            copyPasswordDictionary();   // ���������ֵ�:
            return "";
        }

        // ��������ĩβ��ȡһ���ַ������������Ա�֤Ч��(��ѭ��������ȡÿ��ֵ���ٶ�Ҫ��ö�,���ڴ滻�����ٶ�)
        std::string extractedPassword = _passwordDictionary.back();
        _passwordDictionary.pop_back();      // �Ƴ��ѳ�ȡ���ַ���

        return extractedPassword;           // ���س�ȡ���ַ���
    }

public:
    // ��ȡ�����ֵ�
    std::unordered_set<std::string> getDictionary() { return passwordDictionary; }
};