COMPILER  = g++
CFLAGS    = -g -MMD -MP -Wextra -Wno-format -Wno-sign-compare -Winit-self -Wno-missing-field-initializers -Wno-psabi -DBOOST_LOG_DYN_LINK -DVACANCY_USE_STB -std=c++11
ifeq "$(shell getconf LONG_BIT)" "64"
  LDFLAGS = -L/usr/lib64 -L/usr/local/lib -L./lib
else
  LDFLAGS = -L/usr/local/lib
endif
LIBS      = -lpthread -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_features2d -lboost_log -lboost_log_setup -lboost_program_options -lboost_thread
INCLUDE   = -I./include
TARGET    = ./a.out
SRCDIR    = ./src
ifeq "$(strip $(SRCDIR))" ""
  SRCDIR  = .
endif
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR    = ./obj
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR  = .
endif
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS)
	echo "$(OBJECTS)"
	$(COMPILER) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) $(LIBS) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)

-include $(DEPENDS)
