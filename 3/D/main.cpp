#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

uint64_t gcd(uint64_t a, uint64_t b) {
    if (b == 0) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

uint64_t multiply_modulo(uint64_t a, uint64_t b, uint64_t mod) {
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

uint64_t pow(uint64_t a, uint64_t b, uint64_t n) {
    if (b == 0) {
        return 1;
    }
    uint64_t a2 = pow(a, b / 2, n);
    uint64_t ans = multiply_modulo(a2, a2, n);
    if (b % 2 == 1) {
        ans = multiply_modulo(ans, a, n);
    }
    return ans;
}

bool is_prime(uint64_t a, std::mt19937_64 &mt_rand) {
    if (a == 1) {
        return false;
    }
    if (a == 2 || a == 3) {
        return true;
    }
    uint64_t s = a - 1;
    size_t k = 0;
    while (s % 2 == 0) {
        s /= 2;
        k++;
    }
    for (size_t i = 0; i < 5; i++) {
        uint64_t x = mt_rand() % (a - 3) + 2;
        if (gcd(x, a) != 1) {
            return false;
        }
        uint64_t pw = pow(x, s, a);
        if (pw == 1 || pw == a - 1) {
            continue;
        }
        bool prime = false;
        for (size_t j = 0; j + 1 < k; j++) {
            pw = pow(pw, 2, a);
            if (pw == 1) {
                return false;
            } else if (pw == a - 1) {
                prime = true;
                break;
            }
        }
        if (!prime && pw != 1) {
            return false;
        }
    }
    return true;
}

int main() {
    std::mt19937_64 mt_rand(static_cast<uint32_t>(time(nullptr)));
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        uint64_t a;
        std::cin >> a;
        std::cout << (is_prime(a, mt_rand) ? "YES\n" : "NO\n");
    }
    return 0;
}
