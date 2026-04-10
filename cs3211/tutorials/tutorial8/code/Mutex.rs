
use std::thread;
use std::sync::Mutex;

fn main() {
  let counter = Mutex::new(0);

  thread::scope(|s| {
    s.spawn(|| { 
        // THE FOLLOWING IS NOT IDIOMATIC RUST 
        // (what *counter.lock().unwrap() += 1 actually does)
        use std::ops::DerefMut;

        let lock_result = counter.lock();
        let mut lock_guard = lock_result.unwrap();
        let counter_ref: &mut i32 = lock_guard.deref_mut();
        *counter_ref += 1;
        // END UNIDIOMATIC RUST
    });
    s.spawn(|| { *counter.lock().unwrap() += 1; });
  });

  println!("{}", *counter.lock().unwrap());
}
