#ifndef BIGINT_H
#define BIGINT_H


#include <string>
#include <memory>

class BigInt {
public:
    typedef uint32_t uint;
    typedef uint64_t ull;
    const static size_t SIZE = 2;
    BigInt();
    BigInt(int a);
    BigInt(uint a);
    BigInt(bool negative, size_t l);
    BigInt(BigInt const& other) noexcept;
    explicit BigInt(std::string const& str);
    explicit BigInt(std::string const& str, int base);

    BigInt& operator=(BigInt const& other) noexcept;
    BigInt operator-() const;
    BigInt operator+() const;
    BigInt operator~() const;

    BigInt& operator++();
    BigInt const operator++(int);

    BigInt& operator--();
    BigInt const operator--(int);
    friend BigInt abs(const BigInt &a);

    friend bool operator==(BigInt const& a, BigInt const& b);
    friend bool operator!=(BigInt const& a, BigInt const& b);
    friend bool operator<(BigInt const& a, BigInt const& b);
    friend bool operator>(BigInt const& a, BigInt const& b);
    friend bool operator<=(BigInt const& a, BigInt const& b);
    friend bool operator>=(BigInt const& a, BigInt const& b);

    friend BigInt operator+(BigInt a, BigInt const& b);
    friend BigInt operator-(BigInt const &a, BigInt const& b);
    friend BigInt operator*(BigInt const &a, BigInt const& b);
    friend BigInt operator/(BigInt a, BigInt b);
    friend BigInt operator%(BigInt const &a, BigInt const& b);

    BigInt& operator+=(BigInt const &other);
    BigInt& operator-=(BigInt const &other);
    BigInt& operator*=(BigInt const &other);
    BigInt& operator/=(BigInt const &other);
    BigInt& operator%=(BigInt const &other);

    friend BigInt operator&(BigInt a, BigInt const& b);
    friend BigInt operator|(BigInt a, BigInt const& b);
    friend BigInt operator^(BigInt a, BigInt const& b);
    friend BigInt operator<<(BigInt const &a, uint b);
    friend BigInt operator>>(BigInt const &a, uint b);

    BigInt& operator&=(BigInt const &other);
    BigInt& operator|=(BigInt const &other);
    BigInt& operator^=(BigInt const &other);
    BigInt& operator<<=(uint other);
    BigInt& operator>>=(uint other);

    friend std::string to_string(BigInt const& a);
    friend std::string to_string_hex(BigInt const& a);
    friend std::string to_string_bi(BigInt const& a);
    ~BigInt();

private:
    bool neg;
    size_t tsize;
    union {
        uint static_data[SIZE];
        struct {
            uint *data;
            size_t capacity;
        } dynamic;
    } digits;

    uint *get_data();
    uint const *get_data() const;
    void copy_on_write();

    size_t len() const;
    void normalize();
    void change_len(size_t nlen);
    void resize(size_t nlen, uint val = 0, bool cap = true);
    friend void shl_sub(BigInt &a, BigInt const &b, size_t sh);
    friend bool shl_more(BigInt const &a, BigInt const &b, size_t sh);

    friend BigInt mul_uint_bigint(BigInt const &num, uint b);
    uint safe_get(size_t i, uint zero, uint *digits) const;

    void push_back(uint val);
    void pop_back();
    uint back();

};

std::ostream& operator<<(std::ostream& s, BigInt const& a);

#endif // BIGINT_H
