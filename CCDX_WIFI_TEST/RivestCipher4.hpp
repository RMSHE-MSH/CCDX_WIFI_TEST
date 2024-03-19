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
 * ע��: ����Ĵ���ԭ����ΪǶ��ʽϵͳ��д��;
 */

#pragma once
#include <array>
#include <chrono>

class RivestCipher4 {
public:
    /**
    * @brief ���캯��
    * @param �������`key`��ΪRC4���ܵ���Կ
    */
    explicit RivestCipher4(const std::string &key) {
        // ��ʼ�� S ��
        for (uint16_t i = 0; i < 256; ++i) S_box_[i] = static_cast<uint8_t>(i);

        // �� S �н����û�
        uint32_t j = 0;
        for (uint16_t i = 0; i < 256; ++i) {
            // ��ѭ���ķ�ʽ��չ key
            j = (j + S_box_[i] + key[i % key.size()]) & 0xFF;
            uint8_t tmp = S_box_[i];
            S_box_[i] = S_box_[j];
            S_box_[j] = tmp;
        }
    }

    // ���ܺ���(�ṩҪ���ܵ��ı�)
    std::string encrypt(const std::string &plaintext) {
        // ��ʼ��һ������ַ�����
        std::ostringstream ciphertext_stream;
        // ʹ��ʮ��������������� 0 ���δ����λ���ֽ�
        ciphertext_stream << std::hex << std::setfill('0');

        // ��ʼ������״̬���� i �� j
        uint8_t i = 0, j = 0;
        // ���������ַ����е�ÿ���ֽ�
        for (size_t index = 0; index < plaintext.size(); ++index) {
            // ����״̬���� i �� j
            i = (i + 1) & 0xFF;
            j = (j + S_box_[i]) & 0xFF;
            // ���� S ���е�����Ԫ��
            std::swap(S_box_[i], S_box_[j]);
            // ���� k �� temp
            uint8_t t = (S_box_[i] + S_box_[j]) & 0xFF;
            uint8_t k = S_box_[t];
            uint8_t temp = static_cast<uint8_t>(plaintext[index]) ^ k;

            // �����ܺ���ֽ�д�������
            ciphertext_stream << std::setw(2) << static_cast<uint16_t>(temp);
        }

        // ���ؼ��ܺ���ַ���
        return ciphertext_stream.str();
    }

    // ���ܺ���(�ṩҪ���ܵ��ı�)
    std::string decrypt(const std::string &ciphertext) { return encrypt(ciphertext); }

private:
    // S ��
    std::array<uint8_t, 256> S_box_;
};