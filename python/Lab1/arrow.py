import math

ab = float(input("AB="))
cd = float(input("CD="))

ad = ab/2
oa = (ad*ad+cd*cd)/(2*cd)

aob = 2*math.asin(ad/oa)

sector = aob/(2*3.14)*3.14*oa*oa

triangle = oa*oa*math.sin(aob)/2

arch = sector-triangle

print("Airea:%.4f"%arch)
