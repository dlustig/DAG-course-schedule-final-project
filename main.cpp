// Dan Lustig
// 4/18/13
// DAG acyclic graph course scheduler
// CPSC 230 final project

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
using namespace std;


//***************** insert and remove classes into matrix ***************//

template <int N>
class Graph {
public:
    // start with no edges at all
    Graph( ) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                matrix[i][j] = 0;
            }
        }
    }
    
    // insert an edge
    void insertEdge(int from, int to, char cost) {
        matrix[from][to] = cost;
    }
    
    // remove an edge
    void remove(int from, int to) {
        matrix[from][to] = 0;
    }
    
    //print matrix
    void printMatrix(){
        for (int x = 0; x < 100; x++){
            for (int y = 0; y < 100; y++){
                cout << matrix [x][y];
            }
            cout <<endl;
        }
    }
    
    int readMatrix(int from, int to){
        return matrix[from][to];
    }
    
    
private:
    int matrix[N][N];
};






//************************************//
//class for ints 

//class for queue based on double linked list of integers
class ListQueueTime {
public:
    //constructor
    ListQueueTime();
    
    //destructor
    ~ListQueueTime();
    
    //copy constructor and assignment opperator
    ListQueueTime(const ListQueueTime& other);
    ListQueueTime& operator=(const ListQueueTime& other);
    
    //add time at start
    void enqueueTime(int new_data);
    
    //remove time from end
    int dequeueTime();
    
    //read first node on list
    bool checkTime();
    
    //delete function
    void remove(int);
    
    
private:
    struct Node {
        int data;
        Node* next;
        Node* prev;
    };
    Node* head;
    Node* tail;
};




// constructor
ListQueueTime::ListQueueTime() {
    head = NULL;
    tail = NULL;
}

// destructor
ListQueueTime::~ListQueueTime() {
    while(head != NULL){remove(head->data);
    }
}

// copy constructor
ListQueueTime::ListQueueTime(const ListQueueTime& other) {
    Node* current = other.head;
    head = tail = NULL;
    while (current != NULL){
        enqueueTime(current->data);
        current = current ->next;
    }
}

// assignment operator
ListQueueTime& ListQueueTime::operator=(const ListQueueTime& other) {
    // clear the list
    while(head != NULL) {
        remove(head->data);
    }
    
    // copy over the other
    Node* current = other.head;
    head = tail = NULL;
    while(current != NULL) {
        dequeueTime();
        current = current->next;
    }
    
    
    return *this;
}

// add an element to the end
void ListQueueTime::enqueueTime(int new_data) {
    // make the node
    Node* node = new Node;
    node->data = new_data;
    node->next = NULL;
    
    // if tail is NULL, it's the ONLY one
    if(tail == NULL & head == NULL) {
        head = node;
        tail = node;
        node->prev = NULL;
    } else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
}


// remove an element from beginning of line
int ListQueueTime::dequeueTime() {
    // take off the head
    int holder;
    Node* current = head;
    current -> prev = NULL;
    holder = current-> data;
    head = current->next;
    if(head != NULL) {
        head->prev = NULL;
    } else {
        tail = NULL;
    }
    
    // delete node
    delete current;
    
    // return
    return holder;
}


bool ListQueueTime::checkTime(){
    Node* current = head;
    if (current == NULL){
        return true;
    }
    else {
        return false;
    }
}

// remove an element
void ListQueueTime::remove(int number) {
    // find the node with our number
    Node* current = head;
    
    while(current != NULL) {
        if(current->data == number) {
            // FOUND IT
            
            // if the previous is NULL, we are removing head!
            if(current->prev == NULL) {
                head = current->next;
                if(head != NULL) {
                    head->prev = NULL;
                } else {
                    tail = NULL;
                }
            } else {
                // point previous's next pointer at the one after current
                current->prev->next = current->next;
                
                // also point next's previous pointer to current's prev
                if(current->next != NULL) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
            }
            
            // delete node
            delete current;
            
            // return
            return;
        }
        
        current = current->next;
    }
    
    // if we got here we didn't find it!
    //cout << number << " was not found in the list!" << endl;
}



int main(int argc, const char * argv[]) {
    // create a graph with 6 nodes
    Graph<100> graph;
    // create a queue
    ListQueueTime queue;
    ListQueueTime list;

    string className;
    string prereq;
    int numPrereq = 0;
    string classArray[100];
    int numPrereqArray[100];
    int x=0,y=0;
    ifstream input;
    //input.open("/Users/daniellustig/Desktop/scheduler input.txt");
    input.open(argv[1]);
    if (input)
    {
        //cout << "open!!!\n\n";
        
        //********* cycle through entire list inputing only the first class. no prereqs.
        while (!input.eof()){
            input >> (className);
            classArray[y] = className;
            //cout << className<< endl;
            
            // cycle through unnecessary info
            input >> numPrereqArray[y];
            for (int b = 0; b < numPrereqArray[y]; b++){
                input >> className;
            }
            y++;
            
        }
        
        input.clear();
        input.seekg(0);
        while (!input.eof())
        {
            
            //input the root class and num of prereqs
            input >> (className);
            //cout << className;
            input >> numPrereq;
            //cout << " " << numPrereq << endl;
            
            if (numPrereq == 0) // if class is first root of tree
            {
                graph.insertEdge(0, x, 0);
                x++;
                continue;
            }
            for (int i = 0; i < numPrereq; i++) // input prereqs
            {
                
                input >> prereq;
                //cout << "inserting prereqs : " << prereq;
                
                for (int a = 1; a <= y; a++) //find prereqs already in tree (a pointer classArray)
                {
                    if (prereq.compare(classArray[a]) == 0)
                    {
                        graph.insertEdge(a, x, 1);
                        //cout << "  " << a << endl;
                    }
                }
            }
            x++;
        }
        input.close();
    }
    else {cout << "bad file\n";}
    
    
    cout << endl <<endl;
  
    
    
    //find all root nodes
    int frontNodepop;

    //find node with no indegree edges
    for (int t = 0; t < 100; t++){
        if (numPrereqArray[t] == 0){
            queue.enqueueTime(t);
        }
    }
    
    cout << endl;
    int count=0;
   while(queue.checkTime() == false)
   {
       count++;
        // dequeue front line class
        frontNodepop = queue.dequeueTime();
        if (classArray[frontNodepop][0]!= ' ')
           {
               for (int print =0; print < 7; print++){
                   if (print == 4){
                       cout << " ";
                       
                   }
                   cout << classArray[frontNodepop][print];
               }
               cout <<endl;
           }
       
            for (int edgedel = 0; edgedel < 100; edgedel++){
                //check for indegree edges
                if (graph.readMatrix(frontNodepop, edgedel) == 1){
                    numPrereqArray[edgedel] = numPrereqArray[edgedel] - 1;
                    if (numPrereqArray[edgedel] == 0){
                        queue.enqueueTime(edgedel);
                    }
                }
            }

   }
            //cout<< endl;
           //graph.printMatrix();
            for (int read = 0; read < 100; read++){
                //cout << numPrereqArray[read];
                if (numPrereqArray[read]!= 0){
                    cout << endl << "error: there is a cycle in this list. courses listed above happen before the cycle" <<endl;
                    return 0;
                }
            }
    
    return 0;
}

