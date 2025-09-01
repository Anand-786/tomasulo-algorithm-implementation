#include <iostream>
#include <vector>
#include <cmath>
#include "victimCache.cpp"
using namespace std;

struct CacheLine{
    bool valid;
    bool dirty;
    int tag;
    int lru_counter;
    int cache_line_address;

    CacheLine() : valid(false), dirty(false), tag(0), lru_counter(0), cache_line_address(0) {}
};

struct CacheSet{
    vector<CacheLine*> set;
    int valid_lines;

    CacheSet(int associativity){
        set.resize(associativity);
        valid_lines = 0;
        for(int i=0;i<associativity;i++)
            set[i] = new CacheLine();
    }
};

class Cache{
    private:
        vector<CacheSet*> sets;
        int num_sets;
        int associativity;
        int block_size;
        int address_bits;
        int mem_access_latency;
        int wb_latency;
        int vc_access_latency;
        bool victim_cache_enabled;
        VictimCache *victim_cache;
    public:
        Cache(int ns, int as, int bs, int ab, int mm, int wb, int vc, int vc_s, bool vc_e){
            num_sets = ns;
            associativity = as;
            block_size = bs;
            address_bits = ab;
            mem_access_latency = mm;
            wb_latency = wb;
            vc_access_latency = vc;
            victim_cache_enabled = vc_e;
            victim_cache = new VictimCache(vc_s);
            sets.resize(num_sets);
            for(int i=0;i<num_sets;i++)
                sets[i] = new CacheSet(associativity);
        }

        int getBlockOffsetBits(){
            return (int)ceil(log2(block_size));
        }

        int getSetIndexBits(){
            return (int)ceil(log2(num_sets));
        }

        int getAssociativityBits(){
            return (int)ceil(log2(associativity));
        }

        int getL1Size(){
            //Total Size = Num Sets * Associativity * Block Size;
            //Total Size (int KB) = 2^x KB
            int total_bits = getSetIndexBits() + getAssociativityBits() + getBlockOffsetBits();
            return total_bits - 10;
        }

        int getAddressWithoutBO(int address){
            int address_without_BO = address >> getBlockOffsetBits();
            return address_without_BO;
        }

        pair<int, int> getTagAndSetIndex(int address){
            int block_offset_bits = getBlockOffsetBits();
            int set_index_bits = getSetIndexBits();

            int address_without_BO = address >> block_offset_bits;

            int set_index_mask = (1 << set_index_bits) -1;
            int set_index = (address_without_BO & set_index_mask);
            
            int tag = address >> (block_offset_bits + set_index_bits);

            return {tag, set_index};
        }

        int l1CacheSearchAndUpdate(int current_cycle, int address, bool setDirty){
            pair<int, int> tag_and_set = getTagAndSetIndex(address);
            int tag = tag_and_set.first;
            int set_index = tag_and_set.second;

            CacheSet *search_set = sets[set_index];
            for(auto cache_line : search_set->set){
                if(cache_line->valid && (cache_line->tag == tag)){
                    //HIT
                    //Case 1 : Cache Hit
                    cache_line->lru_counter = current_cycle;
                    cache_line->dirty = setDirty ? true : cache_line->dirty;
                    return 0;
                }
            }

            //MISS
            if(search_set->valid_lines < search_set->set.size()){
                //Case 2 : Miss with Empty Cache Line in set (Compulsory Miss)
                for(auto cache_line : search_set->set){
                    if(!cache_line->valid){
                        cache_line->valid = true;
                        cache_line->lru_counter = current_cycle;
                        cache_line->tag = tag;
                        cache_line->dirty = setDirty;
                        cache_line->cache_line_address = getAddressWithoutBO(address);
                        search_set->valid_lines += 1;
                        return mem_access_latency - 1;
                    }
                }
            }
            //Case 3 : Miss with No Space (Conflict Miss)
            //Replacement Policy : LRU (Least Recently Used)
            CacheLine *evictLine = NULL;
            int least_recently_used_cycle = current_cycle;
            for(auto cache_line : search_set->set){
                if(cache_line->valid){
                    if(cache_line->lru_counter < least_recently_used_cycle){
                        least_recently_used_cycle = cache_line->lru_counter;
                        evictLine = cache_line;
                    }
                }
            }

            bool isEvictLineDirty = evictLine->dirty;

            //Check in Victim Cache
            bool hitInVictimCache;
            if(victim_cache_enabled)
                hitInVictimCache = victim_cache->checkInVictimCache(getAddressWithoutBO(address), evictLine->cache_line_address);

            //Overwrite here, but return Penalty accoridng to Dirty Bit
            evictLine->valid = true;
            evictLine->dirty = setDirty;
            evictLine->lru_counter = current_cycle;
            evictLine->tag = tag;
            evictLine->cache_line_address = getAddressWithoutBO(address);

            if(isEvictLineDirty){
                //Case 3A : Replacement of Dirty Block
                if(victim_cache_enabled){
                    if(hitInVictimCache)
                        return (vc_access_latency + wb_latency - 1);
                    else
                        return (vc_access_latency + mem_access_latency + wb_latency - 1);
                }
                else{
                    return (mem_access_latency + wb_latency - 1);
                }
            }
            //Case 3B : Overwrite Clean Block
            if(victim_cache_enabled){
                if(hitInVictimCache)
                    return (vc_access_latency - 1);
                return (vc_access_latency + mem_access_latency - 1);
            }
            return (mem_access_latency - 1);
        }
};