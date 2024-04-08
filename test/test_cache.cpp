#include "cache.h"
#include "data.h"
#include <gtest/gtest.h>
void print_set(std::set<int> &s)
{
    for (auto &elem : s)
    {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

class CacheTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Set up any common test setup here
    }

    void TearDown() override
    {
        // Clean up any resources used in the tests
    }
};

TEST_F(CacheTest, WriteBack_LRU)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = true,
                           .writeType = WriteType::WriteBack,
                           .evict = EvictionType::LRU};
    Cache cache(params);

    // Check load miss
    for (int i = 0; i < 8; i++)
    {
        cache.proc_request({Load, i});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
    ASSERT_EQ(cache.get_statistics().cycles, ((MEM_DELAY * 16) + 2) + 7);
    // Check load miss
    int prev_cycles = ((MEM_DELAY * 16) + 2 + 7);
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Load, i * 8 * 8});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 7 * ((MEM_DELAY * 16) + 2));

    // Check load-hits
    cache.proc_request({Load, 7 * 8 * 8});
    ASSERT_EQ(cache.get_statistics().load_hits, 8);

    // Check eviction policy
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Load, i * 64 * 64});
    }
    cache.proc_request({Load, 0});
    ASSERT_EQ(cache.get_statistics().load_hits, 9);
    cache.proc_request({Load, 2 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 10);
    cache.proc_request({Load, 9 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 10);
    cache.proc_request({Load, 1 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 10);

    cache = Cache{params};
    // check save hit
    cache.proc_request({Save, 0});
    ASSERT_EQ(cache.get_statistics().cycles, 2 + MEM_DELAY * 16);
    prev_cycles = cache.get_statistics().cycles;
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Save, i});
    }
    ASSERT_EQ(cache.get_statistics().cycles, 2 * 7 + prev_cycles);
    prev_cycles = cache.get_statistics().cycles;
    // TODO check dirty bit with load
    // All of these should be non dirty non clean misses
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Save, i * 64 * 64});
    };
    ASSERT_EQ(cache.get_statistics().cycles, 7 * (MEM_DELAY * 16 + 2) + prev_cycles);
    prev_cycles = cache.get_statistics().cycles;
    // Check set full and dirty bit
    cache.proc_request({Save, 9 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 * MEM_DELAY * 16 + 2);

    // Check only set full
    cache = Cache{params};

    for (int i = 0; i < 8; i++)
    {
        cache.proc_request({Load, 64 * 64 * i});
    }
    prev_cycles = cache.get_statistics().cycles;
    cache.proc_request({Save, 64 * 64 * 9});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 + (1 * MEM_DELAY * 16));

    // check only dirty bit
    cache = Cache{params};
    for (int i = 0; i < 5; i++)
    {
        cache.proc_request({Save, 64 * 64 * i});
    }
    prev_cycles = cache.get_statistics().cycles;
    cache.proc_request({Save, 64 * 64 * 5});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 + MEM_DELAY * 16);
}

TEST_F(CacheTest, WriteBack_FIFO)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = true,
                           .writeType = WriteType::WriteBack,
                           .evict = EvictionType::FIFO};
    Cache cache(params);

    // Check load miss
    for (int i = 0; i < 8; i++)
    {
        cache.proc_request({Load, i});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
    ASSERT_EQ(cache.get_statistics().cycles, ((MEM_DELAY * 16) + 2) + 7);
    // Check load miss
    int prev_cycles = ((MEM_DELAY * 16) + 2 + 7);
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Load, i * 8 * 8});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 7 * ((MEM_DELAY * 16) + 2));

    // Check load-hits
    cache.proc_request({Load, 7 * 8 * 8});
    ASSERT_EQ(cache.get_statistics().load_hits, 8);

    // Check eviction policy
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Load, i * 64 * 64});
    }
    cache.proc_request({Load, 0});
    ASSERT_EQ(cache.get_statistics().load_hits, 9);
    cache.proc_request({Load, 2 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 10);
    cache.proc_request({Load, 9 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 10);
    cache.proc_request({Load, 1 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 11);
    cache.proc_request({Load, 0 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().load_hits, 11);

    cache = Cache{params};
    // check save hit
    cache.proc_request({Save, 0});
    ASSERT_EQ(cache.get_statistics().cycles, 2 + MEM_DELAY * 16);
    prev_cycles = cache.get_statistics().cycles;
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Save, i});
    }
    ASSERT_EQ(cache.get_statistics().cycles, 2 * 7 + prev_cycles);
    prev_cycles = cache.get_statistics().cycles;
    // TODO check dirty bit with load
    // All of these should be non dirty non clean misses
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Save, i * 64 * 64});
    };
    ASSERT_EQ(cache.get_statistics().cycles, 7 * (MEM_DELAY * 16 + 2) + prev_cycles);
    prev_cycles = cache.get_statistics().cycles;
    // Check set full and dirty bit
    cache.proc_request({Save, 9 * 64 * 64});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 * MEM_DELAY * 16 + 2);

    // Check only set full
    cache = Cache{params};

    for (int i = 0; i < 8; i++)
    {
        cache.proc_request({Load, 64 * 64 * i});
    }
    prev_cycles = cache.get_statistics().cycles;
    cache.proc_request({Save, 64 * 64 * 9});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 + (1 * MEM_DELAY * 16));

    // check only dirty bit
    cache = Cache{params};
    for (int i = 0; i < 5; i++)
    {
        cache.proc_request({Save, 64 * 64 * i});
    }
    prev_cycles = cache.get_statistics().cycles;
    cache.proc_request({Save, 64 * 64 * 5});
    ASSERT_EQ(cache.get_statistics().cycles, prev_cycles + 2 + MEM_DELAY * 16);
}

/* TEST_F(CacheTest, WriteThrough_LRU) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = true, */
/*                            .writeType = WriteType::WriteThrough, */
/*                            .evict = EvictionType::LRU}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */

/* TEST_F(CacheTest, WriteThrough_FIFO) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = true, */
/*                            .writeType = WriteType::WriteThrough, */
/*                            .evict = EvictionType::FIFO}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */

/* TEST_F(CacheTest, WriteBack_NoAllocate_LRU) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = false, */
/*                            .writeType = WriteType::WriteBack, */
/*                            .evict = EvictionType::LRU}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */

/* TEST_F(CacheTest, WriteBack_NoAllocate_FIFO) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = false, */
/*                            .writeType = WriteType::WriteBack, */
/*                            .evict = EvictionType::FIFO}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */

/* TEST_F(CacheTest, WriteThrough_NoAllocate_LRU) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = false, */
/*                            .writeType = WriteType::WriteThrough, */
/*                            .evict = EvictionType::LRU}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */

/* TEST_F(CacheTest, WriteThrough_NoAllocate_FIFO) */
/* { */
/*     CacheParameters params{.sets = 64, */
/*                            .blocks = 8, */
/*                            .bytes_per_block = 64, */
/*                            .allocate_on_write = false, */
/*                            .writeType = WriteType::WriteThrough, */
/*                            .evict = EvictionType::FIFO}; */
/*     Cache cache(params); */
/*     // Add your test logic here */
/*     ASSERT_EQ(false, true); */
/* } */
