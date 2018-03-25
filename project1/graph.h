#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <functional>


using std::vector;
using std::string;
using std::stringstream;
using std::queue;
using std::sort;
using std::greater;
using std::cout;
using std::endl;

// Forward decleration of Action so State can use Actions, since
//    Action have States in them. lol.
//   "class Action"

class Graph {
 private:
  class Action;
  class State {
   public:
    string stateName;
    vector<Action*> actionList;
    State(string);
    ~State();
  };
  class Action {
   public:
    State* v;
    string transition;
    Action(State*, string);
    ~Action();
  };

  void Expand(State*, State*);
  int charCount(string, char);
  State* isMember(string);
  bool validateState(string);
  void transformState(string, string, State*, queue<State*>&, char, string);
  void ExpandLeft(State*, queue<State*>&);
  void ExpandRight(State*, queue<State*>&);
  vector<State*> stateSpace;
  vector<Action*> actionSpace;
  State* root;
 public:
  Graph(string, string);
  ~Graph();
  void printStateSpaceSize();
  void printStateSpaceEdges();

};


#endif /* GRAPH_H */

