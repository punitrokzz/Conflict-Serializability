#include<bits/stdc++.h>
using namespace std;
int main()
{
    /*Take input and identify individual transactions and vertices (transaction_no) from history*/

    stringstream transactions;
    string history_string,temp,operation_temp,data_value_temp,transaction;
    int vertex,visited_counter=0;
    vector<int> vertices,vertices_zero_indegree;vector<string> history_elements;

    getline(cin,history_string);
    transactions << history_string; //pass the input history tring to "transactions" stringstream
    while(!transactions.eof())
    {
        transactions >> temp;
        if(stringstream(temp)>>vertex)
            if(find(vertices.begin(),vertices.end(),vertex)== vertices.end())
                vertices.push_back(vertex); //construct vertices vector by pushing numerical elements in history string

        if(stringstream(temp)>>transaction)
            history_elements.push_back(transaction); //construct history_elements vector by pushing individual elements within the history string

    }

    vector<int> vertices_indegree(vertices.size(),0);   //vector to store indegree[no. of incoming edges] of all possible vertices obtained from stringstream

    list<int>* adj = new list<int>[vertices.size()];    //adjacency 2D list representation for directed precedence graph node connection


    /*Populate edge connection to adjacent vertices for all vertices using nested loop
      Traverse all succeeding unit transactions of form ['Operation','Transaction Number','Data element'] for a particular unit transaction in history string
      For conflicting operations, push back the corresponding vertex as adjacent node */

    for(int i=0;i<history_elements.size();i+=3)
    {
        for(int j=i+3;j<history_elements.size();j+=3)
        {
            operation_temp = history_elements[i];
            transaction = history_elements[i+1];
            data_value_temp = history_elements[i+2];

            if((!((operation_temp=="r") && (history_elements[j]=="r"))) && (data_value_temp == history_elements[j+2]) && (transaction != history_elements[j+1]))
                if((find(adj[stoi(transaction)-1].begin(),adj[stoi(transaction)-1].end(),stoi(history_elements[j+1])-1)) == (adj[stoi(transaction)-1].end()))
                    adj[stoi(transaction)-1].push_back(stoi(history_elements[j+1])-1);
        }

    }

    /*perform acyclic check on adjacency list representation using BFS */

    for(int i=0;i<vertices.size();i++)
        for(auto j:adj[i])
            ++vertices_indegree[j];     //populate indegree for each vertex by traversing adjacency list

    for(int i=0;i<vertices.size();i++)
        if(!vertices_indegree[i])
            vertices_zero_indegree.push_back(i);  //populate vertices with zero indegree

    while(!vertices_zero_indegree.empty())
    {
        int temp_vertex = *(vertices_zero_indegree.begin());    //Fetch first vertex from vector
        vertices_zero_indegree.erase(vertices_zero_indegree.begin());
        for(auto v:adj[temp_vertex])
            if(!(--vertices_indegree[v]))               //reduce the indegree count of vertex by traversing neighbouring nodes
                vertices_zero_indegree.push_back(v);    //push to vertices_zero_indegree vector if indegree of that vertex becomes zero

        ++visited_counter;              //count of visited vertices
    }

    if(visited_counter == vertices.size())  //count of visited vertices should be equal to number of vertices in the precedence graph for acyclicity
        cout<<"True";       //history is serializable as no cycles were found
    else
        cout<<"False";


    return 0;
}
