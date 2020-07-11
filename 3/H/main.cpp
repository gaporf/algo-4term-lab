#include <iostream>
#include <cstring>
#include <vector>
#include <map>

static uint64_t const N = 524288;
static uint64_t const P = 524812289;
static uint64_t const G = 194516551;

struct big_integer {
    explicit big_integer(std::string const &str) {
        sign = str[0] == '-';
        for (size_t i = str.length(); i > 0; i--) {
            if (str[i - 1] != '-') {
                digits.push_back(static_cast<uint64_t>(str[i - 1] - '0'));
            }
        }
        while (digits.size() < N) {
            digits.push_back(0);
        }
    }
    std::vector<uint64_t> digits;
    bool sign;
};

std::vector<uint64_t> fft(std::vector<uint64_t> const &a, uint64_t w) {
    if (a.size() == 1) {
        return {a[0]};
    }
    std::vector<uint64_t> a0;
    a0.reserve(a.size() / 2);
    for (size_t i = 0; i < a.size(); i += 2) {
        a0.push_back(a[i]);
    }
    std::vector<uint64_t> a1;
    a1.reserve(a.size() / 2);
    for (size_t i = 1; i < a.size(); i += 2) {
        a1.push_back(a[i]);
    }
    auto u0 = fft(a0, w * w % P);
    auto u1 = fft(a1, w * w % P);
    std::vector<uint64_t> ans(a.size());
    size_t M = u0.size();
    uint64_t g = 1;
    for (size_t i = 0; i < a.size(); i++) {
        ans[i] = (u0[i % M] + g * u1[i % M]) % P;
        g = g * w % P;
    }
    return ans;
}

uint64_t pow(uint64_t a, uint64_t b, uint64_t mod) {
    if (b == 0) {
        return 1;
    }
    uint64_t a2 = pow(a, b / 2, mod);
    a2 = a2 * a2 % mod;
    if (b % 2 == 1) {
        a2 = a2 * a % mod;
    }
    return a2;
}

std::vector<uint64_t> inverse_fft(std::vector<uint64_t> const &a, uint64_t w) {
    auto b = fft(a, w);
    std::vector<uint64_t> ans(b.size());
    auto n = pow(N, P - 2, P);
    ans[0] = b[0] * n % P;
    for (size_t i = 1; i < N; i++) {
        ans[i] = b[static_cast<size_t>(N - i)] * n % P;
    }
    return ans;
}

bool is_prime(uint64_t p) {
    for (uint64_t i = 2; i * i <= p; i++) {
        if (p % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string str1, str2;
    std::cin >> str1 >> str2;
    big_integer a(str1), b(str2);
    auto ffta = fft(a.digits, G);
    auto fftb = fft(b.digits, G);
    std::vector<uint64_t> c(N);
    for (size_t i = 0; i < N; i++) {
        c[i] = ffta[i] * fftb[i] % P;
    }
    auto ans = inverse_fft(c, G);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i + 1 < ans.size()) {
            ans[i + 1] = ans[i + 1] + ans[i] / 10;
        }
        ans[i] %= 10;
    }
    while (!ans.empty() && ans.back() == 0) {
        ans.pop_back();
    }
    if (ans.empty()) {
        std::cout << "0";
    } else {
        if (a.sign != b.sign) {
            std::cout << "-";
        }
        for (size_t i = ans.size(); i > 0; i--) {
            std::cout << ans[i - 1];
        }
    }
    std::cout << std::endl;
    return 0;
}
