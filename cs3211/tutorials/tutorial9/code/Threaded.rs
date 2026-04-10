use std::io::prelude::*;
use std::io::BufReader;
use std::net::{SocketAddr, TcpListener, TcpStream};
use std::thread;

fn handle_client(stream: TcpStream) -> std::io::Result<()> {
    let mut reader = BufReader::new(stream);
    let mut buf: Vec<u8> = Vec::new();
    loop {
        let size = reader.read_until(b'\n', &mut buf)?;
        if size == 0 || buf[size - 1] != b'\n' {
            break;
        }
        reader.get_mut().write_all(&buf[..size])?;
        buf.clear();
    }
    Ok(())
}

fn main() -> std::io::Result<()> {
    let port = std::env::args()
        .nth(1)
        .map(|s| s.parse().unwrap())
        .unwrap_or(50000u16);
    let listener = TcpListener::bind(SocketAddr::from(([127, 0, 0, 1], port)))?;
    loop {
        let (socket, _) = listener.accept()?;
        thread::spawn(move || {
            eprintln!("Accepted connection");
            std::mem::drop(handle_client(socket));
            eprintln!("Connection ended");
        });
    }
}
