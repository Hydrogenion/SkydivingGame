#ifndef GCD_H
#define GCD_H
//#include "bigint.h"

//BigInt gcd(BigInt a, BigInt b) {
//    return b==BigInt("0")?a:gcd(b,a%b);
//}

//void extendedEuclidean(BigInt a, BigInt b, BigInt &x, BigInt &y, const BigInt& m) {
//    if (a == BigInt("0")) {
//        x = BigInt("0");
//        y = BigInt("1");
//        return;
//    }
//    BigInt c = b / a, d = b % a;
//    extendedEuclidean(d, a, y, x, m);
//    x = (x + m - (c * y) % m) % m;
//}

//BigInt modReverse(BigInt a, BigInt mod) {
//    if (!(gcd(a, mod) == BigInt("1"))) return BigInt("0");
//    BigInt x, y;
//    a = a % mod;
//    extendedEuclidean(a, mod, x, y, mod);
//    return x;
//}
#endif // GCD_H
