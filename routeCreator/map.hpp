#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <thread>
#include <map>

using namespace std;
using namespace cv;
using namespace chrono;
using namespace this_thread;

class Country {

    private:
        int routeID = 0;
        Mat countryMap;
        Mat route; 
        bool flagRoadLine = false;
        bool flagNewRoute = false;
        Point tmp;
        Point wheel;
        map <int, vector<Point>> routeVector;

    public:
        Country(string);
        Mat Access();
        int updateMap();
        void drawRoutes();
        void checkRoutes();
        static void mouseAttacher(int ,int ,int ,int ,void *);
        void Mouse(int ,int ,int ,int);
};

#endif // MAP_HPP