#include "file_system.h"
#include "common.h"

#define FS_MAX_FILE_SIZE (80 * 1024 * 1024)

#define PHYSFS_getError() (PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()))

void fileSystemInit(const char* arg) {
    if(!PHYSFS_init(arg)) {
        logger(FATAL_LEVEL, "Unable to initialize file system: %s \n", PHYSFS_getError());
    }

    char* baseDir = SDL_GetBasePath();
    logger(INFO_LEVEL, "Base directory path: '%s' \n", baseDir);

    if(!PHYSFS_mount(baseDir, NULL, 0)) {
        logger(FATAL_LEVEL, "Unable to mount base directory '%s' with file system: %s \n", baseDir, PHYSFS_getError());
    }

    const char* assetsDir = "../assets";
    if(!PHYSFS_mount(assetsDir, NULL, 0)) {
        logger(FATAL_LEVEL, "Unable to mount base directory '%s' with file system: %s \n", assetsDir, PHYSFS_getError());
    }

    char **rc = PHYSFS_enumerateFiles("/");
    char **i = NULL;
    for(i=rc; *i != NULL; ++i) {
        // TODO: if ends with pak, append to physfs assets
        printf("Pak file: '%s' \n", *i);
    }

    SDL_free(baseDir); 
    PHYSFS_freeList(rc);
}


void fileSystemFree() {
    PHYSFS_deinit();
}

int fileSystemFileExist(const char* filename) {
    return PHYSFS_exists(filename);
}

SDL_RWops* fileSystemReadFully(const char* filename) {
    PHYSFS_File* file = PHYSFS_openRead(filename);
    if(!file) {
        logger(ERROR_LEVEL, "Failed to open file '%s' - %s \n", filename, PHYSFS_getError());
        return NULL;
    }

    SDL_RWops* rw = NULL;

    int64_t len = PHYSFS_fileLength(file);
    if(len > FS_MAX_FILE_SIZE) {
        logger(ERROR_LEVEL, "File '%s' is too large to open: %d bytes \n", filename, len);
        goto exit;
    }

    uint8_t* mem = (uint8_t*)siegeMalloc(len);
    PHYSFS_readBytes(file, mem, len);

    rw = SDL_RWFromConstMem(mem, (int)len);

exit:
    PHYSFS_close(file);
    return rw;
}