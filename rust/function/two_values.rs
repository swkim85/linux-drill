#![allow(unused_mut)]

fn calculate(a: i32, b: i32) -> (i32, i32) {
  let sum = a + b;
  let product = a * b;
  (sum, product) // 튜플로 반환
}

fn main() {
  let (sum, product) = calculate(5, 10); // 함수 호출 및 튜플 해제
  println!("Sum: {}, Product: {}", sum, product);
}


