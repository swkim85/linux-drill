
macro_rules! print_all {
  ($($val:expr),*) => {
      $(
          println!("{}", $val);
      )*
  };
}

fn main() {
  print_all!(1, 2, 3, "Hello", true); // 다양한 타입의 인자 전달
}
