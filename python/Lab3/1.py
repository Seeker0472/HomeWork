def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def main():
    # 输入两个正整数
    a = int(input("请输入第一个正整数: "))
    b = int(input("请输入第二个正整数: "))
    
    # 计算并输出最大公约数
    result = gcd(a, b)
    print(f"{a}和{b}的最大公约数是: {result}")

if __name__ == "__main__":
    main()
