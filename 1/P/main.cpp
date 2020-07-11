#include <iostream>
#include <vector>
#include <cstring>

const int64_t MOD = 1'000'000'007;

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    int64_t c;
    std::cin >> n >> c;
    int64_t ans = c;
    std::vector<size_t> my_string;
    my_string.push_back(0);
    std::vector<size_t> p(n, 0);
    std::vector<char> used(n, false);
    for (size_t i = 1; i < n; i++) {
        std::cin >> p[i];
        if (p[i] == 0) {
            int64_t minus = 0;
            size_t k = p[i - 1];
            std::vector<size_t> to_delete;
            while (k > 0) {
                if (!used[my_string[k]]) {
                    used[my_string[k]] = true;
                    to_delete.push_back(my_string[k]);
                    minus++;
                }
                k = p[k - 1];
            }
            if (!used[0]) {
                used[0] = true;
                to_delete.push_back(0);
                minus++;
            }
            for (size_t C = 0;; C++) {
                if (!used[C]) {
                    my_string.push_back(C);
                    break;
                }
            }
            for (size_t v : to_delete) {
                used[v] = false;
            }
            ans = (ans * (c - minus)) % MOD;
        } else {
            my_string.push_back(my_string[p[i] - 1]);
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
