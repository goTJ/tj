// setup image WIDTH and HEIGHT
#define WIDTH 320
#define HEIGHT 240
// max leng for buffer
#define MAX_LENG 100

// camera control port
#define CTRL_PORT 3030

// share memory setting
#define SHMKEY_CTRL ((key_t) 1004)
#define SHMKEY_MASK ((key_t) 1001)
#define SHMKEY_MASKLIST ((key_t) 1002)
#define SHLEN_CTRL 5
#define SHLEN_MASK (WIDTH*HEIGHT)
#define SHLEN_MASKLIST (2 + 4*10)
#define PERMS 0666
