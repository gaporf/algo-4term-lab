#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::string a, b;
    std::cin >> a >> b;
    std::vector<size_t> p(a.size()), ans;
    p[0] = 0;
    for (size_t i = 1; i < a.size(); i++) {
        size_t k = p[i - 1];
        while (k > 0 && a[k] != a[i]) {
            k = p[k - 1];
        }
        if (a[k] == a[i]) {
            k++;
        }
        p[i] = k;
    }
    for (size_t k = 0, i = 0; i < b.size(); i++) {
        while (k > 0 && (k == a.size() || a[k] != b[i])) {
            k = p[k - 1];
        }
        if (b[i] == a[k]) {
            k++;
        }
        if (k == a.size()) {
            ans.push_back(i - a.size() + 2);
        }
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](size_t v) { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
