//Frobenius数 对于两个互质的正整数a和b，最大的不能表示为a和b的非负整数组合的数是ab−a−b，这个结论可以通过以下步骤来证明：
//定义问题：我们需要找到最大的整数KK，使得不存在非负整数xx和yy，使得ax+by=Kax+by=K。
//假设存在一个解：假设对于某个K>ab−a−bK>ab−a−b，存在非负整数xx和yy使得ax+by=Kax+by=K。
//构造新的解：我们可以构造出另一个解x′=x−bx′=x−b和y′=y+ay′=y+a。由于aa和bb互质，x′x′和y′y′也是整数。将x′x′和y′y′代入原方程，我们得到： a(x−b)+b(y+a)=ax+by−ab+ba=Ka(x−b)+b(y+a)=ax+by−ab+ba=K 这表明KK可以被aa和bb表示。
//考虑KK的范围：如果K>ab−a−bK>ab−a−b，那么至少有一个xx或yy必须大于aa或bb。这是因为如果x≤bx≤b且y≤ay≤a，那么ax+by≤abax+by≤ab，这与K>ab−a−bK>ab−a−b矛盾。
//减去abab：由于aa和bb互质，我们可以从KK中减去abab，得到一个新的数K′=K−abK′=K−ab。由于K>ab−a−bK>ab−a−b，我们有K′>−a−bK′>−a−b。这意味着K′K′是一个正整数。
//无限递降：由于K′K′也是一个正整数，我们可以重复上述过程，不断减去abab，直到我们得到一个小于或等于ab−a−bab−a−b的数。这表明所有大于ab−a−bab−a−b的数都可以表示为aa和bb的非负整数组合。
//结论：因此，最大的不能被表示为aa和bb的非负整数组合的数是ab−a−bab−a−b。
//使用C来实现这个数的运算
#include <stdio.h>

int main() {
    int N, M;
    scanf("%d %d", &N, &M); // 读取输入的N和M

    // 计算最大的不能被制作出的总质量K
    int max_unmakeable = N * M - N - M + 1;

    printf("%d\n", max_unmakeable); // 输出结果

    return 0;
}
