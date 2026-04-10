use std::sync::atomic::{AtomicI32, Ordering};
use std::thread;

fn main() {
    let counter = AtomicI32::new(0);

    thread::scope(|s| {
        s.spawn(|| {
            counter.fetch_add(1, Ordering::Relaxed);
        });
        s.spawn(|| {
            counter.fetch_add(1, Ordering::Relaxed);
        });
    });

    println!("{}", counter.load(Ordering::Relaxed));
}

