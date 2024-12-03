
import curses

#stdscr = curses.initscr()

def main(stdscr):
  curses.start_color()
  curses.use_default_colors()

  # 녹색 색상 쌍 정의 (쌍 번호 1, 전경색 녹색, 배경색 기본)
  curses.init_pair(1, curses.COLOR_GREEN, -1)

  # 녹색으로 "안녕" 출력
  stdscr.addstr("안녕", curses.color_pair(1))
  stdscr.refresh()
  stdscr.getch()  # 키 입력 대기

  # 원래 색상으로 복원
  stdscr.addstr(1, 0, "안녕")  # 새 줄에 기본 색상으로 출력
  stdscr.refresh()
  stdscr.getch()  # 키 입력 대기

curses.wrapper(main)


