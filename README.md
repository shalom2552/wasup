# Wasup - terminal chat

A lightweight multi-client TUI chat over TCP, written in C. Supports up to 16 rooms and 64 clients. Single-threaded, poll-based server with no external dependencies.

## Screenshots

<table>
  <tr align="center">
    <th width="25%">Server</th>
    <th width="25%">Client 1 (Sam)</th>
    <th width="25%">Client 2 (Alex)</th>
    <th width="25%">Client 3 (Jordan)</th>
  </tr>
  <tr>
    <td colspan="4">
      <img src="https://github.com/user-attachments/assets/bc71dc10-1b21-44d9-9392-c9a668f84b98" alt="TUI Chat Screenshot" width="100%">
    </td>
  </tr>
</table>

## Future

- Chat history: Show previous messages on login.

## Build

```sh
make
```

Requires: gcc, C99.

## Usage

**Start server:**
```sh
./chat -s [port]
```
> default port: 1313

**Connect as client:**
```sh
./chat [ip] [port]
```
> defaults: 127.0.0.1 1313

On connect, enter username and room number (1–16). Clients in the same room receive each other's messages.

## Commands

| Command | Action |
|---------|--------|
| `/exit` `/quit` `/q` | Disconnect |

## Project structure

```
./                  — entry point (chat.c) + Makefile
inc/                — headers
src/
    client.c        — client loop, server event handling
    server.c        — server loop, broadcast, handshake
    tcp.c           — socket setup, non-blocking connect/accept
    chat_utils.c    — send/recv framing, notification protocol
    ui.c            — display, input, colors, cursor
    log.c           — logging (info, warn, error)
```
