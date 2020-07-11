#include <iostream>
#include <vector>
#include <map>
#include <queue>

const size_t MOD = 10'000;
const size_t M = 110;

struct bor {
    explicit bor(bor *parent = nullptr, char to_parent = 0) : parent(parent),
                                                              suffix_link(nullptr),
                                                              to_parent(to_parent),
                                                              leaf(false) {}

    static void add(bor *root, std::string &str) {
        for (char c : str) {
            root = root->get_next.try_emplace(c, new bor(root, c)).first->second;
        }
        root->leaf = true;
    }

    bor *get_link() {
        if (suffix_link == nullptr) {
            if (parent == nullptr) {
                suffix_link = this;
            } else if (parent->parent == nullptr) {
                suffix_link = parent;
            } else {
                suffix_link = parent->get_link()->go(to_parent);
            }
        }
        return suffix_link;
    }

    bor *go(char c) {
        if (go_next.find(c) == go_next.end()) {
            if (get_next.find(c) == get_next.end()) {
                if (parent == nullptr) {
                    go_next[c] = this;
                } else {
                    go_next[c] = get_link()->go(c);
                }
            } else {
                go_next[c] = get_next[c];
            }
        }
        return go_next[c];
    }

    bor *parent, *suffix_link;
    char to_parent;
    bool leaf;
    std::map<char, bor *> get_next, go_next;
};

std::vector<std::vector<size_t> > pow(std::vector<std::vector<size_t> > &dp, size_t l) {
    if (l == 1) {
        return dp;
    } else {
        std::vector<std::vector<size_t> > a = pow(dp, l / 2);
        std::vector<std::vector<size_t> > b(M, std::vector<size_t>(M, 0));
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < M; j++) {
                for (size_t k = 0; k < M; k++) {
                    b[i][j] = (b[i][j] + a[i][k] * a[k][j]) % MOD;
                }
            }
        }
        if (l % 2 == 1) {
            std::vector<std::vector<size_t> > c(M, std::vector<size_t>(M, 0));
            for (size_t i = 0; i < M; i++) {
                for (size_t j = 0; j < M; j++) {
                    for (size_t k = 0; k < M; k++) {
                        c[i][j] = (c[i][j] + b[i][k] * dp[k][j]) % MOD;
                    }
                }
            }
            b = c;
        }
        return b;
    }
}

size_t pow(size_t l) {
    if (l == 1) {
        return 26;
    } else {
        size_t a = pow(l / 2);
        size_t b = (a * a) % MOD;
        if (l % 2 == 1) {
            b = (b * 26) % MOD;
        }
        return b;
    }
}

int main() {
    size_t n;
    std::cin >> n;
    bor *root = new bor();
    while (n-- > 0) {
        std::string word;
        std::cin >> word;
        bor::add(root, word);
    }
    std::map<bor *, size_t> get_num;
    size_t num = 1;
    get_num[root] = 0;
    std::queue<bor *> handle;
    handle.push(root);
    std::vector<std::vector<size_t> > dp(M, std::vector<size_t>(M, 0));
    while (!handle.empty()) {
        bor *v = handle.front();
        handle.pop();
        size_t m = get_num[v];
        for (char c = 'a'; c <= 'z'; c++) {
            bor *next_v = v->go(c);
            if (get_num.find(next_v) == get_num.end()) {
                for (auto cur_bor = next_v; cur_bor != root; cur_bor = cur_bor->get_link()) {
                    if (cur_bor->leaf) {
                        next_v->leaf = true;
                        break;
                    }
                }
                get_num[next_v] = num++;
                handle.push(next_v);
            }
            if (next_v->leaf) {
                continue;
            }
            size_t num_next_v = get_num[next_v];
            dp[m][num_next_v]++;
        }
    }
    size_t l;
    std::cin >> l;
    std::vector<std::vector<size_t> > ans = pow(dp, l);
    size_t ways = 0;
    for (size_t i = 0; i < M; i++) {
        ways += ans[0][i];
    }
    ways %= MOD;
    size_t all = pow(l);
    if (all >= ways) {
        all -= ways;
    } else {
        all = all + MOD - ways;
    }
    std::cout << all;
    return 0;
}
