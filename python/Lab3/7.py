def main():
    ls = ['the lord of the rings', 'anaconda', 'legally blonde', 'gone with the wind']
    
    choice = input("请输入一个数字 (1-4): ").strip()

    if choice == "1":
        result = [x**3 for x in range(10)]
        print(result)
    
    elif choice == "2":
        result = [x**3 for x in range(10) if x % 2 == 0]
        print(result)
    
    elif choice == "3":
        result = [(x, x**3) for x in range(10) if x % 2 != 0]
        print(result)
    
    elif choice == "4":
        result = [s.title() for s in ls]
        print(result)
    
    else:
        print("结束程序")

if __name__ == "__main__":
    main()
