#![allow(unused)]

fn print_type_of<T>(_: &T) {
    println!("{}", std::any::type_name::<T>());
}

fn main() {
  let mut x;
  x = 1..=5;
  print_type_of(&x);
}
