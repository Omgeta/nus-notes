use std::net::SocketAddr;

use tokio::io::{AsyncBufReadExt, AsyncWriteExt, BufReader};
use tokio::net::{TcpListener, TcpStream};

async fn handle_client(stream: TcpStream) -> std::io::Result<()> {
    let mut reader = BufReader::new(stream);
    let mut buf: Vec<u8> = Vec::new();
    loop {
        let size = reader.read_until(b'\n', &mut buf).await?;
        if size == 0 || buf[size - 1] != b'\n' {
            break;
        }
        reader.get_mut().write_all(&buf[..size]).await?;
        buf.clear();
    }
    Ok(())
}

#[tokio::main]
async fn main() -> std::io::Result<()> {
    let port = std::env::args()
        .nth(1)
        .map(|s| s.parse().unwrap())
        .unwrap_or(50000u16);
    let listener = TcpListener::bind(SocketAddr::from(([127, 0, 0, 1], port))).await?;
    loop {
        let (socket, _) = listener.accept().await?;
        tokio::spawn(async move {
            eprintln!("Accepted connection");
            std::mem::drop(handle_client(socket).await);
            eprintln!("Connection ended");
        });
    }
}
