def main():
    list1 = [('dungeon', 7), ('winterfell', 4), ('bran', 9), ('meelo', 6)]
    list2 = [
        ['Angle', '0121701100106', 99],
        ['Jack', '0121701100107', 86],
        ['Tom', '0121701100109', 65],
        ['Smith', '0121701100111', 100],
        ['Bob', '0121701100115', 77],
        ['Lily', '0121701100117', 59]
    ]

    m = int(input("请输入整数 m: ").strip())
    n = int(input("请输入整数 n: ").strip())

    # 对列表一按第2个元素排序并输出前m项
    sorted_list1 = sorted(list1, key=lambda x: x[1])
    print(sorted_list1[:m])

    # 对列表二按第1个元素排序并输出前n项
    sorted_list2_by_first = sorted(list2, key=lambda x: x[0])
    print(sorted_list2_by_first[:n])

    # 对列表二按第3个元素排序并输出前n项
    sorted_list2_by_third = sorted(list2, key=lambda x: x[2])
    print(sorted_list2_by_third[:n])

if __name__ == "__main__":
    main()
