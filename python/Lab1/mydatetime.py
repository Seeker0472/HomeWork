from datetime import date

today = date.today()
print("Today:", today.strftime("%Y-%m-%d"))
print("Today:", today.strftime("%Y/%m/%d"))
print("Today:", today.strftime("%Y,%m,%d"))
