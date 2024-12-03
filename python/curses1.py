
# https://docs.python.org/ko/3/library/curses.html

import curses

curses.setupterm()
cup_capability = curses.tigetstr("cup")
if cup_capability:
  print("cup 기능 값:", cup_capability)
else:
  print("cup 기능을 찾을 수 없습니다.")

setaf_cap = curses.tigetstr("setaf")
print("setaf 기능 값:", setaf_cap)

color_string = curses.tparm(setaf_cap, curses.COLOR_GREEN)  # 녹색으로 설정
color_string = curses.tparm(setaf_cap, curses.COLOR_RED)  # 빨간색으로 설정
print(color_string.decode())

