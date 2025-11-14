#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include <string>

#include "../CauTrucDuLieu/MangDong.h"

// Node của cây BST
template <typename T>
struct BSTNode
{
    T data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(const T &value) : data(value), left(nullptr), right(nullptr) {}
};

// Binary Search Tree
template <typename T>
class BinarySearchTree
{
private:
    BSTNode<T> *root;

    // Hàm đệ quy thêm node
    BSTNode<T> *insertRecursive(BSTNode<T> *node, const T &value)
    {
        if (node == nullptr)
        {
            return new BSTNode<T>(value);
        }

        if (value < node->data)
        {
            node->left = insertRecursive(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = insertRecursive(node->right, value);
        }

        return node;
    }

    // Hàm đệ quy tìm kiếm
    BSTNode<T> *searchRecursive(BSTNode<T> *node, const T &value) const
    {
        if (node == nullptr || node->data == value)
        {
            return node;
        }

        if (value < node->data)
        {
            return searchRecursive(node->left, value);
        }
        return searchRecursive(node->right, value);
    }

    // Hàm tìm node nhỏ nhất
    BSTNode<T> *findMin(BSTNode<T> *node) const
    {
        while (node && node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    // Hàm đệ quy xóa node
    BSTNode<T> *deleteRecursive(BSTNode<T> *node, const T &value)
    {
        if (node == nullptr)
            return node;

        if (value < node->data)
        {
            node->left = deleteRecursive(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = deleteRecursive(node->right, value);
        }
        else
        {
            // Node cần xóa tìm thấy
            if (node->left == nullptr)
            {
                BSTNode<T> *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                BSTNode<T> *temp = node->left;
                delete node;
                return temp;
            }

            // Node có 2 con
            BSTNode<T> *temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteRecursive(node->right, temp->data);
        }
        return node;
    }

    // Hàm đệ quy duyệt inorder
    void inorderRecursive(BSTNode<T> *node, MangDong<T> &result) const
    {
        if (node != nullptr)
        {
            inorderRecursive(node->left, result);
            result.push_back(node->data);
            inorderRecursive(node->right, result);
        }
    }

    // Hàm đệ quy duyệt preorder
    void preorderRecursive(BSTNode<T> *node, MangDong<T> &result) const
    {
        if (node != nullptr)
        {
            result.push_back(node->data);
            preorderRecursive(node->left, result);
            preorderRecursive(node->right, result);
        }
    }

    // Hàm đệ quy duyệt postorder
    void postorderRecursive(BSTNode<T> *node, MangDong<T> &result) const
    {
        if (node != nullptr)
        {
            postorderRecursive(node->left, result);
            postorderRecursive(node->right, result);
            result.push_back(node->data);
        }
    }

    // Hàm xóa toàn bộ cây
    void destroyTree(BSTNode<T> *node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Hàm đếm số node
    int countNodes(BSTNode<T> *node) const
    {
        if (node == nullptr)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Hàm tính chiều cao
    int heightRecursive(BSTNode<T> *node) const
    {
        if (node == nullptr)
            return 0;
        int leftHeight = heightRecursive(node->left);
        int rightHeight = heightRecursive(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

public:
    // Constructor
    BinarySearchTree() : root(nullptr) {}

    // Copy constructor (Rule of Three)
    BinarySearchTree(const BinarySearchTree &other) : root(nullptr)
    {
        if (other.root != nullptr)
        {
            MangDong<T> items = other.inorderTraversal();
            for (int i = 0; i < items.size(); i++)
            {
                insert(items[i]);
            }
        }
    }

    // Assignment operator (Rule of Three)
    BinarySearchTree &operator=(const BinarySearchTree &other)
    {
        if (this != &other)
        {
            // Xóa cây hiện tại
            destroyTree(root);
            root = nullptr;

            // Copy từ other
            if (other.root != nullptr)
            {
                MangDong<T> items = other.inorderTraversal();
                for (int i = 0; i < items.size(); i++)
                {
                    insert(items[i]);
                }
            }
        }
        return *this;
    }

    // Destructor
    ~BinarySearchTree()
    {
        destroyTree(root);
    }

    // Thêm phần tử
    void insert(const T &value)
    {
        root = insertRecursive(root, value);
    }

    // Tìm kiếm phần tử
    bool search(const T &value) const
    {
        return searchRecursive(root, value) != nullptr;
    }

    // Xóa phần tử
    void remove(const T &value)
    {
        root = deleteRecursive(root, value);
    }

    // Duyệt cây theo thứ tự (inorder)
    MangDong<T> inorderTraversal() const
    {
        MangDong<T> result;
        inorderRecursive(root, result);
        return result;
    }

    // Duyệt cây theo preorder
    MangDong<T> preorderTraversal() const
    {
        MangDong<T> result;
        preorderRecursive(root, result);
        return result;
    }

    // Duyệt cây theo postorder
    MangDong<T> postorderTraversal() const
    {
        MangDong<T> result;
        postorderRecursive(root, result);
        return result;
    }

    // Kiểm tra cây rỗng
    bool isEmpty() const
    {
        return root == nullptr;
    }

    // Đếm số node
    int size() const
    {
        return countNodes(root);
    }

    // Tính chiều cao cây
    int height() const
    {
        return heightRecursive(root);
    }

    // Xóa toàn bộ cây
    void clear()
    {
        destroyTree(root);
        root = nullptr;
    }
};

#endif // BINARYSEARCHTREE_H
