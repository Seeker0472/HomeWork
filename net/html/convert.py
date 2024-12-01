import base64

IMG_NAME=""

# 打开并读取 PNG 文件
with open(IMG_NAME, "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode('utf-8')

# 准备 HTML 内容
html_content = f'<img src="data:image/png;base64,{encoded_string}" alt="Description">'

# 将结果写入文件
with open("output.html", "w") as html_file:
    html_file.write(html_content)

