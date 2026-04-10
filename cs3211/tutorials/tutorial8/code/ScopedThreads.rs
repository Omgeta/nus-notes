use std::thread;

fn main() {
  let mut counter = 0;

  thread::scope(|s| {
    s.spawn(|| { counter += 1; });
  });

  println!("{}", counter);
}

