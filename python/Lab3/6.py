def main():
    n = int(input("请输入命令数量: ").strip())
    lst = []

    for _ in range(n):
        command = input().strip().split()
        cmd = command[0]
        
        if cmd == "insert":
            index = int(command[1])
            value = int(command[2])
            lst.insert(index, value)
        
        elif cmd == "print":
            print(lst)
        
        elif cmd == "remove":
            value = int(command[1])
            lst.remove(value)
        
        elif cmd == "append":
            value = int(command[1])
            lst.append(value)
        
        elif cmd == "sort":
            lst.sort()
        
        elif cmd == "pop":
            lst.pop()
        
        elif cmd == "reverse":
            lst.reverse()

if __name__ == "__main__":
    main()
