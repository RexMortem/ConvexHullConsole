#include <iostream> 
#include <stdio.h>
#include <vector> 
#include <string>

#define minColumns 2
#define maxColumns 50
#define minRows 2 
#define maxRows 50 

#define backgroundCharacter '.' 
#define pointCharacter 'X'

using namespace std; // while this is bad practice for big projects, we have a small global state so it's okay 

vector<vector<char>> scene; // use vectors so users can define the dimensions; scene is also referred to as "space" or "plane"
int nColumns;
int nRows; 

class Point {
    public:
        int x; // members are public because there's no point in having getters/setters for this object 
        int y;

        Point(int initX, int initY){
            this->x = initX;
            this->y = initY;
        }
};

// redeclares the space with the (newly set) nColumns and nRows 
void resetSpace(){
    vector<vector<char>> newScene(nRows);

    for(int y = 0; y < nRows; y++){
        newScene[y] = vector<char>(nColumns);

        for(int x = 0; x < nColumns; x++){ // we do y then x so that we can easily print the space to terminal
            newScene[y][x] = backgroundCharacter;
        }
    }

    scene = newScene;
}

// gets user input for defining the space, and sets variables for declaring the space
void initialiseSpace(){
    while(1){
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
    
    while(1){
        cout << "Enter number of rows: ";

        int nReplacements = scanf("%d", &nRows);  

        if(nReplacements == 1){ // we check if scanf consumed the input
            if(nColumns < minRows){
                cout << "Need at least " << minRows << " rows" << endl;
                continue;
            }else if(nColumns > maxRows){
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

// prints the space to the console 
void outputSpace(){
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

// adding points to the space 
void addPoints(){
    int x;
    int y;
    string input; // check for "quit" or just to clear input 

    cout << "\nPlease enter x y space-separated e.g. 5 5\n\n";

    while(1){
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

    while(1){
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
char orientation(Point a, Point b, Point c){ 
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
    cout << "runProgram - run the algorithm\n";
    cout << "viewSpace - view the plane of points (no hull)\n";

    cout << "\n";

    cout << "quit/q - quit the program\n";

    cout << "\n";
}

void menu(){
    string option; 
    printMenu();

    while(1){
        cout << "Menu> ";
        cin >> option; 
        
        if(option == "viewMenu"){
            printMenu();
        }else if(option == "dimensions"){
            initialiseSpace();
        }else if(option == "addPoints"){
            addPoints();
        }else if (option == "removePoints"){
            removePoints();
        }else if(option == "viewSpace"){
            outputSpace();
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