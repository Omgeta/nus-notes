use std::thread;
use std::sync::{Mutex, Arc};

fn main() {
  let counter = Arc::new(Mutex::new(0));

  let t0 = {
    let counter = counter.clone();
    thread::spawn(move || {
      // THE FOLLOWING IS NOT IDIOMATIC RUST
      use std::ops::{Deref, DerefMut};

      let mutex: &Mutex<i32> = counter.deref();
      let lock_result = mutex.lock();
      let mut lock_guard = lock_result.unwrap();
      let counter_ref: &mut i32 = lock_guard.deref_mut();
      *counter_ref += 1;
      // END UNIDIOMATIC RUST
    })
  };
  let t1 = {
    let counter = counter.clone();
    thread::spawn(move || {
      *counter.lock().unwrap() += 1;
    })
  };

  t0.join().unwrap();
  t1.join().unwrap();

  println!("{}", *counter.lock().unwrap());
}
