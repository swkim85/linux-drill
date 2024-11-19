
fn sum(numbers: &[i32]) -> i32 {
  let mut total = 0;
  for &num in numbers {
      total += num;
  }
  total
}

fn main() {
  let result = sum(&[1, 2, 3, 4, 5]); // 슬라이스로 전달
  println!("Sum: {}", result);
}
