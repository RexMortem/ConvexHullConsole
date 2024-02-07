/*  Tested for: 
 (1 1) (5 1) (7 6) (5 5) (4 4) (9 6) (8 5) (5 9) (1 6) 
 (0 0) (4 4) (4 0) 
 (0 0) (4 4) (4 0) (0 4)
 (0 0) (4 4) (0 4)
*/

#include <iostream> 
#include <stdio.h>
#include <vector> 
#include <string>
#include <cmath>

#define minColumns 2
#define maxColumns 50
#define minRows 2 
#define maxRows 50 

#define backgroundCharacter '.' 
#define pointCharacter 'X'

using namespace std; // while this is bad practice for big projects, we have a small global state so it's okay 

vector<vector<char>> scene; // use vectors so users can define the dimensions; scene is also referred to as "space" or "plane"
vector<vector<char>> hullScene; // caching hull state if no change in points 

char sceneChanged = true; // for whether to use cached hull state; change in points means have to run algorithm again

int nColumns;
int nRows; 
int nPoints; // part of scene state; used for checking whether algorithm can be run

class Point {
    public:
        int x; // members are public because there's no point in having getters/setters for this object 
        int y;

        Point(int initX, int initY){
            this->x = initX;
            this->y = initY;
        }
};

// redeclares the space with the (newly set) nColumns and nRows; this clears the points 
void resetSpace(){
    vector<vector<char>> newScene(nRows);

    for(int y = 0; y < nRows; y++){
        newScene[y] = vector<char>(nColumns);

        for(int x = 0; x < nColumns; x++){ // we do y then x so that we can easily print the space to terminal
            newScene[y][x] = backgroundCharacter;
        }
    }

    scene = newScene;
    nPoints = 0;
    sceneChanged = true;
}

// redeclares hull space 
void resetHullSpace(){
    vector<vector<char>> newHullScene(nRows);

    for(int y = 0; y < nRows; y++){
        newHullScene[y] = vector<char>(nColumns);

        for(int x = 0; x < nColumns; x++){ // we do y then x so that we can easily print the space to terminal
            newHullScene[y][x] = backgroundCharacter;
        }
    }

    hullScene = newHullScene;
}

// gets user input for defining the space, and sets variables for declaring the space
void initialiseSpace(){
    while(true){
        cout << "\nEnter number of columns: ";

        int nReplacements = scanf("%d", &nColumns);  

        if(nReplacements == 1){ // we check if scanf consumed the input
            if(nColumns < minColumns){
                cout << "Need at least " << minColumns << " columns" << endl;
                continue;
            }else if(nColumns > maxColumns){
                cout << "Can only have " << maxColumns << " or fewer columns" << endl; 
                continue; 
            }

            break; // nColumns successfully chosen 
        }else{ // want to clear the buffer since scanf didn't consume input 
            scanf("%*s");
            cout << '\n'; // want it to look a bit nicer so newline
        }
    }
    
    while(true){
        cout << "\nEnter number of rows: ";

        int nReplacements = scanf("%d", &nRows);  

        if(nReplacements == 1){ // we check if scanf consumed the input
            if(nRows < minRows){
                cout << "Need at least " << minRows << " rows" << endl;
                continue;
            }else if(nRows > maxRows){
                cout << "Can only have " << maxRows << " or fewer rows" << endl; 
                continue; 
            }

            break; // nRows successfully chosen 
        }else{ // want to clear the buffer since scanf didn't consume input 
            scanf("%*s");
            cout << '\n'; // want it to look a bit nicer so newline
        }
    }

    resetSpace();

    cout << endl;
    cout << "Remember indexing is 0-based so " << 0 << " <= x < " << nColumns << " and " << 0 << " <= y < " << nRows << '\n' << endl;
}

// outputs scene of points 
void outputSpace(){
    cout << endl;

    for(int x = 0; x < nColumns; x++){
        cout << '=';
    }
    

    cout << "\n\n";

    for(int y = 0; y < nRows; y++){
        for(int x = 0; x < nColumns; x++){
            cout << scene[y][x];
        }

        cout << '\n'; // linebreak between lines 
    }

    cout << '\n';

    for(int x = 0; x < nColumns; x++){
        cout << '=';
    }

    cout << "\n\n";
}

// outputs scene with convex hull in it
void outputHull(){
    cout << endl;

    for(int x = 0; x < nColumns; x++){
        cout << '=';
    }
    

    cout << "\n\n";

    for(int y = 0; y < nRows; y++){
        for(int x = 0; x < nColumns; x++){
            cout << hullScene[y][x];
        }

        cout << '\n'; // linebreak between lines 
    }

    cout << '\n';

    for(int x = 0; x < nColumns; x++){
        cout << '=';
    }

    cout << "\n\n";
}

// adding points to the space 
void addPoints(){
    int x;
    int y;
    string input; // check for "quit" or just to clear input 

    cout << "\nPlease enter x y space-separated e.g. 5 5\n\n";

    while(true){
        cout << "Enter (x,y): ";

        int nReplacements = scanf("%d %d", &x, &y);  

        if(nReplacements == 2){ // we check if scanf consumed the input
            if(x >= nColumns){
                cout << "Cannot have x greater than (or equal to): " << nColumns << " (number of columns)" << '\n' << endl;
                continue;
            }else if(x < 0){
                cout << "Cannot have x smaller than: " << 0 << '\n' << endl; 
                continue; 
            }else if(y >= nRows){
                cout << "Cannot have y greater than (or equal to): " << nRows << " (number of rows)" << '\n' << endl;
                continue;
            }else if(y < 0){
                cout << "Cannot have y smaller than: " << 0 << '\n' << endl; 
                continue; 
            }

            // point successfully chosen 

            scene[y][x] = pointCharacter;
            nPoints++;
            sceneChanged = true;

            cout << "Successfully added point! Continue adding or enter quit/q to leave" << '\n' << endl;
        }else{ // coordinates not read correctly; let's see if they're quitting 
            cin >> input;

            if((input == "quit") || (input == "q")){
                cout << "\n";
                break;
            }

            cout << "Point unsuccessfully read. Perhaps you meant to type quit/q to leave" <<  '\n' << endl;
        }
    }
}

// removing points from the space 
void removePoints(){
    int x;
    int y;
    string input; // check for "quit" or just to clear input 

    cout << "\nPlease enter x y space-separated e.g. 5 5\n\n";

    while(true){
        cout << "Enter (x,y): ";

        int nReplacements = scanf("%d %d", &x, &y);  

        if(nReplacements == 2){ // we check if scanf consumed the input
            if(x >= nColumns){
                cout << "Cannot have x greater than (or equal to): " << nColumns << " (number of columns)" << '\n' << endl;
                continue;
            }else if(x < 0){
                cout << "Cannot have x smaller than: " << 0 << '\n' << endl; 
                continue; 
            }else if(y >= nRows){
                cout << "Cannot have y greater than (or equal to): " << nRows << " (number of rows)" << '\n' << endl;
                continue;
            }else if(y < 0){
                cout << "Cannot have y smaller than: " << 0 << '\n' << endl; 
                continue; 
            }

            // point is within the bounds of the array 

            if(scene[y][x] == pointCharacter){
                cout << "Successfully removed point! Continue removing or enter quit/q to leave" << '\n' << endl;
                nPoints--;
                sceneChanged = true;
            }else{
                cout << "There was no point at (" << x << "," << y << ")" << '\n' << endl;
            }

            scene[y][x] = backgroundCharacter; // regardless, let's set to backgroundCharacter

        }else{ // coordinates not read correctly; let's see if they're quitting 
            cin >> input;

            if((input == "quit") || (input == "q")){
                cout << "\n";
                break;
            }

            cout << "Point unsuccessfully read. Perhaps you meant to type quit/q to leave" << '\n' << endl;
        }
    }
}

/* ORIENTATION  
    Returns 0 for collinear, 1 for clockwise, and -1 for anticlockwise (a,b,c are distinct)

    We do this by comparing gradients however we want gradients in a form without division so 
    that we can deal ignore division by zero cases. 

    abGradient = (by - ay)/(bx - ax)
    bcGradient = (cy - by)/(cx - bx)

    we compare these directly and are in the following possible cases:
    abGradient > bcGradient
    abGradient == bcGradient 
    abGradient < bcGradient 

    so therefore we can manipulate both sides by multiplying up. 
    We will represent >, ==, and < by the symbol ?. 

    abGradient ? bcGradient
    (by - ay)/(bx - ax) ? (cy - by)/(cx - bx)
    (by - ay) * (cx - bx) ? (cy - by) * (bx - ax)

    so we will refer to abGradient = (by - ay) * (cx - bx)
    and bcGradient = (cy - by) * (bx - ax)

    even though these don't represent real gradients 
*/
int orientationOfPoints(Point a, Point b, Point c){ 
    int abGradient = (b.y - a.y) * (c.x - b.x);
    int bcGradient = (c.y - b.y) * (b.x - a.x);

    if(abGradient > bcGradient){ 
        return 1; // clockwise
    }else if (abGradient < bcGradient){
        return -1; // anti-clockwise
    }else{
        return 0; // collinear
    }
}

vector<Point> getPoints(){
    vector<Point> points;

    for(int y = 0; y < nRows; y++){
        for(int x = 0; x < nColumns; x++){
            if(scene[y][x] == pointCharacter){
                Point newPoint(x,y); 
                points.push_back(newPoint); // add to vector 
            }
        }
    }

    return points;
}

/*
    Run algorithm uses an implementation of Jarvis march (aka the gift wrapping algorithm)
    Initial point is the leftermost point which is guaranteed to be in the convex hull (CH)
    Next point is the point where every other point is counter-clockwise of the previous point and next point
    This means it's the most "outwards" point and therefore builds a perimeter around the other points 

    function assumes that there are >= 3 points 
*/
vector<Point> jarvisMarch(){
    vector<Point> points = getPoints();
    vector<Point> hull;

    // get leftermost point as it's guaranteed to be in the hull 

    int lI = 0; // lI is leftermostIndex

    for(int i = 1; i < points.size(); i++){ // search for leftermost point
        if(points[i].x < points[lI].x){
            lI = i;
        }
    }

    int cPi = lI; // current point index 
    hull.push_back(points[cPi]);

    while(true){
        int mCPi; // mCPi is index of most counterclockwise point (is counterclockwise with the current point over all other points)

        for(int i = 0; i < points.size(); i++){ // we need to initially guess mCPi as anything other than current point 
            if(i == cPi){
                continue;
            }

            mCPi = i;
        }

        while(true){ // this loop is for finding most counterclockwise point 
            int isBetter = false; // whether there is a more counterclockwise point 

            for(int i = 0; i < points.size(); i++){ // consider other points which could be more counterclockwise
                if((cPi == i) || (mCPi == i)){ // don't need to compare against current point or current most counterclockwise point 
                    continue;
                }     

                if(orientationOfPoints(points[cPi], points[mCPi], points[i]) == 1){ // let's prefer this point
                    mCPi = i;
                    isBetter = true;
                    break;
                }
            }

            if(!isBetter){
                break;
            }
        }

        if(mCPi == lI){ // hull is complete if next point is starting point
            break;
        }else{
            hull.push_back(points[mCPi]);
            cPi = mCPi;
        }
    }

    return hull;
}

void drawLineOnHull(Point a, Point b){
    int dx = (b.x - a.x);
    int dy = (b.y - a.y);

    float gradient = ((float) dy)/((float) dx); 
    int initX, initY, endX; // always step x from neg to positive 

    if(dx > 0){
        initX = a.x;
        initY = a.y;
        endX = b.x;
    }else if (dx < 0){
        initX = b.x;
        initY = b.y;
        endX = a.x;
    }else{ // special case: vertical line 
        int endY;

        if((b.y - a.y) > 0){ // we always go from neg to pos 
            initY = a.y;
            endY = b.y;
        }else{ // these points are distinct so if x is same, then y won't be same 
            initY = b.y;
            endY = a.y;
        }

        for(int y = initY; y <= endY; y++){
            hullScene[y][a.x] = pointCharacter;
        }

        return;
    }

    int lastY = initY;

    for (int x = initX; x <= endX; x++){
        float fromStartX = (x+1) - initX; // widening conversion for floor
        float yStep = floor(fromStartX * gradient);
        int nextY = initY + (int) yStep;
        
        int dy = (nextY - lastY); 
        int yDir = dy/abs(dy); // -1, 0, or 1; direction to draw multiple ys for same x if steep line 

        if(dy == 0){
            hullScene[nextY][x] = pointCharacter;
        }else{
            for(int y = lastY; y != nextY; y += yDir){
                hullScene[y][x] = pointCharacter;
            }
        }

        lastY = nextY;
    }
}

void runAlgorithm(){
    if(!sceneChanged){ // no change so let's not run the algorithm and update state again 
        outputHull();
        return;
    }

    if(nPoints < 3){
        cout << "\nWe cannot run a convex hull algorithm with fewer than 3 points!\n" << endl;
        return;
    }

    vector<Point> hull = jarvisMarch();

    // initialise hull
    resetHullSpace();

    cout << "\nNumber of points in hull: " << hull.size() << "\n" << endl;
    
    for(int i = 0; i < hull.size(); i++){
        Point x = hull[i];
    }

    // draw lines between points 
    for(int i = 0; i < hull.size() - 1; i++){ 
        drawLineOnHull(hull[i], hull[i+1]);
    }

    drawLineOnHull(hull[hull.size()-1], hull[0]); // last point and first point

    sceneChanged = false;
    outputHull();
}


/*  TO-DO:

    implement run-algorithm 
    implement displaying results of algorithm (drawline with | and - ??)
*/

void printMenu(){
    cout << "\nMenu \n";
    cout << "====\n";

    cout << "viewMenu - prints this menu out\n";
    cout << "dimensions - edit the number of rows and columns\n";
    cout << "addPoints - add points to the plane\n";
    cout << "removePoints - remove points from the plane\n";
    cout << "clearPoints - clear all points from the plane\n";
    cout << "viewSpace - view the plane of points (no hull)\n";
    cout << "viewHull - view the convex hull generated\n";

    cout << "\n";

    cout << "quit/q - quit the program\n";

    cout << "\n";
}

void menu(){
    string option; 
    printMenu();

    while(true){
        cout << "Menu> ";
        cin >> option; 
        
        if(option == "viewMenu"){
            printMenu();
        }else if (option == "dimensions"){
            initialiseSpace();
        }else if (option == "addPoints"){
            addPoints();
        }else if (option == "removePoints"){
            removePoints();
        }else if (option == "clearPoints"){
            resetSpace();
        }else if (option == "viewSpace"){
            outputSpace();
        }else if (option == "viewHull"){
            runAlgorithm();
        }else if ((option == "q") || (option == "quit")){
            break; // return control to main to handle quitting 
        }else{
            cout << "invalid option\n\n";
            continue;
        }
    }
}

// entry point: let's pass control to methods depending on state
int main(){
    cout << "Initialising program" << "\n\n";

    initialiseSpace();
    menu();

    cout << "\nGoodbye!\n\n";
}