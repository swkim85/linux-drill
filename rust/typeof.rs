// typeof.rs
fn print_type_of<T>(_: &T) {
  println!("{}", std::any::type_name::<T>())
}

fn main() {
  let s = "Hello";
  let i = 42;

  print_type_of(&s); // &str
  print_type_of(&i); // i32
  print_type_of(&main); // playground::main
  print_type_of(&print_type_of::<i32>); // playground::print_type_of<i32>
  print_type_of(&{ || "Hi!" }); // playground::main::{{closure}}
}

