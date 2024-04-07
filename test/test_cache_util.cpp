#include "cache_util.h"
#include "gtest/gtest.h"

class CacheUtilTest : public ::testing::Test
{
  protected:
    CacheParameters params;

    void SetUp() override
    {
        params.bytes_per_block = 16;
        params.sets = 4;
        params.blocks = 2;
        params.allocate_on_write = true;
        params.writeType = WriteType::WriteBack;
    }
};

TEST_F(CacheUtilTest, GetBlockNum)
{
    CacheUtil cacheUtil(params);

    EXPECT_EQ(0, cacheUtil.get_block_num(0));
    EXPECT_EQ(0, cacheUtil.get_block_num(15));
    EXPECT_EQ(1, cacheUtil.get_block_num(16));
    EXPECT_EQ(3, cacheUtil.get_block_num(63));
}

TEST_F(CacheUtilTest, GetSetNum)
{
    CacheUtil cacheUtil(params);

    // Test for different block numbers within the same set
    EXPECT_EQ(0, cacheUtil.get_set_num(0));
    EXPECT_EQ(0, cacheUtil.get_set_num(8));

    // Test for block numbers overflowing to the next set
    EXPECT_EQ(1, cacheUtil.get_set_num(16));
    EXPECT_EQ(3, cacheUtil.get_set_num(48));
}

TEST_F(CacheUtilTest, GetMemDelay)
{
    CacheUtil cacheUtil(params);

    // Calculate expected memory delay based on parameters
    int expected_delay = MEM_DELAY * (params.bytes_per_block / NUM_TRANSFER_BYTES);
    EXPECT_EQ(expected_delay, cacheUtil.get_mem_delay());
}

TEST_F(CacheUtilTest, IsFull)
{
    CacheUtil cacheUtil(params);

    // Create a set with all blocks filled
    std::set<int> full_set;
    for (int i = 0; i < params.blocks; ++i)
    {
        full_set.insert(i);
    }

    EXPECT_TRUE(cacheUtil.is_full(full_set));

    // Create a set with one block empty
    std::set<int> partial_set;
    partial_set.insert(0);

    EXPECT_FALSE(cacheUtil.is_full(partial_set));
}

TEST_F(CacheUtilTest, AllocOnWrite)
{
    CacheUtil cacheUtil(params);

    EXPECT_EQ(params.allocate_on_write, cacheUtil.alloc_on_write());
}

TEST_F(CacheUtilTest, GetWriteType)
{
    CacheUtil cacheUtil(params);

    EXPECT_EQ(params.writeType, cacheUtil.get_write_type());
}
