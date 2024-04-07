#include "cache.h" // Include your eviction handler header files
#include "gtest/gtest.h"

// Test fixture for EvictionHandler
class EvictionHandlerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() override
    {
        // Create eviction handlers for testing
        lru_handler = std::make_unique<LRUEvictionHandler>(2); // 2 sets
        fifo_handler = std::make_unique<FIFOEvictionHandler>(2);
    }

    std::unique_ptr<LRUEvictionHandler> lru_handler;
    std::unique_ptr<FIFOEvictionHandler> fifo_handler;
};

TEST_F(EvictionHandlerTest, LRUEvictionHandler_getSize_Empty)
{
    EXPECT_EQ(lru_handler->get_size(0), 0);
    EXPECT_EQ(lru_handler->get_size(1), 0);
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_getSize_AfterInsert)
{
    lru_handler->insert(0, 10);
    EXPECT_EQ(lru_handler->get_size(0), 1);
    lru_handler->insert(1, 20);
    EXPECT_EQ(lru_handler->get_size(1), 1);
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_getToEvict_Empty)
{
    EXPECT_THROW(lru_handler->get_to_evict(0), std::runtime_error);
    EXPECT_THROW(lru_handler->get_to_evict(1), std::runtime_error);
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_getToEvict_AfterInsert)
{
    lru_handler->insert(0, 10);
    EXPECT_EQ(lru_handler->get_to_evict(0), 10);
    lru_handler->insert(0, 20);
    EXPECT_EQ(lru_handler->get_to_evict(0), 10); // LRU element (10) should be evicted
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_insert)
{
    lru_handler->insert(0, 10);
    lru_handler->insert(1, 20);
    // No need to verify return value (void)
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_erase)
{
    lru_handler->insert(0, 10);
    lru_handler->erase(0);
    EXPECT_EQ(lru_handler->get_size(0), 0);
}

TEST_F(EvictionHandlerTest, LRUEvictionHandler_touch)
{
    lru_handler->insert(0, 10);
    lru_handler->insert(0, 20);
    lru_handler->touch(0, 10);
    EXPECT_EQ(lru_handler->get_to_evict(0), 20); // 10 becomes most recently used
}

TEST_F(EvictionHandlerTest, FIFOEvictionHandler_getSize_Empty)
{
    EXPECT_EQ(fifo_handler->get_size(0), 0);
    EXPECT_EQ(fifo_handler->get_size(1), 0);
}

TEST_F(EvictionHandlerTest, FIFOEvictionHandler_getSize_AfterInsert)
{
    fifo_handler->insert(0, 10);
    EXPECT_EQ(fifo_handler->get_size(0), 1);
    fifo_handler->insert(1, 20);
    EXPECT_EQ(fifo_handler->get_size(1), 1);
}

TEST_F(EvictionHandlerTest, FIFOEvictionHandler_getToEvict_Empty)
{
    EXPECT_THROW(fifo_handler->get_to_evict(0), std::runtime_error);
    EXPECT_THROW(fifo_handler->get_to_evict(1), std::runtime_error);
}
