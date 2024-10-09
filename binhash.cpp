#include <string.h>
#include <iostream>
#include "zmorton.hpp"
#include "binhash.hpp"

/*@q
 * ====================================================================
 */

/*@T
 * \subsection{Spatial hashing implementation}
 * 
 * In the current implementation, we assume [[HASH_DIM]] is $2^b$,
 * so that computing a bitwise of an integer with [[HASH_DIM]] extracts
 * the $b$ lowest-order bits.  We could make [[HASH_DIM]] be something
 * other than a power of two, but we would then need to compute an integer
 * modulus or something of that sort.
 * 
 *@c*/

#define HASH_MASK (HASH_DIM-1)

unsigned particle_bucket(particle_t* p, float h)
{
    unsigned ix = p->x[0]/h;
    unsigned iy = p->x[1]/h;
    unsigned iz = p->x[2]/h;
    return zm_encode(ix & HASH_MASK, iy & HASH_MASK, iz & HASH_MASK);
}

unsigned particle_neighborhood(unsigned* buckets, particle_t* p, float h)
{
    unsigned ix = p->x[0]/h;
    unsigned iy = p->x[1]/h;
    unsigned iz = p->x[2]/h;

    int num_neighbors = 0;
    for (int i = -1; i <= 1 ; ++i) 
    { 
	for (int j = -1; j <= 1; ++j) 
	{
	    for (int k = -1; k <= 1; ++k) 
	    {
		unsigned bucket_hash = zm_encode((ix + i) & HASH_MASK,
						 (iy + j) & HASH_MASK,
						 (iz + k) & HASH_MASK);
                buckets[num_neighbors] = bucket_hash;
		num_neighbors++;
             }
         }
    }
    return num_neighbors;    
    
}


void hash_particles(sim_state_t* s, float h)
{
    /* clear the bin pointers */
    for (int i = 0; i < STATE_HASH_SIZE; ++i) {
        s->hash[i] = nullptr; 
    }

    // loop through each particle and add to bin

    for (int i =0; i < s->n; ++i)
    { 
	    particle_t* p = &s->part[i];
        std::cout << "particle at i = " << i << "is " << p << std::endl; 
        unsigned bucket_hash = particle_bucket(p,h);
        std::cout << "bucket hash: " << bucket_hash << std::endl;
	    p->next = s->hash[bucket_hash]; // we don't know why this worked
        std::cout << p->next << std::endl;
	    s->hash[bucket_hash] = p;
        std::cout << " p : " << p << std::endl;
		
    }
    /* END TASK */
}
