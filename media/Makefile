#compiler
GCC += gcc

#FLAGS
CFLAGS += -std=c11
LDFLAGS += `pkg-config --cflags --libs gstreamer-1.0 gtk+-3.0` -lm -lgstreamer-1.0

#saida
OUT += -o bin/media.out

#source
SRC += src/gmain.c

#all
All:
	$(GCC) $(SRC) $(CFLAGS) $(LDFLAGS) $(OUT)
	@echo "BUILD SUCCESSFUL"
	
#run successful
Run:
	@./bin/media.out
