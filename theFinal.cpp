#include<iostream>
#include<fstream>
#include<string>
#include<cstring>

using namespace std;

//function declarations
void mainMinimization(int);
int noOfNodes();
int noOfAlphabets();
void storeNodeNames(string*, int, int);
void storeAlphabetNames(string*, int);
void storeTransition(int**, int, int);
void graphMatrix(int**, int**, int, int);
void closureMatrix(int**, int**, int);
void storeStateData(int*, int, int);
void removeNonReachables(int**, int*, int, int);
int fsc(int*, int);
int nfsc(int*, int);
void makeTwoTables(int*, int*, int*, int);
void checkForEqualityInTable(int**, int, int*, int, int*);

int acceptanceNumber(); // used only for acceptance of dfa

//End of function declarations

//Minimization of DFA
//mainMinimization function definition

//++++++++++++
//Place where the main function call ends. In the transition.txt file, the transition table for the dfa will be given. So using that n and m wil b calculated which is the number of 
//alphabets and no of nodes. 
void mainMinimization(int theIP){
  int n,m;

  n = noOfAlphabets();
  m = noOfNodes();

  //creating string array to store all the names of alphabets, nodes etc.
  int statesOfNodes[m] = {0};
  string namesOfNodes[m];
  string namesOfAlphabets[n];
  
  //once the array is created, these functions store the node names, alphabet names and state data(which is final state, which is non final state) using following
  //three function calls.
  storeNodeNames(namesOfNodes, m, n);
  storeAlphabetNames(namesOfAlphabets, n);
  storeStateData(statesOfNodes, m, n);

  //after the names are stored, it's place to store the transition table. Before that it dynamically creates a 2d array. 
  int **transitionMatrix = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       transitionMatrix[r] = (int *)malloc(n * sizeof(int));

  //since 2d array created, now the function call stores the matrix
  storeTransition(transitionMatrix, m, n);


  //********************************************  WORKS ONLY IF THE IP==2 (ACCEPTANCE OF DFA CALLED FROM MAIN FUNCTION ) ***************************************
  
  if(theIP == 2){
    int currentState = 0, flag = 0, value = 0;
    //this function gets the number of alphabets in the input langauge
    int numberOfInput = acceptanceNumber();
    //creates a string array to store that
    string theAcceptanceInput[numberOfInput];
    
    //this opens the file and reads all the input alphabet given and stores then in the array
    fstream theFile;
    int smallN;
    theFile.open("accept.txt", ios::in);
    if(!theFile){
      cout<<"File doesn't Exists, Please Check";
      exit(1);
    }else{
      theFile >> smallN;
      for(int i=0;i<numberOfInput;i++){
        theFile >> theAcceptanceInput[i];
      }

    }
    
    //this checks for each alphabet, which state it moves after each turn. if the alphabet is not present program is exited
    for(int i=0;i<numberOfInput;i++){
      flag = 0;
      for(int j=0;j<n;j++){
        if(theAcceptanceInput[i]==namesOfAlphabets[j]){
          flag = 1;
          value = j;
          break;
        }
      }
      if(flag==0){
        cout<<"\nThe Given Language doesn't work for the respective Transition table given ! Sorry";
        exit(0);
      }
      //changes current state according to where it moves
      currentState = transitionMatrix[currentState][value];
    }
    
    cout<<"\nYour Given Language :  ";
    for(int j=0;j<numberOfInput;j++){
      cout<<theAcceptanceInput[j];
    }
    if(statesOfNodes[currentState]==1){
      cout<<" is accepted !";
    }else{
      cout<<" is not accepted !";
    }


    exit(0);

  }
   //*************************************  END OF ACCEPTANCE OF DFA FUNCTION ... EXECUTED ONLY IF CASE 2 IN MAIN CALLED **********************************************

  //to find minimization of dfa we need to remove the states which are unreachable from teh initial state with any moves.
  //for that purpose we have to find the create a graph and closure of it (check transitive closure of a graph in google for more details). 
  //both arrays are created dynamically.
  
  
  int **theGraph = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       theGraph[r] = (int *)malloc(m * sizeof(int));

  int **theClosure = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       theClosure[r] = (int *)malloc(m * sizeof(int));


  //the data for both is stored in the arrays.
  graphMatrix(theGraph, transitionMatrix, m, n);
  closureMatrix(theClosure, theGraph, m);
  removeNonReachables(theClosure, statesOfNodes, m, n);
  
  //this counts the no of final and non final states.
  int finalStatesCount = fsc(statesOfNodes, m);
  int nonFinalStatesCount = nfsc(statesOfNodes, m);

  //array for final and non final states
  int FinalStates[finalStatesCount];
  int NonFinalStates[nonFinalStatesCount];

  //the next step to find minimzation is to make two tables one containing all final states, otehr non final states. find if there is any equal rows.
  //teh following funcction does those. (check this for more explanation : https://www.javatpoint.com/minimization-of-dfa
  makeTwoTables(FinalStates, NonFinalStates, statesOfNodes, m);
  checkForEqualityInTable(transitionMatrix, n, FinalStates, finalStatesCount, statesOfNodes);
  checkForEqualityInTable(transitionMatrix, n, NonFinalStates, nonFinalStatesCount, statesOfNodes);

  //this prints the final output
  cout<<"\nTHE FINAL TRANSITION TABLE IS";
  cout<<"\n\n\n";
  for(int i=0;i<n;i++){
    if(statesOfNodes[i]==0 || statesOfNodes[i]==1){
      cout<<"\t"<<namesOfAlphabets[i];
    }
  }
  cout<<"\n";
  for(int i=0;i<m;i++){
    if(statesOfNodes[i]==0 || statesOfNodes[i]==1){
      cout<<namesOfNodes[i];
      for(int j=0;j<n;j++){
        cout<<"\t"<<namesOfNodes[transitionMatrix[i][j]];
      }
      cout<<"\n";
    }
  }
}

// no of alphabets in teh language used in acceptance of dfa only
int acceptanceNumber(){
  fstream theFile;
  int smallN;
  theFile.open("accept.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    theFile >> smallN;
  }
  return smallN;

}

//find no of alphabets 
int noOfAlphabets(){
  fstream theFile;
  int numberOfAlphabets;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    theFile >> numberOfAlphabets;
  }
  theFile.close();
  return numberOfAlphabets;
}


int noOfNodes(){
  fstream theFile;
  string line;
  int numberOfNodes = 0;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    while (getline(theFile, line)){
      numberOfNodes ++;
    }
    return numberOfNodes-2;
  }
}

void storeNodeNames(string *nodeNames, int m, int n){
  fstream theFile;
  string line, temp;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    getline(theFile, line);
    for(int i=0;i<m;i++){
      theFile >> nodeNames[i];
      for(int j=0;j<n;j++){
        theFile >> temp;
      }
    }
  }
}

void storeAlphabetNames(string *alphabetNames, int n){
  fstream theFile;
  int smallN;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    theFile >> smallN;
    for(int i=0;i<n;i++){
      theFile >> alphabetNames[i];
    }

  }
}

void storeStateData(int *statesOfNodes, int m, int n){
  fstream theFile;
  string line;
  int inte;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
  }else{
    for(int i=0;i<=m;i++){
      getline(theFile, line);
    }
    theFile >> line;
    while(1){
      theFile >> inte;
      statesOfNodes[inte] = 1;
      if(theFile.eof())
        break;
    }
  }
}

void storeTransition(int** transitionMat, int m, int n){
  fstream theFile;
  string temp, line;
  theFile.open("transition.txt", ios::in);
  if(!theFile){
    cout<<"File doesn't Exists, Please Check";
    exit(1);
  }else{
    getline(theFile, line);
    for(int i=0;i<m;i++){
      theFile >> temp;
      for(int j=0;j<n;j++){
        theFile >> transitionMat[i][j];
      }
    }
  }
}

void graphMatrix(int **theGraph, int **transitionMatrix, int m, int n){
  for(int i=0;i<m;i++){
    for(int j=0;j<m;j++){
      theGraph[i][j] = 0;
    }
  }

  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      theGraph[i][transitionMatrix[i][j]] = 1;
    }
  }
}

void closureMatrix(int **theClosure, int **theGraph, int m){
  int i, j, k;
  for (i = 0; i < m; i++){
      for (j = 0; j < m; j++){
          theClosure[i][j] = theGraph[i][j];
      }
  }
  for (k = 0; k < m; k++){
      for (i = 0; i < m; i++){
          for (j = 0; j < m; j++){
              theClosure[i][j] = theClosure[i][j] || (theClosure[i][k] && theClosure[k][j]);
          }
      }
  }
}

void removeNonReachables(int **theClosure, int *statesOfNodes, int m, int n){
  for(int i=0;i<m;i++){
    if(theClosure[0][i]==0){
      statesOfNodes[i] = -1;
    }
  }
}

int fsc(int *statesOfNodes, int m){
  int count = 0;
  for(int i=0;i<m;i++){
    if(statesOfNodes[i] == 1){
      count++;
    }
  }
  return count;
}

int nfsc(int *statesOfNodes, int m){
  int count = 0;
  for(int i=0;i<m;i++){
    if(statesOfNodes[i] == 0){
      count++;
    }
  }
  return count;
}

void makeTwoTables(int *FinalStates, int *NonFinalStates, int *statesOfNodes, int m){
  int index1 = 0, index2 = 0;
  for(int i=0;i<m;i++){
    if(statesOfNodes[i] == 1){
      FinalStates[index1++] = i;
    }else if(statesOfNodes[i] == 0){
      NonFinalStates[index2++] = i;
    }
  }
}

void checkForEqualityInTable(int **transitionMatrix, int n, int *FinalStates, int finalStatesCount, int *statesOfNodes){
  int flag = 0;
  for(int i=0;i<finalStatesCount;i++){
    for(int j=i+1;j<finalStatesCount;j++){
      flag =0 ;
      if(FinalStates[i]!=-1 && FinalStates[j]!=-1){
        for(int k=0;k<n;k++){
          if(transitionMatrix[FinalStates[i]][k] == transitionMatrix[FinalStates[j]][k]){

          }else{
            flag = 1;
            break;
          }
        }
        if(flag == 0){
          statesOfNodes[FinalStates[j]] = -1;
          FinalStates[j] = -1;

        }
      }
    }
  }
}


//main
//+++++++
//This is the driver function of the entire program. It calls mainMinimization function with paramter 1 if DFA has to be minimized. If the acceptance is to be found, then it is called with paramter 2

int main(){
  int choice;
  cout<<"=============================================";
  cout<<"\n\t\t COCA PROJECT";
  cout<<"\n=============================================";
  cout<<"\n\n 1. Minimization of DFA";
  cout<<"\n 2. Acceptance of DFA";
  cout<<"\n 3. EXIT";
  cout<<"\n\nENTER YOUR CHOICE : ";
  cin>>choice;
  switch (choice) {
    case 1:
      mainMinimization(1);
    break;
    case 2:
      mainMinimization(2);
    break;
    case 3:
    exit(0);
    break;
  }
  return 0;
}
