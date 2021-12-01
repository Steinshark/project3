#include "ll.h"
#include <ctime>
#include <fstream>





////////////////////////////////////////////////////////////////////////////////
//                             HELPER FUNCTIONS                               //
////////////////////////////////////////////////////////////////////////////////

    // calculate distance of a complete waypoint list
double calc_dist(Node* start){
	// running counter
	double distance = 0;

	// loop parametrics
	Node* last_node = start;
	Node* cur_node = start->next;

    // loop
	while(cur_node != NULL){
		distance += gps_dist(last_node->data.coord,cur_node->data.coord);
		cur_node = cur_node->next;
		last_node = last_node->next;
	}

	return distance;
}

    // calculate time of a complete waypoint list
double calc_time(Node* start){
	//running counter
	double cumulative_time = 0;

	//loop parametrics
	Node* last_node = start;
	Node* cur_node = start->next;

    //loop
	while(cur_node!= NULL){
		cumulative_time += time_dist(*(last_node->data.timestamp),*(cur_node->data.timestamp));
		cur_node = cur_node->next;
		last_node = last_node->next;
	}
	return cumulative_time;
}

    // compute distance between two tm structs
double time_dist(tm t1, tm t2){
    //return difference in seconds
    time_t time1_as_time_t = mktime(&t1);
    time_t time2_as_time_t = mktime(&t2);
    return difftime(time2_as_time_t, time1_as_time_t);
}

    // neatly display time in HH : MM : SS given input of seconds
void write_time(double timeInSeconds){
    int hours = timeInSeconds / 3600;
    int minutes = int(timeInSeconds / 60) - (hours*60);
    int seconds = timeInSeconds - (minutes * 60) - (hours * 3600);
    cout << "Total time: ";
    cout << hours << " hours, " << minutes << " minutes, " << seconds << " seconds" << endl;
}

    // neatly display distance
void write_dist(double distance){
    cout << "Total distance: " << distance << endl;
}

    // build a struct containing everything needed to compute
void write_speed(double time, double distance){
    cout << "Average speed: " << distance / time << " mph" << endl;
}

    // values for a given workout
TrackFile* build_track(string filename){

        //init
        TrackFile* newTrack = new TrackFile;

        //file IO
        ifstream newfile(filename);

        string lat,lon,date,time;
        string line = "";

        // get nodes
        newfile >> newTrack->nodes;

        // get first node
        newfile >> lat >> lon >> date >> time;

        Node* newNode = new Node;
        newNode->data.coord.lat = stod(lat);
        newNode->data.coord.lon  = stod(lon);
        newNode->data.timestamp = input_to_tm(date,time);
        newTrack->start = newNode;

        Node* lastNode = newNode;

        while(newfile >> lat >> lon >> date >> time){

            newNode = new Node;
            lastNode->next = newNode;
            //location data
            newNode->data.coord.lat = stod(lat);
            newNode->data.coord.lon  = stod(lon);

            //time data
            newNode->data.timestamp = input_to_tm(date,time);
            lastNode = newNode;
        }
        return newTrack;
}

    // values for a given list of landmarks
LandmarkFile* build_landmark(string filename){

        //init
        LandmarkFile* lmFile = new LandmarkFile;

        //file IO
        ifstream newfile(filename);
        string lat,lon,name;
        string line = "";

        // get size
        newfile >> lmFile->nodes;
        lmFile->list = new Landmark[lmFile->nodes];

        // get all landmarks
        for(int i = 0; i < lmFile->nodes;i++){
            newfile >> lat >> lon >> name;
            Landmark* newL = new Landmark;
            newL->coord.lat = stod(lat);
            newL->coord.lon = stod(lon);
            newL->name      = name;
            lmFile->list[i] = *newL;
        }

        return lmFile;
}

    // convert date and time strings into a single tm struct
tm* input_to_tm(string date, string time){
    tm* newTm = new tm;
    newTm->tm_year = stoi(date.substr(0,4)) - 1900;
    newTm->tm_mon =  stoi(date.substr(5,2)) -    1;
    newTm->tm_mday = stoi(date.substr(8,2))       ;
    newTm->tm_hour = stoi(time.substr(0,2))       ;
    newTm->tm_min = stoi(time.substr(3,2))        ;
    newTm->tm_sec = stoi(time.substr(6,2))        ;
    return newTm;
}

    // give all stats about a given workout
void stats(TrackFile* trackFile){
    Node* start = trackFile->start;
    // write time
    int time = calc_time(start);
    write_time(time);

    // write distance
    double distance = calc_dist(start);
    write_dist(distance);
    // write speed
    write_speed(time,distance);
}

    // print closest distance along route
    // to each waypoint in a user specified file
void landmarks(TrackFile* trackFile){
    string filename;
    cout << "filename: ";
    cin >> filename;

    LandmarkFile* importedLandmarks = build_landmark(filename);

    Landmark* landmarkList = importedLandmarks->list;

    for(int i = 0; i < importedLandmarks->nodes;i++){
        closest_waypoint(&landmarkList[i],trackFile);
    }

    print_sorted(importedLandmarks);
}

    // find how far away a given landmark is from any waypoint
    // on the route
void closest_waypoint(Landmark* landmark, TrackFile* trackFile){
    double minDist  = 100000.0;
    Node* curNode   = trackFile->start;

    while(curNode != NULL){
        double dist = gps_dist(landmark->coord,curNode->data.coord);
        if(dist < minDist){
            minDist = dist;
        }
        curNode = curNode->next;
    }
    landmark->closestDistance = minDist;
}

    // print a list of waypoints in increasing order
    // of distance
void print_sorted(LandmarkFile* lFile){
    int cur_placing = 0;
    for(int placing = 0; placing < ((lFile->nodes) - 1); placing ++){
        double curMin = 10000;
        int minIndex = placing;
        for(int index = placing; index < ((lFile->nodes)); index++){
                double val = lFile->list[index].closestDistance;
                if(val < curMin){
                    curMin = val;
                    minIndex = index;
                }
        }
        Landmark current    = lFile->list[placing];
        lFile->list[placing]      = lFile->list[minIndex];
        lFile->list[minIndex]     = current;
        cout << lFile->list[placing].name << " : " << lFile->list[placing].closestDistance << endl;
    }
    cout << lFile->list[lFile->nodes-1].name << " : " << lFile->list[lFile->nodes-1].closestDistance << endl;
}



////////////////////////////////////////////////////////////////////////////////
//                             PART FUNCIONALITY                              //
////////////////////////////////////////////////////////////////////////////////
    // PART1
void mainloop_part1(){
    // handle fileIO
    cout << "filename: ";
    string filename;
    cin >> filename;

    // read data into nice, neat, struct
    TrackFile* newTrack = build_track(filename);

    // write statistics
    cout << "Opened " << filename << " with " << (*newTrack).nodes << " waypoints" << endl;

    // command loop
    string command;
    while(cout << "command: " && cin >> command && command != "quit"){
        // exec commands
        if(command == "linked"){
            cout << "yes" << endl;
        }
    }
}

    // PART2
void mainloop_part2(){
    // handle fileIO
    cout << "filename: ";
    string filename;
    cin >> filename;

    // read data into nice, neat, struct
    TrackFile* newTrack = build_track(filename);

    // write statistics
    cout << "Opened " << filename << " with " << (*newTrack).nodes << " waypoints" << endl;

    // command loop
    string command;
    while(cout << "command: " && cin >> command && command != "quit"){
        // exec commands
        if(command == "linked"){
            cout << "yes" << endl;
        }
        else if(command == "stats" ){
            stats(newTrack);
        }
        else if(command == "landmarks"){
            landmarks(newTrack);
        }
    }
}
