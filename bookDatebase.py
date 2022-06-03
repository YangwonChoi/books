import sqlite3
import time
import os

class Library:
    def bookGive(self):  # 기부 기능
        conn = sqlite3.connect("dbDate.db")
        cur = conn.cursor()
        mybook = ['작별인사', '남매', '세상', '히어로', '행성']
        givelist = []
        while True:
            check = 0
            print()
            print('<보유중인 도서>',
                  *mybook, sep='\n')
            slt = input('1) 기증하기   2) 내 책 보기  3) 기부한 목록 4) 종료 \n>>')
            os.system('clear')
            if slt == '1':
                bookbye = input('기증할 책 이름을 적으세요 >>')
                for x in mybook:
                    if x == bookbye:
                        mybook.remove(bookbye)
                        a = int(input('책번호 입력하세요 >>'))
                        b = input('도서관 이름입력하세요 >>')
                        c = input('저자를 입력하세요 >>')
                        d = input('출판사를 입력해주세요 >>')
                        e = int(input('출판년도를 입력해주세요  >>'))
                        g = input('책 유형 (ex) 책, 부록) >>')

                        cur.execute("insert into booklist values(?,?,?,?,?,?,?)", (a,b,bookbye,c,d,e,g)) # 실행문
                        conn.commit() #저장기능
                        givelist.append(bookbye)

                        check = 1
                        os.system('clear')
                if check == 0:
                    print("보유중인 중인 책이 없어 '기증'할 수 없습니다.")
                    print()
            elif slt == '2':
                os.system('clear')
                print(mybook)
                home = input('아무키나 입력하세요\n>>')
                os.system('clear')
            elif slt == '3':
                os.system('clear')
                print(givelist)
            elif slt == '4':
                print('종료합니다.')
                conn.close()
                break
            else:
                print('잘못입력했습니다.')

    def bookSearch(self): # 숫자로 검색    ########
            conn = sqlite3.connect('dbDate.db')
            cur = conn.cursor()
            author = int(input("고유번호 입력하세요\n>>"))
            cur.execute("select number, bookname, writer, publisher, year from booklist where number = ?", (author,))
            rows=cur.fetchone()
            for row in rows:
                print(row)
            num = input("장바구니에 담겠습니까? 수락(y) 거절(n)\n>>")
            if num == 'y':
                userid = input("장바구니에 담을 아이디를 입력해주세요\n>>")
                cur.execute("insert into user values(?,?,?,?,?,?,date('now'))", (userid, rows[0], rows[1], rows[2], rows[3], rows[4]))
                print(f'{rows[1]}이(가) 장바구니에 담았습니다.')
                conn.commit()
                conn.close()
            elif num == 'n':
                return

            home=input('아무키나 누르면 홈으로 돌아갑니다.\n>>')

    def outhorSearch(self):  # 이름 검색   ########
            conn = sqlite3.connect('dbDate.db')
            cur = conn.cursor()
            num= input("입력\n>>")
            num = '%' + num + '%' # 입력뒤에 % 붙여줌
            cur.execute("select number, bookname, writer, publisher, year from booklist where bookname like ?", (num,)) #관련글자 전부 출력
            row1=cur.fetchall()
            for row in row1:
                print(row)
            num = input("장바구니에 담겠습니까? 수락(y) 거절(n)\n>>")
            if num == 'y':
                booknumber = int(input("장바구니에 담을 책번호 입력해주세요 : "))
                userid = input("장바구니에 담을 아이디를 입력해주세요\n>>")
                cur.execute("select number, bookname, writer, publisher, year from booklist where number like ?", (booknumber,))
                rows=cur.fetchone()
                for row in rows:
                    print(row)
                time.sleep(1)
                cur.execute("insert into user values(?,?,?,?,?,?,date('now'))", (userid, rows[0], rows[1], rows[2], rows[3], rows[4]))
                conn.commit()
                conn.close()
                print(f'{rows[1]}이(가) 장바구니에 담았습니다.')
            elif num == 'n':
                return

            home=input('아무키나 누르면 홈으로 돌아갑니다.\n>>')

    def bookReturn(self):
            conn = sqlite3.connect('dbDate.db')
            cur = conn.cursor()
            cur.execute("select number, bookname from user")
            row1=cur.fetchall()
            for row in row1:
                print(row)
            num = input("반납를 하시겟습니까? 수락(y) 거절(n)\n>>")
            if num == 'y':
                booknumber = int(input("반납할 책번호 입력해주세요 : "))
                cur.execute("select number, bookname, writer, publisher, year from user where number like ?", (booknumber,))
                rows=cur.fetchone()
                for row in rows:
                    print(row)
                time.sleep(1)
                userid = input("반납하는 유저 아이디를 입력해주세요\n>>")
                cur.execute("insert into bookRetun values(?,?,?,?,?,?,date('now'))", (userid,rows[0], rows[1], rows[2], rows[3], rows[4]))
                conn.commit()
                cur.execute("delete from user where number like ?", (booknumber,))
                conn.commit()
                conn.close()
                print(f'{rows[1]}이(가) 반납이 완료되었습니다.')
                #self.basket.clear()
            elif num == 'n':
                pass
    
def SelectMenu():
        os.system('cls')
        b = Library()
        sw = True
        while sw:
            print("            *도서관리 프로그램*")
            print("               1:도서 조회")
            print("               2:도서 대여")
            print("               3:도서 반납")
            print("               4:도서 기증")
            print("               5:나의 정보")
            print("               0:종료")
            user_input = input("메뉴 선택 ☞: ")
            if user_input == '0':
                return
            elif user_input == '1':
                slt = int(input('1) 고유번호로 조회   2) 책이름 조회 3) 나가기\n>>'))
                if slt == 1:
                    b.bookSearch()
                elif slt == 2:
                    b.outhorSearch()
                elif slt == 3:
                    return
            elif user_input == '3':
                b.bookReturn()
            elif user_input == '4':
                b.bookGive()


SelectMenu()