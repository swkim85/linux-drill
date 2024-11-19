// own4.rs
fn main() {
  let s1 = func1();
  let s2 = String::from("hello");
  let s3 = func2(s2);
  println!("s1={}", s1);
  //println!("s2={}", s2);
  println!("s3={}", s3);
}

fn func1() -> String { 
  let some_string = String::from("hello");
  some_string
}

fn func2(a_string: String) -> String {
  a_string
}


