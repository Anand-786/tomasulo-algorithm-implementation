#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct CacheLine{
    bool valid;
    bool dirty;
    int tag;
    int lru_counter;

    CacheLine() : valid(false), dirty(false), tag(0), lru_counter(0) {}
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
        int miss_penalty_no_replacement;
        int miss_penalty_with_replacement;
    public:
        Cache(int ns, int as, int bs, int ab, int mp_no_rp, int mp_w_rp){
            num_sets = ns;
            associativity = as;
            block_size = bs;
            address_bits = ab;
            miss_penalty_no_replacement = mp_no_rp;
            miss_penalty_with_replacement = mp_w_rp;
            sets.resize(num_sets);
            for(int i=0;i<num_sets;i++)
                sets[i] = new CacheSet(associativity);
        }

        pair<int, int> getTagAndSetIndex(int address){
            int block_offset_bits = (int)ceil(log2(block_size));
            int set_index_bits = (int)ceil(log2(num_sets));

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
                        search_set->valid_lines += 1;
                        return miss_penalty_no_replacement;
                    }
                }
            }
            else{
                //Case 3 : Miss with No Space (Conflict Miss)
                //Replacement Policy : LRU (Least Recently USed)
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

                //Overwrite here, but return Penalty accoridng to Dirty Bit
                evictLine->valid = true;
                evictLine->dirty = setDirty;
                evictLine->lru_counter = current_cycle;
                evictLine->tag = tag;

                if(isEvictLineDirty){
                    //Case 3A : Replacement of Dirty Block
                    return miss_penalty_with_replacement;
                }
                else{
                    //Case 3B : Overwrite Clean Block
                    return miss_penalty_no_replacement;
                }
            }
        }
};