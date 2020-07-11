#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

struct bor {
    explicit bor(bor *parent = nullptr, char to_parent = 0) : to_parent(to_parent),
                                                              parent(parent),
                                                              suffix_link(nullptr),
                                                              next_ans(this) {}

    void add(std::string &str, size_t num) {
        bor *cur_bor = this;
        for (char c : str) {
            cur_bor = cur_bor->get_next.try_emplace(c, new bor(cur_bor, c)).first->second;
        }
        cur_bor->numbers.push_back(num);
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

    void find_all_ans(std::vector<char> &ans) {
        bor *cur_bor = this;
        while (cur_bor != nullptr) {
            for (size_t num : cur_bor->numbers) {
                ans[num] = true;
            }
            cur_bor->numbers.clear();
            cur_bor->get_link();
            cur_bor->next_ans = nullptr;
            cur_bor = cur_bor->suffix_link->next_ans;
        }
    }

    char to_parent;
    bor *parent, *suffix_link, *next_ans;
    std::vector<size_t> numbers;
    std::map<char, bor *> get_next, go_next;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    bor root;
    root.next_ans = nullptr;
    size_t s;
    std::cin >> s;
    std::vector<char> ans(s, false);
    for (size_t i = 0; i < s; i++) {
        std::string cur_string;
        std::cin >> cur_string;
        root.add(cur_string, i);
    }
    std::string t;
    std::cin >> t;
    bor *cur_bor = &root;
    for (char c : t) {
        cur_bor = cur_bor->go(c);
        cur_bor->find_all_ans(ans);
    }
    std::for_each(ans.begin(), ans.end(), [](char ans) {
        std::cout << (ans ? "YES" : "NO") << std::endl;
    });
    return 0;
}
