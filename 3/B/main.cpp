#include <iostream>
#include <cstring>

int const N = 1000001;

static int d[N];

int main() {
    std::memset(d, 0, N);
    for (int i = 2; i < N; i++) {
        if (d[i] == 0) {
            d[i] = i;
            for (int j = 2; i * j < N; j++) {
                if (d[i * j] == 0) {
                    d[i * j] = i;
                }
            }
        }
    }
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int a;
        scanf("%d", &a);
        while (d[a] != a) {
            printf("%d ", d[a]);
            a /= d[a];
        }
        printf("%d\n", a);
    }
    return 0;
}
