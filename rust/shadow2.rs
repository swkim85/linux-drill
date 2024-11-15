// shadow2.rs
fn main() {
  let x = 5;

  println!("address of x: {:p}", &x);

  let x = x + 1;
  println!("address of x: {:p}", &x);
}
