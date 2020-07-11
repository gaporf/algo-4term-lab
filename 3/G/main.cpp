#include <iostream>
#include <unordered_map>
#include <cmath>

uint64_t mulmodulo(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t ans = 0;
    while (b > 0) {
        if (b % 2 == 1) {
            ans = (ans + a) % mod;
            b--;
        }
        if (b > 0) {
            a = (2 * a) % mod;
            b /= 2;
        }
    }
    return ans;
}

uint64_t gcd(uint64_t a, uint64_t b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

int main() {
    uint64_t a, b, n;
    std::cin >> a >> b >> n;
    std::unordered_map<uint64_t, uint64_t> big_step;
    uint64_t m = static_cast<uint64_t>(std::sqrt(n)) + 1;
    uint64_t g = 1;
    uint64_t gm = 1;
    for (uint64_t i = 0; i < m; i++) {
        gm = mulmodulo(gm, a, n);
    }
    for (uint64_t i = 1; i <= m; i++) {
        g = mulmodulo(g, gm, n);
        if (big_step.find(g) == big_step.end()) {
            big_step[g] = i;
        }
    }
    for (uint64_t i = 0; i <= m; i++) {
        if (big_step.find(b) != big_step.end()) {
            uint64_t ans = big_step[b] * m - i;
            if (ans < n) {
                std::cout << ans << std::endl;
                return 0;
            }
        }
        b = mulmodulo(b, a, n);
    }
    std::cout << -1 << std::endl;
    return 0;
}
