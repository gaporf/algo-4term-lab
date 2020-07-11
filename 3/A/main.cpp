#include <iostream>
#include <cstring>

size_t const N = 20000001;

static bool prime[N];

int main() {
    std::memset(prime, true, N);
    prime[0] = prime[1] = false;
    for (size_t i = 2; i < N; i++) {
        if (prime[i]) {
            for (size_t j = 2; i * j < N; j++) {
                prime[i * j] = false;
            }
        }
    }
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int a;
        scanf("%d", &a);
        if (prime[a]) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
    return 0;
}
