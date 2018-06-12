// Jim Skon, Kenyon College, 2017

#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <string>
#include <iostream>
#include <algorithm> // Needed for transform
#include <fstream>
#include <dirent.h> // Needed for directory search
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>
// Time
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#define XML_USE_STL

void generateSigs(int n);

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

ofstream logFile;
int trans1[] = {0, 1, -1, 2, -2, 3};
int trans2[] = {0, 1, 2, -2, 3};

int main() {
  Cgicc cgi;    // Ajax object    
  
  // Create AJAX objects to recieve information from web page.
  form_iterator lengthIt = cgi.getElement("length");
  string length = **lengthIt;
  int ilength = atoi(length.c_str());
  
  // Debug to file example
  //logFile.open("/tmp/skon.log",ios::out | ios::app);
  //logFile << "Got: " <<  term << ":" << showMatchesStr<< endl;
  //logFile.close();  
  

  // output required message for AJAX
  cout << "Content-Type: text/plain\n\n";
  generateSigs(ilength);


}

void displaySig(vector<int> v) {

  for (int i = 0; i < v.size(); i++) {
    cout << v[i];
  }
  //cout << endl;
}

void displayTuple(vector<int> v) {
  cout << "(";
  for (int i = 0; i < v.size(); i++) {
    cout << v[i];
    if (i<v.size()-1) {
      cout << ',';
    }
  }
  cout << ")<br />" << endl;
}


vector<int> initialSig(int n) {
  vector<int> v;
  for (int i = 0; i < n; i++) {
    v.push_back(0);
  }
  return v;
}

bool nextSig(vector<int> &sig) {
  int l = sig.size() - 1;
  for (int i = l; i >= 0; i--) {
    if (i == l && i > 0) {
      if (sig[i] < 4) {
	sig[i]++;
	return true;
      }
      if (sig[i] == 4) {
	sig[i] = 0;
      }
    } else if (i > 0) {
      if (sig[i] < 5) {
	sig[i]++;
	return true;
      }
      if (sig[i] == 5) {
	sig[i] = 0;
      }
    } else {
      if (sig[l] == 0) {
	sig = initialSig(l + 1);
	sig[0] = 1;
	return false;
      }
    }

  }
  return false;
}

vector<int> sig2Tuple(vector<int> sig) {
  vector<int> tuple;
  int l = sig.size();
  for (int i = 0; i < l; i++) {
    if (i<l-1) {
      tuple.push_back(trans1[sig[i]]);
    } else {
      tuple.push_back(trans2[sig[i]]);
    }
  }
  return tuple;
}

void generateSigs(int n) {
  vector<int> sig = initialSig(n);
  bool more = true;
  //  displaySig(sig);
  displayTuple(sig2Tuple(sig));
  while (more) {
    more = nextSig(sig);
    //displaySig(sig);
    displayTuple(sig2Tuple(sig));
  }
}

