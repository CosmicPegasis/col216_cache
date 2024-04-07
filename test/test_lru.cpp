#include "lru.h" // Include your LRUCache header
#include "gtest/gtest.h"

TEST(LRUCacheTest, get_lru_empty_cache)
{
    LRUCache cache;
    EXPECT_THROW(cache.get_lru(), std::runtime_error);
}

TEST(LRUCacheTest, get_lru_returns_lru_element)
{
    LRUCache cache;
    cache.insert(10);
    cache.insert(20);
    EXPECT_EQ(cache.get_lru(), 10); // 10 should now be the LRU element
}

TEST(LRUCacheTest, insert_updates_lru)
{
    LRUCache cache;
    cache.insert(10);
    cache.insert(20);
    cache.insert(10); // Re-inserting 10 should make it the most recently used
    EXPECT_EQ(cache.get_lru(), 20);
}

TEST(LRUCacheTest, get_size_returns_correct_size)
{
    LRUCache cache;
    EXPECT_EQ(cache.get_size(), 0);
    cache.insert(10);
    EXPECT_EQ(cache.get_size(), 1);
    cache.insert(20);
    EXPECT_EQ(cache.get_size(), 2);
}

TEST(LRUCacheTest, erase_removes_lru_element)
{
    LRUCache cache;
    cache.insert(10);
    cache.insert(20);
    cache.erase();
    EXPECT_EQ(cache.get_size(), 1);
    EXPECT_EQ(cache.get_lru(), 20);
}
