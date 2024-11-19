// hello.rs
macro_rules! hello {
  () => { println!("Hello!"); };
}
fn main() {
  hello!();
}
