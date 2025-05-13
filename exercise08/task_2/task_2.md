- The example uses `INADDR_ANY`. Could we also use `INADDR_LOOPBACK`?

Yes, loopback refers to the loopback ip adress 127.0.0.1 which is the localhost.
- The example uses `SOCK_STREAM`. Name two other types of sockets and briefly explain their difference compared to `SOCK_STREAM`.

SOCK_DGRAM: Protocol used for UDP connections. Trustless
SOCK_RAW: sends just raw ip packets without TCP or UDP headers
SOCK_STREAM: uses TCP to send the packets which first opens a stable connection to the client via handshake

- What is the range of possible ports that we assign to `addr.sin_port` in the server?

0–65535

- Why is it a good idea to use a port greater or equal to `1024`?

because prots lower than 1024 are priviledged ports and are mostly reserved. They require root privileges
