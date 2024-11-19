//#[warn(unused_mut)]
#![allow(unused)]

fn print_type_of<T>(_: &T) {
  println!("{}", std::any::type_name::<T>());
}

fn add_vectors(v1: &Vec<i32>, v2: &Vec<i32>) -> Vec<i32> {
  v1.iter().zip(v2.iter()).map(|(&a, &b)| a + b).collect()
}
fn print_vector(name: String, v: &Vec<i32>) {
  println!("{}: {:?}", name, v);
}

fn main() {
  let mut msg = "hello";
  print_type_of(&msg);

  let mut v: Vec<i32> = Vec::new();
  v.push(1); v.push(2); v.push(3);
  print_type_of(&v);
  v[0] = 0;
  //print_vector("v", &v);

  let w = vec![4, 5, 6];
  print_type_of(&w);

  let result = add_vectors(&v, &w);
  println!("결과: {:?}", result);

  let mut sum = 0;
  for n in &v { sum += n; }
  println!("length:{}, sum:{}", v.len(), sum);
}


