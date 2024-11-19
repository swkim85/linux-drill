// shadow2.rs
fn main() {
  let x = 5;
  println!("address of x: {:p}", &x);
  {
    let x = "hello"; // 타입이 변경됨
    println!("address of x: {:p}", &x);
  }
  let x = x + 1;
  println!("address of x: {:p}", &x);
}
