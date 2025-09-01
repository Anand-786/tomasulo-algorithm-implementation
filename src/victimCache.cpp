#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class VictimCache{
    private:
        list<int> victim_buffer;
        unordered_map<int, list<int>::iterator> lookup_map;
        int size;

    public:
        VictimCache(int maxSize){
            size = maxSize;
        }

        void handleVictimCacheMiss(int evictedLine){
            if(victim_buffer.size() == size){
                int oldest_line = victim_buffer.front();
                lookup_map.erase(oldest_line);
                victim_buffer.pop_front();
            }
            //We have space in Victim Cache
            victim_buffer.push_back(evictedLine);
            lookup_map[evictedLine] = prev(victim_buffer.end());
        }

        void handleVictimCacheHit(int requestedLine, int evictedLine){
            auto buffer_it = lookup_map[requestedLine];
            *buffer_it = evictedLine;
            victim_buffer.splice(victim_buffer.end(), victim_buffer, buffer_it);
            lookup_map.erase(requestedLine);
            lookup_map[evictedLine] = prev(victim_buffer.end());
        }

        bool checkInVictimCache(int requestedLine, int evictedLine){
            if(lookup_map.find(requestedLine) != lookup_map.end()){
                handleVictimCacheHit(requestedLine, evictedLine);
                return true;
            }
            handleVictimCacheMiss(evictedLine);
            return false;
        }
};