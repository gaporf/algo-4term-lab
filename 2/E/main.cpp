#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>

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
    size_t n;
    {
        size_t m;
        std::cin >> n >> m;
    }
    std::vector<size_t> arr(n);
    std::for_each(arr.begin(), arr.end(), [](auto &v) {
        std::cin >> v;
    });
    arr.push_back(0);
    n++;
    std::vector<size_t> ans(n);
    std::iota(ans.begin(), ans.end(), 0);
    {
        {
            size_t cnt[11];
            std::fill(cnt, cnt + 11, 0);
            std::for_each(arr.begin(), arr.end(), [&cnt](size_t v) {
                cnt[v]++;
            });
            std::partial_sum(cnt, cnt + 11, cnt, std::plus<>());
            std::vector<size_t> new_per(n);
            for (size_t i = 0; i < n; i++) {
                new_per[--cnt[arr[i]]] = ans[i];
            }
            ans = new_per;
        }
        std::vector<size_t> get_class(n);
        get_class[ans[0]] = 0;
        std::vector<size_t> cnt(n, 0);
        cnt[0] = 1;
        for (size_t i = 1; i < n; i++) {
            get_class[ans[i]] = get_class[ans[i - 1]] + (arr[ans[i]] != arr[ans[i - 1]]);
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
            while (std::max(i + k, j + k) < n && arr[i + k] == arr[j + k]) {
                k++;
            }
            lcp[invert_ans[i]] = k;
        }
    }
    segment_tree st(lcp);
    uint64_t prod = n - 1, len = n - 1, num = 0;
    for (size_t i = 0; i < lcp.size(); i++) {
        if (lcp[i] == 0) {
            continue;
        }
        uint64_t new_len = lcp[i],
                new_num = ans[i];
        size_t l = 0,
                r = i + 1;
        while (r - l > 1) {
            size_t mid = (l + r) / 2;
            if (st.get_min(mid, i) == lcp[i]) {
                r = mid;
            } else {
                l = mid;
            }
        }
        size_t ll = i,
                rr = lcp.size();
        while (rr - ll > 1) {
            size_t mid = (ll + rr) / 2;
            if (st.get_min(i, mid) == lcp[i]) {
                ll = mid;
            } else {
                rr = mid;
            }
        }
        uint64_t new_cnt = ll - r + 2;
        if (new_cnt * new_len >= prod) {
            prod = new_cnt * new_len;
            len = new_len;
            num = new_num;
        }
    }
    std::cout << prod << std::endl << len << std::endl;
    std::for_each(arr.begin() + static_cast<size_t>(num),
                  arr.begin() + static_cast<size_t>(num + len), [](size_t v) {
                std::cout << v << " ";
            });
    std::cout << std::endl;
    return 0;
}
