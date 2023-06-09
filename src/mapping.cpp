#include "include/mapping.hpp"

string image_address = "/home/mahdi/Desktop/Earth-Map-Route-Planning/map_image/Iran.png";

// country class constructor
Country::Country() {
    mapGenerator();
    namedWindow("Map", 1);
    setMouseCallback("Map", mouseAttacher, this);
    updateMap();
}

// read image of map from file and creat matrix
void Country::mapGenerator() {
    countryMap = imread(image_address);
    if (countryMap.empty()) { 
        cout<<"Error loading the map"<<endl;
        exit(0);
    }
}

// map matrix accessor
Mat Country::Access() {
    return countryMap;
}

// return vector of routes as a map <int, vector<Point>>
map <int, vector<Point>> Country::getRouteVector() {
    return routeVector;
}

// return two center od selected city
Point Country::getTwoCenter() {
    return * cityCenter;
}

//update displaying frame base on refresh time and listen to keyboard commands
int Country::updateMap() {
    double refreshRate = 20.0 / 1000.0;
    while (true) {
        sleep_for(milliseconds((int)refreshRate * 1000));
        drawRoutes();      
        switch (waitKey(1)){
            
            // new route lines
            case (int('n')):
                flagMouseCallBack *= -1;
                break;

            // select city centers
            case (int('s')):
                flagRoadLine = false;
                if (flagMouseCallBack == 1)
                    flagMouseCallBack *= -1;
                selectCenter();
                break;

            // reset map
            case (int('r')):
                flagRoadLine = false;
                flagSelectCenter = false;
                flagMouseCallBack *= -1;
                mapGenerator();
                routeID = 0;
                routeVector.clear();
                break;

            // exit program
            case (int('q')):
                destroyAllWindows();
                return 0;

            // printout route vector
            case (int('p')):
                plr.printVector(connectionRoutes, routeID);

            // calculate G for a-star
            // case (int('g')):
                // plr.calculateDistance(routeVector, routeID);
        }
        imshow("Map", countryMap);
    }
}

// select two city centers in order to planning
int Country::selectCenter() {
    if (routeVector.empty()) {
        cout << "There is no any city center !!\n";
        flagSelectCenter = false;
        return 0;
    }
    flagSelectCenter = true;
    return 1;
}

// draw routs base on mouse callback
void Country::drawRoutes() {
    
    if (flagRoadLine) {
        circle(countryMap, routeVector[routeID][0], 10, Scalar(0, 255, 0), FILLED);
        line(countryMap, tmp, wheel, Scalar(255, 0, 0),3, LINE_8);
        tmp = wheel;
        // if (flagCenterNumber){
            Point center = Point(routeVector[routeID][0].x - 5, routeVector[routeID][0].y - 13);
            string rId = to_string(number);
            putText(countryMap, rId, center, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
        // }
    } 
}

// initializing Connection Routes vector
void Country::initializingConnectionRoutes() {

        for(int i=0; i<(int)(routeVector[routeID].size()); i++){
            connectionRoutes[make_pair(routeID, tmpID)].push_back(routeVector[routeID][i]);
        } 

}

// mouse intraction function that commands to draw routs
void Country::Mouse(int event, int x, int y, int flags){
    
    if (flagMouseCallBack == 1) {
        if (event == EVENT_RBUTTONDOWN){
            routeVector[routeID].push_back(Point(x, y));
            tmp = Point(x+1,y+1);
            wheel = Point(x,y);
            flagRoadLine = true;
        }
        else if (event == EVENT_LBUTTONDOWN){
            if (flagRoadLine == true){
                // initializingConnectionRoutes();
                routeID += 1;
                number += 1;
                for(int i=0; i<=routeID; i++){
                    double distance = plr.pointDistance(routeVector[i][0], Point(x,y));
                    // cout<<"i : "<<i<<"  dis : "<<distance<<endl;
                    if((int)distance <= 12){
                        routeVector[routeID].push_back(Point(0,0));
                        routeVector[routeID][0] = routeVector[i][0];
                        number = i;
                        break;
                    }
                    else {
                        routeVector[routeID].push_back(Point(x, y));
                        tmpID = routeID + 1;
                    }
                }
            }
        }
        else if (event == EVENT_MOUSEMOVE) {
            if (flagRoadLine == true){
                wheel = Point(x,y);
                routeVector[routeID].push_back(wheel);
            }
        }
        else if (event == EVENT_MBUTTONDOWN) {
            flagRoadLine = false;
            flagMouseCallBack *= -1;
        }
    }

    else if (flagSelectCenter) {
        if (event == EVENT_LBUTTONDOWN) {
            for(int j=0; j<=routeID; j++){
                double distance = plr.pointDistance(routeVector[j][0], Point(x,y));
                if (distance <= 8){
                    cityCenter[0] = routeVector[j][0];
                    circle(countryMap, cityCenter[0], 11, Scalar(130, 63, 185), 2);
                }
            }
        }
        if (event == EVENT_RBUTTONDOWN) {
            for(int j=0; j<=routeID; j++){
                double distance = plr.pointDistance(routeVector[j][0], Point(x,y));
                if (distance <= 8){
                    cityCenter[1] = routeVector[j][0];
                    circle(countryMap, cityCenter[1], 11, Scalar(0, 0, 0), 2);
                }
            }
        }
    }
    updateMap();
}

// mouse callback attacher function
void Country::mouseAttacher(int event, int x, int y, int flags, void *data){
    Country *pointer = reinterpret_cast<Country* >(data);
    pointer -> Mouse(event, x, y, flags);
}
