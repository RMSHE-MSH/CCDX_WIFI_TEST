/**
 * @file CSV_Operations.hpp
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
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

class CSV_Operations {
public:
    /**
     * ������׷��д��CSV�ļ�������ļ����ڣ������ļ�ĩβ׷�������ݣ�����ļ������ڣ��򴴽��ļ���
     * @param data ��д������ݣ�ÿ���ڲ���������һ�У��ڲ�������ÿ��Ԫ�ش���һ���ֶΡ�
     * @param filePath CSV�ļ���·����
     */
    void appendToCSV(const std::vector<std::vector<std::string>> &data, const std::string &filePath) {
        // ʹ���ļ���������ļ�������Ϊ׷��ģʽ��
        std::ofstream outFile(filePath, std::ios::app);

        // ����ļ��Ƿ�ɹ��򿪡�
        if (!outFile.is_open()) {
            std::cerr << "[appendToCSV] Unable to open file��" << filePath << std::endl;
            return;
        }

        // ���������е�ÿһ�С�
        for (const auto &row : data) {
            // �������е�ÿ���ֶΡ�
            for (auto it = row.begin(); it != row.end(); ++it) {
                outFile << *it; // д���ֶ�ֵ��

                // ����������һ���ֶΣ����϶��ŷָ�����
                if (it != row.end() - 1) outFile << ",";
            }
            outFile << "\n"; // ÿд��һ�У���ӻ��з���
        }

        // �ر��ļ���
        outFile.close();
    }

    /**
     * ��CSV�ļ���ȡ���ݡ�
     * @param filePath CSV�ļ���·����
     * @return һ�������ļ����ݵ�����������ÿ���ڲ����������ļ��е�һ�У��ڲ�������ÿ��Ԫ�ش���һ�С�
     */
    std::vector<std::vector<std::string>> readFromCSV(const std::string &filePath) {
        std::vector<std::vector<std::string>> data; // ���ڴ洢��ȡ������
        std::ifstream inFile(filePath); // ���ļ�

        // ����ļ��Ƿ�ɹ���
        if (!inFile.is_open()) {
            std::cerr << "[readFromCSV] Unable to open file��" << filePath << std::endl;
            return data; // ���ؿյ���������
        }

        std::string line;
        // ���ж�ȡ�ļ�����
        while (std::getline(inFile, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line); // ��������ת��Ϊ�ַ����������㴦��
            std::string cell;

            // ���ж�ȡ����
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell); // ��ÿ�е�������ӵ���������
            }

            data.push_back(row); // ����ȡ������ӵ�����������
        }

        // �ر��ļ�
        inFile.close();

        return data; // ���ض�ȡ������
    }

    /**
     * ���ָ��·�����ļ��Ƿ���ڡ�
     * @param filePath Ҫ�����ļ�·����
     * @return ����ļ����ڷ���true�����򷵻�false��
     */
    bool isFileExists(const std::filesystem::path &filePath) {
        // std::filesystem::exists���ڼ�����·�����ļ���Ŀ¼�Ƿ����
        return std::filesystem::exists(filePath);
    }

    /**
     * ����ָ��·�����ļ�������ļ����ڵ�Ŀ¼�����ڣ����ȴ���Ŀ¼��
     * ����ļ��Ѵ��ڣ��򲻻Ḳ��ԭ���ļ���
     * @param filePath Ҫ�������ļ�������·����
     * @return ����ļ��ѳɹ��������Ѵ��ڣ��򷵻�true���������ʧ�ܣ��򷵻�false��
     */
    bool createFile(const std::filesystem::path &filePath) {
        // ����ļ��Ѵ��ڣ���ֱ�ӷ��سɹ��������и��ǡ�
        if (std::filesystem::exists(filePath)) return true; // �ļ��Ѵ��ڣ����贴����

        // ����ļ�����Ŀ¼�����ڣ����Դ���Ŀ¼��
        if (!std::filesystem::exists(filePath.parent_path())) {
            // ����Ŀ¼���������ʧ�ܣ��򷵻�ʧ�ܡ�
            if (!std::filesystem::create_directories(filePath.parent_path())) {
                return false; // �޷�����Ŀ¼������ʧ�ܡ�
            }
        }

        // �����ļ�������ֻ�賢�Դ��ļ�������ļ������ڣ����Զ�������
        std::ofstream fileStream(filePath);
        if (!fileStream) return false; // �ļ�����ʧ�ܡ�

        return true; // �ļ������ɹ���
    }

    /**
     * ɾ��ָ��·�����ļ���Ŀ¼�������Ŀ¼����ݹ�ɾ���������ݡ�
     * @param path Ҫɾ�����ļ���Ŀ¼��·����
     * @return �����Ƿ�ɹ���ɡ�
     */
    bool deletePath(const std::filesystem::path &path) {
        if (!std::filesystem::exists(path)) return false; // ·�������ڣ�����ʧ��

        std::error_code ec; // ���ڲ���ɾ�������п��ܷ����Ĵ���
        if (std::filesystem::is_directory(path)) {
            // Ŀ¼���ݹ�ɾ��
            std::filesystem::remove_all(path, ec);
        } else {
            // �ļ���ֱ��ɾ��
            std::filesystem::remove(path, ec);
        }

        return !ec; // ���û�д��󣬷���true�������ɹ���
    }
};