def calculate_tax_and_salary():
    try:
        # 输入应发工资、五险一金和个税免征额
        gross_salary = float(input("请输入应发工资："))
        insurance = float(input("请输入五险一金金额："))
        tax_exemption = float(input("请输入个税免征额："))

        # 检查输入是否有效
        if gross_salary <= 0 or insurance < 0 or tax_exemption <= 0:
            print("error")
            return

        # 计算应纳税所得额
        taxable_income = gross_salary - insurance - tax_exemption

        # 初始化应缴税款
        tax = 0.0

        # 计算应缴税款
        if taxable_income <= 0:
            tax = 0.0
        elif taxable_income <= 3000:
            tax = taxable_income * 0.03
        elif taxable_income <= 12000:
            tax = taxable_income * 0.10 - 210
        elif taxable_income <= 25000:
            tax = taxable_income * 0.20 - 1410
        elif taxable_income <= 35000:
            tax = taxable_income * 0.25 - 2660
        elif taxable_income <= 55000:
            tax = taxable_income * 0.30 - 4410
        elif taxable_income <= 80000:
            tax = taxable_income * 0.35 - 7160
        else:
            tax = taxable_income * 0.45 - 15160

        # 计算实发工资
        net_salary = gross_salary - insurance - tax

        # 输出结果
        print(f"应缴税款：{tax:.2f}")
        print(f"实发工资：{net_salary:.2f}")

    except ValueError:
        print("error")

calculate_tax_and_salary()
