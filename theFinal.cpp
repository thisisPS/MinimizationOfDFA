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
int acceptanceNumber();

//End of function declarations

//Minimization of DFA
//mainMinimization function definition

void mainMinimization(int theIP){
  int n,m;

  n = noOfAlphabets();
  m = noOfNodes();

  int statesOfNodes[m] = {0};
  string namesOfNodes[m];
  string namesOfAlphabets[n];

  storeNodeNames(namesOfNodes, m, n);
  storeAlphabetNames(namesOfAlphabets, n);
  storeStateData(statesOfNodes, m, n);

  int **transitionMatrix = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       transitionMatrix[r] = (int *)malloc(n * sizeof(int));

  storeTransition(transitionMatrix, m, n);



  if(theIP == 2){
    int currentState = 0, flag = 0, value = 0;
    int numberOfInput = acceptanceNumber();
    string theAcceptanceInput[numberOfInput];

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

  int **theGraph = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       theGraph[r] = (int *)malloc(m * sizeof(int));

  int **theClosure = (int **)malloc(m * sizeof(int *));
  for (int r = 0; r < m; r++)
       theClosure[r] = (int *)malloc(m * sizeof(int));


  graphMatrix(theGraph, transitionMatrix, m, n);
  closureMatrix(theClosure, theGraph, m);
  removeNonReachables(theClosure, statesOfNodes, m, n);
  int finalStatesCount = fsc(statesOfNodes, m);
  int nonFinalStatesCount = nfsc(statesOfNodes, m);

  int FinalStates[finalStatesCount];
  int NonFinalStates[nonFinalStatesCount];

  makeTwoTables(FinalStates, NonFinalStates, statesOfNodes, m);
  checkForEqualityInTable(transitionMatrix, n, FinalStates, finalStatesCount, statesOfNodes);
  checkForEqualityInTable(transitionMatrix, n, NonFinalStates, nonFinalStatesCount, statesOfNodes);

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
