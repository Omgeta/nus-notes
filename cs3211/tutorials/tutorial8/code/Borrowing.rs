fn f(x: Vec<i32>) {
    // f now owns x!

    // not ok
    // x[0] = 0;

    // ok
    print!("{}\n", x[0]);

    // x is destroyed!
}

fn g(x: &Vec<i32>) {
    // not ok
    // x[0] = 0;

    // ok
    print!("{}\n", x[0]);
}

fn h(x: &mut Vec<i32>) {
    // ok
    x[0] = 0;
    print!("{}\n", x[0]);
}

fn main() {
    let mut x = Vec::new();
    x.push(1);
    x.push(2);
    x.push(3);

    // invalid! ownership has been passed to y
    // let y = x;
    // print!("{}\n", x[0]);

    // invalid! ownership has been passed to f
    // f(x);
    // print!("{}\n", x[0]);

    g(&x);
    // ok
    print!("{}\n", x[0]);

    h(&mut x);
    // ok
    print!("{}\n", x[0]);
}

