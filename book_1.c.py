import re
import time
import os

'''
class booklist:
    def __init__(self, num, bookname, writer, publisher, year, stock):
        self.num = num
        self.bookname = bookname
        self.writer = writer
        self.publisher = publisher
        self.year = year
        self.stock = stock

    def printbook(self):
        s = f'| 번호: {self.num} |'
        s += f' 제목: {self.bookname} |'
        s += f' 저자: {self.writer} |'
        s += f' 출판사: {self.publisher} |'
        s += f' 출판년도: {self.year} |'
        s += f' 재고: {self.stock} |'
        print(s)
'''

books = [{"num": 1, "제목": "안드로이드앱개발", "저자": "최전산", "출판사": "PCB", "출판년도": "2017", "재고": 1},
         {"num": 2, "제목": "파이썬", "저자": "강수라", "출판사": "연두", "출판년도": "2019", "재고": 1},
         {"num": 3, "제목": "자바스크립트", "저자": "박정식", "출판사": "SSS", "출판년도": "2018", "재고": 1},
         {"num": 4, "제목": "HTML5", "저자": "주환", "출판사": "대한", "출판년도": "2011", "재고": 1},
         {"num": 5, "제목": "컴파일러", "저자": "장진웅", "출판사": "PCB", "출판년도": "2011", "재고": 1},
         {"num": 6, "제목": "C언어", "저자": "헝말숙", "출판사": "한국", "출판년도": "2010", "재고": 1},
         {"num": 7, "제목": "프로그래밍언어론", "저자": "현정숙", "출판사": "정의출판", "출판년도": "2010", "재고": 1},
         {"num": 8, "제목": "안드로이드", "저자": "이광희", "출판사": "한국", "출판년도": "2013", "재고": 1},
         {"num": 9, "제목": "앱인벤터", "저자": "박규진", "출판사": "대한", "출판년도": "2015", "재고": 1}]
        #bookLental() > 장보구니에 담긴 것으로 간주 하여 삭제 기능
        #bookGive()   > 도서 전체 책 리스트로 간주하여 추가

class Library:
    def bookReturn(self):
        for i in books:
            print(i)  # 리스트 줄바꿈하여 출력
        booknumber = int(input("반납할 책번호 입력해주세요 : "))
        choice = next((item for item in books if item['num'] == booknumber), None)
        choice['제고'] = 1
        print(choice['제목'], "|제고: ", choice['재고'], "|")
        for i in books:
            print(i)  # 리스트 줄바꿈하여 출력
        print("반납이 완료되었습니다.")
        time.sleep(2)

    def bookLental(self):  # 대여 기능
        for i in books:
            print(i)  # 리스트 줄바꿈하여 출력
        booknumber = int(input("대여할 책번호 입력해주세요 : "))
        choice = next((item for item in books if item['num'] == booknumber), None)
        choice['제고'] = 0
        print(choice['제목'], "|제고: ", choice['재고'], "|")
        for i in books:
            print(i)  # 리스트 줄바꿈하여 출력
        print("대여가 완료되었습니다.")
        time.sleep(2)

    def bookGive(self):
        mybook = ['작별인사', '남매', '세상', '히어로', '행성']
        give = []
        while True:
            check=0
            print(*books, sep='\n')
            slt = input('1) 기증하기   2) 내 책 보기  3) 종료 \n>>')
            os.system('clear')
            if slt == '1':
                bookbye = input('기증할 책 이름을 적으세요\n>>')
                for x in mybook:
                    if x == bookbye:
                        mybook.remove(bookbye)
                        a = len(books)+1
                        c = input('저자를 입력해주세요\n>>')
                        d = input('출판사를 입력해주세요\n>>')
                        e = input('출판 년도를 입력해주세요\n>>')
                        f = int(input("기부수량을 입력해주세요\n>>"))
                        give = {'num': a, '제목': bookbye, '저자': c, '출판사': d, '출판 년도': e, '재고':f}
                        books.append(give)
                        print(give)
                        check=1
                        os.system('clear')
                if check == 0:
                    print("보유중인 중인 책이 없어 '기증'할 수 없습니다.")
                    print()
            elif slt =='2':
                os.system('clear')
                print(mybook)
                home = input('아무키나 입력하세요\n>>')
                os.system('clear')
            elif slt == '3':
                print('종료합니다.')
                break
            else:
                print('잘못입력했습니다.')

def SelectMenu():
    os.system('cls')
    b=Library()
    sw = True
    while sw:
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
        elif user_input == '1':
            for i in books:
                print(i)  # 리스트 줄바꿈하여 출력
        elif user_input == '2':
            b.bookLental()
        elif user_input == '3':
            b.bookReturn()
        elif user_input == '4':
            b.bookGive()
        elif user_input == '5':
            '''
            for i in books.keys():
                for ii in i:
                    if ii['재고'] > 0:
                        print(ii)
            
            print(tuple(m['재고'] for m in books))
            '''
            choice = next((item for item in books if item['재고'] == 0), None)
            print(choice)
            
class makelogin: #회원가입 클래스
    def make_id(self, regist_user):
        regist_id = []
        while True:
            uid = str(input('회원 아이디 입력: '))
            if uid in regist_user:
                print('이미 등록된 id입니다.')
                ex = input('메인 화면으로 이동하시겠습니까? (y/n): ')
                if ex == 'y' or ex == 'Y':
                    return False
                else:
                    continue
            else:
                res_id = self.chk_id(uid)
                if not res_id:
                    continue
                else:
                    regist_id.append(uid)
                    break
        # print(regist_id)

        while True:
            pwd = str(input('비밀번호 입력: '))
            res_pwd = self.chk_password(pwd)
            if not res_pwd:
                continue
            else:
                regist_id.append(pwd)
                break
        # print(regist_id)
        return regist_id
    def chk_id(self, id):
        result = True
        reg = r'^[A-Za-z0-9_]{4,20}$'
        if not re.search(reg, id):
            print('아이디 생성 기준에 부적당합니다!')
            result = False
        return result
    def chk_password(self, pwd):
        result = True
        reg = r'^(?=.*[A-Za-z])(?=.*\d)(?=.*[!@#$%&*?])[A-Za-z\d!@#$%&*?]{8,20}$'
        if not re.search(reg, pwd):
            print('비밀번호 기준에 부적당합니다!')
            result = False
        return result
    def edit_password(self, uid, pwd):
        n_pwd = ''
        while True:
            pw = str(input('새로운 비밀번호 입력: '))
            if pw == pwd:
                print(f'기존 비번하고 같음!')
                continue
            else:
                res_pwd = self.chk_password(pw)
                if not res_pwd:
                    continue
                else:
                    n_pwd = pw
                    break
        print(f'id:{uid}, n_pwd:{n_pwd}')
        return uid, n_pwd

def main():
    regist_user = {'haha': 'gg!!11'}
    sw = True
    a = makelogin()
    while sw:
        print('-----------------------------')
        print('1. 로그인 하기')
        print('2. 회원 아이디 생성')
        print('3. 비밀번호 변경')
        print('4. 전체 회원 아이디 목록 보기')
        print('5. 종료')
        print('-----------------------------\n')

        select_no = int(input('번호 선택(1~5): '))
        if select_no == 1:
            print("로그인을 시작합니다. ")
            x = input("ID를 입력해주십시오 : ")
            if not(x in regist_user.keys()):
                print("ID또는 비밀번호가 알맞지 않습니다. ")
                print("다시 로그인 해주세요!")
                time.sleep(1)
            else:
                y = input("비밀번호를 입력해 주십시오 : ")
                if not(y in regist_user.get(x)):
                    print("ID또는 비밀번호가 알맞지 않습니다. ")
                    print("다시 로그인 해주세요!")
                    time.sleep(1)
                else:
                    print("안녕하세요 회원님!")
                    time.sleep(1)
                    SelectMenu()
                    ##메인화면으로 연결 하는 코드 쓰는곳
        if select_no == 2:
            id_result = a.make_id(regist_user)
            if id_result:
                regist_user[id_result[0]] = id_result[1]
                print(regist_user)

        if select_no == 3:
            uid = str(input('회원 아이디: '))
            if uid in regist_user:
                print(f'{uid} / {regist_user[uid]}')
                n_pwd = a.edit_password(uid, regist_user[uid])
                regist_user[uid] = n_pwd
                print('비밀번호 변경 완료!\n')
            else:
                print('등록된 아이디가 아닙니다.\n')
                continue

        if select_no == 4:
            for k, v in regist_user.items():
                print(f'id: {k} / pw: {v}')
            print()

        if select_no == 5:
            sw = False

main() #로그인 불러오기