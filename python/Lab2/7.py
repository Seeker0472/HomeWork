def calculate_months_to_save():
    # 输入房子的总价
    total_cost = float(input("请输入房子的总价："))
    # 输入年薪
    annual_salary = float(input("请输入您的年薪："))
    # 输入每月存款百分比
    portion_saved = float(input("请输入每月存款的百分比（例如50代表50%）：")) / 100

    # 首付款比例
    portion_down_payment = 0.30
    # 首付款金额
    down_payment = total_cost * portion_down_payment

    # 每月工资
    monthly_salary = annual_salary / 12
    # 当前存款
    current_savings = 0.0
    # 年利率假设为4%
    annual_return = 0.04
    # 计算月份数
    months = 0

    while current_savings < down_payment:
        # 每月存款
        current_savings += current_savings * (annual_return / 12)
        current_savings += monthly_salary * portion_saved
        months += 1

    # 输出结果
    print(f"需要存够首付的月份数目：{months}")

calculate_months_to_save()
