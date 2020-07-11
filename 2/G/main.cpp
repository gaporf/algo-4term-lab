#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

size_t const SIZE_OF_ALPHABET = 256;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string s, t;
    std::cin >> s >> t;
    std::string str = s + '#' + t + '$';
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
    std::size_t num = 0, len = 0;
    for (size_t i = 0; i < lcp.size(); i++) {
        if (lcp[i] == 0 || (ans[i] < s.length()) == (ans[i + 1] < s.length())) {
            continue;
        }
        if (lcp[i] > len) {
            num = std::min(ans[i], ans[i + 1]);
            len = lcp[i];
        }
    }
    std::string common = s.substr(num, len);
    std::cout << common << std::endl;
    return 0;
}
