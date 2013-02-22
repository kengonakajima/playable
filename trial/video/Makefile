
LIBS = moyai/libmoyai.a moyai/libfmodex.dylib moyai/libsoil.a -framework OpenGL -framework GLUT  -framework CoreFoundation  -m64  moyai/libpng-1.5.13/.libs/libpng15.a moyai/libftgl.a moyai/zlib-1.2.7/libz.a moyai/freetype-2.4.10/objs/.libs/libfreetype.a moyai/bzip2-1.0.6/libbz2.a


all: gen

gen: gen.cpp lz4.o
	g++ -g -O3 -I./moyai gen.cpp $(LIBS) lz4.o -o gen 

lz4.o : lz4/lz4.c
	g++ -c lz4/lz4.c 

clean:
	rm -f gen
