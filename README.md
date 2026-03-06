# 1. 桌面电话薄管理系统

> 选取一种已经学过的动态搜索树结构（BST、AVL、RBT、B-tree），设计并实现一个桌面电话簿软件。本项目中选用的是 AVL 树，以保证高效的查找、插入和删除操作。

## 1.1 系统功能

本桌面电话簿管理系统提供以下核心功能：

- **联系人管理**：添加、删除、编辑联系人信息
- **群组管理**：创建、删除、编辑联系人分组
- **数据导入/导出**：支持从文件导入联系人数据，导出联系人数据到文件
- **多维度查询**：支持按姓名、城市、电话、邮箱等多种方式查询联系人
- **模糊搜索**：支持姓名的拼音和简拼搜索
- **批量展示**：分页显示所有联系人信息

## 1.2 技术实现

### 1.2.1 数据结构

- **AVL 树**：作为底层数据结构，保证插入、删除和查找操作的时间复杂度为 $O(log n)$
- **TelephoneItem**：联系人信息类，存储联系人的详细信息
- **TelephoneDirect**：电话簿管理类，封装了对联系人的各种操作

### 1.2.2 算法思想

1. **AVL 树操作**：
   - **插入操作**：执行标准的二叉搜索树插入，然后根据平衡因子进行旋转操作以保持树的平衡
   - **删除操作**：执行标准的二叉搜索树删除，然后同样进行平衡调整
   - **查找操作**：利用二叉搜索树的特性进行高效查找
   - **模糊搜索**：通过遍历树的所有节点，使用自定义的模糊匹配函数来查找相似的键

2. **旋转操作**：
   - **左旋**：用于处理右右不平衡的情况
   - **右旋**：用于处理左左不平衡的情况
   - **左右旋**：用于处理左右不平衡的情况
   - **右左旋**：用于处理右左不平衡的情况

3. **模糊搜索实现**：
   - 支持基于字符串包含关系的模糊匹配
   - 支持姓名的拼音和简拼搜索，通过加载拼音映射表实现

### 1.2.3 系统架构

- **数据层**：使用 AVL 树存储联系人数据
- **业务层**：实现联系人管理、群组管理、数据导入导出等功能
- **界面层**：命令行交互界面，提供用户操作入口

## 1.3 核心特性

1. **高效的数据操作**：基于 AVL 树的数据结构，保证了高效的插入、删除和查找操作
2. **多维度查询**：支持按多种字段查询联系人信息
3. **智能搜索**：支持姓名的拼音和简拼搜索，提高用户体验
4. **数据持久化**：支持从文件导入和导出联系人数据
5. **群组管理**：支持将联系人分组管理，方便用户组织联系人

## 1.4 技术亮点

1. **AVL 树的实现**：完整实现了 AVL 树的插入、删除、查找等操作，并保证了树的平衡性
2. **模糊搜索**：通过自定义的模糊匹配函数，实现了灵活的搜索功能
3. **拼音搜索**：通过加载拼音映射表，实现了基于拼音的搜索功能
4. **模块化设计**：代码结构清晰，模块划分合理，便于维护和扩展

---

# 2. 项目结构

```bash
.
├── CMakeLists.txt
├── README.md
├── cmake_build_debug.sh
├── data
│   ├── 10000.txt
│   ├── 100000.txt
│   ├── 5000.txt
│   └── pinyin.txt
├── example
│   └── CMakeLists.txt
├── format_count.sh
├── include
│   ├── algori.hpp
│   └── utils.h
├── lib
│   ├── CMakeLists.txt
│   └── utils.cpp
├── src
│   ├── CMakeLists.txt
│   └── main.cpp
└── test
    ├── CMakeLists.txt
    ├── sub_foo
    │   └── sub_test_1.cpp
    ├── test_1.cpp
    └── test_2.cpp
```

---

# 3. CMake 优化指南

## 3.1 最小化 GLOB 操作

~~`file(GLOB ...)`~~

推荐：
```cmake
set(SOURCES
    main.cpp
    utiles/utiles.cpp
    ...
)

add_executable(myApp ${SOURCES})
```

>> - **CMake 文档明确指出：**
>> "We do not recommend using GLOB to collect a list of source files from your source tree. If no CMakeLists.txt file changes when a source is added or removed then the generated build system cannot know when to ask CMake to regenerate."

## 3.2 使用现代 CMake

~~`include_directories(${YoUR_DIRECToRY})`~~

~~`link_directories(${YOUR_DIRECTORY})`~~

推荐：
```cmake
target_include_directories(myLib PRIVATE include/)
target_link_libraries(myApp PRIVATE myLib)
```

## 3.3 使用 Ninja

> `Ninja`的构建速度很快。

如果采用的是 LLVM：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

如果采用的是 GNU：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

如果采用的是 MSVC：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

## 3.4 指定目标编译

如果只想编译 `src` 目录下的代码文件：

```bash
cmake --build build --target my_program -j12
```

> 这里的 `my_program` 与 `src/CMakeLists.txt` 下的 `add_executable(my_program ${ALL_SRCS})` 相对应。

---

# 4. 一些问题

> 下面是可能在编写代码时遇到的问题，以及对应的解决办法。

## 4.1 Windows 下编码问题

> Windows 的控制台编码一般采用的是 GB2312 编码的，但源代码编码采用的是 UTF-8 编码（不建议把源代码文件编码改为 GB2312 格式！），所以当有中文在控制台输出时，会产生乱码。解决方法如下：

- 方式一：如果使用的是 **cmd**，在每次从终端执行可编译好的程序之前，执行：

```shell
chcp 65001
```

执行完后，再执行程序就不会乱码。

- 方式二：如果使用的是 **PowerShell**，可以修改配置文件：

打开配置文件（如果没有的话，会创建）：

```shell
notepad $PROFILE
```

在配置文件中写入：

```shell
# 设置默认编码为 UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[Console]::InputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# 设置环境变量，让子进程也使用 UTF-8
$env:PYTHONIOENCODING = "utf-8"
```

修改完毕后，重启 PowerShell 即可。

如果是在 VS 中运行程序，因为默认打开的是 cmd，所以建议在代码 `main` 函数开头添加：

```cpp
int main(){
  // 设置控制台为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  
  // 其它代码......
}
```

如果采用 MSVC 编译，需要在 `CMakeLists.txt` 中给目标程序添加编译选项：

```cmake
# MSVC UTF-8 支持
if(MSVC)
  target_compile_options(my_app PRIVATE /utf-8)
endif()
```

## 4.2 无法找到头文件

> 在 **VSCode + clangd** 配置下，可能会出现无法找到标准库头文件的情况，导致 clangd 报错，但是编译是可以通过的。一般单独分别安装 llvm 和 mingw 时，会出现这个错误。这个错误是因为 **clangd 无法找到 MinGW 的标准库头文件路径**。虽然你生成了 `compile_commands.json`，但里面的编译器是 MinGW 的 `g++.exe`，而 clangd 需要知道这些头文件在哪里。

其问题根源是 clangd 和 MinGW 是两个不同的工具链：
- **clangd** 基于 LLVM/Clang，默认在 MSVC 或自身 libc++ 环境下查找头文件
- **MinGW** 使用 GCC 的 libstdc++，头文件路径不同

### 4.2.1 方案 1：使用集成有 llvm 的 mingw

下载：[winlibs](https://winlibs.com/)

选择 **UCRT runtime** 版并且包含有 **LLVM/Clang/LLD/LLDB** 的下载。

### 4.2.2 方案 2：配置 clangd 参数

> 比如我这里使用的是 CLion 集成的 mingw。

在 VS Code 的 `settings.json` 中添加：

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build", // 指定 compile_commands.json 所在目录
        "--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe", // 查询指定编译器的系统 include 路径
        "--background-index", // 后台索引整个项目，加速代码跳转
        "--header-insertion=iwyu", // 自动插入头文件时遵循 "Include What You Use" 原则
        "--completion-style=bundled", // 代码补全风格：打包式
        "--pch-storage=memory", // 预编译头文件存储在内存中
        "--cross-file-rename", // 启用跨文件重命名功能
        "--enable-config", // 允许从 .clangd 文件读取配置
    ],
}
```

**关键参数**：
- `--query-driver`：告诉 clangd 查询指定编译器的系统 include 路径

再在项目根目录下创建 `.clangd` 文件，在里面手动指定 MinGW 的 C++ 标准库路径：

```yaml
CompileFlags:
  Add:
    - -std=c++20
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++/x86_64-w64-mingw32
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/x86_64-w64-mingw32/include
  Compiler: C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe
```

> 把上面的路径改成你自己的。

### 4.2.3 方案 3：改用 Clang 编译器

可以尝试直接用 Clang 编译，避免工具链混用：

```bash
# 在 CMake 配置时指定 Clang
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

确保 `clang++` 能找到 MinGW 的 libstdc++，或使用 Clang 自带的 libc++。

## 4.3 使用 MSVC 编译器

> 有时候不可避免得要用到 MSVC 编译器，因为网上有不少预编译好的 Windows 第三方库，都是用的 MSVC 编译器。

下载后 VS 时（比如下载的是 VS 2022 Community），会自动安装 **Developer Command Prompt for VS 2022** 和 **Developer PowerShell for VS 2022**，这两个终端集成了 MSVC 编译器的环境变量。在该终端中使用指令 `where.exe cl` 会有如下输出：

```shell
PS C:\Apps\Microsoft Visual Studio\2022\Community> where.exe cl
C:\Apps\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe
```

打开 **Developer PowerShell for VS 2022**，然后用 `cd` 命令进入到项目根目录，输入：

```shell
# 打开 VSCode
code .

# 或者打开 CLion
clion64.exe .
```

这样打开的 IDE 就会继承  **Developer PowerShell for VS 2022** 中的环境。

> 前提是要把 VSCode 和 CLion 添加到环境变量中。这里建议使用  **Developer PowerShell for VS 2022** 而不是 **Developer Command Prompt for VS 2022**，因为前者的命令行功能更强大。

在编译时指定编译器：`-DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl`。

如果使用 MSVC 生成器（Visual Studio Generator），是不会生成 `compile_commands.json` 文件的：

```shell
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
```

因此，除非要使用 VS 进行开发，其他情况生成器请选择 Ninja 或者 Unix Makefiles。
