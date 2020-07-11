#include <iostream>
#include <vector>
#include <algorithm>

size_t get_ans(std::string &str) {
    std::vector<size_t> prefix(str.length(), 0);
    size_t max_k = 0;
    std::reverse(str.begin(), str.end());
    for (size_t i = 1; i < str.length(); i++) {
        size_t k = prefix[i - 1];
        while (k > 0 && str[k] != str[i]) {
            k = prefix[k - 1];
        }
        if (str[k] == str[i]) {
            k++;
        }
        prefix[i] = k;
        max_k = std::max(max_k, k);
    }
    std::reverse(str.begin(), str.end());
    return str.length() - max_k;
}

int main() {
    std::string str;
    std::cin >> str;
    size_t n = str.length();
    std::vector<std::vector<size_t> > ans(n, std::vector<size_t>(n, 0));
    std::string s;
    for (size_t i = 0; i < n; i++) {
        s += str[i];
        ans[0][i] = (i == 0 ? 0 : ans[0][i - 1]) + get_ans(s);
    }
    for (size_t i = 0; i + 1 < n; i++) {
        s = str.substr(i);
        std::vector<size_t> prefix(s.length(), 0);
        size_t max_k = 0;
        for (size_t j = 1; j < s.length(); j++) {
            size_t k = prefix[j - 1];
            while (k > 0 && s[k] != s[j]) {
                k = prefix[k - 1];
            }
            if (s[k] == s[j]) {
                k++;
            }
            prefix[j] = k;
            max_k = std::max(max_k, k);
            ans[i + 1][i + j] = ans[i][i + j] - (j + 1 - max_k);
        }
    }
    size_t q;
    std::cin >> q;
    while (q-- > 0) {
        size_t l, r;
        std::cin >> l >> r;
        std::cout << ans[--l][--r] << std::endl;
    }
    return 0;
}
