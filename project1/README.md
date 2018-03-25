3.b) It is a good Idea to avoid repeated states because the algorithm I choose
     will eventually explore all children at that state. So that state
     shouldn't be reconsidered because it will create a cycle in traversing.
     After the initial stateSpace is generated I use IDDFS to find the goal
     state.
