/**
 * @file rivest_cipher_4.hpp
 * @date 05.04.2023
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2023 RMSHE. All rights reserved.
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
 *
 * 注意: 这里的代码原本是为嵌入式系统编写的;
 */

#pragma once
#include <array>
#include <chrono>

class RivestCipher4 {
public:
    /**
    * @brief 构造函数
    * @param 传入参数`key`作为RC4加密的密钥
    */
    explicit RivestCipher4(const std::string &key) {
        // 初始化 S 盒
        for (uint16_t i = 0; i < 256; ++i) S_box_[i] = static_cast<uint8_t>(i);

        // 对 S 盒进行置换
        uint32_t j = 0;
        for (uint16_t i = 0; i < 256; ++i) {
            // 用循环的方式扩展 key
            j = (j + S_box_[i] + key[i % key.size()]) & 0xFF;
            uint8_t tmp = S_box_[i];
            S_box_[i] = S_box_[j];
            S_box_[j] = tmp;
        }
    }

    // 加密函数(提供要加密的文本)
    std::string encrypt(const std::string &plaintext) {
        // 初始化一个输出字符串流
        std::ostringstream ciphertext_stream;
        // 使用十六进制输出，并用 0 填充未满两位的字节
        ciphertext_stream << std::hex << std::setfill('0');

        // 初始化两个状态变量 i 和 j
        uint8_t i = 0, j = 0;
        // 遍历明文字符串中的每个字节
        for (size_t index = 0; index < plaintext.size(); ++index) {
            // 更新状态变量 i 和 j
            i = (i + 1) & 0xFF;
            j = (j + S_box_[i]) & 0xFF;
            // 交换 S 盒中的两个元素
            std::swap(S_box_[i], S_box_[j]);
            // 计算 k 和 temp
            uint8_t t = (S_box_[i] + S_box_[j]) & 0xFF;
            uint8_t k = S_box_[t];
            uint8_t temp = static_cast<uint8_t>(plaintext[index]) ^ k;

            // 将加密后的字节写入输出流
            ciphertext_stream << std::setw(2) << static_cast<uint16_t>(temp);
        }

        // 返回加密后的字符串
        return ciphertext_stream.str();
    }

    // 解密函数(提供要解密的文本)
    std::string decrypt(const std::string &ciphertext) { return encrypt(ciphertext); }

private:
    // S 盒
    std::array<uint8_t, 256> S_box_;
};