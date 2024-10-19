//这是一个判断两个数是否互素的C语言程序
#include <stdio.h>

// 函数声明，用于计算两个数字的最大公约数
int gcd(int a, int b);

int main() {
    int num1, num2;
    printf("请输入两个整数：");
    scanf("%d %d", &num1, &num2);

    // 调用gcd函数计算最大公约数
    int result = gcd(num1, num2);

    // 如果最大公约数为1，那么两个数字互素
    if (result == 1) {
        printf("%d 和 %d 是互素的。\n", num1, num2);
    } else {
        printf("%d 和 %d 不是互素的。\n", num1, num2);
    }

    return 0;
}

// 函数定义，使用辗转相除法计算最大公约数
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
