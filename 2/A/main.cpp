#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

struct trie {
    std::map<char, trie *> get_child;
};

struct solution {
    solution(size_t from, size_t to, char symbol) : from(from), to(to), symbol(symbol) {}

    size_t from, to;
    char symbol;
};

void add_suffix(std::string const &word, trie *root) {
    for (char c : word) {
        root = root->get_child.try_emplace(c, new trie()).first->second;
    }
}

int main() {
    trie *root = new trie();
    std::string str;
    std::cin >> str;
    {
        std::string suffix;
        std::for_each(str.rbegin(), str.rend(), [&suffix, &root](char c) {
            suffix = c + suffix;
            add_suffix(suffix, root);
        });
    }
    std::vector<solution> ans;
    {
        std::map<trie *, size_t> get_num;
        size_t cur = 1;
        get_num[root] = cur++;
        std::queue<trie *> q;
        q.push(root);
        while (!q.empty()) {
            trie *v = q.front();
            q.pop();
            for (auto child : v->get_child) {
                get_num[child.second] = cur++;
                ans.emplace_back(get_num[v], get_num[child.second], child.first);
                q.push(child.second);
            }
        }
    }
    std::cout << ans.size() + 1 << " " << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](auto &sol) {
       std::cout << sol.from << " " << sol.to << " " << sol.symbol << std::endl;
    });
    return 0;
}
