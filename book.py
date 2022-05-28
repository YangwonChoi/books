import os

def SelectMenu():
    os.system('cls')
    print("            *도서관리 프로그램*")
    print("┏━━━━━━━━━━━━━━━━추천 도서━━━━━━━━━━━━━━━━━┓")
    print("┃ 1.천사의 부름 2.미움 받을용기 3. 긴긴밤  ┃")
    print("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛")
    print("1:도서 조회")
    print("2:도서 대여")
    print("3:도서 반납")
    print("4:도서 기증")
    print("5:나의 정보")
    print("0:종료")
    user_input = input("메뉴 선택 ☞: ")
    if user_input == '0':
        return

while 1:
    SelectMenu()
    break