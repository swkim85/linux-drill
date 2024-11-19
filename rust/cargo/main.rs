//#[allow(unused_imports)]
#![allow(unused_must_use)]

use stack_stack::Stack;
//use stack_stack::{Stack, stack};

/*
//Manual creation

//Pushing returns a result
assert_eq!(s1.push(6), Ok(()));
assert_eq!(s1.push(2), Ok(()));
assert_eq!(s1.push(8), Ok(()));
assert_eq!(s1, [6,2,8]);

//We can ergonomically ignore the #[must_use] warning if needed with `Result::ok()`
s1.push(3).ok();
s1.push(1).ok();
assert_eq!(s1, [6,2,8,3,1]);

//Overflows return return the extra value(s) in a `Result::Err()`
assert_eq!(s1.push(101), Err(101));

//Creation using a list of values and a capacity
let s2 = stack![6,2,8,3,1; 10];
assert_eq!(s2, [6,2,8,3,1]);
assert_eq!(s2.capacity(), 10);

//Repeating a value of `3` 4x times with a capacity of 5
let s3 = stack![3; 4; 5];
assert_eq!(s3, [3,3,3,3]);
assert_eq!(s3.len(), 4);
assert_eq!(s3.capacity(), 5);

fn stack_to_string<T: std::fmt::Debug, const N: usize>(stack: &Stack<T, N>) -> String {
  let mut result = String::from("Stack contents: [");
  for (index, item) in stack.iter().enumerate() {
    if index > 0 {
      result.push_str(", ");
    }
    result.push_str(&format!("{:?}", item));
  }
  result.push(']');
  result
}
*/

const N :usize = 5;

fn print_stack<T: std::fmt::Debug>(stack: &Stack<T, N>) {
  println!("length/capacity={}/{}", stack.len(), stack.capacity());
  print!("[");
  for (index, item) in stack.iter().enumerate() {
    if index > 0 { print!(","); }
    print!("{:?}", item);
  }
  println!("]\n");
}

fn main() {
  let mut s1 = Stack::with_capacity::<N>();
  s1.push(6);
  s1.push(2);
  s1.push(8);
  print_stack(&s1);
}
