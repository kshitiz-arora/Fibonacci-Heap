# Fibonacci-Heap
Program to compare time colmplexity of fibonacci heap with other data structures in Johnson's algorithm


### Input Format
First line: Number of Test Cases T and then follows their description\
For each test case,\
First line/row indicate N D (single space separated)\
where,\
N is number of vertices in directed graph where vertices are labelled 1 to N (not 0 indexing)\
D specifies whether Directed Graph or undirected graph (D=0 means undirected, else it is directed)\
This is then followed by N lines each containing N integers (basically NxN Adjacency or say weight matrix\
representation of the graph of that test case).\
The NxN matrix values / integers denote the edge-weights, there are no self-loops in the graph, and\
the weight=999999 indicates that there is no edge between those two vertices considered.\
//Diagonal entries will always be zero and one can safely assume no edge from vertex to itself.\

### Output Format
For each test case,\
If there is any negative weight cycle in graph, print just -1 as the shortest path is not well defined for atleast\
some source vertices.\
Otherwise in output for that test case:\
there are as many rows as many number of vertices in that graph under consideration (unless there is negative\
Consider if the test case has N vertices, then N rows each containing N single space separated values s.t. the\
kth value in ith row tells the distance of kth vertex from the (source) vertex i. (Kind of NxN shortest path matrix)\
Print 999999 if the vertex is not reachable from the source vertex considered.\
Lastly, there is one last row at end of outputs for T testcases, where last row has T entries indicative of\
time taken for different test cases that were considered.
