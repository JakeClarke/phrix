OBJS =	phrix.o game/game.o assetmanager/assetmanager.o game/jobmanager.o game/servicemanager.o game/scene.o

TARGET =	phrix

SDL_CFLAGS := $(shell sdl2-config --cflags)

SDL_LDFLAGS := $(shell sdl2-config --libs)

CXXFLAGS += -O0 -std=c++11 -g $(SDL_CFLAGS)

LIBS = $(SDL_LDFLAGS)

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET) 