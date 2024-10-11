def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

def is_palindrome(num):
    return str(num) == str(num)[::-1]

def find_palindromic_primes(n):
    palindromic_primes = []
    for num in range(2, n):
        if is_prime(num) and is_palindrome(num):
            palindromic_primes.append(num)
    return palindromic_primes

def main():
    n = int(input("请输入一个正整数: "))
    result = find_palindromic_primes(n)
    print(" ".join(map(str, result)))

if __name__ == "__main__":
    main()
