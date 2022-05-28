while True
    print("""도서 관리 시스템에 오신 걸 환영합니다!
    원하시는 메뉴를 고르시오.

1. 도서 조회
2. 도서 대여
3. 도서 반납
4. 도서 기증
5. 나의 정보
6. 나가기
""")

    menu = input()

    if menu == "1":
        manager.showbooks()
    elif menu == "2":
        title = input("제목을 입력하세요 : ")
        author = input("저자를 입력하세요 : ")
        manager.appendbook(book(title, author))
    elif menu == "3":
        pass
    elif menu == "4":
        pass
    elif menu == "5":
        pass
    else:
        print("올바른 번호를 입력하세요!!")