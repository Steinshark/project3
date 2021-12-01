#include "gps.h"
#include <iostream>

using namespace std;

// structs
struct Data{
	gpsco       coord;
	tm*         timestamp;
};
struct Landmark{
    gpsco       coord;
    string      name;
    double      closestDistance;
};
struct Node{
	Data   data;
	Node*  next;
};
struct TrackFile{
    int nodes;
    Node* start;
    double runtime;
    double distance;
};
struct LandmarkFile{
    int nodes;
    Landmark* list;
};

//Helper functions
double calc_dist(Node*);
double calc_time(Node*);
double time_dist(tm,tm);
void write_time(double);
void write_dist(double);
void write_speed(double,double);
TrackFile* build_track(string);
LandmarkFile* build_landmark(string);
tm* input_to_tm(string,string);
void stats(TrackFile*);
void closest_waypoint(Landmark*,TrackFile*);
void landmarks(TrackFile*);
void print_sorted(LandmarkFile*);
// project functionality implementation
void mainloop_part1();
void mainloop_part2();
