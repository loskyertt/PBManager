#ifndef __TREE_H__
#define __TREE_H__

#include <functional>
#include <iostream>
#include <vector>

template <typename Key, typename Value>
struct AVLNode {
  Key key;
  Value data;
  int height;
  AVLNode *left, *right;

  AVLNode(const Key &k, const Value &v)
      : key(k), data(v), height(1), left(NULL), right(NULL) {}
};

template <typename Key, typename Value>
class AVLTree {
 public:
  typedef std::function<bool(const Key &k1, const Key &k2)> fuzzy_func;

 private:
  AVLNode<Key, Value> *root;

  int height(AVLNode<Key, Value> *node);
  int getBalance(AVLNode<Key, Value> *node);
  AVLNode<Key, Value> *rotateRight(AVLNode<Key, Value> *y);
  AVLNode<Key, Value> *rotateLeft(AVLNode<Key, Value> *x);
  AVLNode<Key, Value> *insert(AVLNode<Key, Value> *node, const Key &key,
                              const Value &value);
  AVLNode<Key, Value> *minValueNode(AVLNode<Key, Value> *node);
  AVLNode<Key, Value> *deleteNode(AVLNode<Key, Value> *root, const Key &key);
  Value &findValue(AVLNode<Key, Value> *node, const Key &key);
  AVLNode<Key, Value> *_M_find(AVLNode<Key, Value> *node, const Key &key);
  int _M_getTotalNodes(AVLNode<Key, Value> *node);
  std::vector<Value> _M_findValues(AVLNode<Key, Value> *node, const Key &key,
                                   std::vector<Value> &matches,
                                   const fuzzy_func &f);

  int time = 0;

 public:
  std::function<bool(const Key &k1, const Key &k2)> fuzzy_ptr = nullptr;

 public:
  AVLTree() : root(nullptr) {}

  void insert(const Key &key, const Value &value);
  void remove(const Key &key);
  void displayInOrder();

  Value &operator[](const Key &key) { return findValue(root, key); }

  bool find(const Key &key) { return (_M_find(root, key) != nullptr); }

  size_t length() { return _M_getTotalNodes(root); }

  auto getFuzzyFunc() { return fuzzy_ptr; }

  void setFuzzyFunc(
      const std::function<bool(const Key &k1, const Key &k2)> ptr) {
    std::cout << "set fuzzy func success\n";
    this->fuzzy_ptr = ptr;
    time++;
  }

  std::vector<Value> findValues(const Key &k, const fuzzy_func &ptr) {
    std::vector<Value> matches;
    _M_findValues(root, k, matches, ptr);
    return matches;
  }
};

template <typename Key, typename Value>
std::vector<Value> AVLTree<Key, Value>::_M_findValues(
    AVLNode<Key, Value> *node, const Key &key, std::vector<Value> &matches,
    const AVLTree<Key, Value>::fuzzy_func &ptr) {
  if (node == nullptr)
    return matches;

  if (ptr != nullptr && ptr(node->key, key)) {
    matches.push_back(node->data);
  }

  _M_findValues(node->left, key, matches, ptr);
  _M_findValues(node->right, key, matches, ptr);

  return matches;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::_M_getTotalNodes(AVLNode<Key, Value> *node) {
  if (node == nullptr) {
    return 0;
  }

  // 递归计算左子树和右子树的节点数量，加上当前节点
  return 1 + _M_getTotalNodes(node->left) + _M_getTotalNodes(node->right);
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::_M_find(AVLNode<Key, Value> *node,
                                                  const Key &key) {
  if (node == nullptr) {
    return nullptr;
  }

  if (key == node->key) {
    return node;
  }

  if (key < node->key) {
    return _M_find(node->left, key);
  }

  return _M_find(node->right, key);
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value> *node) {
  return node ? node->height : 0;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::getBalance(AVLNode<Key, Value> *node) {
  return node ? height(node->left) - height(node->right) : 0;
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value> *y) {
  AVLNode<Key, Value> *x = y->left;
  AVLNode<Key, Value> *T2 = x->right;

  // 右旋
  x->right = y;
  y->left = T2;

  // 更新高度
  y->height = std::max(height(y->left), height(y->right)) + 1;
  x->height = std::max(height(x->left), height(x->right)) + 1;

  return x;
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value> *x) {
  AVLNode<Key, Value> *y = x->right;
  AVLNode<Key, Value> *T2 = y->left;

  // 左旋
  y->left = x;
  x->right = T2;

  // 更新高度
  x->height = std::max(height(x->left), height(x->right)) + 1;
  y->height = std::max(height(y->left), height(y->right)) + 1;

  return y;
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::insert(AVLNode<Key, Value> *node,
                                                 const Key &key,
                                                 const Value &value) {
  if (!node) {
    return new AVLNode<Key, Value>(key, value);
  }

  // 执行标准 BST 插入
  if (key < node->key) {
    node->left = insert(node->left, key, value);
  } else if (key > node->key) {
    node->right = insert(node->right, key, value);
  } else {
    return node;
  }

  // 更新当前高度节点
  node->height = 1 + std::max(height(node->left), height(node->right));

  // 平衡系数
  int balance = getBalance(node);

  if (balance > 1 && key < node->left->key) {
    return rotateRight(node);
  }

  if (balance > 1 && key > node->left->key) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (balance < -1 && key > node->right->key) {
    return rotateLeft(node);
  }

  if (balance < -1 && key < node->right->key) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::minValueNode(
    AVLNode<Key, Value> *node) {
  AVLNode<Key, Value> *current = node;

  while (current->left) {
    current = current->left;
  }

  return current;
}

template <typename Key, typename Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::deleteNode(AVLNode<Key, Value> *root,
                                                     const Key &key) {
  if (!root) {
    return root;
  }

  // 标准 BST 删除操作
  if (key < root->key) {
    root->left = deleteNode(root->left, key);
  } else if (key > root->key) {
    root->right = deleteNode(root->right, key);
  } else {
    if (!root->left || !root->right) {
      AVLNode<Key, Value> *temp = root->left ? root->left : root->right;

      if (!temp) {
        temp = root;
        root = nullptr;
      } else {
        *root = *temp;
      }

      delete temp;
    } else {
      AVLNode<Key, Value> *temp = minValueNode(root->right);
      root->key = temp->key;
      root->data = temp->data;
      root->right = deleteNode(root->right, temp->key);
    }
  }

  if (!root) {
    return root;
  }

  root->height = 1 + std::max(height(root->left), height(root->right));

  int balance = getBalance(root);

  if (balance > 1 && getBalance(root->left) >= 0) {
    return rotateRight(root);
  }

  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }

  if (balance < -1 && getBalance(root->right) <= 0) {
    return rotateLeft(root);
  }

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }

  return root;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::insert(const Key &key, const Value &value) {
  root = insert(root, key, value);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key &key) {
  root = deleteNode(root, key);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::displayInOrder() {
  // 中序遍历AVL树
  std::vector<AVLNode<Key, Value> *> stack;
  AVLNode<Key, Value> *current = root;

  while (current || !stack.empty()) {
    while (current) {
      stack.push_back(current);
      current = current->left;
    }

    current = stack.back();
    stack.pop_back();

    std::cout << current->key << ": " << current->data << std::endl;

    current = current->right;
  }
}

template <typename Key, typename Value>
Value &AVLTree<Key, Value>::findValue(AVLNode<Key, Value> *node,
                                      const Key &key) {
  if (!node) {
    throw "not found";
  }

  if (node->key == key) {
    return node->data;
  }

  if (key < node->key) {
    return findValue(node->left, key);
  } else if (key > node->key) {
    return findValue(node->right, key);
  }
}

#endif