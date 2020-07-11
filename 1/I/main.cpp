#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

struct bor {
    explicit bor(bor *parent = nullptr, char to_parent = 0) : parent(parent),
                                                              to_parent(to_parent),
                                                              suffix_link(nullptr),
                                                              next_ans(this) {}

    static void add(bor *root, std::string &str, size_t num) {
        for (char c : str) {
            root = root->get_next.try_emplace(c, new bor(root, c)).first->second;
        }
        root->numbers.push_back(num);
    }

    static void find_all_ans(bor *cur_bor, std::vector<char> &ans) {
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

    bor *parent;
    char to_parent;
    bor *suffix_link, *next_ans;
    std::vector<size_t> numbers;
    std::map<char, bor *> get_next, go_next;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    bor *root = new bor();
    std::string t;
    std::cin >> t;
    size_t s;
    std::cin >> s;
    std::vector<char> ans(s, false);
    for (size_t i = 0; i < s; i++) {
        std::string word;
        std::cin >> word;
        bor::add(root, word, i);
    }
    for (char c : t) {
        root = root->go(c);
        bor::find_all_ans(root, ans);
    }
    std::for_each(ans.begin(), ans.end(), [](char ans) {
        std::cout << (ans ? "Yes" : "No") << std::endl;
    });
    return 0;
}
