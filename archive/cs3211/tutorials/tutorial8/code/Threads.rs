use std::thread;

fn main() {
  let mut counter = 0;

  let t0 = thread::spawn(|| { counter += 1; });

  t0.join();

  println!("{}", counter);
}

