def convert_number():
    # 输入一个十进制整数
    decimal_number = int(input("请输入一个十进制整数："))
    
    # 转换为八进制并加上前缀0o
    octal_number = oct(decimal_number)
    
    # 转换为十六进制并加上前缀0x
    hexadecimal_number = hex(decimal_number)
    
    # 输出结果
    print(octal_number)
    print(hexadecimal_number)

convert_number()
