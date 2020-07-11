#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>

const size_t MAX_LEN = 2'000'001;

struct trie {
    explicit trie(size_t parent = std::numeric_limits<size_t>::max(), char to_parent = 0) :
            parent(parent),
            suffix_link(std::numeric_limits<size_t>::max()),
            to_parent(to_parent) {}

    size_t parent, suffix_link;
    char to_parent;
    std::map<char, size_t> get_next, go_next;
};

size_t num = 1;
trie tries[MAX_LEN];

size_t add_word(size_t root, std::string const &word) {
    for (char c : word) {
        if (tries[root].get_next.find(c) == tries[root].get_next.end()) {
            tries[root].get_next[c] = num;
            tries[num++] = trie(root, c);
        }
        root = tries[root].get_next[c];
    }
    return root;
}

size_t go(size_t root, char c);

size_t get_link(size_t root) {
    if (tries[root].suffix_link == std::numeric_limits<size_t>::max()) {
        if (root == 0) {
            tries[root].suffix_link = root;
        } else if (tries[tries[root].parent].parent == std::numeric_limits<size_t>::max()) {
            tries[root].suffix_link = tries[root].parent;
        } else {
            tries[root].suffix_link = go(get_link(tries[root].parent), tries[root].to_parent);
        }
    }
    return tries[root].suffix_link;
}

size_t go(size_t root, char c) {
    if (tries[root].go_next.find(c) == tries[root].go_next.end()) {
        if (tries[root].get_next.find(c) == tries[root].get_next.end()) {
            if (root == 0) {
                tries[root].go_next[c] = root;
            } else {
                tries[root].go_next[c] = go(get_link(root), c);
            }
        } else {
            tries[root].go_next[c] = tries[root].get_next[c];
        }
    }
    return tries[root].go_next[c];
}

void dfs0(size_t v, std::vector<std::vector<size_t> > &g,
          std::vector<size_t> &cnt, std::vector<size_t> &deep) {
    for (size_t to : g[v]) {
        deep[to] = deep[v] + 1;
        dfs0(to, g, cnt, deep);
        cnt[v] += cnt[to];
    }
}

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<std::pair<size_t, size_t> > &segment, std::vector<std::vector<size_t> > &t,
         std::vector<char> &used, std::vector<size_t> &p, std::vector<size_t> &next_vertex) {
    for (size_t to : g[v]) {
        dfs(to, g, segment, t, used, p, next_vertex);
    }
    if (!used[v]) {
        size_t cur_t = t.size(),
                cur_num = 0,
                u = v;
        for (; u != next_vertex[v]; u = p[u]) {
            segment[u] = {cur_t, cur_num++};
            used[u] = true;
        }
        used[u] = true;
        segment[u] = {cur_t, cur_num++};
        t.emplace_back(4 * cur_num, 0);
    }
}

size_t get_next(size_t v, std::vector<size_t> &next) {
    if (next[v] != v) {
        next[v] = get_next(next[v], next);
    }
    return next[v];
}

void update(size_t v, size_t l, size_t r, size_t tl, size_t tr, std::vector<size_t> &seg) {
    if (tl > tr) {
        return;
    } else if (tl == l && tr == r) {
        seg[v]++;
    } else {
        size_t mid = (l + r) / 2;
        update(2 * v + 1, l, mid, tl, std::min(tr, mid), seg);
        update(2 * v + 2, mid + 1, r, std::max(tl, mid + 1), tr, seg);
    }
}

size_t get(size_t v, size_t l, size_t r, size_t pos, std::vector<size_t> &seg) {
    if (l == r) {
        return seg[v];
    } else {
        size_t mid = (l + r) / 2;
        if (pos <= mid) {
            return seg[v] + get(2 * v + 1, l, mid, pos, seg);
        } else {
            return seg[v] + get(2 * v + 2, mid + 1, r, pos, seg);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t root = 0;
    tries[0] = trie();
    size_t n;
    std::cin >> n;
    std::vector<size_t> barbarian(n);
    for (size_t i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;
        barbarian[i] = add_word(root, word);
    }
    std::vector<std::vector<size_t> > g(num);
    std::vector<size_t> p(num, std::numeric_limits<size_t>::max());
    for (size_t i = 1; i < num; i++) {
        p[i] = get_link(i);
        g[get_link(i)].push_back(i);
    }
    std::vector<size_t> cnt(num, 1),
            deep(num, 0);
    dfs0(0, g, cnt, deep);
    std::vector<char> used(num, false);
    std::vector<size_t> next_vertex(num, std::numeric_limits<size_t>::max());
    std::queue<size_t> que;
    for (size_t i = 0; i < num; i++) {
        if (g[i].empty()) {
            used[i] = true;
            que.push(i);
        }
    }
    next_vertex[0] = 0;
    while (!que.empty()) {
        size_t v = que.front();
        que.pop();
        if (v == 0) {
            continue;
        }
        if (cnt[v] * 2 > cnt[p[v]]) {
            next_vertex[v] = p[v];
        } else {
            next_vertex[v] = v;
        }
        if (!used[p[v]]) {
            que.push(p[v]);
            used[p[v]] = true;
        }
    }
    for (size_t i = 0; i < num; i++) {
        next_vertex[i] = get_next(i, next_vertex);
    }
    used.assign(num, false);
    std::vector<std::vector<size_t> > t;
    std::vector<std::pair<size_t, size_t> > segment(num);
    dfs(0, g, segment, t, used, p, next_vertex);
    size_t quiries;
    std::cin >> quiries;
    std::vector<size_t> ans(num, 0);
    while (quiries-- > 0) {
        size_t type;
        std::cin >> type;
        if (type == 1) {
            std::string word;
            std::cin >> word;
            size_t cur_trie = root;
            std::map<size_t, bool> is_visit;
            std::map<size_t, size_t> get_deep;
            for (char c : word) {
                cur_trie = go(cur_trie, c);
                size_t v = cur_trie;
                while (v != 0) {
                    if (next_vertex[v] == v) {
                        if (!is_visit[v]) {
                            ans[v]++;
                            is_visit[v] = true;
                            v = p[v];
                        } else {
                            break;
                        }
                    } else {
                        size_t segment_root = next_vertex[v];
                        if (get_deep.find(segment_root) != get_deep.end()
                            && deep[v] <= get_deep[segment_root]) {
                            break;
                        } else {
                            size_t from,
                                    to = deep[v];
                            if (get_deep.find(segment_root) == get_deep.end()) {
                                from = deep[segment_root] + 1;
                            } else {
                                from = get_deep[segment_root] + 1;
                            }
                            std::vector<size_t> &cur_t = t[segment[v].first];
                            update(0, 0, cur_t.size() / 4 - 1, segment[v].second,
                                   segment[v].second + to - from, cur_t);
                            get_deep[segment_root] = deep[v];
                            v = segment_root;
                        }
                    }
                }
            }
        } else {
            size_t m;
            std::cin >> m;
            m--;
            size_t v = barbarian[m];
            if (next_vertex[v] == v) {
                std::cout << ans[v] << std::endl;
            } else {
                std::vector<size_t> &cur_t = t[segment[v].first];
                std::cout << get(0, 0, cur_t.size() / 4 - 1, segment[v].second, cur_t) << std::endl;
            }
        }
    }
    return 0;
}
