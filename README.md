# Ray-traC

I got insomnia again, so i decided to explore something I really wanted to dive into, ray-tracing. And of course, i'm gonna do it in C.

The program generates 2 (can generate more, haven't tested though) entites, with at least 1 as a light source.
The program will compute every mathematical nightmare embedded in itself to simulate graphically light and shadow (basic ray-tracing).

Tested en MacOS.

## Screenshots
<img width="1000" src="https://github.com/user-attachments/assets/f4125397-8065-4400-adac-9aff064b14d4" />

<img width="1000" src="https://github.com/user-attachments/assets/7ef1692e-b74d-4800-95b2-b0aae88eab6b" />

<img width="1000" src="https://github.com/user-attachments/assets/fe15b074-267a-4077-8678-be0a494868e3" />

<img width="1000" src="https://github.com/user-attachments/assets/3bf4660d-c1e4-4366-8921-5a1136786c54" />

(i'm not showing it, but there are sometimes some weird bugs, don't tell anyone)

## Build

You can build the project with this command: 
```c
gcc ray-tracing.c -I/opt/homebrew/opt/raylib/include -L/opt/homebrew/lib -std=c17 -Wall -Wextra -o ray-tracing -lraylib -framework  OpenGL -framework  Cocoa -framework  IOKit -framework CoreVideo
```
