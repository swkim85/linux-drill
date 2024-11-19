// own1.rs
fn main() {

  let mut x = 5; let y = x; x = 0;
  println!("x={} y={}", x, y);

  let mut s1 = String::from("hello");
  //let s2 = s1;
  let s2 = s1.clone(); // s1을 s2로 복사. 소유권 이전이 아니므로, s1을 계속 사용가능
  s1.push_str(" rust");
  println!("s1={} s2={}", s1, s2);

  let s3 = s1; // 소유권이 s3로 이전됨. 이후부터 s1은 사용할 수 없다.
  println!("s3={}", s3);
}
