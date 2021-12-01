# Makefile for GPS tracks project

#==========================================================================================
# NOTE: ADD YOUR .h .cpp FILES BELOW
# =========================================================================================
HEADER_FILES = gps.h ll.h
IMPLEMENTATION_FILES = gps.cpp ll.cpp 



#==========================================================================================
# DO NOT TOUCH WHAT IS BELOW
# ==========================================================================================
CFLAGS = -Wall -Wextra -Wno-unused -Wno-sign-compare -O2 -g $(shell pkg-config --cflags libxml-2.0 gtk+-3.0 cairo)
LDLIBS = $(shell pkg-config --libs libxml-2.0 gtk+-3.0 cairo) -lm

gtkexample: gtkexample.cpp
	g++ $(CFLAGS) gtkexample.cpp $(LDLIBS) -o $@

xmlexample: xmlexample.cpp
	g++ $(CFLAGS) xmlexample.cpp $(LDLIBS) -o $@

track: track.cpp $(HEADER_FILES) $(IMPLEMENTATION_FILES)
	g++ $(CFLAGS) track.cpp $(IMPLEMENTATION_FILES) $(LDLIBS) -o $@

run:
	@./track || :

.PHONY: %_test
runpy_%: %.py
	@python3 $< || :
