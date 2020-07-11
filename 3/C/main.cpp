#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

bool is_prime(size_t n) {
    for (size_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int32_t stupid_solution(size_t n, int32_t x) {
    int32_t h = 0;
    for (size_t i = 2; i <= n; i++) {
        if (is_prime(i)) {
            h = h * x + static_cast<int32_t>(i);
        }
    }
    return h;
}

int main() {
    size_t n;
    int32_t x;
    std::cin >> n >> x;
    int32_t h = 0;
    if (n <= 9) {
        h = stupid_solution(n, x);
    } else {
        size_t m = static_cast<size_t>(std::sqrt(n)) + 5;
        std::vector<char> prime(m, true);
        prime[0] = prime[1] = false;
        for (size_t i = 2; i < m; i++) {
            if (prime[i]) {
                for (size_t j = 2; i * j < m; j++) {
                    prime[i * j] = false;
                }
                h = h * x + static_cast<int32_t>(i);
            }
        }
        for (size_t k = m; k <= n; k += m) {
            std::vector<char> new_prime(m, true);
            for (size_t i = 2; i < m; i++) {
                if (prime[i]) {
                    size_t j = (k / i);
                    while (i * j < k) {
                        j++;
                    }
                    for (; i * j < k + m && i * j <= n; j++) {
                        new_prime[(i * j) % m] = false;
                    }
                }
            }
            for (size_t i = 0; i < std::min(m, n - k + 1); i++) {
                if (new_prime[i]) {
                    h = h * x + static_cast<int32_t>(k + i);
                }
            }
        }
    }
    std::cout << h << std::endl;
    return 0;
}
