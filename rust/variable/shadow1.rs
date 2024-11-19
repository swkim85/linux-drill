// shadow1.rs
fn main() {
  let x = 5;
  let x = x + 1;
  let x = x * 2;
  println!("The value of x is: {}", x);

  let spaces = "   ";
  println!("space: //{}//", spaces);
  let spaces = spaces.len();
  println!("space: //{}//", spaces);

  let x = 5;
  {
    let x = x * 2; // 내부 스코프에서 x는 10
    println!("Inner scope x: {}", x);
  }
  println!("Outer scope x: {}", x); // 여기서 x는 여전히 5
}
