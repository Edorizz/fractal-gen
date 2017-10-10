.POSIX:
.SUFFIXES:

CC	:= gcc
CFLAGS	:= -std=c99 -pedantic -Wall
LDLIBS := -ldmatrix -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm
RM	:= rm -f

fractal-gen: fractal-gen.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@ -g

.PHONY:
clean:
	$(RM) fractal-gen

