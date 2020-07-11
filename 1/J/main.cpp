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
    std::string s;
    size_t ans = 0;
    for (char c : str) {
        s += c;
        ans += get_ans(s);
        std::cout << ans << std::endl;
    }
    return 0;
}
