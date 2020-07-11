#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

size_t const INF = std::numeric_limits<size_t>::max();

struct edge {
    edge(size_t p, size_t c, size_t l, size_t r) : p(p), c(c), l(l), r(r) {}

    size_t p, c, l, r;
};

struct node {
    node(size_t l, size_t r, node *parent) : l(l), r(r), parent(parent), suffix_link(nullptr) {
        std::fill(next_char, next_char + 26, nullptr);
    }

    size_t get_len() {
        return r - l;
    }

 public:
    node *next_char[26];
    size_t l, r;
    node *parent, *suffix_link;
};

struct suffix_trie {
    suffix_trie() : offset(0), root(0, 0, nullptr), cur(&root) {}

    void add_char(char c) {
        text += c;
        node *prev = nullptr;
        while (true) {
            if (has_edge(c)) {
                if (prev != nullptr) {
                    prev->suffix_link = cur;
                }
                go(c);
                break;
            } else {
                if (is_real_vertex()) {
                    cur->next_char[c - 'a'] = new node(text.length() - 1, INF, cur);
                    if (prev != nullptr) {
                        prev->suffix_link = cur;
                    }
                    prev = nullptr;
                } else {
                    node *parent_ = cur->parent;
                    parent_->next_char[text[cur->l] - 'a'] = new node(cur->l, cur->l + offset,
                                                                      parent_);
                    node *split = parent_->next_char[text[cur->l] - 'a'];
                    cur->parent = split;
                    split->next_char[c - 'a'] = new node(text.length() - 1, INF, split);
                    split->next_char[text[cur->l + offset] - 'a'] = cur;
                    cur->l += offset;
                    if (prev != nullptr) {
                        prev->suffix_link = split;
                    }
                    prev = split;
                    cur = split;
                    offset = cur->get_len();
                }
                if (cur == &root) {
                    break;
                }
                get_suffix_link();
            }
        }
    }

    bool ask(std::string const &str) {
        node *a = &root;
        for (size_t i = 0; i < str.length();) {
            if (a->next_char[str[i] - 'a'] == nullptr) {
                return false;
            } else {
                a = a->next_char[str[i] - 'a'];
                for (size_t j = a->l; i < str.length() && j < a->r; i++, j++) {
                    if (text[j] != str[i]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

 private:
    bool has_edge(char c) {
        if (is_real_vertex()) {
            return cur->next_char[c - 'a'] != nullptr;
        } else {
            return text[cur->l + offset] == c;
        }
    }

    void go(char c) {
        if (is_real_vertex()) {
            cur = cur->next_char[c - 'a'];
            offset = 1;
        } else {
            offset++;
        }
    }

    bool is_real_vertex() {
        return offset == cur->get_len();
    }

    void get_suffix_link() {
        node *ans;
        size_t completed;
        if (cur->parent == &root) {
            if (cur->get_len() == 1) {
                cur = &root;
                offset = 0;
                return;
            } else {
                ans = root.next_char[text[cur->l + 1] - 'a'];
                offset--;
                completed = 1;
            }
        } else {
            ans = cur->parent->suffix_link->next_char[text[cur->l] - 'a'];
            completed = 0;
        }
        while (true) {
            if (ans->get_len() < offset) {
                offset -= ans->get_len();
                completed += ans->get_len();
                ans = ans->next_char[text[cur->l + completed] - 'a'];
            } else {
                break;
            }
        }
        cur = ans;
    }

    std::string text;
    size_t offset;
    node root;
    node *cur;
};

int main() {
    char s[100'005];
    suffix_trie sf;
    {
        std::string str;
        while (scanf("%s", s) != EOF) {
            char c = s[0];
            scanf("%s", s);
            str = s;
            std::for_each(str.begin(), str.end(), [](char &c) {
                c = (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
            });
            if (c == '?') {
                printf("%s\n", (sf.ask(str) ? "YES" : "NO"));
            } else {
                for (char s : str) {
                    sf.add_char(s);
                }
            }
        }
    }
    return 0;
}
