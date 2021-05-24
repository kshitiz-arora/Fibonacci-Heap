#include <iostream> 
#include <bits/stdc++.h>
using namespace std; 

vector<int> bellman_ford(vector<vector<int>> adjList, int N, int D, int num_edges);

void dijkstra1(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values); // array based

void dijkstra2(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values); // binary heap based

void dijkstra3(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values); // binomial heap based

void dijkstra4(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values); // fibonacci heap based

void johnson(int type);

int main(int argc, char** argv) 
{
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    int type; // denotes what type of implementation is to be used

	if (argc == 1) // only ./a.out is written
	{
		type = 2;
	}
	else // ./a.out x	input is of this form, x is denoting which part to execute
	{
		
		if (*argv[1] == '1')
		{
			type = 1;
		}
		
		else if (*argv[1] == '2')
		{
			type = 2;
		}

		else if (*argv[1] == '3')
		{
            type = 3;
		}

		else if (*argv[1] == '4')
		{
            type = 4;
		}
	
	}

    int cases;
    cin >> cases;

    vector<double> time_elapsed(cases);
    clock_t start, end;

    for (int i = 0; i < cases; i++)
    {
        start = clock();
        johnson(type);
        end = clock();

        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

        time_elapsed[i] = time_taken;
    }

    for (int i = 0; i < cases; i++)
    {
        cout << time_elapsed[i] << " ";
    }
    cout << "\n";

	return 0; 
} 


void johnson(int type)
{
    int N, D;

    cin >> N;
    if (N == 0) // if N == 0 : print -1
    {
        cout << "-1\n";
        return;
    }
    cin >> D;

    // 999999 is considered infinity

    int total_edges = 0;

    vector<vector<int>> adjList(N , vector<int> (N, 0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> adjList[i][j];
            if (adjList[i][j] < 999999 && i!=j)
            {
                total_edges++;
            }
        }
    }

    // ensuring no self loops
    for (int i = 0; i < N; i++)
    {
        adjList[i][i] = 999999;
    }

    // this will give the required values to update edge weights to remove negative edge weights
    vector<int> update_values = bellman_ford(adjList, N, D, total_edges);
    if (update_values.size() == 1) // when neg cycle is found we return {-1}
    {
        cout << "-1\n";
        return;
    }

    // for (int i = 0; i < N; i++)
    // {
    //     cout << update_values[i] << " ";
    // }
    // cout << "\n";

    vector<vector<int>> newAdjList(N , vector<int> (N, 999999)); // **this vector is of length N+1**

    // updating edges
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (adjList[i][j] < 999999)
            {
                newAdjList[i][j] = adjList[i][j] + update_values[i] - update_values[j];
            }
        }
    }
    
    // cout << "\ntype: " << type << "\n";

    for (int s = 1; s <= N; s++)
    {
        if (type == 1)
        {
            dijkstra1(newAdjList, N, D, s, update_values);
        }    
        else if (type == 2)
        {
            dijkstra2(newAdjList, N, D, s, update_values);
        }
        else if (type == 3)
        {
            dijkstra3(newAdjList, N, D, s, update_values);
        }
        else if (type == 4)
        {
            dijkstra4(newAdjList, N, D, s, update_values);
        }

    }
    return;

}



vector<int> bellman_ford(vector<vector<int>> adjList, int N, int D, int num_egdes)
{
    
    // ----999999 is considered infinity---- //


    // Adding new vertex which will act as source

    // if directed, we add edge from new vertex to other vertices
    if (D == 1)
    {
        for (int i = 0; i < N; i++)
        {
            adjList[i].push_back(999999);
        }
    }
    
    // else if undirected, we add edge from new vertex to other vertices
    else if (D == 1)
    {
        for (int i = 0; i < N; i++)
        {
            adjList[i].push_back(0);
        }
    }

    vector<int> newS;
    newS.assign(N+1, 0);
    adjList.push_back(newS);


    // defining source
    int s = N+1;

    // since new vertex is added, we change N to N+1
    N++;


    // distances of every vertex from source
    vector<int> distance(N);
    for (int i = 0; i < N; i++)
    {
        distance[i] = 999999;
    }
    distance[s-1] = 0; // source to itself is 0 distance
    

    // N times because in last iteration we can check if there is any change in last iteration
    // this will help in checking negative cycles
    bool change;
    for (int iter = 0; iter < N; iter++)
    {   
        change = false;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (adjList[i][j] < 999999)
                {    
                    if ((distance[i] + adjList[i][j]) < distance[j])
                    {
                        distance[j] = distance[i] + adjList[i][j];
                        change = true;
                    }
                }
            }
        }
    }

    if (change)
    {
        vector<int> neg_cycle = {-1};
        return neg_cycle;
    }


    // for (int i = 0; i < N; i++)
    // {
    //     cout << distance[i] << " ";
    // }

    return distance;
}


void dijkstra1(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values)
{

    // 999999 is considered infinity

    for (int i = 0; i < N; i++)
    {
        adjList[i][i] = 999999;
    }

    bool visited[N];
    for (int i = 0; i < N; i++)
    {
        visited[i] = false;
    }

    int distance[N];
    for (int i = 0; i < N; i++)
    {
        distance[i] = 999999;
    }
    distance[s-1] = 0;

    vector <int> ver;
    ver.push_back(s);

    int curr; // = s;

    while (1)
    {
        //finding closest vertex
        int curr = -1;

        int v;
        //for (auto v : ver)
        for (int ind5 = 0; ind5 < ver.size(); ind5++)
        {
            v = ver[ind5];
            if (!visited[v-1])
            {
                if (curr == -1)
                {
                    curr = v;
                }
                else if (distance[v-1] < distance[curr-1])
                {
                    curr = v;
                }
            }
        }

        if (curr == -1) // no vertex left to discover
        {
            break;
        }

        if (visited[curr-1]) // means curr is not changed
        {
            break;
        }

        //assigning distance to neighbors
        for (int i = 0; i < N; i++)
        {
            if (adjList[curr-1][i] < 0)
            {
                cout << "-1\n";
                return;
            }

            if (!visited[i])
            {
                if ((distance[curr-1] + adjList[curr-1][i]) < distance[i])
                {
                    distance[i] = distance[curr-1] + adjList[curr-1][i];
                    ver.push_back(i+1);
                }
            }            
        }

        // now curr can be marked visited
        visited[curr-1] = true;
    }

    // for (int i = 0; i < N; i++)
    // {
    //     cout << distance[i] << " ";
    // }
    // cout << "\n";

    for (int i = 0; i < N; i++)
    {
        if (distance[i] == 999999)
        {
            cout << 999999 << " ";
        }
        else
        {
            cout << distance[i] + update_values[i] - update_values[s-1] << " ";
        }
    }
    cout << "\n";

    return;
}


void decreaseBinary(vector<pair<int, int>> &binaryHeap, int node, int newVal, vector<int> &position)
{
    pair<int,int> newNode (node, newVal);
    binaryHeap[position[node-1]] = newNode;

    // int curr = node;
    while (1)
    {
        int pos = position[node-1];
        if (pos == 0) // root node
        {
            break;
        }
        int parent = binaryHeap[(pos-1)/2].first;
        if (binaryHeap[pos].second < binaryHeap[(pos-1)/2].second)
        {
            // swap the nodes
            pair<int, int> temp;
            temp = binaryHeap[pos];
            binaryHeap[pos] = binaryHeap[(pos-1)/2];
            binaryHeap[(pos-1)/2] = temp;

            int tempPos;
            tempPos = position[node-1];
            position[node-1] = position[parent-1];
            position[parent-1] = tempPos;
        }
        else
        {
            break;
        }        
    }
}

void percolateDownBinary(vector<pair<int, int>> &binaryHeap, int node, vector<int> &position, int activeNodes)
{
    while (1)
    {
        int pos = position[node-1];
        if ((2*pos + 1) >= activeNodes) // no child
        {
            break;
        }
        else if ((2*pos + 2) >= activeNodes) // one child (left)
        {
            if (binaryHeap[pos].second > binaryHeap[2*pos + 1].second)
            {
                int left_child = binaryHeap[2*pos + 1].first;

                // swap the nodes
                pair<int, int> temp;
                temp = binaryHeap[pos];
                binaryHeap[pos] = binaryHeap[2*pos + 1];
                binaryHeap[2*pos + 1] = temp;

                int tempPos;
                tempPos = position[node-1];
                position[node-1] = position[left_child-1];
                position[left_child-1] = tempPos;
            }
            else
            {
                break;
            }            
        }
        else // both children
        {   
            // right children is smaller
            if (binaryHeap[2*pos + 2].second < binaryHeap[2*pos + 1].second)
            {
                // if we need to percolate down (swap)
                if (binaryHeap[2*pos + 2].second < binaryHeap[pos].second)
                {
                    int right_child = binaryHeap[2*pos + 2].first;

                    // swap the nodes
                    pair<int, int> temp;
                    temp = binaryHeap[pos];
                    binaryHeap[pos] = binaryHeap[2*pos + 2];
                    binaryHeap[2*pos + 2] = temp;

                    int tempPos;
                    tempPos = position[node-1];
                    position[node-1] = position[right_child-1];
                    position[right_child-1] = tempPos;
                }
                else
                {
                    break;
                }
                
            }
            // else: left child is smaller (or equal)
            else
            {
                // if we need to percolate down (swap)
                if (binaryHeap[2*pos + 1].second < binaryHeap[pos].second)
                {
                    int left_child = binaryHeap[2*pos + 1].first;

                    // swap the nodes
                    pair<int, int> temp;
                    temp = binaryHeap[pos];
                    binaryHeap[pos] = binaryHeap[2*pos + 1];
                    binaryHeap[2*pos + 1] = temp;

                    int tempPos;
                    tempPos = position[node-1];
                    position[node-1] = position[left_child-1];
                    position[left_child-1] = tempPos;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return;
}

pair<int, int> extractMinBinary(vector<pair<int, int>> &binaryHeap, vector<int> &position, int activeNodes)
{
    // min is root itself
    pair<int, int> ans = binaryHeap[0];

    // swap with bottom-rightmost element
        // pair<int,int> temp = binaryHeap[position[node-1]];
        // binaryHeap[position[node-1]] = binaryHeap[activeNodes-1]; // bottomright node
        // binaryHeap[activeNodes-1] = temp;
    pair<int,int> temp = binaryHeap[0];
    binaryHeap[0] = binaryHeap[activeNodes-1]; // bottomright node
    binaryHeap[activeNodes-1] = temp;

    int tempPos = position[binaryHeap[0].first - 1];
    position[binaryHeap[0].first - 1] = position[binaryHeap[activeNodes-1].first - 1];
    position[binaryHeap[activeNodes-1].first - 1] = tempPos;


    int node = binaryHeap[0].first;

    percolateDownBinary(binaryHeap, node, position, activeNodes-1);

    return ans;
}


void dijkstra2(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values)
{
    // 999999 is infinity

    for (int i = 0; i < N; i++)
    {
        adjList[i][i] = 999999;
    }

    vector<int> position(N); // i th vertex position on (i-1)th index
    
    vector<pair<int, int>> binaryHeap(N); // (node, distance)
    for (int i = 0; i < N; i++)
    {
        pair<int, int> node (i+1, 999999);
        binaryHeap[i] = node;
        position[i] = i;
    }

    int activeNodes = N; // count of nodes in binary heap

    decreaseBinary(binaryHeap, s, 0, position);

        // // swapping with bottom-rightmost node
        // pair<int,int> temp = binaryHeap[position[s-1]];
        // binaryHeap[position[s-1]] = binaryHeap[activeNodes-1]; // bottomright node
        // binaryHeap[activeNodes-1] = temp;

    pair<int, int> exmin = extractMinBinary(binaryHeap, position, activeNodes);
    activeNodes--;
    
    int curr = exmin.first;
    int curr_dist = exmin.second;

    while (1)
    {
        for (int i = 0; i < N; i++)
        {
            if (adjList[curr-1][i] < 999999) // edge exists
            {
                int pos = position[i];
                if ((adjList[curr-1][i] + curr_dist) < binaryHeap[pos].second)
                {
                    decreaseBinary(binaryHeap, i+1, (adjList[curr-1][i] + curr_dist), position);
                }
            }
        }

        exmin = extractMinBinary(binaryHeap, position, activeNodes);
        activeNodes--;
        
        curr = exmin.first;
        curr_dist = exmin.second;

        if (activeNodes == 0)
        {
            break;
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (binaryHeap[position[i]].second == 999999)
        {
            cout << 999999 << " ";
        }
        else
        {
            cout << binaryHeap[position[i]].second + update_values[i] - update_values[s-1] << " ";
        }
    }
    cout << "\n";

    return;
    
}




//==============================================================================================================
//==============================================================================================================
//==============================================================================================================


// binomial tree node
struct Node {
    int value, vertex;
    Node *sibling, *child, *parent;
    // child is leftmost child(highest order)
    // sibling is towards right
};

Node* nodeBinomialTree(int ver, int distance)
{
    Node* temp = (Node*) malloc (sizeof(Node));
    temp->vertex = ver;
    temp->value = distance;
    temp->sibling = NULL;
    temp->child = NULL;
    temp->parent = NULL;
    return temp;
}

// list<pair<Node*, int>> groupBinHeap(list<pair<Node*, int>> rootList)
void groupBinomial(list<pair<Node*, int>>& rootList)
{
    vector<int> order;

    list<pair<Node*, int>> :: iterator itr1, itr2, temp;

    int num = rootList.size();
    int ct = 0;

    itr1 = rootList.begin();
    itr2 = rootList.begin();
    itr2++;

    while (1)
    {
        if ((*itr1).second == (*itr2).second) // same order trees
        {
            if (((*itr1).first)->value >= ((*itr2).first)->value)
            {
                Node* to_move = (*itr1).first;
                Node* ini_child = ((*itr2).first)->child;

                to_move->parent = (*itr2).first;
                to_move->sibling = ini_child;
                ((*itr2).first)->child = to_move;
                
                (*itr2).second++;

                rootList.erase(itr1);

                itr1 = itr2;
                itr2++;

                // break;
            }
            else
            {
                Node* to_move = (*itr2).first;
                Node* ini_child = ((*itr1).first)->child;

                to_move->parent = (*itr1).first;
                to_move->sibling = ini_child;
                ((*itr1).first)->child = to_move;
                
                (*itr1).second++;

                temp = itr2;
                temp++;

                rootList.erase(itr2);

                itr2 = temp;

                // break;
            }
        }
        else
        {
            itr1++;
            itr2++;            
        }
        

        if (itr2 == rootList.end())
        {
            break;
        }

    }

    // while (1)
    // {
    //     bool change = false;

    //     itr2 = rootList.begin();
    //     itr2++;

    //     for (itr1 = rootList.begin(); itr2 != rootList.end(); itr1++)
    //     {
    //         if ((*itr1).second == (*itr2).second) // same order trees
    //         {
    //             change = true;

    //             if (((*itr1).first)->value >= ((*itr2).first)->value)
    //             {
    //                 Node* to_move = (*itr1).first;
    //                 Node* ini_child = ((*itr2).first)->child;

    //                 to_move->sibling = ini_child;
    //                 ((*itr2).first)->child = to_move;
                    
    //                 (*itr2).second++;

    //                 rootList.erase(itr1);
    //                 break;
    //             }
    //             else
    //             {
    //                 Node* to_move = (*itr2).first;
    //                 Node* ini_child = ((*itr1).first)->child;

    //                 to_move->sibling = ini_child;
    //                 ((*itr1).first)->child = to_move;
                    
    //                 (*itr1).second++;

    //                 rootList.erase(itr2);
    //                 break;
    //             }
    //         }

    //         if (itr2 == rootList.end())
    //         {
    //             break;
    //         }

    //         itr2++;
    //     }
        
    //     if (!change)
    //     {
    //         break;
    //     }
    // }
    
}

// Node* insertBinHeap(int data, list<pair<Node*, int>>rootList)
Node* insertBinomial(int ver, int distance, list<pair<Node*, int>>& rootList)
{
    Node* newNode = nodeBinomialTree(ver, distance);
    pair <Node*, int> pr (newNode, 0); // 0 corresponds to order of that binomial tree
    rootList.push_front(pr);

    // list<pair<Node*, int>> rootList1 = groupBinHeap(rootList);
    groupBinomial(rootList);

    return newNode;
}

void decreaseKeyBinomial(Node* to_dec, int newVal, vector<Node*>& position, list<pair<Node*, int>> rootList)
{
    to_dec->value = newVal;
    Node* par;
    Node* curr;

    curr = to_dec;

    // percolate up
    while (1)
    {
        par = curr->parent;

        if (par == NULL)
        {
            break;
        }

        if (par->value > curr->value)
        {
            // swap nodes
            int tempVer = par->vertex;
            int tempVal = par->value;
            par->vertex = curr->vertex;
            par->value = curr->value;
            curr->vertex = tempVer;
            curr->value = tempVal;

            Node* tempPos = position[curr->vertex -1];
            position[curr->vertex -1] = position[par->vertex -1];
            position[par->vertex -1] = tempPos;

            curr = par;

                // int tempVer = par->vertex;
                // int tempVal = par->value;
                // Node* tempChild = par->child;
                // Node* tempParent = par->parent;
                // Node* tempSibling = par->sibling;

                // par-> vertex = to_dec->vertex;
                // par-> value = to_dec->value;
                // par-> child = to_dec->child;
                // par-> parent = to_dec->parent;
                // par-> sibling = to_dec->sibling;

                // to_dec->vertex = tempVer;
                // to_dec->value = tempVal;
                // to_dec->child = tempChild;
                // to_dec->parent = tempParent;
                // to_dec->sibling = tempSibling;
        }
        else
        {
            break;
        }        
    }
    return;
}


Node* extractMinBinomial(vector<Node*>& position, list<pair<Node*, int>>& rootList)
{
    // min element is one in the 'rootList'
    pair<Node*, int> minRoot;

    list<pair<Node*, int>> :: iterator itr, min_itr;

    itr = rootList.begin();
    min_itr = itr;
    minRoot = *itr;

    if (rootList.begin() == rootList.end())
    {
        cout << "yay\n";
    }

    for (itr = rootList.begin(); itr != rootList.end(); itr++)
    {
        if (((*itr).first)->value < (minRoot.first)->value)
        {
            minRoot = *itr;
            min_itr = itr;
        }
    }

    rootList.erase(min_itr);

    int orderOfMin = minRoot.second;
    vector<Node*> children;
    if (orderOfMin > 0)
    {
        Node* child = (minRoot.first)->child;
        children.push_back(child);
        for (int i = 0; i < orderOfMin-1; i++)
        {
            children.push_back(child->sibling);
            child = child->sibling;
        }
        for (int i = 0; i < orderOfMin; i++)
        {
            children[i]->parent = NULL;
            children[i]->sibling = NULL;
        }
    }

    (minRoot.first)->child = NULL;

    // inserting the children in rootList
    list<pair<Node*, int>> :: iterator itr1;
    for (int i = 0; i < orderOfMin; i++)
    {
        // node is children[i], order
        pair<Node*, int> newNode(children[i], orderOfMin-i-1);
        bool isAdded = false;
        for (itr1 = rootList.begin(); itr1 != rootList.end(); itr1++)
        {
            if ((*itr1).second >= (orderOfMin-i))
            {                
                rootList.insert(itr1, newNode);
                groupBinomial(rootList);
                isAdded = true;
                break;
            }
        }
        if (!isAdded)
        {
            rootList.push_back(newNode);
        }
    }


    return minRoot.first;
}


void dijkstra3(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values)
{
    // 999999 is infinity

    for (int i = 0; i < N; i++)
    {
        adjList[i][i] = 999999;
    }

    list<pair<Node*, int>> rootList;

    vector<Node*> position(N);
    for (int i = 0; i < N; i++)
    {
        position[i] = insertBinomial(i+1, 999999, rootList);
    }

    decreaseKeyBinomial(position[s-1], 0, position, rootList);

    Node* exmin = extractMinBinomial(position, rootList);

    int curr = exmin->vertex;
    int curr_dist = exmin->value;

    int visited = 1;

    while (visited < N)
    {
        for (int i = 0; i < N; i++)
        {
            if (adjList[curr-1][i] < 999999)
            {
                Node* pos = position[i];
                if ((adjList[curr-1][i] + curr_dist) < pos->value)
                {
                    decreaseKeyBinomial(pos, (adjList[curr-1][i] + curr_dist), position, rootList);
                }
            }
        }

        exmin = extractMinBinomial(position, rootList);

        curr = exmin->vertex;
        curr_dist = exmin->value;

        visited++;
    }

    int dist;
    for (int i = 0; i < N; i++)
    {
        dist = position[i]->value;
        if (dist == 999999)
        {
            cout << 999999 << " ";
        }
        else
        {
            cout << dist + update_values[i] - update_values[s-1] << " ";
        }
    }
    cout << "\n";

    return;
}


//==============================================================================================================
//==============================================================================================================
//==============================================================================================================


// fibonacci tree-heap node
struct fNode {
    int value, vertex, rank;
    fNode *left, *right, *child, *parent;
    bool isMarked;
};

fNode* nodeFib(int ver, int distance)
{
    fNode* temp = (fNode*) malloc (sizeof(fNode));
    temp->value = distance;
    temp->vertex = ver;
    temp->rank = 0;
    temp->left = temp;
    temp->right = temp;
    temp->child = NULL;
    temp->parent = NULL;
    temp->isMarked = false;
    return temp;
}

fNode* insertFib(int ver, int distance, fNode* &minptr)
{
    fNode* newNode = nodeFib(ver, distance);
    //pair <fNode*, int> pr (newNode, 0); // 0 is order of node

    if (minptr == NULL)
    {
        minptr = newNode;
        return newNode;
    }
    
    if (minptr->right == minptr)
    {
        newNode->right = minptr;
        newNode->left = minptr;
        minptr->right = newNode;
        minptr->left = newNode;
    }
    else
    {
        newNode->right = minptr->right;
        (newNode->right)->left = newNode;
        newNode->left = minptr;
        minptr->right = newNode;
    }

    if (minptr->value > newNode->value)
    {
        minptr = newNode;
    }

    return newNode;
}

void decreaseKeyFib(fNode* to_dec, int newVal, vector<fNode*>& position, fNode* &minptr)
{
    to_dec->value = newVal;
    fNode* curr = to_dec;
    fNode* par;

    while (1)
    {    
        par = curr->parent;

        if (par == NULL)
        {
            if (minptr->value > curr->value)
            {
                minptr = curr;
            }
            break;
        }

        if (par->value <= newVal && (!curr->isMarked))
        {
            break;
        }

        // remove and put it in rootlist
        if (curr->right == curr) // only child
        {
            curr->parent = NULL;
            par->child = NULL;

            // decrease parent's rank by 1
            par->rank--;

            // add to rootList
            curr->isMarked = false;

            // curr->right = minptr->right;
            // curr->left = minptr;
            // minptr->right = curr;
            if (minptr->right == minptr)
            {
                curr->right = minptr;
                curr->left = minptr;
                minptr->right = curr;
                minptr->left = curr;
            }
            else
            {
                curr->right = minptr->right;
                (curr->right)->left = curr;
                curr->left = minptr;
                minptr->right = curr;
            }

            if (minptr->value > curr->value)
            {
                minptr = curr;
            }

            if (par->isMarked)
            {
                curr = par;
                // break;
            }
            else
            {
                if (par->parent != NULL)
                {
                    par->isMarked = true;
                }
                break;
            }        
        }
        else
        {
            if ((curr->parent)->child == curr) // this node is directly connected to parent
            {
                par->child = curr->right;
            }
            
            (curr->right)->left = curr->left;
            (curr->left)->right = curr->right;
            curr->left = NULL;
            curr->right = NULL;
            curr->parent = NULL;

            // decrease parent's rank by 1
            par->rank--;

            // add to rootList
            curr->isMarked = false;

            if (minptr->right == minptr)
            {
                curr->right = minptr;
                curr->left = minptr;
                minptr->right = curr;
                minptr->left = curr;
            }
            else
            {
                curr->right = minptr->right;
                (curr->right)->left = curr;
                curr->left = minptr;
                minptr->right = curr;
            }

            if (minptr->value > curr->value)
            {
                minptr = curr;
            }

            if (par->isMarked)
            {
                curr = par;
                // break;
            }
            else
            {
                if (par->parent != NULL)
                {
                    par->isMarked = true;
                }
                break;
            }
        }
    }

    return;    
}

void groupFib(fNode* &minptr, int maxR)
{
    if (minptr == NULL)
    {
        return;
    }

    fNode* temp = minptr;
    // fNode* maxRank = temp;
    // temp = temp->right;

    // while (temp != minptr)
    // {
    //     if (temp->rank > maxRank->rank)
    //     {
    //         maxRank = temp;
    //     }
    //     temp = temp->right;
    // }

    // int r = maxRank->rank;

    if (minptr->right == minptr)
    {
        return;
    }

    int r = maxR;

    vector<fNode*> nodeList (r+1);
    for (int i = 0; i < r+1; i++)
    {
        nodeList[i] = NULL;
    }

    // traverse rootList and start adding to nodeList (combine if required)
    temp = minptr;
    int rk = temp->rank;
    nodeList[temp->rank] = temp;
    temp = temp->right;

    (nodeList[rk]->right)->left = nodeList[rk]->left;
    (nodeList[rk]->left)->right = nodeList[rk]->right;
    nodeList[rk]->left = nodeList[rk];
    nodeList[rk]->right = nodeList[rk];

    fNode* next;

    while (temp != minptr)
    {
        next = temp->right;

        rk = temp->rank;
        if (nodeList[rk] == NULL) // no node of same order(rank)
        {
            nodeList[rk] = temp;
            (nodeList[rk]->right)->left = nodeList[rk]->left;
            (nodeList[rk]->left)->right = nodeList[rk]->right;
            nodeList[rk]->left = nodeList[rk];
            nodeList[rk]->right = nodeList[rk];
        }
        else // node of same rank already exists
        {
            fNode* curr = temp;
            int curr_rnk = rk;
            
            (curr->left)->right = curr->right;
            (curr->right)->left = curr->left;

            while (1)
            {
                if (nodeList[curr_rnk] == NULL)
                {
                    (curr->left)->right = curr->right;
                    (curr->right)->left = curr->left;
                    nodeList[curr_rnk] = curr;
                    nodeList[curr_rnk]->left = nodeList[curr_rnk];
                    nodeList[curr_rnk]->right = nodeList[curr_rnk];
                    break;
                }
                // join both heap-trees of same rank
                if (curr->value < nodeList[curr_rnk]->value)
                {
                    if (curr->child == NULL)
                    {
                        curr->child = nodeList[curr_rnk];
                        (curr->child)->parent = curr;
                        (curr->child)->right = curr->child;
                        (curr->child)->left = curr->child;
                    }
                    else
                    {
                        // nodeList[curr_rnk]->right = (curr->child)->right;
                        // (curr->child)->right = nodeList[curr_rnk];
                        // ((curr->child)->right)->left = curr->child;
                        if (curr->rank == 1)
                        {
                            nodeList[curr_rnk]->parent = curr;
                            nodeList[curr_rnk]->right = curr->child;
                            nodeList[curr_rnk]->left = curr->child;
                            (curr->child)->left = nodeList[curr_rnk];
                            (curr->child)->right = nodeList[curr_rnk];
                        }
                        else
                        {
                            nodeList[curr_rnk]->parent = curr;
                            nodeList[curr_rnk]->right = (curr->child)->right;
                            ((curr->child)->right)->left = nodeList[curr_rnk];
                            (curr->child)->right = nodeList[curr_rnk];
                            ((curr->child)->right)->left = curr->child;
                        }
                        
                    }
                    curr->rank++;
                    nodeList[curr_rnk] = NULL;
                    curr_rnk = curr->rank;

                    (curr->left)->right = curr->right;
                    (curr->right)->left = curr->left;

                    curr->left = curr;
                    curr->right = curr;
                }
                else
                {
                    if (nodeList[curr_rnk]->child == NULL)
                    {
                        nodeList[curr_rnk]->child = curr;
                        (nodeList[curr_rnk]->child)->parent = nodeList[curr_rnk];
                        (nodeList[curr_rnk]->child)->right = nodeList[curr_rnk]->child;
                        (nodeList[curr_rnk]->child)->left = nodeList[curr_rnk]->child;
                    }
                    else
                    {
                        // curr->right = (nodeList[curr_rnk]->child)->right;
                        // (nodeList[curr_rnk]->child)->right = curr;
                        // ((nodeList[curr_rnk]->child)->right)->left = nodeList[curr_rnk]->child;
                        if (nodeList[curr_rnk]->rank == 1)
                        {
                            (curr->left)->right = curr->right;
                            (curr->right)->left = curr->left;

                            curr->parent = nodeList[curr_rnk];
                            curr->right = nodeList[curr_rnk]->child;
                            curr->left = nodeList[curr_rnk]->child;
                            (nodeList[curr_rnk]->child)->left = curr;
                            (nodeList[curr_rnk]->child)->right = curr;
                        }
                        else
                        {
                            curr->parent = nodeList[curr_rnk];
                            curr->right = (nodeList[curr_rnk]->child)->right;
                            ((nodeList[curr_rnk]->child)->right)->left = curr;
                            (nodeList[curr_rnk]->child)->right = curr;
                            ((nodeList[curr_rnk]->child)->right)->left = nodeList[curr_rnk]->child;
                        }   
                    }
                    nodeList[curr_rnk]->rank++;
                    curr = nodeList[curr_rnk];
                    nodeList[curr_rnk] = NULL;                    
                    curr_rnk = curr->rank;

                    // (curr->left)->right = curr->right;
                    // (curr->right)->left = curr->left;

                    curr->left = curr;
                    curr->right = curr;
                }
                
            }
        }
        
        // temp = temp->right;
        if (temp == next)
        {
            break;
        }
        temp = next;
        
    }

    fNode* newRoot = NULL;
    for (int i = 0; i < r+1; i++)
    {
        if (nodeList[i] != NULL)
        {
            nodeList[i]->isMarked = false;
            if (newRoot == NULL)
            {
                newRoot = nodeList[i];
                newRoot->right = newRoot;
                newRoot->left = newRoot;
            }
            else
            {
                nodeList[i]->right = newRoot->right;
                nodeList[i]->left = newRoot;
                (newRoot->right)->left = nodeList[i];
                newRoot->right = nodeList[i];
            }
            
        }
    }
    
    minptr = newRoot;
    for (int i = 0; i < r+1; i++)
    {
        newRoot = newRoot->right;
        if (minptr->value > newRoot->value)
        {
            minptr = newRoot;
        }
    }

    return;
}

fNode* extractMinFib(vector<fNode*>& position, fNode* &minptr, int maxR)
{
    // fNode* initNode = minptr;
    // fNode* rootnode = minptr;
    // while(1)
    // {    groupFib(minptr, maxR);
    //     if(rootnode->value < minptr->value)
    //     {
    //         minptr = rootnode;
    //     }
    //     rootnode = rootnode->right;
    //     if (rootnode == initNode)
    //     {
    //         break;
    //     }
    // }

    // rootnode = minptr; // this will be returned

    fNode* ch; // child
    fNode* temp;

    fNode* ans;
    ans = minptr; // to be returned
    // ans->rank = 0;
    // ans->child = NULL;
    // ans->right = NULL;
    // ans->left = NULL;

    ch = minptr->child;

    if (ch != NULL)
    {
        // make parent NULL
        ch->parent = NULL;
        ch->isMarked = false;
        ch = ch->right;

        while (ch!=minptr->child)
        {
            ch->parent = NULL;
            ch->isMarked = false;
            ch = ch->right;
        }

        ch = minptr->child;   
        temp = ch->right;

        // add children to rootList
        ch->right = minptr->right;
        (ch->right)->left = ch;

        temp->left = minptr->left;
        (temp->left)->right = temp;

        // updating min ptr
        minptr = ch;
        while (temp!=ch)
        {
            if (temp->value < minptr->value)
            {
                minptr = temp;
            }
            temp = temp->right;
        }
    }

    else
    {
        temp = minptr;
        if (temp->right == temp)
        {
            minptr = NULL;
        }
        else
        {
            (temp->left)->right = temp->right;
            (temp->right)->left = temp->left;
            minptr = temp->right;
            fNode* t1 = minptr;
            while (1)
            {
                if (t1->value < minptr->value)
                {
                    minptr = t1;
                }
                t1 = t1->right;
                if (t1 == temp->right)
                {
                    break;
                }
            }

        }
        
    }

    groupFib(minptr, maxR);

    return ans;    
}

void dijkstra4(vector<vector<int>> adjList, int N, int D, int s, vector<int> update_values)
{

    int maxR = 0;
    int n1 = N;
    int ctr = 0;
    while (n1 != 0)
    {
        ctr++;
        if (n1%2 == 1)
        {
            maxR = ctr;
        }
        n1/=2;
    }

    // 999999 is infinity

    for (int i = 0; i < N; i++)
    {
        adjList[i][i] = 999999;
    }

    // list<pair<fNode*, int>> rootList;

    fNode* minptr = NULL;

    vector<fNode*> position(N);
    for (int i = 0; i < N; i++)
    {
        position[i] = insertFib(i+1, 999999, minptr);
    }

    decreaseKeyFib(position[s-1], 0, position, minptr);

    fNode* exmin = extractMinFib(position, minptr, maxR);

    int curr = exmin->vertex;
    int curr_dist = exmin->value;

    int visited = 1;

    while (visited < N)
    {
        for (int i = 0; i < N; i++)
        {
            if (adjList[curr-1][i] < 999999)
            {
                fNode* pos = position[i];
                if ((adjList[curr-1][i] + curr_dist) < pos->value)
                {
                    decreaseKeyFib(pos, (adjList[curr-1][i] + curr_dist), position, minptr);
                }
            }
        }

        exmin = extractMinFib(position, minptr, maxR);

        curr = exmin->vertex;
        curr_dist = exmin->value;

        visited++;
    }

    int dist;
    for (int i = 0; i < N; i++)
    {
        dist = position[i]->value;
        if (dist == 999999)
        {
            cout << 999999 << " ";
        }
        else
        {
            cout << dist + update_values[i] - update_values[s-1] << " ";
        }
    }
    cout << "\n";

    return;
}
