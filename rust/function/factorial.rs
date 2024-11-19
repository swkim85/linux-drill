// factorial.rs
fn factorial(n: u64) -> u64 {
  if n == 0 || n == 1 {
    return 1;
  } else {
    n * factorial(n - 1)
  }
}
fn main() {
  let v = factorial(6);
  println!("{}", v);
}
