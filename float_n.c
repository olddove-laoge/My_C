#include <stdio.h>

int main() {
    int a, b, n;
    scanf("%d %d %d", &a, &b, &n);

    // 用于存储余数的变量
    long long remainder = a;
    int position = 0; // 小数点后的位数
    int digits[10000]; // 存储小数点后的每一位数字

    // 循环直到余数为 0 或者达到第 n 位
    while (position < n) {
        digits[position] = (int)(remainder / b); // 计算当前位的数字
        remainder = remainder % b * 10; // 更新余数
        position++; // 移动到下一位
    }

    // 输出第 n 位的数字
    if (position >= n) {
        printf("%d\n", digits[n - 1]);
    } else {
        printf("0\n");
    }

    return 0;
}
