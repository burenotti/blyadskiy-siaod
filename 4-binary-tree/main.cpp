#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>


struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int value = 0;


    Node(int value, Node* left, Node* right) : value(value), left(left), right(right) {

    }

    explicit Node(int value) : Node(value, nullptr, nullptr) {
    }

    Node(const Node& source) {

        value = source.value;

        if (source.left)
            left = new Node(*source.left);

        if (source.right)
            right = new Node(*source.right);
    }

    Node& operator=(const Node& source) {
        if (&source == this)
            return *this;

        std::swap(*this, *new Node(source));
        return *this;
    }

    ~Node() {
        delete left;
        delete right;
    }


    [[nodiscard]]
    size_t height() const {
        auto left_height = (this->left) ? this->left->height() : 1;
        auto right_height = (this->right) ? this->right->height() : 1;
        return std::max(left_height, right_height) + 1;
    }

    template<class BidirIter>
    static Node* from_iter(BidirIter begin, BidirIter end) {
        if (begin == end) {
            return nullptr;
        }
        auto dist = distance(begin, end);
        Node* node = new Node(*begin);
        node->value = *begin;

        if (dist == 1)
            return node;

        std::advance(begin, 1);
        auto left_end = begin;
        std::advance(left_end, dist / 2);
        node->left = Node::from_iter(begin, left_end);
        node->right = Node::from_iter(left_end, end);
        return node;
    }
};


int count_even_numbers(Node* tree) {
    if (tree == nullptr)
        return 0;

    return count_even_numbers(tree->left) + count_even_numbers(tree->right) + (tree->value % 2 == 0);
}


void print_centered(std::ostream& out, const std::string& str, int width, char filler = ' ') {
    int align = (width - (int) str.size() + 1) / 2;
    out << std::setfill(filler) << std::setw(align) << filler << str << std::setw(align) << filler;
}

std::ostream& operator<<(std::ostream& out, const Node& node) {
    std::queue<const Node*> q;
    q.push(&node);
    int alignment = node.height();
    while (!q.empty()) {
        auto size = q.size();
        for (size_t i = 0; i < size; i++) {
            auto* current = q.front();
            q.pop();
            if (current != nullptr) {
                print_centered(out, std::to_string(current->value), std::pow(2, alignment));
                q.push(current->left);
                q.push(current->right);
            }
        }
        alignment--;
        out << std::endl;
    }
    return out;
}

void rotate(Node* tree) {
    if (tree) {
        std::swap(tree->left, tree->right);
        rotate(tree->left);
        rotate(tree->right);
    }
}

int main() {
    using namespace std;
    cout << "Введите количество элементов дерева: " << endl;
    int n;
    cin >> n;
    vector<int> items(n);
    cout << "Введите " << n << " чисел: " << endl;
    copy_n(istream_iterator<int>{cin}, n, items.begin());
    Node* tree = Node::from_iter(items.begin(), items.end());
    cout << "Построенное дерево: " << endl;
    cout << *tree;
    INPUT_OP:
    cout << "1. Сравнить количество четных чисел в левом и правом поддеревьях\n"
         << "2. Скопировать дерево\n"
         << "3. Вывести дерево справа налево\n"
         << "Введите номер операции [1-3]: ";
    int op;
    cin >> op;
    if (op == 1) {
        auto left = count_even_numbers(tree->left);
        auto right = count_even_numbers(tree->right);
        if (left > right) {
            cout << "В левом поддереве больше четных чисел\n";
        } else if (left < right) {
            cout << "В правом поддереве больше четных чисел\n";
        } else {
            cout << "В обоих деревьях одинаковое количество четных чисел\n";
        }

    } else if (op == 2) {
        Node tree_copy{*tree};
        cout << "Скопированное дерево:\n" << tree_copy;
    } else if (op == 3) {
        rotate(tree);
        cout << endl << *tree;
    } else {
        cout << "Неверный номер операции:";
        delete tree;
        return 0;
    }
    goto INPUT_OP;
}