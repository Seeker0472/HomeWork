def days_in_month(year, month):
    # 检查闰年
    if month == 2:
        if (year % 4 == 0 and year % 100 != 0) or (year % 400 == 0):
            return 29
        else:
            return 28
    # 4, 6, 9, 11 月有 30 天
    elif month in {4, 6, 9, 11}:
        return 30
    # 其他月份有 31 天
    else:
        return 31

def main():
    date_str = input("请输入8位年月日: ").strip()
    
    # 提取年份和月份
    year = int(date_str[:4])
    month = int(date_str[4:6])
    
    # 获取该月的天数
    days = days_in_month(year, month)
    
    print(days)

if __name__ == "__main__":
    main()
