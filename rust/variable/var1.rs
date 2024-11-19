// var1.rs
#![allow(unused_mut)]

use std::mem;

fn print_type_of<T>(_: &T) {
  println!("{}", std::any::type_name::<T>());
}

fn main() {
  let a: i8 = 127;
  println!("a = {}", a);
  print_type_of(&a);

  let pi: f64 = 3.141592653589793238462643383;
  println!("pi = {}", pi);
  print_type_of(&pi);

  let s1 = "hello";
  println!("s1 = {}", s1);
  print_type_of(&s1);

  let mut s2 = String::from("hello");
  println!("s2 = {}", s2);
  print_type_of(&s2);

  let flag1 = true; let flag2:bool = false;
  println!("flag1={} flag2={}", flag1, flag2);
  print_type_of(&flag1);

  let mut tup: (i32, f64, u8) = (500, 6.4, 1);
  tup.0 = 123;
  println!("The value of y is: {}, {}, {}", tup.0, tup.1, tup.2);
  print_type_of(&tup);

  let c: char = 'A';
  println!("c={}", c);
  print_type_of(&c);
  println!("Size of char: {} byte", mem::size_of_val(&c));

}
