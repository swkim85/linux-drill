// for1.rs

#![allow(warnings)] // convert the identifier to upper case: `NUMBERS` 경고를 무시
static numbers: [i32; 5] = [1, 2, 3, 4, 5];

fn main() {
  for num in numbers {  // 배열
    print!("{}", num);
  }

  for i in 1..=5 {  // 범위
    print!("{}", i);
  }
}
