def solve_chicken_rabbit_problem():
    # 总头数和总脚数
    total_heads = 35
    total_legs = 94

    # 计算鸡和兔的数量
    for chickens in range(total_heads + 1):
        rabbits = total_heads - chickens
        if 2 * chickens + 4 * rabbits == total_legs:
            print(f"鸡有 {chickens} 只，兔有 {rabbits} 只")
            return

solve_chicken_rabbit_problem()
