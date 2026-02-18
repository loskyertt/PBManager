# 1.桌面电话薄管理系统

> 选取一种已经学过的动态搜索树结构（BST、AVL、RBT、B-tree），设计并实现一个桌面电话簿软件。本项目中选用的是 AVL。

## 1.1 算法思想

AVL 树用于存储键值对，可以保证插入、删除和查找的时间复杂度都是 $O(log n)$。根据给定的键和模糊函数来查找树中所有相似键对应的值，并将它们存储到向量中。再通过递归的方式来遍历树中的所有节点，并判断每个节点的键是否与给定的键相似，如果相似，就将节点的值添加到向量中。

AVL 数的插入、删除和查找的时间复杂度都是 $O(log n)$。

1. 执行标准的二叉搜索树的插入操作，根据给定的键和当前节点的键的大小关系，将给定的键值对插入到当前节点的左子树或右子树中，如果给定的键和当前节点的键相等，就不进行插入，直接返回当前节点，因为不允许有重复的键。

2. 根据平衡因子的值和给定的键和当前节点的子节点的键的大小关系，判断树是否失去平衡，如果失去平衡，就进行相应的旋转操作来恢复平衡，然后返回旋转后的节点。旋转操作有四种情况，分别是左左情况、左右情况、右右情况和右左情况，它们分别对应于平衡因子大于 `1` 或小于 `-1`，以及给定的键小于或大于当前节点的左子节点或右子节点的键的组合。旋转操作的具体实现在另外两个私有的成员函数 `rotateLeft` 和 `rotateRight` 中，它们分别用于进行左旋和右旋。


3. 删除操作：根据给定的键和当前节点的键的大小关系，将给定的键从当前节点的左子树或右子树中删除。剩余操作与插入操作一样。

---

# 2.说明

```bash
.
├── cmake_build_debug.sh
├── CMakeLists.txt
├── data
│   ├── 100000.txt
│   ├── 10000.txt
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
├── README.md
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

# 3.CMake 优化指南

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

## 3.2 使用 Ninja

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

`Ninja`的构建速度很快。

## 3.3 使用现代 CMake

~~`include_directories(${YOUR_DIRECTORY})`~~

~~`link_directories(${YOUR_DIRECTORY})`~~

推荐：
```cmake
target_include_directories(myLib PRIVATE include/)
target_link_libraries(myApp PRIVATE myLib)
```

## 3.4 指定目标编译

如果只想编译`src`目录下的代码文件：
```bash
cmake --build build --target my_program -j12
```

这里的`my_program`与`src/CMakeLists.txt`下的`add_executable(my_program ${ALL_SRCS})`相对应。