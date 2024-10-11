# import math

# def calculate_pi(iterations):
#     # 初始边长为正六边形的边长
#     side_length = 1.0
#     sides = 6

#     print(f"{'边数':<10}{'计算的圆周率':<20}{'偏差':<20}")

#     for _ in range(iterations):
#         # 计算新的边长
#         side_length = math.sqrt(2 - 2 * math.sqrt(1 - (side_length / 2) ** 2))
#         sides *= 2

#         # 计算圆周率
#         pi_approx = sides * side_length / 2

#         # 计算偏差
#         error = abs(math.pi - pi_approx)

#         # 输出结果
#         print(f"{sides:<10}{pi_approx:<20.15f}{error:<20.15f}")

# # 调用函数，设定迭代次数
# calculate_pi(10)
import math

def calculate_pi_by_polygon(n):
    # 初始边长为正六边形的边长
    side_length = 1.0
    # 初始边数为6
    sides = 6

    for _ in range(n):
        # 使用割圆术公式计算新的边长
        side_length = math.sqrt(2 - 2 * math.sqrt(1 - (side_length / 2) ** 2))
        # 边数加倍
        sides *= 2

    # 计算圆周率
    pi_approx = sides * side_length / 2

    # 输出结果
    print(f"分割{n}次，边数为{sides}，圆周率为{pi_approx:.6f}")
    print(f"math库中的圆周率常量值为{math.pi:.6f}")

# 输入割圆次数
n = int(input("请输入分割次数："))
calculate_pi_by_polygon(n)
