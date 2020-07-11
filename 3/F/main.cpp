#include <iostream>

using namespace std;

void gcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) {
        x = 1;
        y = 0;
    } else {
        int64_t xx, yy;
        gcd(b, a % b, xx, yy);
        x = -yy;
        y = -xx - yy * (a / b);
    }
}

int64_t get_modulo(int64_t a, int64_t mod) {
    if (a < 0) {
        a += (std::abs(a) / mod) * mod;
        while (a < 0) {
            a += mod;
        }
    }
    return a %= mod;
}

int64_t multiply_modulo(int64_t a, int64_t b, int64_t mod) {
    int64_t ans = 0;
    int64_t r = 1;
    if ((a > 0 && b < 0) || (a < 0 && b > 0)) {
        r *= -1;
    }
    a = std::abs(a);
    b = std::abs(b);
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
    return ans * r;
}

int64_t solve(int64_t a, int64_t b, int64_t n, int64_t m) {
    int64_t x, y;
    if (b == a) {
        x = y = 0;
    } else {
        gcd(n, m, x, y);
        x *= (b - a);
    }
    return get_modulo(a + multiply_modulo(x, n, n * m), n * m);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t N;
    std::cin >> N;
    for (size_t i = 0; i < N; i++) {
        int64_t a, b, n, m;
        std::cin >> a >> b >> n >> m;
        std::cout << solve(a, b, n, m) << std::endl;
    }
    return 0;
}
