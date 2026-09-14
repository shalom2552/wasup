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

## Features

- Rooms 1–16, up to 64 clients on one server.
- Chat history: see past messages on join.
- Room join and leave notifications.
- Live online count for the room.
- Timestamped messages.

## Build

```sh
git clone https://github.com/shalom2552/wasup
cd wasup/
make
```

Requires: gcc, make, C99, Linux (POSIX sockets, poll).

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

Blank name: `Anonymous`. Blank or invalid room: room 0.

## Commands

| Command | Action |
|---------|--------|
| `/exit` `/quit` `/q` | Disconnect |
| `Ctrl+C` | Quit |

## Project structure

```
./                  — Makefile
inc/                — headers
src/
    chat.c          — entry point, arg parsing
    client.c        — client loop, server event handling
    server.c        — server loop, broadcast, handshake
    history.c       — per-room message history
    tcp.c           — socket setup, connect, non-blocking accept
    chat_utils.c    — send/recv framing, notification protocol
    ui.c            — display, input, colors, cursor
    log.c           — logging (error, warn, info, debug)
build/              — object files (generated)
```
