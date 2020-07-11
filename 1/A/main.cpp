#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::string p, t;
    std::cin >> p >> t;
    std::vector<size_t> ans;
    for (size_t i = 0; i + p.size() <= t.size(); i++) {
        bool find = true;
        for (size_t j = i; j < i + p.size(); j++) {
            if (t[j] != p[j - i]) {
                find = false;
                break;
            }
        }
        if (find) {
            ans.push_back(i + 1);
        }
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](size_t v) { std::cout << v << " "; });
    return 0;
}
