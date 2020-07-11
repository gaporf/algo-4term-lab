#include <iostream>
#include <vector>
#include <algorithm>

std::vector<size_t> get_z_function(std::string &a, std::string &b) {
    std::string str = a + "#" + b;
    std::vector<size_t> z(str.size(), 0), ans(b.size());
    size_t left = 0, right = 0;
    for (size_t i = 1; i < str.size(); i++) {
        if (i <= right) {
            z[i] = std::min(right - i, z[i - left]);
        }
        while (i + z[i] < str.size() && str[z[i]] == str[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
    }
    std::copy(z.begin() + a.size() + 1, z.end(), ans.begin());
    return ans;
}

int main() {
    std::string a, b;
    std::cin >> a >> b;
    std::string a_t = a, b_t = b;
    std::reverse(a_t.begin(), a_t.end());
    std::reverse(b_t.begin(), b_t.end());
    std::vector<size_t> z = get_z_function(a, b),
            z_t = get_z_function(a_t, b_t);
    std::reverse(z_t.begin(), z_t.end());
    std::vector<size_t> ans;
    for (size_t i = 0; i + a.size() <= b.size(); i++) {
        if (z[i] == a.size() || z[i] == a.size() - 1) {
            ans.push_back(i + 1);
        } else {
            size_t j = i + z[i],
                    rest = a.size() - z[i] - 1,
                    k = j + rest;
            if (z[i] + z_t[k] + 1 >= a.size()) {
                ans.push_back(i + 1);
            }
        }
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](size_t v) { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
