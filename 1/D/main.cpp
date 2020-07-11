#include <iostream>
#include <vector>

int main() {
    std::string a;
    std::cin >> a;
    std::vector<size_t> z(a.size(), 0);
    size_t left = 0, right = 0;
    for (size_t i = 1; i < a.size(); i++) {
        if (i <= right) {
            z[i] = std::min(right - i, z[i - left]);
        }
        while (i + z[i] < a.size() && a[z[i]] == a[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
        std::cout << z[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
