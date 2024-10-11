def is_valid_word(word):
    return word.isalpha()

def can_form_word(m, n):
    return set(m).issubset(set(n))

def main():
    m = input("请输入字符串 m: ").strip()
    
    if not is_valid_word(m):
        print("ERROR")
        return
    
    n = input("请输入字符串 n: ").strip()
    
    if can_form_word(m, n):
        print("FOUND")
    else:
        print("NOT FOUND")

if __name__ == "__main__":
    main()
