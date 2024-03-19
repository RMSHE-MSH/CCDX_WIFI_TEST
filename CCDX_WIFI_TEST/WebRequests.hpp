#pragma once
#include "RivestCipher4.hpp"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

class WebRequests {
private:
    std::string username = "";              // �û���;
    std::string password = "";              // ��¼����;

    std::string timestamp = "";             // ʱ���;
    std::string encryptedPassword = "";     // ʹ��ʱ������ܺ�ĵ�¼����;

    std::string url = "http://1.1.1.2/ac_portal/login.php";
    cpr::Header headers = { {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36"} };

public:
    /**
    * @brief ���캯��
    * @param username���û���;
    * @param password����¼����;
    * @note Ԥ�����¼����,���Ȼ�ȡ��ǰʱ���ʱ���,Ȼ��ʹ��ʱ������ܵ�¼����;
    */
    explicit WebRequests(const std::string &_username, const std::string &_password) {
        // ��ʼ��(��ֵ)private;
        username = _username; password = _password;

        // ��ȡ��ǰʱ���������ת��Ϊ"string"����;
        timestamp = std::to_string(getCurrentTimestamp());

        // ����RC4����: ��ʱ�����Ϊ������Կ;
        RivestCipher4 rc4(timestamp);

        // ʹ��RC4�㷨���ܵ�¼����;
        encryptedPassword = rc4.encrypt(password);
    }

    // ���͵�¼����;
    std::vector<std::pair<std::string, std::string>> login() {
        /**
        * @brief ������¼����
        * @param opr�������룬����ϵͳ����Ҫ���е�¼��
        * @param userName�����ڵ�¼���û�����
        * @param rememberPwd���Ƿ��ס����, ����Ĭ��Ϊ"1"������ϵͳ��ס���롣
        * @param auth_tag��ʱ�����������֤�����ʱЧ��(ע��:�����ʱ������������ڼ��������ʱ�����ͬ)��
        */
        cpr::Payload payload{
            {"opr", "pwdLogin"},
            {"userName", username},
            {"pwd", encryptedPassword },
            {"rememberPwd", "1"},
            {"auth_tag", timestamp}
        };

        try {
            // ��ϵͳ���͵�¼(POST)����
            auto response = cpr::Post(cpr::Url{ url }, headers, payload);

            // ʹ��nlohmann::json������Ӧ
            auto resp_json = nlohmann::json::parse(response.text);

            // ���success�ֶ�, ȷ���Ƿ�ɹ���¼(����ɹ���¼������û�����¼������ȷ)
            // ����һ����¼�ɹ�����Ӧ����: {"success":true,"msg":"logon success","action":"location","pop":0,"userName":"0431119390025965", "location":"http://1.1.1.2/homepage/mobile_detail.html"}
            // ����һ����¼ʧ�ܵ���Ӧ����: {"success":false,"msg":"用户名或密码错误","action":"location","pop":0,"userName":"0431119390025542","location":""}
            if (resp_json.contains("success") && resp_json["success"] == true) {
                // ��¼�ɹ�

                /**
                * @brief ������Ӧ���;
                * @param response��ϵͳ��Ӧ�����
                * @param username���û�����
                * @param password����¼���롣
                * @param timestamp��ʱ�����
                * @param encPassword��ʹ��ʱ������ܺ�ĵ�¼���롣
                */
                return {
                    {"result", "true"},
                    {"response", response.text},
                    {"username", username},
                    {"password", password},
                    {"timestamp", timestamp},
                    {"encPassword", encryptedPassword}
                };
            } else {
                // ��¼ʧ��
                return {
                    {"result", "false"},
                    {"response", response.text},
                    {"username", username},
                    {"password", password},
                    {"timestamp", timestamp},
                    {"encPassword", encryptedPassword}
                };
            }
        } catch (...) {
            //����POST������ʧ�ܵ���Ϣ;
            return {
                {"error", "post_fail"},
                {"username", username},
                {"password", password},
                {"timestamp", timestamp},
                {"encPassword", encryptedPassword}
            };
        }
    }

private:
    // ��ȡ��ǰʱ���;
    long long getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }
};