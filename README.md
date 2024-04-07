# CCDX WIFI TEST 
# CCDX-WIFI账号密码测试程序

## 1. 简介

CCDX-WIFI账号密码测试程序，这是目前为长春大学校园网开发的完成度最高、功能最完善的工具。它通过自动化的方式(使用不同的用户名和密码组合)尝试登录账户，避免了低效的密码穷举法，转而利用密码字典和科学的用户名生成方法，尤其针对弱密码实施有效检测。此外，程序还封装了多个辅助组件，以更全面地支持整个测试流程，旨在提升校园网账户的安全性。

---

## 2. 核心特性

- **用户名生成**：基于账号的数字递增和集群特性，通过使用一个或多个已知的有效用户名作为种子，来生成新的用户名。这是一个简洁且有效的方法。支持随机模式、顺序模式、增量随机及多种子模式，满足不同的测试需求。
- **密码字典**：集成字典管理功能，从本地字典文件中随机不重复地抽取密码进行登录尝试，确保测试全面覆盖常见的弱密码组合。
- **有效账号跟踪**：程序在测试中发现有效的用户名和密码组合后，会自动将其保存至`EffectiveAccount.csv`文件。这样做旨在提高测试效率和节约请求资源，程序在下次启动时会在生成用户名列表阶段自动排除已验证的有效账户，避免重复测试。
- **无效密码跟踪**：程序在测试中发现某个特定用户名所尝试的密码无效时，会自动保存这些信息到`InvalidAccounts.csv`文件中。这样做旨在提高测试效率并节约请求资源。当程序在下次测试同一用户名时，将自动排除其所有已知的无效密码，避免重复测试。
- **实时反馈机制**：在每次登录尝试中，程序提供即时反馈信息，包括当前尝试的用户名、密码及有效账户的统计数据，实现测试过程的透明化。
- **自优化特性**：具备使用程序历史运行数据优化新测试实例的能力，以此提升测试效率。
- **测试高效性**：本项目采用`C++20`标准编写，并通过优化测试流程、数据结构和数据流，尽可能提高代码运行效率。当前唯一限制测试速度的因素是网络延迟和服务器响应速度(你可以永远放心`C++`的运行效率)。

---

## 3. 更新记录

| 更新日期   | 版本号                | 主要更新内容                                                 |
| ---------- | --------------------- | ------------------------------------------------------------ |
| 2024.03.20 | Beta.2024.03.20.Mark0 | `first publish`                                              |
| 2024.03.21 | Beta.2024.03.21.Mark1 | **1.** 调整测试参数默认值, 提高测试效率.<br/>**2.** 更新`有效账号跟踪`功能, 程序在生成用户名列表阶段自动排除已验证的有效账户, 避免重复测试. |
| 2024.03.22 | Beta.2024.03.22.Mark2 | **1.** 更新`无效密码跟踪`功能, 程序会自动记录测试中某特定用户名尝试过的无效密码，以确保在后续对同一用户名的测试中排除这些已知的无效密码，从而避免重复测试并提高测试效率。 |
| 2024.03.23 | Beta.2024.03.22.Mark3 | **1.** 优化测试流程, 通过在密码测试前完成所有`无效密码跟踪`工作, 提高测试效率.<br/>**2.** 修复若干Bug, 及时释放无用内存, 改善控制台信息显示. |
| 2024.03.26 | Dev.2024.03.26.Mark4  | **1.** 我们为用户名生成器引入了一种新的、高效且具备自优化特性的生成模式：`multiSeedsGenerate`（多种子模式）。这种方法首先插入所有有效的用户名到种子集合中，然后基于这些中心点（种子），在每个中心点的周围生成新用户名。 |
| 2024.03.27 | Dev.2024.03.26.Mark5  | **1.**  引入新测试参数`maxInvalidAttempts`（最大无效密码尝试次数），如果对某个用户名尝试的密码数量超过设定的阈值而未找到有效密码，程序将停止对该用户名的密码尝试。通过设置该参数，用户可以过滤掉那些密码极难破解的账户。例如，若对一个用户名尝试了100,000个不同的密码仍未成功，这表明该密码的强度和复杂度极高，破解的可能性极低，此时应停止尝试，以避免进一步的无效努力。 |
| 2024.04.07 | Dev.2024.03.26.Mark6  | **1. 引入了一些密码学工具：**<br />**`字符串强度评估：`** 根据`字符串长度`、`字符多样性`、`重复性`和`熵`来评估密码强度。<br />**`字符串相似度评估：`** 通过`莱文斯坦距离`、`N-gram相似度`和`余弦相似度`来评价两个密码的相似度。<br />**2. 引入适应度函数：** 计算给定密码在字符串集合中的适应度得分。适应度函数是**`遗传算法`**的核心，意味着我们计划使用遗传算法来优化密码字典。程序每次运行均采用增量式数据保存，允许我们积累大量无效密码。利用这些无效密码作为演化目标，对密码字典进行迭代演化。找到可能的收敛迭代数后，可以利用此密码群体排除字典中的无效密码，从而保留有效密码，提高测试效率。 |

---

## 4. 使用指南

本节将详细介绍 `WIFI_Test 类`的使用方法和测试参数设置，旨在提供一个高级概述而非深入底层代码细节。本部分将解释代码的使用方法，而非编译后程序的操作指南。请自行编译程序。若需编译好的可执行文件，请留意我们将发布的发行版控制台程序。

### 4.1 测试参数介绍

下表列出了在使用`WIFI_Test`类进行WIFI账户测试时可配置的测试参数及其默认值。

| 数据类型        | 测试参数名           | 描述                                                         | 默认值                       |
| --------------- | -------------------- | ------------------------------------------------------------ | ---------------------------- |
| `string`        | `gen_mode`           | 指定用户名列表的生成模式：<br/>`"r"`：随机模式<br/>`"s"`：顺序模式<br/>`"ir"`：增量随机模式<br/>`"m"`：多种子模式 | `"m"`                        |
| `uint32_t`      | `pwdTryNum`          | 指定密码尝试次数，即对于一个用户名从密码字典中抽取密码进行尝试的次数 | `100`                        |
| `uint32_t`      | `amount`             | 指定期望生成的用户名数量                                     | `1000`                       |
| `uint16_t`      | `maxInvalidAttempts` | 最大无效密码尝试次数，如果对某个用户名尝试的密码数量超过设定的阈值而未找到有效密码，程序将停止对该用户名的密码尝试。 | `200`                        |
| `set<uint64_t>` | `usernamesSeed`      | 用于生成用户名的`种子集合`, 通常是使用两个或多个已知有效的用户名作为种子(注意: 不能少于两个种子) | `{ 8143086109, 9390026092 }` |
| `uint64_t`      | `maxDecrement`       | 在增量随机模式下有效，指定生成用户名时随机递减值的最大限度   | `8`                          |
| `uint16_t`      | `minDelayMs`         | 爬虫随机延时机制: 最小延时时间(毫秒)                         | `0`                          |
| `uint16_t`      | `maxDelayMs`         | 爬虫随机延时机制: 最大延时时间(毫秒)                         | `0`                          |
| `string`        | `usernamePrefix`     | 指定用户名前缀(取决于运营商规则)                             | `"043111"`                   |
| `string`        | `initialPassword`    | 设置账户的初始密码(取决于校园网的默认配置)                   | `"000000"`                   |

- **随机模式**：在种子集合最值范围内生成新的随机用户名(随机用户名不会重复)。
- **顺序模式**：按顺序生成新的用户名，从种子集合的最大值开始，逐步递减至种子集合的最小值。
- **增量随机模式**：从种子集合的最大值开始，在指定范围内生成新的增量随机用户名。每次生成时，数值会以随机递减值减少，直至达到种子集合的最小值或超过用户名生成数量`amount`的限制。`maxDecrement`参数用于控制递减值的最大限度。
- **多种子模式**：以每一个种子为中心点, 并在每个中心点的附近顺序生成新用户名。这是目前最高效的用户名生成方式。

---


### 4.2 测试参数设置建议

1. **用户名列表的生成模式`gen_mode`**：推荐使用多种子模式或顺序模式。多种子模式通常可以使测试更高效。由于随机模式效率较低，故不推荐使用。
2. **密码尝试次数`pwdTryNum`**：此参数影响每个用户名下的测试耗时。过高的尝试次数会增加单个账户的测试时长，从而降低整体测试效率。
3. **用户名生成数量`amount`**：测试所需时间与生成的用户名数量直接相关；用户名越多，所需时间越长。由于多种因素影响此参数，用户设置的是一个预期数量，而非确切值。
4. **最大无效密码尝试次数`maxInvalidAttempts`**：该值的的设置应根据用户提供的字典质量而定。高质量的字典能迅速找到有效密码。通常建议将该值设置得偏低一些(200次左右), 如果设置得过高可能导致在单一用户名上耗费过多时间, 降低测试效率。
5. **种子集合`usernamesSeed`**：种子数量越多以及种子间的数值跨度越大，测试效率越高。 考虑到CCDX-WIFI用户名的数值与学生年级正相关，建议设置种子集合时至少包括一个较低年级和一个较高年级的用户名，以覆盖更广的潜在有效账户。在设置种子时，请确保去除了用户名前缀`"043111"`。
6. **最大增量`maxDecrement`**：仅在增量随机模式下有效。较大的数值意味着用户名之间可能存在更大的跳跃，这可能会导致一些有效账号被遗漏。
7. **最小延时时间`minDelayMs`与最大延时时间`maxDelayMs`**：目前`CCDX-WIFI`没有反爬虫机制, 建议设置为`0`毫秒。
8. **用户名前缀`usernamePrefix`**：对于CCDX-WIFI直接使用默认值`"043111"`即可,不需要修改.
9. **初始密码`initialPassword`**：对于CCDX-WIFI直接使用默认值`"000000"`即可,不需要修改.

---

### 4.3 密码字典的格式要求

为确保密码字典能被程序正确导入并在测试中有效应用，对字典的格式有严格的要求。我们规定的格式非常简明，以便于操作和处理。具体来说，字典文件应为`CSV`格式，且仅第一列包含数据，每行代表一个独立的密码项。确保其他列留空，以避免导入过程中的任何混淆或错误。以下展示了两个遵循此格式要求的示例，以供参考：

#### **示例 1**：

|       |    **A**    | **B** | **C** | **D** | **…** |
| :---: | :---------: | :---: | :---: | :---: | :---: |
| **1** |  123456789  |       |       |       |       |
| **2** |   000000    |       |       |       |       |
| **3** |   62656d    |       |       |       |       |
| **4** |   a123456   |       |       |       |       |
| **5** |   123456    |       |       |       |       |
| **…** | . . . . . . |       |       |       |       |

该示例清晰展示了一个符合格式要求的CSV文件视图，每个密码独占一行，仅在第一列中呈现。

#### **示例 2**：

```latex
123456789\n000000\n62656d\n123456\n......
```

此例展示了一个纯文本表示的密码列表，每个密码之间通过换行符`\n`分隔，模拟了CSV文件中每行第一列的数据排列。

---

### 4.4 密码字典的导入方法

为了确保密码字典能够被本程序有效地导入并应用，我们设计了一个简单导入流程，适用于程序的不同运行方式。具体的导入步骤取决于您运行程序的方式，以下为两种常见场景的导入指导：

- **直接运行已编译好的可执行文件**：首先，需要将您的密码字典文件重命名为`Dictionary.csv`。随后，把重命名后的文件放置于本程序的可执行文件所在的根目录下。这样，当程序运行时，它会自动寻找并加载这个根目录下的`Dictionary.csv`文件，从而实现密码字典的导入。

![import_dictionary_exe](./IMG/import_dictionary_exe.png)

- **通过Visual Studio 2022进行Debug时**：在这种开发环境下，您同样需要将密码字典文件重命名为`Dictionary.csv`。然而，与直接运行可执行文件不同，您应将这个重命名后的文件放入项目目录中——即源代码文件(`.cpp`、`.hpp`等)所在的目录。这一步骤确保了，在使用Visual Studio进行调试时，程序能够正确地找到并加载密码字典。

![import_dictionary_debug](./IMG/import_dictionary_debug.png)

无论采取哪种方式导入密码字典，关键在于确保`Dictionary.csv`文件位于程序可以正确访问的路径。

---

### 4.5 使用示例

本节通过两个示例展示如何利用`WIFI_Test`类执行WIFI账户有效性测试。首个示例采用默认参数执行测试，第二个示例则演示了如何根据特定需求使用自定义测试参数。

- #### 使用默认测试参数运行测试

下方代码示例演示了如何利用`WIFI_Test`类的默认参数来初始化和执行WIFI账户有效性测试。此方法不需手动配置任何参数。

```c++
int main() {
    // 利用默认参数创建WIFI_Test类的实例
    WIFI_Test test;
    // 执行runTests方法以进行测试
    test.runTests();
    
    return 0;
}
```

- #### 使用自定义测试参数运行测试

`WIFI_Test`类的构造函数支持通过明确指定参数来自定义测试配置。

构造函数参数的定义如下：

```c++
    explicit WIFI_Test(const std::string gen_mode = "m",                                     
                       const uint32_t pwdTryNum = 100,                                       
                       const uint32_t amount = 1000,                                           
                       const uint16_t maxInvalidAttempts = 200,                               
                       const std::set<uint64_t> usernamesSeed = { 8143086109, 9390026092 },   
                       const uint64_t maxDecrement = 8,                                       
                       const uint16_t minDelayMs = 0,                                         
                       const uint16_t maxDelayMs = 0,                                         
                       const std::string &usernamePrefix = "043111",                         
                       const std::string &initialPassword = "000000") :                       
        gen_mode(gen_mode),
        pwdTryNum(pwdTryNum),
        amount(amount),
        maxInvalidAttempts(maxInvalidAttempts),
        usernamesSeed(usernamesSeed),
        maxDecrement(maxDecrement),
        minDelayMs(minDelayMs),
        maxDelayMs(maxDelayMs),
        usernamePrefix(usernamePrefix),
        initialPassword(initialPassword) {}
```

以下代码示例展示了如何根据自定义参数创建`WIFI_Test`实例并执行测试：

```C++
int main() {
    // 根据自定义参数创建WIFI_Test类的实例
    // 示例设定：使用多种子模式("m")，密码尝试次数为100次, 
    // 生成用户名数量约为5000个,最大无效密码尝试次数为100个, 设置3个有效用户名为种子;
     WIFI_Test test("m", 100, 5000, 100, { 8143086109, 9390026092, 9490026082 });
    // 执行runTests方法以进行测试
    test.runTests();
    return 0;
}
```

自定义测试参数允许用户针对特定的测试需求调整测试的细节和覆盖范围，使得WIFI账户的有效性测试过程更加灵活。通过合理选择和配置这些参数，可以有效地提升测试效率和成功率。

---
## 5. 主要工作原理(执行一次登录操作的流程)

为了实现对 CCDX-WIFI 的登录，假定我们已经拥有了一个有效的用户名 `username` 和密码 `password`。登录过程主要包括以下几个步骤：

1. **获取当前时间戳**：首先，程序会获取当前的时间戳，记为 `timestamp`。这一步是确保请求的时效性的关键。

2. **密码加密**：程序使用 RC4 加密算法，并以 `timestamp` 作为密钥，对登录密码 `password` 进行加密，得到加密后的密码 `encryptedPassword`。这一过程增加了密码传输的安全性。

3. **构建登录请求数据**：接着，程序构建登录请求的数据结构。该数据结构包括操作码（表示进行登录操作）、用户名、加密后的密码、是否记住密码的标志，以及用于验证请求时效性的时间戳。

```C++
cpr::Payload payload{
     {"opr", "pwdLogin"},          // 操作码，标识进行登录操作。
     {"userName", username},       // 登录所用的用户名。
     {"pwd", encryptedPassword},   // 经RC4算法加密后的密码。
     {"rememberPwd", "1"},         // 是否记住密码，此处默认为"1"，表示告知系统记住密码。
     {"auth_tag", timestamp}       // 时间戳，用于验证请求的时效性（注意：此时间戳需与加密密码所用时间戳一致）。
};
```

4. **发送登录请求**：随后，程序以构建好的登录数据向服务器发送登录（POST）请求。请求的 URL 和 HTTP 头部信息如下所示：
   - URL: 
   
     ```C++
     "http://1.1.1.2/ac_portal/login.php"
     ```
   
     
   - HTTP头部信息：
   
     ```C++
     {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36"}
     ```
   
     
   
5. **解析服务器响应**：最后，程序解析服务器的响应数据，判断登录操作是否成功。

   - 成功的响应示例：
   
     ```C++
     {"success":true, "msg":"logon success", "action":"location", "pop":0,"userName":"0431219790045971", "location":"http://1.1.1.2/homepage/mobile_detail.html"}
     ```
   
     
   - 失败的响应示例：
   
     ```C++
     {"success":false, "msg":"鐢ㄦ埛鍚嶆垨瀵嗙爜閿欒", "action":"location", "pop":0,"userName":"0431119390025542", "location":""}
     ```
   
     

---

## 6. Q&A FAQ

### 为什么测试效率低?

虽然我们采取了众多策略以提升测试效率，但“测试”的本质工作机制本身却是低效的。因此，我们才引入了密码字典作为提高测试效率的关键。正确选择字典能显著提高测试速度。我们建议用户寻找适合的字典，并避免使用包含复杂密码的字典，因为这将大幅降低破解正确密码的几率。建议优先考虑弱密码策略。

---

## 目标与应用

CCDX-WIFI账号密码测试工具旨在提醒和教育用户关注个人信息安全，尤其是在频繁使用校园网的环境中。通过揭示弱密码的风险，本工具助力提升校园网整体安全水平，降低账号被盗风险。

此外，该工具为校园网管理员提供强大辅助，帮助识别和处理存在安全风险的账户，维护校园网络环境健康与安全。

## 使用须知

CCDX-WIFI账号密码测试程序仅供教育、学术研究和网络安全加固使用。使用本工具进行测试时，请确保有权进行相关操作，并遵守相关法律法规。我们强烈反对任何形式的非法入侵活动，对于因滥用工具而引起的任何法律问题，使用者将承担全部责任。
