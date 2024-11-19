macro_rules! add_two {
    ($x:expr, $y:expr) => {
        $x + $y
    };
}

fn main() {
    println!("{}", add_two!(1, 2));
}
