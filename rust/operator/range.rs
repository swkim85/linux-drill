// range.rs
fn main() {
  for i in 0..5 { // 0부터 4까지의 범위 (상한 제외 ..)
    print!("{} ", i);
  }
  println!();
  for c in 'a'..='e' { // 문자 범위 (상한 포함 ..=)
    print!("{} ", c);
  }
  println!();
    
  // match 표현식에서 사용
  let x = 5;
  match x {
    1..=5 => println!("x는 1에서 5 사이입니다"),
    _ => println!("x는 범위를 벗어났습니다"),
  }
}

