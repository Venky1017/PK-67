#include <stdio.h>
#include <stdint.h>
#include <openssl/ripemd.h>
#include <omp.h>
#include <string.h>

// Target hash in hex
unsigned char target_hash[RIPEMD160_DIGEST_LENGTH] = {
    0x73, 0x94, 0x37, 0xbb, 0x3d, 0xd6, 0xd1, 0x98, 0x3e, 0x66,
    0x62, 0x9c, 0x5f, 0x08, 0xc7, 0x0e, 0x52, 0x76, 0x93, 0x71
};

// Function to apply RIPEMD-160 hashing
void apply_ripemd160(const char* key, unsigned char* output) {
    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);
    RIPEMD160_Update(&ctx, (unsigned char*)key, strlen(key));
    RIPEMD160_Final(output, &ctx);
}

// Function to check if a candidate key matches the target hash
int is_match(unsigned char* candidate_hash) {
    return memcmp(candidate_hash, target_hash, RIPEMD160_DIGEST_LENGTH) == 0;
}

int main() {
    uint64_t start_range = 0x40000000000000000;
    uint64_t end_range = 0x7ffffffffffffffff;
    unsigned char candidate_hash[RIPEMD160_DIGEST_LENGTH];

    // Number of threads for parallelization
    int num_threads = 4;

    #pragma omp parallel for num_threads(num_threads) private(candidate_hash)
    for (uint64_t i = start_range; i <= end_range; i++) {
        char candidate_key[32];  // Storing the hex representation of the key
        snprintf(candidate_key, sizeof(candidate_key), "%lx", i);  // Convert to hex string

        // Apply the RIPEMD-160 hash to the candidate private key
        apply_ripemd160(candidate_key, candidate_hash);

        // Check if the generated hash matches the target
        if (is_match(candidate_hash)) {
            #pragma omp critical
            {
                printf("Found private key: %s\n", candidate_key);
            }
            break;  // Exit the loop if the key is found
        }

        // Print the search range progress periodically
        if (i % (end_range - start_range) / 10 == 0) {
            #pragma omp critical
            {
                printf("Searching range: %lx - %lx\n", i, end_range);
            }
        }
    }

    return 0;
}
