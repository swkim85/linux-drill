// var1.rs

fn main() {
  let a: i8 = 127;
  println!("a = {}", a);
  //a += 2;
  //println!("a = {}", a);

  let pi: f64 = 3.141592653589793238462643383;
  println!("pi = {}", pi);


  let flag1 = true;
  let flag2:bool = false;
  println!("flag1={} flag2={}", flag1, flag2);

  let mut tup: (i32, f64, u8) = (500, 6.4, 1);
  tup.0 = 123;
  println!("The value of y is: {}, {}, {}", tup.0, tup.1, tup.2);
}
