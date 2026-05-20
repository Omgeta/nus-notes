fn magic_sum(from: u128, to: u128) -> u128 {
    (from..to).filter(|i| i % 7 == i % 5).sum()
}

fn main() {
    let (from, to) = {
        // Comment out the line below if you are using the Rust Playground
        let mut args = std::env::args();
        // Use the line below instead if you are using the Rust Playground
        // let mut args = ["", "0", "100000000"].iter();
        args.next(); // skip argv[0]
        (args.next().unwrap(), args.next().unwrap())
    };
    println!("{}", magic_sum(from.parse().unwrap(), to.parse().unwrap()));
}
