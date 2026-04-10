use std::thread;
use std::sync::atomic::{AtomicI32, Ordering};

static COUNTER: AtomicI32 = AtomicI32::new(0);

fn main() {
  let t0 = thread::spawn(|| { COUNTER.fetch_add(1, Ordering::Relaxed); });
  let t1 = thread::spawn(|| { COUNTER.fetch_add(1, Ordering::Relaxed); });

  t0.join().unwrap();
  t1.join().unwrap();

  println!("{}", COUNTER.load(Ordering::Relaxed));
}
