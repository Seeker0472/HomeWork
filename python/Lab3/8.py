def desensitize_student_info(n, student_data):
    desensitized_data = []

    for data in student_data:
        student_id, name, phone = data.split()

        # 学号脱敏
        student_id_desensitized = student_id[:4] + '*' * 7 + student_id[11:]

        # 姓名脱敏
        name_desensitized = name[0] + '*' + name[2:]

        # 电话号码脱敏
        phone_desensitized = phone[:3] + '****' + phone[7:]

        desensitized_data.append([student_id_desensitized, name_desensitized, phone_desensitized])

    return desensitized_data

def main():
    try:
        n = int(input("请输入学生数量 n: ").strip())
        if n <= 0:
            print("ERROR")
            return

        student_data = []
        for _ in range(n):
            student_data.append(input().strip())

        result = desensitize_student_info(n, student_data)
        print(result)

    except ValueError:
        print("ERROR")

if __name__ == "__main__":
    main()
