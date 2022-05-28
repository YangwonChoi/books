import re
import time

class makelogin: #회원가입 클래스
    def make_id(slef, regist_user):
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
                res_id = slef.chk_id(uid)
                if not res_id:
                    continue
                else:
                    regist_id.append(uid)
                    break
        # print(regist_id)

        while True:
            pwd = str(input('비밀번호 입력: '))
            res_pwd = slef.chk_password(pwd)
            if not res_pwd:
                continue
            else:
                regist_id.append(pwd)
                break
        # print(regist_id)
        return regist_id
    def chk_id(slef, id):
        result = True
        reg = r'^[A-Za-z0-9_]{4,20}$'
        if not re.search(reg, id):
            print('아이디 생성 기준에 부적당합니다!')
            result = False
        return result
    def chk_password(slef, pwd):
        result = True
        reg = r'^(?=.*[A-Za-z])(?=.*\d)(?=.*[!@#$%&*?])[A-Za-z\d!@#$%&*?]{8,20}$'
        if not re.search(reg, pwd):
            print('비밀번호 기준에 부적당합니다!')
            result = False
        return result
    def edit_password(slef, uid, pwd):
        n_pwd = ''
        while True:
            pw = str(input('새로운 비밀번호 입력: '))
            if pw == pwd:
                print(f'기존 비번하고 같음!')
                continue
            else:
                res_pwd = slef.chk_password(pw)
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
#git test
#신입테스트
#리눅스테스트1