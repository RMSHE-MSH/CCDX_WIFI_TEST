/**
 * @file PasswordOptimizer.hpp
 * @date 27.03.2024
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
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

 // �����Ż���: ʹ���Ŵ��㷨���Ż������ֵ��е����룬������Ч����ı���
class PasswordOptimizer {
public:
    // ���캯����������Ч�˻����ݺ������ֵ���Ϊ����
    PasswordOptimizer(std::unordered_map<std::string, std::unordered_set<std::string>> invalidAccounts,
                      std::unordered_set<std::string> passwordDictionary);

    double calculateFitness(const std::string &password);
private:
    // �洢������Ч���뼰�����Ƶ��
    std::unordered_map<std::string, uint32_t> invalidPassword;

    // ���ô��ݵ���Ч�˻�����
    std::unordered_map<std::string, std::unordered_set<std::string>> &invalidAccounts;

    // ���ô��ݵ������ֵ�
    std::unordered_set<std::string> &passwordDictionary;

private:
    // ͳ��invalidAccounts�е���Ч���뼰�����Ƶ�ʡ�
    void invalidPasswordsCount();
};