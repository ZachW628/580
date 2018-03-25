#include "graph.h"


int main(int argc, char** argv) {
  Graph newGraph("BCCCMMM|", "|BCCCMMM");
  //Graph newGraph(argv[1], argv[2]);
  newGraph.printStateSpaceSize();
  newGraph.printStateSpaceEdges();
}


//----------------------------------------------------------------------------
// Graph member fucntions

// Public FxNs
Graph::Graph(string initialState, string goalState) {
  cout << initialState << " ||| " << goalState << endl;
  State* initial = new State(initialState);
  root = initial;
  State* goal = new State(goalState);
  stateSpace.push_back(initial);
  stateSpace.push_back(goal);
  Expand(initial, goal);
}


Graph::~Graph() {
  void* temp;
  while (!stateSpace.empty()) {
    temp = (void*) stateSpace.back();
    stateSpace.pop_back();
    delete (State*) temp;
  }
  while (!actionSpace.empty()) {
    temp = (void*) actionSpace.back();
    actionSpace.pop_back();
    delete (Action*) temp;
  }
}


void Graph::printStateSpaceSize() {
  cout << stateSpace.size() << endl;
}


void Graph::printStateSpaceEdges() {
  for (unsigned int i = 0; i < stateSpace.size(); i++) {
    cout << stateSpace[i]->stateName << ": ";
    for (unsigned int j = 0; j < stateSpace[i]->actionList.size(); j++) {
      cout << "(----> " << stateSpace[i]->actionList[j]->v->stateName  << ") ";
    } 
    cout << endl;
  }
}


// Private FxNs
void Graph::Expand(State* initialState, State* goalState) {
  if (initialState == goalState) {
    // We've reached a goal state
    return;
  } else {
    string left, right;
    State* tempState;
    stringstream ss(initialState->stateName);
    queue<State*> stateQueue;
    getline(ss, left, '|');
    if (left.find('B') != -1) {
      // Boat is on the Left side
      ExpandLeft(initialState, stateQueue);
    } else {
      // Boat is on the Right side
      ExpandRight(initialState, stateQueue);
    }
    while(!stateQueue.empty()) {
      tempState = stateQueue.front();
      stateQueue.pop();
      Expand(tempState, goalState);
    }
    return;
  }
}


int Graph::charCount(string line, char toFind) {
  int count = 0;
  for (unsigned int i = 0; i < line.length(); i++) {
    if (toFind == line[i])
      count++;
  }
  return count;  
}


Graph::State* Graph::isMember(string newStateName) {
  for (unsigned int i = 0; i < stateSpace.size(); i++) {
    if (newStateName == stateSpace[i]->stateName)
      return stateSpace[i];
  }
  return NULL;
}


bool Graph::validateState(string newStateName) {
  stringstream ss(newStateName);
  string left, right;
  getline(ss, left, '|');
  getline(ss, right);
  int leftMissionaryCount = charCount(left, 'M');
  int leftCannibalCount = charCount(left, 'C');
  if (leftMissionaryCount && leftCannibalCount > leftMissionaryCount)
    return false;
  int rightMissionaryCount = charCount(right, 'M');
  int rightCannibalCount = charCount(right, 'C');
  if (rightMissionaryCount && rightCannibalCount > rightMissionaryCount)
    return false;
  return true;
}


/*
* This FxN Should if the boat is on the left subtract the mod characters from
* left then concat MOD characters on Right then sort right side. then add 
* (left + '|' + right) for new state name. Left right vic versa for mode R.
* After new state validate if valid, lookup state in state vector.
* Once we have state assuming its valid, add edge on initial state to the new
* state, then if the state was just created we add it to exploration queue.
*/
void Graph::transformState(string left, string right, State* initialState,
                           queue<State*> &stateQueue, char mode, string mod) {
  string newStateName;
  State* newState;
  Action* newAction;
  size_t charPos;
  if (mode == 'L') {  // boats on left.
    for (unsigned int i = 0; i < mod.length(); i++) {
      charPos = left.find_first_of(mod[i]); 
      left.erase(charPos, 1);
    }
    right += mod;
    sort(right.begin(), right.end());
    newStateName = left + '|' + right;
  } else {
    for (unsigned int i = 0; i < mod.length(); i++) {
      charPos = right.find_first_of(mod[i]);
      right.erase(charPos, 1);
    }
    left += mod;
    sort(left.begin(), left.end());
    newStateName = left + '|' + right;
  }
  
  if (false || validateState(newStateName)) {
    if (!(newState = isMember(newStateName))) {
      newState = new State(newStateName);
      stateSpace.push_back(newState);
      stateQueue.push(newState);
    }

    newAction = new Action(newState, mod);
    actionSpace.push_back(newAction);
    initialState->actionList.push_back(newAction);
    return; 
  } else {  // Invalid State
    return;
  }
}


void Graph::ExpandLeft(State* initialState, queue<State*> &stateQueue) {
  stringstream ss(initialState->stateName);
  string tempLeft, left, tempRight, right, tempString = "B";
  getline(ss, left, '|');
  getline(ss, right);
  int missionaryCount = charCount(left, 'M');
  int cannibalCount = charCount(left, 'C');

  for (unsigned int i = 0; (i < missionaryCount && i < 2); i++) {
    tempString += "M";
    transformState(left, right, initialState, stateQueue, 'L', tempString);
  }
  tempString = "B";
  for (unsigned int i = 0; (i < cannibalCount && i < 2); i++) {
    tempString += "C";
    transformState(left, right, initialState, stateQueue, 'L', tempString);
  }
  tempString = "B";
  if (missionaryCount >= 1 && cannibalCount >= 1) {
    tempString += "CM";
    transformState(left, right, initialState, stateQueue, 'L', tempString);
  }
}


void Graph::ExpandRight(State* initialState, queue<State*> &stateQueue) {
  stringstream ss(initialState->stateName);
  string left, right, tempString = "B";
  getline(ss, left, '|');
  getline(ss, right);
  int missionaryCount = charCount(right, 'M');
  int cannibalCount = charCount(right, 'C');

  for (unsigned int i = 0; (i < missionaryCount && i < 2); i++) {
    tempString += "M";
    transformState(left, right, initialState, stateQueue, 'R', tempString);
  }
  tempString = "B";
  for (unsigned int i = 0; (i < cannibalCount && i < 2); i++) {
    tempString += "C";
    transformState(left, right, initialState, stateQueue, 'R', tempString);
  }
  tempString = "B";
  if (missionaryCount >= 1 && cannibalCount >= 1) {
    tempString += "CM";
    transformState(left, right, initialState, stateQueue, 'R', tempString);
  } 
}


//----------------------------------------------------------------------------
// State member fucntions
Graph::State::State(string name)
 : stateName(name)
{}


Graph::State::~State() {}

//----------------------------------------------------------------------------
// Action member fucntions
Graph::Action::Action(State* toState, string action)
 : v(toState), transition(action)
{} 


Graph::Action::~Action() {}

