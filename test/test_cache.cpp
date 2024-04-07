#include "cache.h"
#include "data.h"
#include <gtest/gtest.h>

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
    // TODO Check LRU
    Cache cache(params);
    // Add your test logic here
    for (int i = 0; i < 8; i++)
    {
        cache.proc_request({Load, i});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
    // Write Back with non dirty blocks
    ASSERT_EQ(cache.get_statistics().cycles,);
    for (int i = 1; i < 8; i++)
    {
        cache.proc_request({Load, i * 8 * 8});
    }
    ASSERT_EQ(cache.get_statistics().load_hits, 7);
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
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteThrough_LRU)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = true,
                           .writeType = WriteType::WriteThrough,
                           .evict = EvictionType::LRU};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteThrough_FIFO)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = true,
                           .writeType = WriteType::WriteThrough,
                           .evict = EvictionType::FIFO};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteBack_NoAllocate_LRU)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = false,
                           .writeType = WriteType::WriteBack,
                           .evict = EvictionType::LRU};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteBack_NoAllocate_FIFO)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = false,
                           .writeType = WriteType::WriteBack,
                           .evict = EvictionType::FIFO};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteThrough_NoAllocate_LRU)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = false,
                           .writeType = WriteType::WriteThrough,
                           .evict = EvictionType::LRU};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}

TEST_F(CacheTest, WriteThrough_NoAllocate_FIFO)
{
    CacheParameters params{.sets = 64,
                           .blocks = 8,
                           .bytes_per_block = 64,
                           .allocate_on_write = false,
                           .writeType = WriteType::WriteThrough,
                           .evict = EvictionType::FIFO};
    Cache cache(params);
    // Add your test logic here
    ASSERT_EQ(false, true);
}
