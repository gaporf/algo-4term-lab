#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

size_t const SIZE_OF_ALPHABET = 256;

struct segment_tree {
    explicit segment_tree(std::vector<size_t> const &arr) {
        t.resize(arr.size() * 4);
        build(0, 0, arr.size() - 1, arr);
    }

    size_t get_min(size_t l, size_t r) {
        return get(0, 0, t.size() / 4 - 1, l, r);
    }

 private:
    void build(size_t v, size_t tl, size_t tr, std::vector<size_t> const &arr) {
        if (tl == tr) {
            t[v] = arr[tl];
        } else {
            size_t mid = (tl + tr) / 2;
            build(2 * v + 1, tl, mid, arr);
            build(2 * v + 2, mid + 1, tr, arr);
            t[v] = std::min(t[2 * v + 1], t[2 * v + 2]);
        }
    }

    size_t get(size_t v, size_t tl, size_t tr, size_t l, size_t r) {
        if (l > r) {
            return std::numeric_limits<size_t>::max();
        } else if (tl == l && tr == r) {
            return t[v];
        } else {
            size_t mid = (tl + tr) / 2;
            return std::min(get(2 * v + 1, tl, mid, l, std::min(mid, r)),
                            get(2 * v + 2, mid + 1, tr, std::max(mid + 1, l), r));
        }
    }

    std::vector<size_t> t;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string str;
    std::cin >> str;
    std::reverse(str.begin(), str.end());
    str += '$';
    size_t n = str.length();
    std::vector<size_t> ans(n);
    std::iota(ans.begin(), ans.end(), 0);
    {
        {
            size_t cnt[SIZE_OF_ALPHABET];
            std::fill(cnt, cnt + SIZE_OF_ALPHABET, 0);
            std::for_each(str.begin(), str.end(), [&cnt](char c) {
                cnt[static_cast<uint8_t>(c)]++;
            });
            std::partial_sum(cnt, cnt + SIZE_OF_ALPHABET, cnt, std::plus<>());
            std::vector<size_t> new_per(n);
            for (size_t i = 0; i < n; i++) {
                new_per[--cnt[static_cast<uint8_t>(str[i])]] = ans[i];
            }
            ans = new_per;
        }
        std::vector<size_t> get_class(n);
        get_class[ans[0]] = 0;
        std::vector<size_t> cnt(n, 0);
        cnt[0] = 1;
        for (size_t i = 1; i < n; i++) {
            get_class[ans[i]] = get_class[ans[i - 1]] + (str[ans[i]] != str[ans[i - 1]]);
            cnt[get_class[ans[i]]]++;
        }
        std::partial_sum(cnt.begin(), cnt.end(), cnt.begin(), std::plus<>());
        for (size_t k = 1; k < n; k *= 2) {
            std::vector<size_t> new_per(n);
            for (size_t j = n - 1; j <= j + 1; j--) {
                size_t l = (ans[j] + n - k) % n;
                new_per[--cnt[get_class[l]]] = l;
            }
            ans = new_per;
            std::vector<size_t> new_class(n);
            std::fill(cnt.begin(), cnt.end(), 0);
            new_class[ans[0]] = 0;
            cnt[0] = 1;
            for (size_t i = 1; i < n; i++) {
                new_class[ans[i]] = new_class[ans[i - 1]] +
                                    (get_class[ans[i]] != get_class[ans[i - 1]] ||
                                     get_class[(ans[i] + k) % n] !=
                                     get_class[(ans[i - 1] + k) % n]);
                cnt[new_class[ans[i]]]++;
            }
            std::partial_sum(cnt.begin(), cnt.end(), cnt.begin(), std::plus<>());
            get_class = new_class;
        }
    }
    std::vector<size_t> invert_ans(ans.size()), lcp(ans.size() - 1);
    for (size_t i = 0; i < invert_ans.size(); i++) {
        invert_ans[ans[i]] = i;
    }
    size_t k = 0;
    for (size_t i = 0; i < lcp.size(); i++) {
        k -= (k > 0);
        if (invert_ans[i] == lcp.size()) {
            k = 0;
        } else {
            size_t j = ans[invert_ans[i] + 1];
            while (std::max(i + k, j + k) < str.length() && str[i + k] == str[j + k]) {
                k++;
            }
            lcp[invert_ans[i]] = k;
        }
    }
    segment_tree st(lcp);
    std::set<size_t> lt;
    std::set<size_t, std::greater<>> gt;
    uint64_t dif = 1;
    std::cout << 1 << std::endl;
    lt.insert(invert_ans[n - 2]);
    gt.insert(invert_ans[n - 2]);
    for (size_t i = n - 3; i <= i + 1; i--) {
        size_t common = 0;
        auto l = lt.lower_bound(invert_ans[i]),
                r = gt.lower_bound(invert_ans[i]);
        if (l != lt.end()) {
            common = std::max(common, st.get_min(invert_ans[i], *l - 1));
        }
        if (r != gt.end()) {
            common = std::max(common, st.get_min(*r, invert_ans[i] - 1));
        }
        dif += n - i - 1 - common;
        std::cout << dif << std::endl;
        lt.insert(invert_ans[i]);
        gt.insert(invert_ans[i]);
    }
    return 0;
}
