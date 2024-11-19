// player.rs
mod player {
  // 비공용 함수
  fn focus() {
    println!("called player::focus");
  }

  // 공용 함수
  pub fn shift() {
    println!("called player::shift");
  }

  pub fn jump() {
    focus(); // 같은 모듈 내에서 비공용 함수 호출 가능
    shift();
    println!("called player::jump");
  }
}

// main.rs
fn main() {
    player::jump(); // 공용 함수 호출
}
