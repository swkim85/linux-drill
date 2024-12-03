
"""
Python의 getattr() 함수는 객체의 속성 값에 동적으로 접근할 때 사용되는 유용한 내장 함수입니다.
 기본 사용법은 다음과 같습니다:
getattr(object, attribute_name[, default])

이 함수는 다음과 같이 작동합니다:
object의 attribute_name이라는 속성 값을 반환합니다13.
해당 속성이 존재하지 않을 경우, default 값이 제공되면 그 값을 반환합니다13.
default 값이 제공되지 않고 속성이 존재하지 않으면 AttributeError 예외가 발생합니다3.

getattr()의 주요 용도와 장점:
동적 속성 접근: 런타임에 속성 이름을 결정할 수 있어 유연한 코드 작성이 가능합니다14.
설정 관리: 애플리케이션 설정 값을 동적으로 가져올 수 있습니다1.
동적 메소드 호출: 사용자 입력에 따라 다른 메소드를 호출할 수 있습니다1.
코드 간소화: 조건문을 줄이고 더 간결한 코드를 작성할 수 있습니다4.

"""

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

person = Person("Jane Doe", 28)

print(getattr(person, 'name'))  # 출력: Jane Doe
print(getattr(person, 'age'))   # 출력: 28
print(getattr(person, 'height', 160))  # 출력: 160 (기본값)


