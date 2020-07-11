#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <queue>

size_t const INF = std::numeric_limits<size_t>::max();

struct edge {
    edge(size_t p, size_t c, size_t l, size_t r) : p(p), c(c), l(l), r(r) {}

    size_t p, c, l, r;
};

struct node {
    node(size_t l, size_t r, node *parent) : l(l), r(r), parent(parent), suffix_link(nullptr) {}

    size_t get_len() {
        return r - l;
    }

 public:
    std::map<char, node *> next_char;
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
                    cur->next_char[c] = new node(text.length() - 1, INF, cur);
                    if (prev != nullptr) {
                        prev->suffix_link = cur;
                    }
                    prev = nullptr;
                } else {
                    node *parent_ = cur->parent;
                    parent_->next_char[text[cur->l]] = new node(cur->l, cur->l + offset, parent_);
                    node *split = parent_->next_char[text[cur->l]];
                    cur->parent = split;
                    split->next_char[c] = new node(text.length() - 1, INF, split);
                    split->next_char[text[cur->l + offset]] = cur;
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

    void get_ans() {
        std::vector<edge> ans;
        std::queue<node *> q;
        q.push(&root);
        std::map<node *, size_t> get_num;
        size_t cnt = 1;
        get_num[&root] = cnt++;
        while (!q.empty()) {
            node *v = q.front();
            q.pop();
            for (char c = 'a'; c <= 'z'; c++) {
                if (v->next_char.find(c) != v->next_char.end()) {
                    node *u = v->next_char[c];
                    get_num[u] = cnt++;
                    ans.emplace_back(get_num[v], get_num[u], u->l + 1,
                                     std::min(text.length(), u->r));
                    q.push(u);
                }
            }
        }
        std::cout << ans.size() + 1 << " " << ans.size() << std::endl;
        for (auto &e : ans) {
            std::cout << e.p << " " << e.c << " " << e.l << " " << e.r << std::endl;
        }
    }

 private:
    bool has_edge(char c) {
        if (is_real_vertex()) {
            return cur->next_char.find(c) != cur->next_char.end();
        } else {
            return text[cur->l + offset] == c;
        }
    }

    void go(char c) {
        if (is_real_vertex()) {
            cur = cur->next_char[c];
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
                ans = root.next_char[text[cur->l + 1]];
                offset--;
                completed = 1;
            }
        } else {
            ans = cur->parent->suffix_link->next_char[text[cur->l]];
            completed = 0;
        }
        while (true) {
            if (ans->get_len() < offset) {
                offset -= ans->get_len();
                completed += ans->get_len();
                ans = ans->next_char[text[cur->l + completed]];
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
    std::string str;
    std::cin >> str;
    suffix_trie sf;
    for (char c : str) {
        sf.add_char(c);
    }
    sf.get_ans();
    return 0;
}
