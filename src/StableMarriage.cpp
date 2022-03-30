#include <Rcpp.h>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

using namespace std; // Stops use having to put std:: before a lot of things (some reason bugs out when doing function definitions)
using namespace Rcpp;

 

// Helper function to convert the vector of tuples to vector of vectors
auto tuple2vector( vector<tuple <string, string> > input) {
  vector <vector <string>  > out;
  for (auto& a : input) {
    out.push_back(vector <string> {get<0>(a), get<1>(a)});
  }
  return wrap(out);
}

// Helper function to convert a dataframe to an unordered_map
auto df2um(DataFrame df) {
  
  // Get keys from dataframe
  CharacterVector keys = df.names();
  
  // Define output
  unordered_map<string, vector<string> > out;
  
  // Loop over df columns
  for (int i=0; i<keys.size(); i++) {
    
    // Add key/values to unordered_map
    string key = as<string>(keys[i]);
    
    vector <string> values = df[key];
    
    out[key] = values;
  }
  return out;
}

// Ensure we are working with c++17 as some functions in the fundamental algorithm require this.
// [[Rcpp::plugins(cpp17)]]  
// [[Rcpp::export]]
List StableMarriageAlgo(DataFrame prefA_R, DataFrame prefB_R)
{
    // Check preference tables are same length, if not return error
    if (prefA_R.nrows() != prefB_R.nrows()  || prefA_R.size() != prefB_R.size()) {
      return wrap(vector<string> {"Incompatable Preference Tables, need to be of equal shape and size."});
    }
    
    // Convert from R list to unordered map
    auto prefA = df2um(prefA_R);
    auto prefB = df2um(prefB_R);
    
    // Initialisation
    int k = 0;
    int n = prefA.size();

    /*
    * Variables that are needing to be predefined
    */
    vector<string> :: iterator itr;  // This is a general variable used later on to recover index positions in vectors
    vector<string> prefs;
    vector<tuple <string, string> > tempMatchings; // Store temp matchings as the function output type
    string womCurrMatch;

    // Create an empty list to store male suitors
    vector<string> men;
    men.reserve(prefA.size());

    // Add undersirable man to the fold and temp engage to women
    for (auto& [key, v] : prefB) {
        v.push_back("undesirable");
        tempMatchings.push_back(tuple <string, string>("undesirable", key));
    }

    // Assign Male keys to a vector to allow easier iteration
    for(auto kv : prefA) {
        men.push_back(kv.first);
    }
    // Add undesirable man to the male preference list to avoid some errors of empty vectors we get later
    prefA["undesirable"] = {};
    men.push_back("undesirable");

    while (k<n) {
        checkUserInterrupt();
        // Assign next man in list as the suitor
        string X = men[k];

        // Get that mans preference list
        prefs = prefA[X];

        // While the suitor isn't the undesirable man
        while (X != "undesirable") {
            // Find best remaining choice on suitors list
            string x = prefs[0];

            // Get index of suitor (X) and fiance from womans (x) list
            for (auto& [m,w] : tempMatchings) {
                if (w==x) {
                    womCurrMatch = m;
                }
            }

            // Find new mans index in womans list
            itr = find(prefB[x].begin(), prefB[x].end(), womCurrMatch);
            int womCurrMatchIndex = distance(prefB[x].begin(), itr);

            // Find index of suitor
            itr = find(prefB[x].begin(), prefB[x].end(), X);
            int womSuitorIdx = distance(prefB[x].begin(), itr);

            // Compare positions
            if (womSuitorIdx < womCurrMatchIndex) {
                // call off womans (x) current marriage and match suitor (X) and woman
                auto it = remove_if(tempMatchings.begin(), tempMatchings.end(), [&](auto & tup){ return (get<0>(tup) == womCurrMatch) && (get<1>(tup) == x); });
                tempMatchings.erase(it, tempMatchings.end()); // resize the vector to only hold live elements

                tempMatchings.push_back(tuple <string, string>(X, x));

                // new suitor is abandoned man
                X = womCurrMatch;

                // Generate the new preference list
                prefs = prefA[X];
            }
            if (X != "undesirable") {
                // Remove x from suitors list
                prefs.erase(remove(prefs.begin(), prefs.end(), x), prefs.end());
            }
        }
        k+=1;
    }
    
    // Return sorted matchings
    sort(tempMatchings.begin(), tempMatchings.end());
    
    // Convert to an R friendly format as R doesn't like tuples
    return tuple2vector(tempMatchings);
}
