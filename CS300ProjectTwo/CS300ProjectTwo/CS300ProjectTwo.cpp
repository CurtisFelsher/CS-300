//Curtis Felsher
//CS-300 Project Two
//February 13, 2023

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

using namespace std;

// structure to hold course information
struct Course {
    string courseNumber;

    string courseName;

    vector<string> preRequisites;

};

// forward declaration
void displayCourse(Course course);

vector<vector<string>> readFile(string csvPath);

// internal node structure for Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course c) : Node() {
        course = c;
    }
};


// Class definition for Binary Search Tree data structure
class BinarySearchTree {

    // private members
private:
    Node* root;

    // private methods
    void addNode(Node* node, Course course);
    void displayInOrder(Node* node);
    void deleteTree(Node* node);

public:
    // constructor
    BinarySearchTree();
    // destructor
    virtual ~BinarySearchTree();
    // insert new course into tree
    void Insert(Course course);
    // print tree in order
    void DisplayInOrder();
    // search for a specific course number
    Course Search(string courseNumber);
};


// Default constructor

BinarySearchTree::BinarySearchTree() {
    // initialize root to null
    root = nullptr;
}

// Destructor
void BinarySearchTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    // delete left sub tree
    deleteTree(node->left);

    // delete right sub tree
    deleteTree(node->right);

    // delete node
    delete node;

}

// destructor
BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}



// Loop through tree and insert new node at the correct place

void BinarySearchTree::addNode(Node* node, Course course) {
    Node* currNode = node;

    // While currrent node is equal to nullptr
    while (currNode != nullptr) {

        // if courseNumber < current node->couse.courseNumber,
        if (course.courseNumber < currNode->course.courseNumber) {
            if (currNode->left == nullptr) {  // if left node is empty
                // assign new course to left node 
                currNode->left = new Node(course);
                break;
            }
            // assign current node to left node
            currNode = currNode->left;
        }
        else {
            // if right node is equal to empty
            if (currNode->right == nullptr) {
                // assign new course to the right node                 
                currNode->right = new Node(course);
                break;
            }
            // assign current node to the right node
            currNode = currNode->right;
        }
    }
}

// Insert function for BinarySearchTree class
void BinarySearchTree::Insert(Course course) {
    // if tree is empty
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        // add course to tree
        addNode(root, course);
    }
}


// Print node(s) in order and formatted
void BinarySearchTree::displayInOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        // recursively print left sub tree
        displayInOrder(node->left);
    }

    std::cout << node->course.courseNumber << ": " << node->course.courseName << std::endl;

    if (node->right != nullptr) {
        // recursively print right sub tree
        displayInOrder(node->right);
    }
}


// Print the tree in order
void BinarySearchTree::DisplayInOrder() {

    // recursively print tree in order
    Node* currNode = root;
    displayInOrder(root);
}


// Search for a specific course number
Course BinarySearchTree::Search(string courseNumber) {
    Course course;

    // start with root node
    Node* currNode = root;

    // loop through tree
    while (currNode != nullptr) {
        if (courseNumber.compare(currNode->course.courseNumber) == 0) {
            // if found, return course
            course = currNode->course;
            break;
        }
        // if courseNumber < current node->couse.courseNumber,
        // loop through left sub tree
        else if (courseNumber.compare(currNode->course.courseNumber) < 0) {
            currNode = currNode->left;
        }
        // loop through right sub tree
        else {
            currNode = currNode->right;
        }
    }

    return course;
}


// Display course information to console with formatting
void displayCourse(Course course) {

    cout << endl << endl << course.courseNumber << endl;
    cout << course.courseName << endl;
    cout << "Pre-Requisite(s): ";

    // if no pre-requisites
    if (course.preRequisites.size() == 0) {
        // display no pre-requisites
        cout << "No Pre-Requisites." << endl << endl;
    }
    // if there are pre-requisites
    else {
        // display pre-requisites
        cout << course.preRequisites.at(0);
        // loop through remaining pre-requisites
        for (unsigned int i = 1; i < course.preRequisites.size(); ++i) {
            cout << ", " << course.preRequisites.at(i);
        }
    }
    cout << endl << endl;
}


// file parse error handling
struct FileParserError : public exception {
    string  msg;
    FileParserError(string err) {
        msg = string("File Parser: ").append(err);
    }
    const char* what() const throw () {
        return msg.c_str();
    }
};




// split string by ',' delimiter
vector<string> parseLine(string line) {
    vector<string> row;
    string delimiter = ",";
    int startIndex = 0;
    int endIndex = line.find(delimiter);

    // loop through line and split by delimiter
    while (endIndex != -1) {
        row.push_back(line.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + delimiter.size();
        endIndex = line.find(delimiter, startIndex);
    }
    row.push_back(line.substr(startIndex, endIndex - startIndex));
    return row;
}


// Read and parse CSV file, then return data in a vector
vector<vector<string>> readFile(string csvPath) {
    ifstream file(csvPath);
    vector<vector<string>> data;
	if (!file.is_open()) {
        throw FileParserError("Failed to open file.");
    }
    // Read the file line by line.
    string line;
    while (getline(file, line)) {
        // Split each line into fields using the comma delimiter.
        vector<string> row;
        istringstream ss(line);
        string field;
        while (getline(ss, field, ',')) {
            row.push_back(field);
        }
        // // Add the row of fields to the data vector.
        data.push_back(row);
    }
    // Return the parsed data
    return data;
}

// Load courses from CSV file
void loadCourses(string csvPath, BinarySearchTree* bst) {
    vector<string> courseTitles;
    try {
        // read file file
        vector<vector<string>> fileContents = readFile(csvPath);

        // extract course titles
        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                courseTitles.push_back(row.at(0));
            }
        }

        // extract course information
        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                Course course;

                // course number
                course.courseNumber = row.at(0);

                // course name
                course.courseName = row.at(1);

                // add pre-requisite(s)
                for (unsigned int j = 2; j < row.size(); ++j) {
                    if (find(courseTitles.begin(), courseTitles.end(), row.at(j)) != courseTitles.end()) {
                        course.preRequisites.push_back(row.at(j));
                    }
                }

                // insert into binary search tree
                bst->Insert(course);
            }
        }
        // Confirmation of successful data structure load
        cout << "Data structure loaded successfully." << endl;
    }
    // catch file parser error
    catch (FileParserError& err) {
        cerr << err.what() << endl;
    }


}

// Display menu for all choices and functions 
void DisplayMenu() {
    // binary search tree to hold all course information
    BinarySearchTree* searchTree = new BinarySearchTree();

    //DisplayMenu Variables
    int displayMenu = 0;
    int specificCount = 0;
    int itemQuantity = 0;
    int userColor = 0;
    string userInput;
    string itemName;
    ifstream fileInput;
    string cyanColor = "\033[36;1m";
    string redColor = "\033[31;1m";
    string whiteColor = "\033[0m";
    string blueColor = "\033[34;1m";
    string greenColor = "\033[32;1m";


    while (displayMenu != 9) {
        string csvPath, courseNumber;
        Course course;
        string number;

        // print menu
        cout << "Welcome to the ABCU course planner!" << endl;
        cout << "[1] Load data structure" << endl;
        cout << "[2] Print Course List" << endl;
        cout << "[3] Print Course" << endl;
        cout << "[8] Change the color of the UI" << endl;
        cout << "[9] Exit" << endl;
        cout << "Please enter a number: 1, 2, 3, 8, or 9" << endl;
        cin >> displayMenu;
        switch (displayMenu)
        {
        case 1:
            // load course into binary tree
            searchTree = new BinarySearchTree();

            cout << "Enter exact file path: ";
            cin.ignore();
            getline(cin, csvPath);

            loadCourses(csvPath, searchTree);
            system("pause");
            system("cls");
            break;
        case 2:
            // display course in ascending course number
            cout << endl;
            cout << "List of all courses:" << endl;
            searchTree->DisplayInOrder();
            cout << endl << endl;
            break;
        case 3:
            // search for a specific course number
            cout << "Enter course number: ";
            cin.ignore();
            getline(cin, courseNumber);

            // check if input is empty
            number = courseNumber;

            // remove whitespace
            number.erase(remove_if(number.begin(), number.end(), isspace), number.end());

            // check if input is empty
            if (number == "") {
                cout << "The course number field cannot be empty. Please Try again." << endl;
            }
            else {
                course = searchTree->Search(courseNumber);
                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "The course number: " << courseNumber << " could not be found." << endl;
                }
            }
            break;
        case 8:
            // Modular menu option I developed for my final project in CS-210 and like to add this to projects to make it more fun
            while (1) {
                system("cls");
                cout << "Please choose a color:" << endl << endl;
                cout << "1. Cyan" << endl;
                cout << "2. Red" << endl;
                cout << "3. Green" << endl;
                cout << "4. Blue" << endl;
                cout << "5. White" << endl << endl;
                cout << "Please enter your choice as a number: 1, 2, 3, 4, or 5." << endl;

                cin >> userColor;
                cout << endl;

                //Validates user input as a number
                if (userColor > 5 || userColor < 1) {
                    cout << endl << "Invalid input." << endl << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    system("pause");
                    system("cls");
                    continue;
                }

                else if (userColor == 1) {
                    cout << cyanColor;
                    cout << "You chose Cyan." << endl;
                    system("pause");
                    system("cls");
                    break;
                }

                else if (userColor == 2) {
                    cout << redColor;
                    cout << "You chose Red." << endl;
                    system("pause");
                    system("cls");
                    break;
                }

                else if (userColor == 3) {
                    cout << greenColor;
                    cout << "You chose Green." << endl;
                    system("pause");
                    system("cls");
                    break;
                }

                else if (userColor == 4) {
                    cout << blueColor;
                    cout << "You chose Blue." << endl;
                    system("pause");
                    system("cls");
                    break;
                }

                else if (userColor == 5) {
                    cout << whiteColor;
                    cout << "You chose White." << endl;
                    system("pause");
                    system("cls");
                    break;
                }

            }
            while (userColor > 5 || userColor < 1);
            break;

        case 9:
            cout << endl;
            cout << "Thank you for using the ABCU course planner! Have a wonderful rest of your day!" << endl;
            exit(1);
            break;

        default:
            cout << endl;
            cout << "Invalid input. Please try again.";
            cout << endl << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("pause");
            system("cls");
            break;
        }
    }
}


// I was taught in CS-210 to keep int main() as small as possible for modularity reasons
int main()
{
    DisplayMenu();

}