// sizeof.rs
use std::mem;

fn main() {
    println!("Size of bool: {} byte", mem::size_of::<bool>());
    println!("Size of u32: {} bytes", mem::size_of::<u32>());
    println!("Size of String: {} bytes", mem::size_of::<String>());
}
