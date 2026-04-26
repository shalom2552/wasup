# Wasup - terminal chat

A lightweight multi-client TUI chat over TCP, written in C.

## Features

- Multi-client support
- Chat rooms
- Non-blocking I/O with `poll()`
- Join/leave notifications

## Roadmap

  - [] Implement structured packet protocol to separate messages from system
  notifications

## Build

```sh
make
```

Requires: gcc, g++, C99, C++17.

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

On connect, enter your username and a room number (1–100). Clients in the same room receive each other's messages.

## Commands

| Command | Action |
|---------|--------|
| `/exit` `/quit` `/q` | Disconnect |

## Project structure

```
./                  — entry point (chat.cpp) + Makefile
inc/                — headers
src/                — source
    client.c        — client loop
    server.c        — server loop, broadcast, handshake
    tcp.c           — socket setup (non-blocking)
    chat_utils.c    — send/recv framing, input helpers
    utils.c         — display, colors, logging
```
