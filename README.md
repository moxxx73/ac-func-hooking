# ac-func-hooking
Assaultcube hacks done via hooking to the function `SDL_SetWindowIcon`.  
For now the code allows for infinite ammo by patching instructions but im   
gonna add more cheats in the future.

This has only been tested on linux so i dont know how it runs on other unix  
systems

## Building and Running ac-1337.so
To build ac-1337.so run gcc as shown below:  
```
gcc -lSDL2 -ldl -lpthread -shared -fPIC ./ac-1337.c -o ./ac-1337.so -Wall -Wextra
```
personally, i place the shared library in the **bin_unix** sub-folder in the assaultcube folder.  
To run assaultcube with the hacks:
```
LD_PRELOAD=./bin_unix/ac-1337.so ./bin_unix/linux_64_client
```
make sure to check the terminal for the ac-1337 output and thats it :)
