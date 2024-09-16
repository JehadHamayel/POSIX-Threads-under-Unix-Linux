CC = gcc

SRCS1 = OpenGl.c
SRCS2 = project3.c
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
LIBS = -lglut -lGLU -lGL -lm
LIBS2 = -lpthread

                                                             
all: opengl project3

opengl: $(OBJS1)

	$(CC) -o $@ $^ $(LIBS)
project3: $(OBJS2)

	$(CC) -o $@ $^ $(LIBS2)
run: opengl project3

	./opengl &
	./project3
runOpenGl: opengl

	./opengl &
		
runProject3: project3

	./project3
	

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(OBJS1) opengl
	rm -f $(OBJS2) project2
