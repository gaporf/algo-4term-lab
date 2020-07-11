#include <iostream>
#include <vector>

const int64_t MOD = 1'000'000'347;

int main() {
    std::string str;
    std::cin >> str;
    size_t n = str.length();
    std::vector<int64_t> base = {29, 53, 71};
    std::vector<std::vector<int64_t> > hashes(3, std::vector<int64_t>(n)),
            p(3, std::vector<int64_t>(n));
    for (size_t j = 0; j < 3; j++) {
        p[j][0] = 1;
        for (size_t i = 1; i < n; i++) {
            p[j][i] = (p[j][i - 1] * base[j]) % MOD;
        }
        for (size_t i = 0; i < n; i++) {
            hashes[j][i] = ((str[i] - 'a' + 1) * p[j][i] + ((i > 0) ? hashes[j][i - 1] : 0)) % MOD;
        }
    }
    size_t q;
    std::cin >> q;
    while (q-- > 0) {
        size_t l1, r1, l2, r2;
        std::cin >> l1 >> r1 >> l2 >> r2;
        if (r1 - l1 != r2 - l2) {
            std::cout << "No" << std::endl;
            continue;
        }
        r1--;
        l1--;
        r2--;
        l2--;
        if (r1 > r2) {
            std::swap(l1, l2);
            std::swap(r1, r2);
        }
        bool equal = true;
        for (size_t j = 0; j < 3; j++) {
            int64_t h1 = hashes[j][r1] - ((l1 > 0) ? hashes[j][l1 - 1] : 0);
            if (h1 < 0) {
                h1 += MOD;
            }
            h1 = (h1 * p[j][l2 - l1]) % MOD;
            int64_t h2 = hashes[j][r2] - ((l2 > 0) ? hashes[j][l2 - 1] : 0);
            if (h2 < 0) {
                h2 += MOD;
            }
            if (h1 != h2) {
                equal = false;
                break;
            }
        }
        if (equal) {
            std::cout << "Yes" << std::endl;
        } else {
            std::cout << "No" << std::endl;
        }
    }
    return 0;
}
