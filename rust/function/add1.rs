#![allow(unused_mut)]

fn myadd(a: i32, b: i32) -> i32 {
  a + b  // 세미콜론 없음, 이 표현식이 반환값
}
fn print_type_of<T>(_: &T) {
  println!("{}", std::any::type_name::<T>())
}
fn main() {
  let mut sum;
  sum = myadd(123, 456);
  println!("sum={}", sum);
  print_type_of(&myadd);
}

