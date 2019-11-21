// Дано число n < 10^6 и последовательность целых чисел из [-2^31...2^31] длиной
// n. Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если
// root->Key <= K, то узел K добавляется в правое поддерево root; иначе - в
// левое поддерево root. Рекурсия запрещена.
//
// 6_1. Выведите элементы в порядке pre-order (сверху вниз).


// Время работы: O(n * log(n))
// Потребляемая память: O(n)


#include <iostream>
#include <stack>


using std::cin;
using std::cout;
using std::stack;


struct TreeNode {
    int value;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(int _value) : left(nullptr), right(nullptr),
                                    value(_value) {}
};


class Tree {
private:
    TreeNode *root;

    static void PreOrderTraversalRecursive(TreeNode *node);

    static void PreOrderTraversalIterative(TreeNode *node);

    static void PostOrderDestructorRecursive(TreeNode *node);

    static void PreOrderDestructorIterative(TreeNode *node);

public:
    Tree() : root(nullptr) {}

    ~Tree();

    void Print() const;

    void Add(int value);
};


// Время работы: O(n)
Tree::~Tree() {
//    PostOrderDestructorRecursive(root);
    PreOrderDestructorIterative(root);
}


// Время работы: O(n)
void Tree::PostOrderDestructorRecursive(TreeNode *node) {
    if (!node) return;

    PostOrderDestructorRecursive(node->left);
    PostOrderDestructorRecursive(node->right);
    delete node;
}


// Время работы: O(n)
void Tree::PreOrderDestructorIterative(TreeNode *node) {
    if (!node) return;

    stack<TreeNode *> node_stack;
    node_stack.push(node);

    while (!node_stack.empty()) {
        node = node_stack.top();
        node_stack.pop();

        if (node->right)
            node_stack.push(node->right);

        if (node->left)
            node_stack.push(node->left);

        delete node;
    }
}


// Время работы: O(n)
void Tree::Print() const {
//    PreOrderTraversalRecursive(root);
    PreOrderTraversalIterative(root);
}


// Время работы: O(n)
void Tree::PreOrderTraversalRecursive(TreeNode *node) {
    if (!node) return;

    cout << node->value << " ";
    PreOrderTraversalRecursive(node->left);
    PreOrderTraversalRecursive(node->right);
}


// Время работы: O(n)
void Tree::PreOrderTraversalIterative(TreeNode *node) {
    if (!node) return;

    stack<TreeNode *> node_stack;
    node_stack.push(node);

    while (!node_stack.empty()) {
        node = node_stack.top();
        node_stack.pop();
        cout << node->value << " ";

        if (node->right)
            node_stack.push(node->right);

        if (node->left)
            node_stack.push(node->left);
    }
}


// Среднее время работы: O(log(n))
void Tree::Add(int value) {
    if (!root) {
        root = new TreeNode(value);

        return;
    }

    TreeNode *current = root;
    while (current) {
        if (value >= current->value) {
            if (current->right) {
                current = current->right;
            } else {
                current->right = new TreeNode(value);
                break;
            }
        } else {
            if (current->left) {
                current = current->left;
            } else {
                current->left = new TreeNode(value);
                break;
            }
        }
    }
}


int main() {
    int n = 0;
    cin >> n;

    Tree tree;
    int value = 0;
    for (int i = 0; i < n; i++) {
        cin >> value;
        tree.Add(value);
    }

    tree.Print();

    return 0;
}
