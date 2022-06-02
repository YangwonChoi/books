import sqlite3
import time
import os

def bookSearch(): # 숫자로 검색    ########
        conn = sqlite3.connect('dbDate.db')
        author = int(input("고유번호 입력하세요\n>>"))
        cur = conn.cursor()
        cur.execute("select number, bookname, writer, publisher, year from booklist where number = ?", (author,))
        rows=cur.fetchone()
        for row in rows:
            print(row)
        num = input("장바구니에 담겠습니까? 수락(y) 거절(n)\n>>")
        if num == 'y':
            cur.execute("insert into user values(?,?,?,?,?,date('now'))", (rows[0], rows[1], rows[2], rows[3], rows[4]))
            print(f'{rows[1]}이(가) 장바구니에 담았습니다.')
            conn.commit()
            conn.close()
        elif num == 'n':
            return

        home=input('아무키나 누르면 홈으로 돌아갑니다.\n>>')

def outhorSearch():  # 이름 검색   ########
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
            cur.execute("select number, bookname, writer, publisher, year from booklist where number like ?", (booknumber,))
            rows=cur.fetchone()
            for row in rows:
                print(row)
            time.sleep(1)
            cur.execute("insert into user values(?,?,?,?,?,date('now'))", (rows[0], rows[1], rows[2], rows[3], rows[4]))
            print(f'{rows[1]}이(가) 장바구니에 담았습니다.')
            conn.commit()
            conn.close()
        elif num == 'n':
            return

        home=input('아무키나 누르면 홈으로 돌아갑니다.\n>>')

def bookReturn():
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
            cur.execute("insert into bookRetun values(?,?,?,?,?,date('now'))", (rows[0], rows[1], rows[2], rows[3], rows[4]))
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
        #b = Library()
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
                    bookSearch()
                elif slt == 2:
                    outhorSearch()
                elif slt == 3:
                    return
            elif user_input == '3':
                bookReturn()


SelectMenu()