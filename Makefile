
# the following lines pertain to setting up compiler options and libraries
OPTS = -Wall -g -std=c++11
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++

endif

all: menu database forum
	./menu

menu: menu.cpp texture.o
	g++ $(OPTS) -o menu texture.o menu.cpp $(LIBS)

database: database.o Opportunity.o date.o
	g++ $(OPTS) -o database Opportunity.o date.o database.o $(LIBS)

database.o: database.cpp Opportunity.h date.h
	g++ $(OPTS) -c database.cpp $(LIBS)

date.o: date.cpp date.h
	g++ $(OPTS) -c date.cpp

Opportunity.o:  Opportunity.cpp Opportunity.h
	g++ $(OPTS) -c Opportunity.cpp

forum: forum.o texture.o post.o
	g++ $(OPTS) -o forum post.o texture.o forum.o $(LIBS)

forum.o: forum.cpp post.h
	g++ $(OPTS) -c forum.cpp $(LIBS)

texture.o: texture.cpp texture.h 
	g++ $(OPTS) $(MACOSX_DEFINE) -c texture.cpp

post.o: post.cpp post.h
	g++ $(OPTS) -c post.cpp

TARBALL_FILES = README Makefile $(wildcard *.cpp *.h *.txt *.pam) 
tarball:  $(TARBALL_FILES)
	tar cfz proj.tar.gz $(TARBALL_FILES)
