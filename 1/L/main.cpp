#include <iostream>
#include <vector>

std::vector<size_t> get_z_function(std::string const &str) {
    std::vector<size_t> z(str.length(), 0);
    size_t left = 0, right = 0;
    for (size_t i = 1; i < str.length(); i++) {
        if (i <= right) {
            z[i] = std::min(z[i - left], right - i);
        }
        while (i + z[i] < str.length() && str[z[i]] == str[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
    }
    return z;
}

struct dsu {
    explicit dsu(size_t n) : next_(n) {
        for (size_t i = 0; i < n; i++) {
            next_[i] = i;
        }
    }

    size_t get_next(size_t v) {
        if (next_[v] != v) {
            next_[v] = get_next(next_[v]);
        }
        return next_[v];
    }

    void set_next(size_t v, size_t next_v) {
        next_[v] = next_v;
    }

 private:
    std::vector<size_t> next_;
};

int main() {
    std::string from, to;
    std::getline(std::cin, from);
    std::getline(std::cin, to);
    std::string ans = "s/" + from + "/" + to + "/g";
    std::vector<size_t> z_from(from.length(), 0);
    std::vector<std::vector<size_t> > zs_from(from.length()),
            check(from.length());
    for (size_t i = 0; i < from.length(); i++) {
        zs_from[i] = get_z_function(from.substr(i));
        for (size_t j = i; j < from.length(); j++) {
            z_from[j] = std::max(z_from[j], zs_from[i][j - i]);
        }
        check[i] = get_z_function(from.substr(i) + "$" + to);
    }
    for (size_t i = 0; i < std::min(from.length(), to.length() + 1); i++) {
        if (i > 0 && from[i - 1] != to[i - 1]) {
            break;
        }
        std::vector<size_t> z = zs_from[i],
                to_z = get_z_function(to.substr(i));
        dsu jump(from.length() + 1);
        for (size_t j = i; j < from.length(); j++) {
            size_t len = j - i + 1;
            if (z_from[i] >= len) {
                continue;
            }
            size_t u = 1;
            size_t a = jump.get_next(j + 1);
            while (a < from.length()) {
                if (z[a - i] >= len) {
                    u++;
                    a += len;
                } else {
                    jump.set_next(a, a + 1);
                }
                a = jump.get_next(a);
            }
            size_t new_len = from.length() - u * len;
            if (to.length() < new_len) {
                continue;
            } else {
                new_len = to.length() - new_len;
                if (new_len % u != 0) {
                    continue;
                } else {
                    new_len /= u;
                }
            }
            a = jump.get_next(j + 1);
            size_t b = j + 1,
                    c = i + new_len;
            bool ok = true;
            while (a < from.length()) {
                if (check[b][from.length() - b + 1 + c] >= a - b) {
                    c += a - b;
                    if (to_z[c - i] >= new_len) {
                        c += new_len;
                    } else {
                        ok = false;
                        break;
                    }
                    a += len;
                    b = a;
                    a = jump.get_next(a);
                } else {
                    ok = false;
                    break;
                }
            }
            if (b >= from.length() || check[b][from.length() - b + 1 + c] >= a - b) {
                c += a - b;
                if (c != to.length()) {
                    ok = false;
                }
            } else {
                ok = false;
            }
            if (ok) {
                std::string new_ans =
                        "s/" + from.substr(i, j - i + 1) + "/" + to.substr(i, new_len) + "/g";
                if (new_ans.length() < ans.length()) {
                    ans = new_ans;
                }
            }
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
