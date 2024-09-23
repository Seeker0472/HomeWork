def calculate_pizzas():
    import math

    # 输入大披萨和小披萨的尺寸
    m = int(input("请输入大披萨的尺寸（英寸）："))
    n = int(input("请输入小披萨的尺寸（英寸）："))

    # 计算大披萨和小披萨的有效直径（去掉小数部分）
    effective_diameter_m = int(m * 2.54)
    effective_diameter_n = int(n * 2.54)

    # 计算大披萨和小披萨的有效面积
    area_m = math.pi * (effective_diameter_m / 2) ** 2
    area_n = math.pi * (effective_diameter_n / 2) ** 2

    # 计算需要多少个小披萨
    num_small_pizzas = math.ceil(area_m / area_n)

    # 输出结果
    print(num_small_pizzas)

calculate_pizzas()
