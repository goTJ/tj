# cross-compiler for arm-linux
CC = arm-linux-gcc

# include directories
LINUXDIR = /usr/src/creator/s3c2410/linux/include
S3C2410DIR = $(LINUXDIR)/asm-arm/arch-creator-s3c2410/lib

CFLAGS = -I$(S3C2410DIR) -I$(LINUXDIR)

EXEC = get_raw
OBJS = creator_lib.o  get_raw.o imagecompare.o
SOURCE = imagecompare.c

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	-rm -f $(EXEC) *.o

# DO NOT DELETE
