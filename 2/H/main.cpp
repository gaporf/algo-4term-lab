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
    node(size_t l, size_t r, node *parent, size_t text_len) : l(l), r(r), parent(parent),
                                                         suffix_link(nullptr) {
        std::fill(next_char, next_char + 26, nullptr);
        len = (parent == nullptr ? 0 : parent->len + std::min(text_len, r) - l);
    }

    size_t get_len() {
        return r - l;
    }

 public:
    size_t len;
    node *next_char[26];
    size_t l, r;
    node *parent, *suffix_link;
};

struct suffix_trie {
    suffix_trie() : offset(0), root(0, 0, nullptr, 0), cur(&root) {}

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
                    cur->next_char[c - 'a'] = new node(text.length() - 1, INF, cur, text.length());
                    if (prev != nullptr) {
                        prev->suffix_link = cur;
                    }
                    prev = nullptr;
                } else {
                    node *parent_ = cur->parent;
                    parent_->next_char[text[cur->l] - 'a'] = new node(cur->l, cur->l + offset,
                                                                      parent_, text.length());
                    node *split = parent_->next_char[text[cur->l] - 'a'];
                    cur->parent = split;
                    split->next_char[c - 'a'] = new node(text.length() - 1, INF, split,
                                                         text.length());
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

    uint64_t get_biggest() {
        return (cur->parent != nullptr ? cur->parent->len + offset : 0);
    }

    bool has_char(char c) {
        return has_edge(c);
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
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    uint64_t ans = 0;
    suffix_trie sf;
    for (size_t i = 1; i <= n; i++) {
        char c;
        std::cin >> c;
        if (c == '?') {
            if (i == 1) {
                std::cout << 'a' << " ";
                sf.add_char('a');
            } else {
                for (char s = 'a'; s <= 'z'; s++) {
                    if (sf.has_char(s)) {
                        sf.add_char(s);
                        std::cout << s << " ";
                        break;
                    }
                }
            }
        } else {
            sf.add_char(c);
        }
        ans += i - sf.get_biggest();
        if (c == '?') {
            std::cout << ans << std::endl;
        }
    }
    return 0;
}
