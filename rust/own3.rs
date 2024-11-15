// own3.rs

fn main() {
  let mut vec: Vec<i32> = Vec::new(); // 빈 벡터 생성
  vec.push(1); // 요소 추가
  vec.push(2);
  vec.push(3);
  println!("{:?}", vec); // [1, 2, 3] 출력
  append(vec);
}

fn append(mut vec: Vec<i32>) {
  vec.push(4);
  vec.push(5);
  println!("{:?}", vec); // [1, 2, 3, 4, 5] 출력
}

