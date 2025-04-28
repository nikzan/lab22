#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

class Tree {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        Node(int val) : data(val), left(nullptr), right(nullptr) {}
    };

    Node* root;
    const int X_CODE = -10;

    Node* buildPostfixTree(const vector<string>& tokens) {
        stack<Node*> st;
        for (const string& token : tokens) {
            if (isdigit(token[0])) {
                st.push(new Node(stoi(token)));
            } else {
                Node* right = st.top(); st.pop();
                Node* left = st.top(); st.pop();
                int op;
                if (token == "+") op = -1;
                else if (token == "-") op = -2;
                else if (token == "*") op = -3;
                else throw invalid_argument("Unknown operator");
                Node* node = new Node(op);
                node->left = left;
                node->right = right;
                st.push(node);
            }
        }
        return st.empty() ? nullptr : st.top();
    }

    vector<string> tPostfix(const string& expr) {
        vector<string> tokens;
        stringstream ss(expr);
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    vector<string> tInfix(const string& expr) {
        vector<string> tokens;
        string current;
        for (char c : expr) {
            if (isspace(c)) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            } else if (isdigit(c)) {
                current += c;
            } else if (c == 'x') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back("x");
            } else if (string("+-*/%^()").find(c) != string::npos) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back(string(1, c));
            }
        }
        if (!current.empty()) {
            tokens.push_back(current);
        }
        return tokens;
    }

    vector<string> seqOper(const vector<string>& tokens) {
        vector<string> output;
        stack<string> op_stack;
        map<string, int> precedence = {
            {"^", 6}, {"*", 5}, {"/", 5}, {"%", 5},
            {"+", 4}, {"-", 4}
        };

        for (const string& token : tokens) {
            if (isdigit(token[0])) output.push_back(token);
            else if (token == "x") output.push_back(token);
            else if (token == "(") op_stack.push(token);
            else if (token == ")") {
                while (!op_stack.empty() && op_stack.top() != "(") {
                    output.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.pop();
            } else {
                while (!op_stack.empty() && op_stack.top() != "(" &&
                    (precedence[op_stack.top()] > precedence[token] ||
                     (precedence[op_stack.top()] == precedence[token]))) {
                    output.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.push(token);
            }
        }

        while (!op_stack.empty()) {
            output.push_back(op_stack.top());
            op_stack.pop();
        }

        return output;
    }

    Node* buildInfixTree(const vector<string>& postfix) {
        stack<Node*> st;
        for (const string& token : postfix) {
            if (token == "x") {
                st.push(new Node(X_CODE));
            } else if (isdigit(token[0])) {
                st.push(new Node(stoi(token)));
            } else {
                Node* right = st.top(); st.pop();
                Node* left = st.top(); st.pop();
                int op;
                if (token == "+") op = -1;
                else if (token == "-") op = -2;
                else if (token == "*") op = -3;
                else if (token == "/") op = -4;
                else if (token == "%") op = -5;
                else if (token == "^") op = -6;
                else throw invalid_argument("Unknown operator");
                Node* node = new Node(op);
                node->left = left;
                node->right = right;
                st.push(node);
            }
        }
        return st.empty() ? nullptr : st.top();
    }

    Node* rSubstr(Node* node) {
        if (!node) return nullptr;

        node->left = rSubstr(node->left);
        node->right = rSubstr(node->right);

        if (node->data == -2) {
            int leftVal = node->left->data;
            int rightVal = node->right->data;
            int result = leftVal - rightVal;
            delete node->left;
            delete node->right;
            return new Node(result);
        }
        return node;
    }

    void transformAdittion(Node* node) {
        if (!node) return;

        transformAdittion(node->left);
        transformAdittion(node->right);

        if (node->data == -1 && node->right && node->right->data == X_CODE) {
            swap(node->left, node->right);
        }
    }

    int evalSeq(Node* node, int x) {
        if (!node) return 0;
        if (node->data == X_CODE) return x;
        if (node->data >= 0) return node->data;

        int left = evalSeq(node->left, x);
        int right = evalSeq(node->right, x);

        switch (node->data) {
            case -1: return left + right;
            case -2: return left - right;
            case -3: return left * right;
            case -4: return left / right;
            case -5: return left % right;
            case -6: return (int)pow(left, right);
            default: throw invalid_argument("Unknown operator");
        }
    }

    void printPrefix(Node* node, ostream& out) {
        if (!node) return;
        if (node->data == X_CODE) out << "x ";
        else if (node->data < 0) out << getOperatorSymbol(node->data) << " ";
        else out << node->data << " ";
        printPrefix(node->left, out);
        printPrefix(node->right, out);
    }

    void printPostfix(Node* node, ostream& out) {
        if (!node) return;
        printPostfix(node->left, out);
        printPostfix(node->right, out);
        if (node->data == X_CODE) out << "x ";
        else if (node->data < 0) out << getOperatorSymbol(node->data) << " ";
        else out << node->data << " ";
    }

    void printInfix(Node* node, ostream& out) {
        if (!node) return;
        if (node->data >= 0 || node->data == X_CODE) {
            if (node->data == X_CODE) out << "x";
            else out << node->data;
            return;
        }
        out << "(";
        printInfix(node->left, out);
        out << " " << getOperatorSymbol(node->data) << " ";
        printInfix(node->right, out);
        out << ")";
    }

    string getOperatorSymbol(int data) {
        switch (data) {
            case -1: return "+";
            case -2: return "-";
            case -3: return "*";
            case -4: return "/";
            case -5: return "%";
            case -6: return "^";
            default: return "?";
        }
    }

public:
    Tree() : root(nullptr) {}
    ~Tree() { /* Реализация деструктора для освобождения памяти */ }

    void buildFromPostfix(const string& filename) {
        ifstream file(filename);
        string expr;
        getline(file, expr);
        vector<string> tokens = tPostfix(expr);
        root = buildPostfixTree(tokens);
    }

    void buildFromInfix(const string& filename) {
        ifstream file(filename);
        string expr;
        getline(file, expr);
        vector<string> tokens = tInfix(expr);
        vector<string> postfix = seqOper(tokens);
        root = buildInfixTree(postfix);
    }

    void removeSubtraction() {
        root = rSubstr(root);
    }

    void transformAddition() {
        transformAdittion(root);
    }

    int evaluate(int x) {
        return evalSeq(root, x);
    }

    void printToFile(const string& filename, int x) {
        ofstream out(filename);
        out << "Результат: " << evaluate(x) << endl;

        transformAddition();

        out << "Префиксная форма: ";
        printPrefix(root, out);
        out << endl;

        out << "Постфиксная форма: ";
        printPostfix(root, out);
        out << endl;

        out << "Инфиксная форма (со скобками) : ";
        printInfix(root, out);
        out << endl;
    }

    void PrintTree(int space = 0, int indent = 4) {
        if (root == nullptr) return;

        int height = getHeight(root);
        int width = (1 << height) * 2;

        vector<vector<pair<Node*, int>>> levels(height);
        queue<tuple<Node*, int, int>> q;
        q.push({root, 0, width / 2});

        while (!q.empty()) {
            auto [node, level, pos] = q.front();
            q.pop();

            levels[level].push_back({node, pos});

            if (node->left)
                q.push({node->left, level + 1, pos - width / (1 << (level + 2))});
            if (node->right)
                q.push({node->right, level + 1, pos + width / (1 << (level + 2))});
        }

        for (size_t i = 0; i < levels.size(); ++i) {
            sort(levels[i].begin(), levels[i].end(), [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

            if (i > 0) {
                string line(width, ' ');
                for (auto& [node, pos] : levels[i]) {
                    int parentPos = -1;
                    for (auto& [pnode, ppos] : levels[i-1]) {
                        if (pnode->left == node || pnode->right == node) {
                            parentPos = ppos;
                            break;
                        }
                    }

                    if (parentPos != -1) {
                        if (parentPos < pos) {
                            for (int j = parentPos + 1; j < pos; ++j)
                                line[j] = '_';
                            line[parentPos] = '/';
                        } else {
                            for (int j = pos + 1; j < parentPos; ++j)
                                line[j] = '_';
                            line[pos] = ' ';
                        }
                    }
                }
                cout << line << endl;
            }

            string nodeLine(width, ' ');
            for (auto& [node, pos] : levels[i]) {
                string nodeStr;
                if (node->data < 0 && node->data != X_CODE)
                    nodeStr = getOperatorSymbol(node->data);
                else if (node->data == X_CODE)
                    nodeStr = "x";
                else
                    nodeStr = to_string(node->data);

                for (size_t j = 0; j < nodeStr.size() && pos + j < width; ++j)
                    nodeLine[pos + j] = nodeStr[j];
            }
            cout << nodeLine << endl;
        }
    }

private:
    int getHeight(Node* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }
};

#endif // EXPRESSIONTREE_H