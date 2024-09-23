from datetime import date

today = date.today()
str = "{y}年{m}月{d}日"
print(str.format(y=today.strftime("%Y"),m=today.strftime("%m"),d=today.strftime("%d")))
