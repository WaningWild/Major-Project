#sudo apt-get install build-essential
#sudo apt-get install ffmpeg
#sudo apt-get install libav-tools
#sudo apt-get install libopencv-dev

CXX           = g++
CXXFLAGS      = -std=c++11 -g -Wall -D__STDC_CONSTANT_MACROS #-v -Q
LIBS          = -lm                     \
                -lpthread               \
                -lavutil                \
                -lavformat              \
                -lavcodec               \
                -lswscale               \
                -lopencv_calib3d        \
                -lopencv_core           \
                -lopencv_features2d     \
                -lopencv_flann          \
                -lopencv_highgui        \
                -lopencv_imgcodecs      \
                -lopencv_imgproc        \
                -lopencv_ml             \
                -lopencv_objdetect      \
                -lopencv_photo          \
                -lopencv_stitching      \
                -lopencv_superres       \
                -lopencv_video          \
                -lopencv_videoio        \
                -lopencv_videostab
OBJS          = ../src/lib/cvdrone/ardrone/ardrone.o	 \
                ../src/lib/cvdrone/ardrone/command.o	 \
                ../src/lib/cvdrone/ardrone/config.o 	 \
                ../src/lib/cvdrone/ardrone/udp.o    	 \
                ../src/lib/cvdrone/ardrone/tcp.o    	 \
                ../src/lib/cvdrone/ardrone/navdata.o	 \
                ../src/lib/cvdrone/ardrone/version.o	 \
                ../src/lib/cvdrone/ardrone/video.o  	 \
		../src/DroneController/droneController.o \
		../src/Globals/globals.o	     	 \
		../src/Vision/vision.o		     	 \
		../src/InputHandling/input.o		 \
		../src/main.o			

PROGRAM       = droneVision

$(PROGRAM):     $(OBJS)
		$(CXX) $(OBJS) -o $(PROGRAM) $(CXXFLAGS) $(LDFLAGS) $(LIBS) 

clean:;         rm -f *.o *~ $(PROGRAM) $(OBJS)

install:        $(PROGRAM)
		install -s $(PROGRAM) $(DEST)
