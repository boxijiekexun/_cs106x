#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */

//
int FindminTimes(Vector<Patient>& p){
    int times = INT_MAX;
    if(p.isEmpty()){
        return 0;
    }
    for(Patient p1 : p){
        times = min(times,p1.hoursNeeded);
    }
    return times;
}

// int COUNT = 0;
bool patientsHelper(Vector<Doctor>& doctors,Vector<Patient>& patients,
                    Map<string, Set<string>>& schedule,int index,Vector<bool>& used){
    //base case
    bool base = true;
    for(bool all: used){
        if(all == false){
            base = false;
        }
    }
    if(index < doctors.size() && doctors[index].hoursFree < FindminTimes(patients)){
        return patientsHelper(doctors,patients,schedule,index + 1,used);
    }
    if(base == true){
        return true;
    }

    else{
        for(int j = 0;j < patients.size();j++){
            Patient p = patients[j];
            if(doctors[index].hoursFree >= p.hoursNeeded && used[j] == false){
                doctors[index].hoursFree -= p.hoursNeeded;
                used[j] = true;
                //cout << patients.size() << endl;
                // cout << "count : " << COUNT++ << " doctor is " << doctors[index].name << " patients is " << p.name << endl;
                schedule[doctors[index].name].add(p.name);
                bool res = patientsHelper(doctors,patients,schedule,index,used);
                // cout << "reback" << doctors[index].name << " " << p.name << endl;
                if(res == true){
                    return true;
                }
                used[j] = false;
                schedule[doctors[index].name].remove(p.name);
                doctors[index].hoursFree += p.hoursNeeded;
            }
        }
    }
    return false;
}

bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
    // cout << doctors << endl;
    // cout << patients << endl;
    if( patients.size() == 0){
        return true;
    }
    Set<string> chosen;
    Vector<Doctor> d = doctors;
    Vector<Patient> p = patients;
    Doctor doctor = doctors[0];
    Vector<bool> used(patients.size(),false);
    bool  res = patientsHelper(d,p,schedule,0,used);
    // cout << schedule.size() << endl;
    // cout << schedule << endl;
    if(res == true){
        return true;
    }
    return false;
}

/* * * * Disaster Planning * * * */

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */


bool DisasterHelper(const Map<string, Set<string>>& roadNetwork,
                            int numCities,Set<string>& locations,
                    Map<string,bool>& used,Set<Set<string>>& res){
    bool base = true;
    for(string road:used){
        if(used[road] == false){
            base = false;
        }
    }
    if(base == true && numCities >= 0){
        return true;
    }
    if(numCities <= 0 && base == false){
        return false;
    }
    // 关键改进：先获取所有未使用道路的副本，在副本上迭代
    Vector<string> unusedRoads;
    for (const string& road : roadNetwork) {
        if (!used[road]) {
            unusedRoads.add(road);
        }
    }

    for(string road: unusedRoads){
        if(used[road]) continue;
        if(used[road] == false){
            // chosen
            // cover
            locations.add(road);
            used[road] = true;
            for(string r: roadNetwork[road]){
                used[r] = true;
            }

            // explore
            bool ans = DisasterHelper(roadNetwork,numCities - 1,locations,used,res);
            if(ans == true){
                return true;
            }
            for(string r: roadNetwork[road]){
                used[r] = false;
            }
            locations.remove(road);
            used[road] = false;


            ans = DisasterHelper(roadNetwork,numCities,locations,used,res);
            if(ans == true){
                return true;
            }
        }
    }
    return false;
}


bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
    // [TODO: Delete these lines and implement this function!]
    (void)(roadNetwork, numCities, locations);
    Map<string,bool> used;
    for(string road : roadNetwork){
        used[road] = false;
    }
    Set<Set<string>> res;
    bool ans = DisasterHelper(roadNetwork,numCities,locations,used,res);
    cout << locations << endl;

    // if(res.size() > 0){
    //     for(Set<string> s: res){
    //         if(s.size() < locations.size()){
    //             locations = s;
    //         }
    //     }
    //     return true;
    // }
    // cout << res << endl;
    if(ans == true){
        return true;
    }
    return false;
}


/* * * * Winning the Election * * * */

/**
 * Given a list of the states in the election, including their popular and electoral vote
 * totals, and the number of electoral votes needed, as well as the index of the lowest-indexed
 * state to consider, returns information about how few popular votes you'd need in order to
 * win that at least that many electoral votes.
 *
 * @param electoralVotesNeeded the minimum number of electoral votes needed
 * @param states All the states in the election (plus DC, if appropriate)
 * @param minStateIndex the lowest index in the states Vector that should be considered
 * if you want to win one state
 * you need most popular votes
 * and than you can win all state's elecotor votes
 * if you want to win
 * you need to get more elector's votes
 */

int getVotes(const Vector<State>& states,int index){
    int res = 0;
    for(int i = index;i < states.size();i++){
        res += states[i].electoralVotes;
    }
    return res;
}

void AtLeastHelper(const Vector<State>& states,int needVotes,int index,int eleVotes,
                     int hadVotes,MinInfo& chosen,Vector<MinInfo>& res){
    if(hadVotes >= needVotes){
        //base case
        res.add(chosen);
    }
    else if(eleVotes < needVotes - hadVotes){
        index = states.size();
        return;
    }
    else{
        for(int i = index;i < states.size();i++){
            //chosen
            eleVotes -= states[i].electoralVotes;
            hadVotes += states[i].electoralVotes;
            chosen.popularVotesNeeded += states[i].popularVotes / 2 + 1;
            chosen.statesUsed.add(states[i]);
            AtLeastHelper(states,needVotes,i + 1,eleVotes,hadVotes,chosen,res);
            eleVotes += states[i].electoralVotes;
            hadVotes -= states[i].electoralVotes;
            chosen.popularVotesNeeded -= states[i].popularVotes / 2 + 1;
            chosen.statesUsed.remove(chosen.statesUsed.size() - 1);
        }
    }
}

MinInfo minPopularVoteToGetAtLeast(int electoralVotesNeeded, const Vector<State>& states, int minStateIndex) {
    // [TODO: Delete these lines and implement this function!]
    (void)(electoralVotesNeeded);
    (void)(states);
    (void)(minStateIndex);
    MinInfo chosen;
    Vector<MinInfo> res;
    chosen.popularVotesNeeded = 0;
    chosen.statesUsed.clear();
    int elevotes = getVotes(states,minStateIndex);
    AtLeastHelper(states,electoralVotesNeeded,minStateIndex,elevotes,0,chosen,res);
    chosen.popularVotesNeeded = INT_MAX;
    cout << "res is " << res.size() << endl;
    for(MinInfo s: res){
        if(chosen.popularVotesNeeded > s.popularVotesNeeded){
            chosen = s;
            cout << chosen.popularVotesNeeded << " ";
        }
    }
    return chosen;
}

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
int getAllVotes(Vector<State>& states){
    int res = 0;
    for(State state: states){
        res += state.electoralVotes;
    }
    return res;
}


void VoteToWinHelper(const Vector<State>& states,int needVotes,int index,int eleVotes,
                     int hadVotes,MinInfo& chosen,Vector<MinInfo>& res){
    if(hadVotes >= needVotes){
        //base case
        res.add(chosen);
    }
    else if(eleVotes < needVotes - hadVotes){
        index = states.size();
        return;
    }
    else{
        for(int i = index;i < states.size();i++){
            //chosen
            eleVotes -= states[i].electoralVotes;
            hadVotes += states[i].electoralVotes;
            chosen.popularVotesNeeded += states[i].popularVotes / 2 + 1;
            chosen.statesUsed.add(states[i]);
            VoteToWinHelper(states,needVotes,i + 1,eleVotes,hadVotes,chosen,res);
            eleVotes += states[i].electoralVotes;
            hadVotes -= states[i].electoralVotes;
            chosen.popularVotesNeeded -= states[i].popularVotes / 2 + 1;
            chosen.statesUsed.remove(chosen.statesUsed.size() - 1);
        }
    }
}

MinInfo minPopularVoteToWin(const Vector<State>& states) {
    // [TODO: Delete these lines and implement this function!]
    Vector<State> state = states;
    int eleVotes = getAllVotes(state);
    int needVotes = eleVotes / 2 + 1;
    Vector<MinInfo> res;
    MinInfo chosen;
    res.clear();
    chosen.popularVotesNeeded = 0;
    chosen.statesUsed.clear();
    VoteToWinHelper(states,needVotes,0,eleVotes,0,chosen,res);
    chosen.popularVotesNeeded = INT_MAX;
    cout << "res is " << res.size() << endl;
    for(MinInfo s: res){
        if(chosen.popularVotesNeeded > s.popularVotesNeeded){
            chosen = s;
            cout << chosen.popularVotesNeeded << " ";
        }
    }
    return chosen;
}
