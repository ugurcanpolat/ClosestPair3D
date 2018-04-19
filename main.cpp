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
#include <iomanip>  // setw
#include <sstream>  // stringstrem
#include <string>   // string
#include <vector>   // vector

using namespace std;

typedef enum CompareBy {
    NONE = 0, COMPARE_BY_X, COMPARE_BY_Y, COMPARE_BY_Z
} CompareBy;

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
    int partition(int p, int r, CompareBy compareBy);
    void quickSort(int p, int r, CompareBy compareBy);
  public:
    Plane(vector<Point> copy);
    const Point& operator[](int i) const;
    void sort(CompareBy compareBy);
};

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
        
        // Create stringstream to parse with '\t'
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
    
    cout << "The distance is " << endl;
    cout << "Number of total distance calculations is " << endl;
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
    int aX = a.getX();
    int aY = a.getY();
    int aZ = a.getZ();
    
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

Plane::Plane(vector<Point> copy) {
    points = copy;
}

int Plane::partition(int p, int r, CompareBy compareBy) {
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
    if (p < r) {
        int q = partition(p, r, compareBy);
        quickSort(p, q-1, compareBy);
        quickSort(q+1, r, compareBy);
    }
}

void Plane::sort(CompareBy compareBy) {
    int size = static_cast<int>(points.size()) - 1;
    quickSort(0, size, compareBy);
}

double distance(Point a, Point b) {
    double X = pow(a.getX() - b.getX(),2);
    double Y = pow(a.getY() - b.getY(),2);
    double Z = pow(a.getZ() - b.getZ(),2);
    return sqrt(X+Y+Z);
}
