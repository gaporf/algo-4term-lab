#include <iostream>

uint32_t pow(uint64_t a, uint64_t b, uint64_t mod) {
    if (b == 0) {
        return 1;
    }
    uint64_t a2 = pow(a, b / 2, mod);
    a2 = a2 * a2 % mod;
    if (b % 2 == 1) {
        a2 = a2 * a % mod;
    }
    return static_cast<uint32_t>(a2);
}

uint64_t get_phi(uint64_t n) {
    uint64_t ans = n;
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            ans -= ans / i;
        }
    }
    if (n > 1) {
        ans -= ans / n;
    }
    return ans;
}

int main() {
    uint32_t n, e, C;
    std::cin >> n >> e >> C;
    uint32_t p = 1, q = 1;
    for (uint32_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            p = i;
            q = n / i;
            break;
        }
    }
    uint64_t phi = static_cast<uint64_t>(p - 1) * (q - 1);
    uint64_t d = pow(e, get_phi(phi) - 1, phi);
    std::cout << pow(C, d, n) << std::endl;
    return 0;
}
