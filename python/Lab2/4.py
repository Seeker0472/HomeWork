import math

def haversine_distance():
    # 输入第一个点的纬度和经度
    lat1 = float(input("请输入第一个点的纬度："))
    lon1 = float(input("请输入第一个点的经度："))
    
    # 输入第二个点的纬度和经度
    lat2 = float(input("请输入第二个点的纬度："))
    lon2 = float(input("请输入第二个点的经度："))
    
    # 将角度转换为弧度
    lat1_rad = math.radians(lat1)
    lon1_rad = math.radians(lon1)
    lat2_rad = math.radians(lat2)
    lon2_rad = math.radians(lon2)
    
    # Haversine公式计算距离
    r = 6371.0  # 地球半径，单位为公里
    dlat = lat2_rad - lat1_rad
    dlon = lon2_rad - lon1_rad
    
    a = math.sin(dlat / 2)**2 + math.cos(lat1_rad) * math.cos(lat2_rad) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    distance = r * c
    
    # 输出结果，保留两位小数
    print(f"两点之间的距离为：{distance:.2f} 公里")

haversine_distance()
