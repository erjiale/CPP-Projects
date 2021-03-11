#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class EquivalenceTable {
    private:
        std::unordered_map<int, std::unordered_set<int>> equiv_table;

    public:
        EquivalenceTable() { 
            equiv_table = std::unordered_map<int, std::unordered_set<int>> (); 
        }

        int getNumOfGroups() { return equiv_table.size(); }

        void setupEquivalence(int label1, int label2) {
            int smaller = label1 < label2 ? label1 : label2;
            int larger = label1 > label2 ? label1 : label2;
            
            if(equiv_table.count(smaller)) {
                if(equiv_table[smaller].count(larger)) return;

                int groupLabel = findLabel(larger);
                if(groupLabel > smaller) {
                    equiv_table[smaller].insert(equiv_table[groupLabel].begin(), equiv_table[groupLabel].end());
                    equiv_table.erase(groupLabel);
                }
                else {
                    equiv_table[groupLabel].insert(equiv_table[smaller].begin(), equiv_table[smaller].end());
                    equiv_table.erase(smaller);
                }
            }
            else {
                for(auto itr : equiv_table) {
                    if(itr.second.count(smaller)) {
                        if(equiv_table.count(larger)) {
                            equiv_table[itr.first].insert(equiv_table[larger].begin(), equiv_table[larger].end());
                            equiv_table.erase(larger);
                        }
                        else {
                            int groupLabel = findLabel(larger);
                            equiv_table[itr.first].insert(equiv_table[groupLabel].begin(), equiv_table[groupLabel].end());
                            if(itr.first != groupLabel) equiv_table.erase(groupLabel);
                        }
                        return;
                    }
                }
            }
        }

        void createKey(int key) {
            equiv_table[key] = std::unordered_set<int> ( {key} );
        }

        int findLabel(int label) {
            for(auto it : equiv_table)
                if(it.second.count(label)) return it.first;
            return -1;
        }

        void printMap() {
            std::cout<< "\n";
            for(auto p : equiv_table) {
                std::cout << "key: [" << p.first << "]\t";
                std::cout << "val: [";
                for(auto set : p.second) {
                    std::cout << set << ", ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }

};