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
     * 将数据追加写入CSV文件。如果文件存在，则在文件末尾追加新内容；如果文件不存在，则创建文件。
     * @param data 待写入的数据，每个内部向量代表一行，内部向量的每个元素代表一个字段。
     * @param filePath CSV文件的路径。
     */
    void appendToCSV(const std::vector<std::vector<std::string>> &data, const std::string &filePath) {
        // 使用文件输出流打开文件，设置为追加模式。
        std::ofstream outFile(filePath, std::ios::app);

        // 检查文件是否成功打开。
        if (!outFile.is_open()) {
            std::cerr << "[appendToCSV] Unable to open file：" << filePath << std::endl;
            return;
        }

        // 遍历数据中的每一行。
        for (const auto &row : data) {
            // 遍历行中的每个字段。
            for (auto it = row.begin(); it != row.end(); ++it) {
                outFile << *it; // 写入字段值。

                // 如果不是最后一个字段，加上逗号分隔符。
                if (it != row.end() - 1) outFile << ",";
            }
            outFile << "\n"; // 每写完一行，添加换行符。
        }

        // 关闭文件。
        outFile.close();
    }

    /**
     * 从CSV文件读取数据。
     * @param filePath CSV文件的路径。
     * @return 一个包含文件数据的向量，其中每个内部向量代表文件中的一行，内部向量的每个元素代表一列。
     */
    std::vector<std::vector<std::string>> readFromCSV(const std::string &filePath) {
        std::vector<std::vector<std::string>> data; // 用于存储读取的数据
        std::ifstream inFile(filePath); // 打开文件

        // 检查文件是否成功打开
        if (!inFile.is_open()) {
            std::cerr << "[readFromCSV] Unable to open file：" << filePath << std::endl;
            return data; // 返回空的数据向量
        }

        std::string line;
        // 逐行读取文件内容
        while (std::getline(inFile, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line); // 将行内容转换为字符串流，方便处理
            std::string cell;

            // 逐列读取数据
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell); // 将每列的数据添加到行向量中
            }

            data.push_back(row); // 将读取的行添加到数据向量中
        }

        // 关闭文件
        inFile.close();

        return data; // 返回读取的数据
    }

    /**
     * 检查指定路径的文件是否存在。
     * @param filePath 要检查的文件路径。
     * @return 如果文件存在返回true，否则返回false。
     */
    bool isFileExists(const std::filesystem::path &filePath) {
        // std::filesystem::exists用于检查给定路径的文件或目录是否存在
        return std::filesystem::exists(filePath);
    }

    /**
     * 创建指定路径的文件，如果文件所在的目录不存在，则先创建目录。
     * 如果文件已存在，则不会覆盖原有文件。
     * @param filePath 要创建的文件的完整路径。
     * @return 如果文件已成功创建或已存在，则返回true；如果操作失败，则返回false。
     */
    bool createFile(const std::filesystem::path &filePath) {
        // 如果文件已存在，则直接返回成功，不进行覆盖。
        if (std::filesystem::exists(filePath)) return true; // 文件已存在，无需创建。

        // 如果文件所在目录不存在，尝试创建目录。
        if (!std::filesystem::exists(filePath.parent_path())) {
            // 创建目录，如果创建失败，则返回失败。
            if (!std::filesystem::create_directories(filePath.parent_path())) {
                return false; // 无法创建目录，操作失败。
            }
        }

        // 创建文件。这里只需尝试打开文件，如果文件不存在，将自动创建。
        std::ofstream fileStream(filePath);
        if (!fileStream) return false; // 文件创建失败。

        return true; // 文件创建成功。
    }

    /**
     * 删除指定路径的文件或目录。如果是目录，则递归删除所有内容。
     * @param path 要删除的文件或目录的路径。
     * @return 操作是否成功完成。
     */
    bool deletePath(const std::filesystem::path &path) {
        if (!std::filesystem::exists(path)) return false; // 路径不存在，操作失败

        std::error_code ec; // 用于捕获删除操作中可能发生的错误
        if (std::filesystem::is_directory(path)) {
            // 目录：递归删除
            std::filesystem::remove_all(path, ec);
        } else {
            // 文件：直接删除
            std::filesystem::remove(path, ec);
        }

        return !ec; // 如果没有错误，返回true（操作成功）
    }
};