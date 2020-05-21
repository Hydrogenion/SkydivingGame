#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <cstdlib>
#include <string>
#include "bigint.h"
using namespace std;
int primes[168] =
{ 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
 101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,
 193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,
 293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,
 409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,
 521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,
 641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,
 757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,
 881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997 };
string randb[10] = { "5","9","21","1227","1357","47","67","89","97","111" };

bool isodd(BigInt b)
{
    return (b%2) == 1;
}

BigInt power(BigInt a, BigInt b, BigInt m)
{
    if (b == BigInt("0")) return BigInt("1");
    if (b == BigInt("1")) return a % m;
    BigInt hb = b/2;
    BigInt phb = power(a, hb, m);
    BigInt res = (phb * phb) % m;
    if (isodd(b))
        res = (a * res) % m;
    return res;
}

BigInt randomBigInteger(int len) {
    if (len == 0) return BigInt("0");
    if (len == 1) return BigInt("1");
    BigInt x("1");
    for (int i = 1; i < len; i++)
        x = x * BigInt("2");
    string s(len, '0');
    for (int i = 0; i < s.length(); i++)
        s[i] += rand() % 10;
    BigInt res = BigInt(s) % x + x;
    if (res % BigInt("2") == BigInt("0"))
        return res + BigInt("1");
    else
        return res;
}
bool isPrime(BigInt b) {
    if (b == BigInt("2")) return true;
    if (!isodd(b)) return false;
    int j = 0;
    for (int i = 0; i < 168; i++)
        if (b % BigInt(primes[i]) == BigInt("0"))
            return false;
    BigInt m = b - BigInt("1");
    while (!isodd(m))
    {
        j++;
        m = m/2;
    }
    for (int i = 0; i < 5; i++)
    {
        BigInt k(randb[i]);
        BigInt v = power(k, m, b);
        if (v == BigInt("1")) continue;
        BigInt testnum = b - BigInt("1");
        int n = 1;
        while (true)
        {
            //cout << n << endl;
            if (v == testnum) break;
            if (n == j) return false;
            v = (v*v) % b;
            n++;
        }

    }
    return true;
}
BigInt generatePrime(int len) {
    BigInt b = randomBigInteger(len);
    while (true) {
        if (isPrime(b)) return b;
        b = b + BigInt("2");

    }
}

BigInt gcd(BigInt a, BigInt b) {
    return b==BigInt("0")?a:gcd(b,a%b);
}

void extendedEuclidean(BigInt a, BigInt b, BigInt &x, BigInt &y, const BigInt& m) {
    if (a == BigInt("0")) {
        x = BigInt("0");
        y = BigInt("1");
        return;
    }
    BigInt c = b / a, d = b % a;
    extendedEuclidean(d, a, y, x, m);
    x = (x + m - (c * y) % m) % m;
}

BigInt modReverse(BigInt a, BigInt mod) {
    if (!(gcd(a, mod) == BigInt("1"))) return BigInt("0");
    BigInt x, y;
    a = a % mod;
    extendedEuclidean(a, mod, x, y, mod);
    return x;
}

#endif // UTILS_H
