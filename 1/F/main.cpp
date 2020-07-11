#include <iostream>
#include <vector>

int main() {
    std::string a;
    std::cin >> a;
    std::vector<size_t> prefix(a.size(), 0);
    for (size_t i = 1; i < a.size(); i++) {
        size_t k = prefix[i - 1];
        while (k > 0 && a[k] != a[i]) {
            k = prefix[k - 1];
        }
        if (a[k] == a[i]) {
            k++;
        }
        prefix[i] = k;
    }
    size_t ans = a.size() - prefix.back();
    if (a.size() % ans == 0) {
        std::cout << ans << std::endl;
    } else {
        std::cout << a.size() << std::endl;
    }
    return 0;
}
