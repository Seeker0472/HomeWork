def calculate_compound_interest():
    # 输入初始存款金额、本金年限和年利率
    principal = int(input("请输入初始存款金额："))
    years = int(input("请输入存款年限："))
    annual_rate = float(input("请输入年利率："))

    # 计算复利终值
    final_amount = principal * (1 + annual_rate) ** years

    # 计算利息
    interest = final_amount - principal

    # 输出结果，保留两位小数
    print(f"利息={interest:.2f}")

calculate_compound_interest()
