use std::io;
use std::io::{Read, Write};
use std::thread;
use std::net::TcpListener;

fn server () -> io::Result<()> {
    // Listen on localhost, port 8080
    let lis = TcpListener::bind("127.0.0.1:8080")?;

    for stream in lis.incoming() {
        let mut stream = match stream {
            Ok(stream) => stream,
            Err(e) => {
                println!("Error: {}", e);
                continue;
            }
        };

        // Spawn a new thread not to block, but to accept a new connection
        let _ = thread::spawn(
            move || -> io::Result<()> {
                loop {
                    let mut buf = [0; 1024];
                    let n = stream.read(&mut buf)?;
                    if n == 0 {
                        return Ok(());
                    } 
                    else {
                        stream.write(b"You just wrote: ");
                        stream.write(&buf[0..n])?;
                    }
                }
            });
    }
    Ok(())
}

fn main() {
    match server() {
        Ok(_) => (),
        Err(e) => println!("{:?}", e),
    }
}
