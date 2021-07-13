# Generic C/C++ makefile for Linux & OSX & Windows 

EXE := bin
FOLDER_TARG := ~

ifeq "${OS}" "Windows_NT"
	detected_OS := "Windows_NT"
	TARGET := $(EXE).exe
	CLEAN := del *.obj $(EXE).exe 
	RUNTARGET := $(EXE).exe 
else 
detected_OS := $(shell uname -s)
	DEPL := cp $(EXE) $(FOLDER_TARG)
	TARGET := $(EXE) 
	CLEAN := rm -f *.o $(EXE) 
	RUNTARGET := ./$(EXE)  
endif 

SOURCES1 := $(wildcard *.c)
SOURCES2 := $(wildcard *.cpp) 

OBJS1 := $(subst .c,.o,$(SOURCES1)) 
OBJS2 := $(subst .cpp,.o,$(SOURCES2)) 
OBJS_UNIX := $(OBJS1) $(OBJS2) 

OBJS3 := $(subst .c,.obj,$(SOURCES1)) 
OBJS4 := $(subst .cpp,.obj,$(SOURCES2))
OBJS_WIN := $(OBJS3) $(OBJS4) 

# OS X
ifeq "$(detected_OS)" "Darwin"
     LDFLAGS = -framework OpenGL -framework GLUT    
endif 

# Linux 
ifeq "$(detected_OS)" "Linux"
	LDFLAGS = -lGL -lGLU -lglut -lm 
endif 

.SUFFIXES: .c .o .cpp  .obj 

.c.o: 
	gcc -c -Wall -Wno-deprecated  $< 

.cpp.o: 
	g++ -c -Wall -Wno-deprecated  $< 

.cpp.obj: 
	cl 	/c /W4   /wd4255  $< /I C:\GLUT

.c.obj: 
	cl 	/c  /W4  /wd4255 $< /I C:\GLUT	
	
all: $(TARGET)

$(EXE): $(OBJS_UNIX)
	g++ -o $(EXE) -Wall $(OBJS_UNIX) # $(LDFLAGS)

$(EXE).exe: $(OBJS_WIN) 	
		@echo $(OBJS_WIN)
		LINK  /WX $(OBJS_WIN) /LIBPATH:C:\GLUT  /OUT:$(EXE_NAME).exe
		
clean: 
	$(CLEAN) 

run:  all   
	$(RUNTARGET)

full: all
	$(RUNTARGET)
	$(DEPL)
