/* @Author
 * Student Name: Ugurcan Polat
 * Student ID : 150140117
 * Date: 20.04.2018
 * * * * * * * * * * * * * * * * * *
 
 Compile command: g++ -std=c++11 150140117.cpp -o project2
 
 * * * * * * * * * * * * * * * * * */

#include <chrono>   // high_resolution_clock
#include <cmath>    // pow, sqrt
#include <fstream>  // ifstream
#include <iostream> // cout
#include <iomanip>  // setprecision
#include <sstream>  // stringstrem
#include <string>   // string
#include <vector>   // vector

using namespace std;

typedef enum CompareBy {
    NONE = 0, COMPARE_BY_X, COMPARE_BY_Y, COMPARE_BY_Z
} CompareBy;

typedef enum Divide {
    LEFT = 0, RIGHT
} Divide;

class Point {
  private:
    int x;
    int y;
    int z;
  public:
    Point() {};
    Point(int x, int y, int z);
    int getX() const;
    int getY() const;
    int getZ() const;
    int compare(const Point& a, CompareBy compareBy) const;
};

class Plane {
  private:
    vector<Point> points;
    int size;
    int partition(int p, int r, CompareBy compareBy);
    void quickSort(int p, int r, CompareBy compareBy);
    double distance(const Point& a, const Point& b) const;
    double distanceByPairwiseCompare() const;
    double distanceClosestPairRec3D(const Plane &px) const;
    double distanceClosestPairRec2D(const Plane &py) const;
  public:
    Plane(const vector<Point> &copy);
    Plane(const Plane& copy, Divide div);
    const Point& operator[](int i) const;
    int getSize() const;
    void sort(CompareBy compareBy);
    double distanceClosestPair() const;
};

int calcCounter = 0;

int main(int argc, const char * argv[]) {
    // Input file name argument must be passed with run command
    if (argc != 2) {
        cout << endl << "Missing arguments to proceed." << endl << endl;
        return 0;
    }
    
    // Get input file name and open the file
    string input_filename(argv[1]);
    ifstream inputFile(input_filename, ifstream::in); // Read
    
    if (!inputFile.is_open()) { // Error while openning the file
        cout << endl << "Error opening input file." << endl << endl;
        return 0;
    }

    string line;
    getline(inputFile, line); // Read the first line
    int numberOfBalls = stoi(line); // Get number of balls
    
    vector<Point> points;
    points.resize(numberOfBalls);
    
    for(int i = 0; !inputFile.eof(); i++) {
        getline(inputFile, line); // Read line
        
        // If line is empty, no need to take action
        if(line.empty()) break;
        
        // Create stringstream to parse with space ' '
        stringstream linestream(line);
        
        string read[3];
        
        // Parse the line
        getline(linestream, read[0], ' ');
        getline(linestream, read[1], ' ');
        linestream >> read[2];
        
        int x = stoi(read[0]);
        int y = stoi(read[1]);
        int z = stoi(read[2]);
        
        Point new_point(x,y,z); // New point
        points[i] = new_point;
    }
    
    // Close the input file since it is no longer needed
    inputFile.close();
    
    Plane allPoints(points);
    
    using namespace chrono;
    auto functionStart = high_resolution_clock::now(); // Begin time stamp
    double distance = allPoints.distanceClosestPair();
    auto functionEnd = high_resolution_clock::now(); // End time stamp
    
    // Get the elapsed time in unit microseconds
    float elapsedTime = duration_cast<milliseconds>(functionEnd - functionStart).count();
    
    cout << "The distance is " << setprecision(6) << distance << endl;
    cout << "Number of total distance calculations is " << calcCounter << endl;
    cout << "Elapsed time is " << elapsedTime << " milliseconds" << endl;
    return 0;
}

Point::Point(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

int Point::getZ() const {
    return z;
}

int Point::compare(const Point& a, CompareBy compareBy) const {
    int aX = a.getX(); // Get compare point's x value
    int aY = a.getY(); // Get compare point's y value
    int aZ = a.getZ(); // Get compare point's z value
    
    switch (compareBy) {
        case COMPARE_BY_X:
            if (x < aX)
                return -1;
            else if (x == aX)
                return 0;
            else
                return 1;
        case COMPARE_BY_Y:
            if (y < aY)
                return -1;
            else if (y == aY)
                return 0;
            else
                return 1;
        case COMPARE_BY_Z:
            if (z < aZ)
                return -1;
            else if (z == aZ)
                return 0;
            else
                return 1;
        default:
            return 0;
    }
}

Plane::Plane(const vector<Point> &copy) {
    points = copy;
    size = static_cast<int>(copy.size());
}

Plane::Plane(const Plane &copy, Divide divideBy) {
    if (divideBy == LEFT) { // Left part of the plane
        size = ceil(static_cast<double>(copy.getSize()) / 2);
        for (int i = 0; i < size; i++) {
            points.push_back(copy[i]);
        }
    } else { // Right part of the plane
        int copySize = copy.getSize();
        size = floor(static_cast<double>(copySize) / 2);
        for (int i = size; i < copySize; i++) {
            points.push_back(copy[i]);
        }
    }
}

const Point& Plane::operator[](int i) const {
    return points[i];
}

int Plane::partition(int p, int r, CompareBy compareBy) {
    // Partition function for quick sort implementation
    
    Point x = points[r];
    int i = p - 1;
    
    for(int j = p; j < r; j++) {
        if (points[j].compare(x, compareBy) <= 0) {
            i++;
            Point temp = points[j];
            points[j] = points[i];
            points[i] = temp;
        }
    }
    
    Point temp = points[r];
    points[r] = points[i+1];
    points[i+1] = temp;
    
    return i+1;
}

void Plane::quickSort(int p, int r, CompareBy compareBy) {
    // compareBy is used to determine sorting by x, y or z
    
    if (p < r) {
        int q = partition(p, r, compareBy);
        quickSort(p, q-1, compareBy);
        quickSort(q+1, r, compareBy);
    }
}

void Plane::sort(CompareBy compareBy) {
    quickSort(0, size-1, compareBy);
}

int Plane::getSize() const {
    return size;
}

double Plane::distance(const Point &a, const Point &b) const {
    // Finds distance between two points
    double X = pow(a.getX() - b.getX(),2);
    double Y = pow(a.getY() - b.getY(),2);
    double Z = pow(a.getZ() - b.getZ(),2);
    calcCounter++;
    return sqrt(X+Y+Z);
}

double Plane::distanceByPairwiseCompare() const {
    // This function finds distance between at most 3 points with
    // brute force approach.
    
    double min = numeric_limits<double>::max();
    double d = 0;
    
    for(int i = 0; i < size - 1; i++) {
        for (int j = i+1; j < size; j++) {
            d = distance(points[i], points[j]);
            if (d < min)
                min = d;
        }
    }
    return min;
}

double Plane::distanceClosestPair() const {
    // Finds distance between closest pair
    
    Plane px(points);
    px.sort(COMPARE_BY_X); // Sort by X coordinates
    return distanceClosestPairRec3D(px);
}

double Plane::distanceClosestPairRec3D(const Plane &px) const {
    // This function is slightly modified version of the pseudocode in the slides to
    // use in 3D. It is used to find distance between closest pair in 3D.
    
    if (px.getSize() <= 3) // If there are less than 4 points use brute force approach
        return px.distanceByPairwiseCompare();
    
    Plane qx(px, LEFT); // Left part
    Plane rx(px, RIGHT); // Right part
    
    Point middle = px[px.getSize()/2]; // Middle point
    
    double dl = distanceClosestPairRec3D(qx); // Distance of left part
    double dr = distanceClosestPairRec3D(rx); // Distance of right part
    double dist = dr < dl ? dr : dl; // Which part has less distance
    
    vector<Point> S; // Vector of strip between two parts
    for (int i = 0; i < px.getSize(); i++) {
        // If point is in the interval of distance add it to the strip
        if(abs(px[i].getX() - middle.getX()) < dist)
            S.push_back(px[i]);
    }
    
    Plane strip(S); // Create strip plane with
    strip.sort(COMPARE_BY_Y); // Sort by y coordinates
    
    // Find the smallest distance in the strip
    double distance2D = distanceClosestPairRec2D(strip);
    
    // Return smallest distance
    return dist > distance2D ? distance2D : dist;
}

double Plane::distanceClosestPairRec2D(const Plane &py) const {
    // This function is very identical to the pseudocode in the slides. It is used to
    // find distance between closest pair in 2D.
    
    if (py.getSize() <= 3)
        return py.distanceByPairwiseCompare();
    
    Point middle = py[py.getSize() / 2]; // Middle point
    
    Plane qy(py, LEFT); // Left part
    Plane ry(py, RIGHT); // Right part
    
    double dl = distanceClosestPairRec2D(qy);
    double dr = distanceClosestPairRec2D(ry);
    double dist = dr < dl ? dr : dl;
    
    vector<Point> S; // Vector of strip between two parts
    for (int i = 0; i < py.getSize(); i++) {
        // If point is in the interval of distance add it to the strip
        if(abs(py[i].getY()-middle.getY()) < dist)
            S.push_back(py[i]);
    }
    
    Plane strip(S); // Vector of strip between two parts
    strip.sort(COMPARE_BY_Z); // Sort strip by z coordinate
    int stripSize = strip.getSize();
    // Find minimum distance in the strip
    for (int i = 0; i < stripSize; i++) {
        // Calculate distances between points only in the d-distance interval
        for (int j = i+1; j < stripSize && (strip[j].getZ() - strip[i].getZ() < dist); j++) {
            double compare = distance(strip[i], strip[j]);
            if(compare < dist) // Smaller than distance
                dist = compare;
        }
    }
    
    // Return smallest distance
    return dist;
}
