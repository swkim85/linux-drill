// range1.rs
use std::ops::RangeInclusive;

fn main() {
    // 3부터 5까지의 범위 (5 포함)
    let range: RangeInclusive<i32> = 3..=5;
    
    // new() 메서드를 사용한 생성
    let same_range = RangeInclusive::new(3, 5);
    
    assert_eq!(range, same_range);
    
    // 범위 내의 값 합계 계산
    assert_eq!(range.sum::<i32>(), 3 + 4 + 5);
    
    // 배열 슬라이싱에 사용
    let arr = [0, 1, 2, 3, 4];
    assert_eq!(&arr[1..=3], &[1, 2, 3]);
    
    // 반복자로 사용
    for i in 1..=5 {
        println!("{}", i);
    }
}
