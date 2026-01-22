# Ray-traC

I got insomnia again, so i decided to explore something I really wanted to dive into, ray-tracing. And of course, i'm gonna do it in C.

The program generates 2 (can generate more, haven't tested though) entites, with at least 1 as a light source.
The program will compute every mathematical nightmare embedded in itself to simulate graphically light and shadow (basic ray-tracing).

Tested en MacOS.

## Screenshots
<img width="1000" src="https://github.com/user-attachments/assets/f4125397-8065-4400-adac-9aff064b14d4" />

<img width="1191" src="https://github.com/user-attachments/assets/9009d7d3-3328-42f2-8f31-33c4429a3d26" />

<img width="1121" src="https://github.com/user-attachments/assets/c9846815-ab0a-4661-a0e2-f37c345dbca5" />

<img width="1000" src="https://github.com/user-attachments/assets/3bf4660d-c1e4-4366-8921-5a1136786c54" />

(i'm not showing it, but there are sometimes some weird bugs, don't tell anyone)

## Build

You'll need the [Raylib library](https://github.com/raysan5/raylib/).
You can build the project with this command: 

Mac
```bash
gcc ray-tracing.c -I/opt/homebrew/opt/raylib/include -L/opt/homebrew/lib -o ray-tracing -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
```

Linux
```bash
gcc ray-tracing.c -o ray-tracing -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
