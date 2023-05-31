#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define COLOR "\e[38;5;197m"
#define CLR "\e[0m"

/* Got offsets through scanmem: https://github.com/scanmem/scanmem */
int healthOffset = 0x100;
int pistolAmmoOffset = 0x140;
int rifleAmmoOffset = 0x154;

uint64_t infiniteAmmo_offset = 0x10306e; // 0x0050306e
uint64_t base_address = 0x400000;

unsigned char ammoPatch[3] = {0x90, 0x90, 0x90};

void *ac1337_main(void *null){
    sleep(5);
    char **static_ptr = (char **)0x005a3518;
    char *playerObj = (char *)(*static_ptr);
    unsigned char *patch_me = (unsigned char *)(base_address+infiniteAmmo_offset);
    unsigned char instruction_save[3] = {0, 0, 0};
    pid_t pid = getpid();
    int memfd = 0, bytes = 0;

    printf("%s[AC 1337-Hax ] PID: %d\n%s", COLOR, pid, CLR);
    //printf("Player Object@%p\n", playerObj);
    memfd = open("/proc/self/mem", O_RDWR);
    if(memfd == -1){
        fprintf(stderr, "%sFailed to process address space\n%s", COLOR, CLR);
        return NULL;
    }
    lseek(memfd, (uint64_t)patch_me, SEEK_SET);

    instruction_save[0] = *(unsigned char *)(patch_me);
    instruction_save[1] = *(unsigned char *)(patch_me+1);
    instruction_save[2] = *(unsigned char *)(patch_me+2);

    /* Patch instructions via /proc/{PID}mem */
    printf("%s[i] Enabling infinite ammo...\n%s", COLOR, CLR);
    bytes = write(memfd, ammoPatch, 3);
    if(bytes != 3){
        fprintf(stderr, "%s[!] Failed to patch bytes...\n%s", COLOR, CLR);
        close(memfd);
        return NULL;
    }
    printf("%s[+] Infinite ammo enabled!\n%s", COLOR, CLR);
    printf("%s[i] Addresses:\n%s", COLOR, CLR);
    printf("\t%s-> Health (%p) = %u\n%s", COLOR, (uint32_t *)(playerObj+healthOffset), *(uint32_t *)(playerObj+healthOffset), CLR);
    printf("\t%s-> AR ammo (%p) = %u\n%s", COLOR, (uint32_t *)(playerObj+rifleAmmoOffset), *(uint32_t *)(playerObj+rifleAmmoOffset), CLR);
    printf("\t%s-> Pistol ammo (%p) = %u\n%s", COLOR, (uint32_t *)(playerObj+pistolAmmoOffset), *(uint32_t *)(playerObj+pistolAmmoOffset), CLR);

    close(memfd);
    return null;
}

void SDL_SetWindowIcon(SDL_Window * window, SDL_Surface * icon){
    pthread_t t1;
    void (*real_func)(SDL_Window *, SDL_Surface *) = NULL;
    
    /* call the real SDL_SetWindowIcon so everything runs as expected */
    real_func = dlsym(RTLD_NEXT, "SDL_SetWindowIcon");    
    real_func(window, icon);
    /* Create a new thread - same address space used */
    pthread_create(&t1, NULL, ac1337_main, NULL);
    
    return;
}
