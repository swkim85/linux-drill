// ref2.rs
fn main() {
  let mut s1 = String::from("hello");
  change(&mut s1);
  println!("s1=\"{}\"", s1);
  println!(r#"s1="{}""#, s1);
}
fn change(some_string: &mut String) {
  some_string.push_str(", Rust");
}
