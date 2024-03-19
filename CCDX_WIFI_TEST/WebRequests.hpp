#pragma once
#include "RivestCipher4.hpp"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

class WebRequests {
private:
    std::string username = "";              // 用户名;
    std::string password = "";              // 登录密码;

    std::string timestamp = "";             // 时间戳;
    std::string encryptedPassword = "";     // 使用时间戳加密后的登录密码;

    std::string url = "http://1.1.1.2/ac_portal/login.php";
    cpr::Header headers = { {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36"} };

public:
    /**
    * @brief 构造函数
    * @param username：用户名;
    * @param password：登录密码;
    * @note 预处理登录数据,首先获取当前时间的时间戳,然后使用时间戳加密登录密码;
    */
    explicit WebRequests(const std::string &_username, const std::string &_password) {
        // 初始化(赋值)private;
        username = _username; password = _password;

        // 获取当前时间戳并将其转换为"string"类型;
        timestamp = std::to_string(getCurrentTimestamp());

        // 构造RC4函数: 将时间戳作为加密密钥;
        RivestCipher4 rc4(timestamp);

        // 使用RC4算法加密登录密码;
        encryptedPassword = rc4.encrypt(password);
    }

    // 发送登录请求;
    std::vector<std::pair<std::string, std::string>> login() {
        /**
        * @brief 构建登录数据
        * @param opr：操作码，告诉系统我们要进行登录。
        * @param userName：用于登录的用户名。
        * @param rememberPwd：是否记住密码, 这里默认为"1"即告诉系统记住密码。
        * @param auth_tag：时间戳，用于验证请求的时效性(注意:这里的时间戳必须与用于加密密码的时间戳相同)。
        */
        cpr::Payload payload{
            {"opr", "pwdLogin"},
            {"userName", username},
            {"pwd", encryptedPassword },
            {"rememberPwd", "1"},
            {"auth_tag", timestamp}
        };

        try {
            // 向系统发送登录(POST)请求
            auto response = cpr::Post(cpr::Url{ url }, headers, payload);

            // 使用nlohmann::json解析响应
            auto resp_json = nlohmann::json::parse(response.text);

            // 检查success字段, 确认是否成功登录(如果成功登录则代表用户名登录密码正确)
            // 这是一个登录成功的响应例子: {"success":true,"msg":"logon success","action":"location","pop":0,"userName":"0431119390025965", "location":"http://1.1.1.2/homepage/mobile_detail.html"}
            // 这是一个登录失败的响应例子: {"success":false,"msg":"鐢ㄦ埛鍚嶆垨瀵嗙爜閿欒","action":"location","pop":0,"userName":"0431119390025542","location":""}
            if (resp_json.contains("success") && resp_json["success"] == true) {
                // 登录成功

                /**
                * @brief 返回响应结果;
                * @param response：系统响应结果。
                * @param username：用户名。
                * @param password：登录密码。
                * @param timestamp：时间戳。
                * @param encPassword：使用时间戳加密后的登录密码。
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
                // 登录失败
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
            //返回POST请求发送失败的信息;
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
    // 获取当前时间戳;
    long long getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }
};