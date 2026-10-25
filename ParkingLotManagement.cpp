#include <iostream>
#include <string>
#include <queue>
#include <stack>
using namespace std;


struct Node {
    int slotNumber;
    bool occupied;
    string vehicleNumber;
    Node* next;

};

struct slotState {
    int slotNumber;
    bool occupied;
    string vehicleNumber;
};

struct Snapshot {
    queue<slotState> slots;
    queue<string> waiting;
};


void saveSnapshot(Node* head, queue<string> waitingQueue, stack<Snapshot>& history) {

    Snapshot Snapshot;

    Node* current = head;

    while(current!= nullptr) {
        slotState state;

        state.slotNumber = current-> slotNumber;
        state.occupied = current -> occupied;
        state.vehicleNumber = current -> vehicleNumber;

        Snapshot.slots.push(state);

        current = current->next;
    }

    Snapshot.waiting = waitingQueue;
    history.push(Snapshot);

    cout << "snapshot saved successfully " << endl;


}

//create parking space
Node* createParkingSlot(int capacity) {
    Node* head = nullptr;
    Node* tail = nullptr;

    for(int i = 1; i <= capacity; i++) {
        Node* newNode = new Node;

        newNode -> slotNumber = i;
        newNode -> occupied = false;
        newNode -> next = nullptr;

        if(head == nullptr) {
            head = newNode;
        } else {
            tail -> next = newNode;
        }

        tail = newNode;
    }
    return head;
}

bool vehicalExists(Node* head, queue<string> waitingQueue, string number) {
    Node* current = head;

    while(current != nullptr) {
        if(current-> occupied && current->vehicleNumber == number ) {
            return true;
        }

        current  = current->next;
    }

    while (!waitingQueue.empty()) {
        if(waitingQueue.front() == number) {
            return true;
        }
        waitingQueue.pop();
    }
    
    return false;

}

//displaying all solts

void displayParking(Node* head) {
    Node* current = head;

    while(current != nullptr) {
        cout <<"Slot " << current->slotNumber;

        if(current->occupied) {
            cout << " Occupied";
        } else {
            cout << " Available";
        }

        cout << endl;
        current = current -> next;
    }
}


// parking the vehical
void ParkVehical(Node* head, queue<string>& waitingQueue, stack<Snapshot>& history) {
    string number;

    cout << "Enter Your Vehical number :";
    cin >> number;

    Node* current = head;

    if(vehicalExists(head, waitingQueue, number)) {
        cout << "vehical is already parked or waiting" << endl;
        return;
    }

    saveSnapshot(head, waitingQueue, history);
    
    

    current = head;
    while(current != nullptr) {
            if(current->occupied == false) {
            current->vehicleNumber = number;
            current->occupied = true;

            cout << "Vehical Parked successfully in slot " << current->slotNumber << endl;

            return;
        }
        current = current->next;
    }

    waitingQueue.push(number);
    cout << "Parking lot is full" << endl;
    cout << "vehical added to waiting queue" << endl;

}

// searching the vehicle

void SearchVehicle(Node* head) {
    string number;

    cout << "Enter you vehicle number: ";
    cin >> number;

    Node* current = head;

    while(current != nullptr) {
        if(current->occupied && current->vehicleNumber == number) {
            cout << "Vehicle found" << endl;
            cout << "Parking Slot" << current->slotNumber << endl;

            return;
        }

        current = current -> next;
    }

    cout << "Vehical not found in the parking lot." << endl;
}


void exitVehicle(Node* head, queue<string>& waitingQueue, stack<Snapshot>& history) {
    string number;

    cout << "Enter vehical number to exit :";
    cin >> number;

    Node* current = head;

    while(current != nullptr) {
        if(current->occupied && current->vehicleNumber == number) {
            saveSnapshot(head, waitingQueue, history);
            
            current->occupied = false;
            current->vehicleNumber = "";

            cout << " Vehical exited successfully" << endl;
            cout << "Slot " << current->slotNumber << " is now available"<< endl;


            if(!waitingQueue.empty()) {
                string nextVehical = waitingQueue.front();
                waitingQueue.pop();

                current->vehicleNumber = nextVehical;
                current->occupied = true;

                cout << "waiting vehical " << nextVehical << "added to slot " << current->slotNumber << endl;
            }

            return;
        }
        current = current-> next;
    }

    cout << "Vehical not found in the parking lot" << endl;
}

//waiting queue

void displayWaitingQueue(queue<string> waitingQueue) {
    if(waitingQueue.empty()) {
        cout << "No vehical in the waiting list" << endl;
        return;
    }

    cout << "  waiting Vehical  " << endl;

    while(!waitingQueue.empty()) {
        cout << waitingQueue.front() << endl;
        waitingQueue.pop();
    }
}


void undoLastOperation(Node* head, queue<string>& waitingQueue, stack<Snapshot>& history) {

    if(history.empty()) {
        cout << "Nothing to undo" << endl;
        return;
    }

    Snapshot Snapshot = history.top();
    history.pop();

    Node* current = head;

    while(current != nullptr && !Snapshot.slots.empty()) {
        slotState state = Snapshot.slots.front();
        Snapshot.slots.pop();

        current-> occupied = state.occupied;
        current-> vehicleNumber = state.vehicleNumber;

        current = current->next;

    }

    waitingQueue = Snapshot.waiting;


    cout << "Last opertaion undone successfully" << endl;


}



int main() {
    int capacity;
    
    cout << "Enter the number of parking slots :";
    cin >> capacity;
    
    if(capacity <= 0) {
        cout << "Capacity must be greater than 0." << endl;
        return 0;
    }
    
    Node* head = createParkingSlot(capacity);
    
    queue<string> waitingQueue; 
    stack<Snapshot> history;
    
    int choice;

    do{
        cout << "               parking Lot             " << endl;
        cout << "1. Park vehicle" << endl;
        cout << "2. Search vehical" << endl;
        cout << "3. Exit Vehicle" << endl;
        cout << "4. Display parking" << endl;
        cout << "5. Display Waiting Vehical" << endl;
        cout << "6. Undo last operation. " << endl;
        cout << "7. Exit program" << endl;
        cout << endl;


        cout << "Enter Choice : ";

        cin >> choice;

        switch (choice)
        {
            case 1: ParkVehical(head, waitingQueue, history); break;

            case 2: 
                SearchVehicle(head);
                break;

            case 3: 
                exitVehicle(head, waitingQueue, history);
                break;


            case 4: 
                displayParking(head);
                break;

            case 5: 
                displayWaitingQueue(waitingQueue);
                break;


            case 6: 
                undoLastOperation(head, waitingQueue, history);
                break;

            case 7: 
                cout << "program Closed" << endl;
                break;

            
            default:
                cout <<"Invalid choice";
                break;
        }
    } while(choice != 0);


    return 0;
}