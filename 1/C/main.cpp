#include <iostream>
#include <vector>

int main() {
    std::string a;
    std::cin >> a;
    std::vector<size_t> p(a.size());
    p[0] = 0;
    std::cout << 0 << " ";
    for (size_t i = 1; i < a.size(); i++) {
        size_t k = p[i - 1];
        while (k > 0 && a[k] != a[i]) {
            k = p[k - 1];
        }
        if (a[k] == a[i]) {
            k++;
        }
        p[i] = k;
        std::cout << k << " ";
    }
    std::cout << std::endl;
    return 0;
}
