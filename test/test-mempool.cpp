#include "gtest/gtest.h"
#include "MemPool.h"

class MemPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        startAddress = new uint8_t[1024];
        memPool = new MemPool(startAddress, 1024);
    }

    void TearDown() override {
        delete memPool;
        delete[] startAddress;
    }

    uint8_t* startAddress;
    MemPool* memPool;
};

TEST_F(MemPoolTest, AllocationWithinPoolSize) {
    uint8_t* address = nullptr;
    EXPECT_TRUE(memPool->allocate(address, 100));
    EXPECT_NE(address, nullptr);
    EXPECT_EQ(memPool->getUsed(), 128);  // Aligned to 128 bytes (4 * 32 bytes)
}

TEST_F(MemPoolTest, AllocationExceedsPoolSize) {
    uint8_t* address = nullptr;
    EXPECT_FALSE(memPool->allocate(address, 2000));
    EXPECT_EQ(address, nullptr);
    EXPECT_EQ(memPool->getUsed(), 0);
}

TEST_F(MemPoolTest, MultipleAllocations) {
    uint8_t* address1 = nullptr;
    uint8_t* address2 = nullptr;
    EXPECT_TRUE(memPool->allocate(address1, 50));
    EXPECT_TRUE(memPool->allocate(address2, 50));
    EXPECT_NE(address1, nullptr);
    EXPECT_NE(address2, nullptr);
    EXPECT_EQ(memPool->getUsed(), 128);  // 2 * 64 bytes (aligned from 50 bytes)
}

TEST_F(MemPoolTest, ResetPool) {
    uint8_t* address = nullptr;
    EXPECT_TRUE(memPool->allocate(address, 100));
    memPool->reset();
    EXPECT_EQ(memPool->getUsed(), 0);
    EXPECT_EQ(memPool->getSize(), 1024);
}

TEST_F(MemPoolTest, UsedPercentage) {
    uint8_t* address = nullptr;
    EXPECT_TRUE(memPool->allocate(address, 512));
    EXPECT_EQ(memPool->getUsedPercentage(), 50);  // 512 out of 1024 bytes used
}

TEST_F(MemPoolTest, AlignSize) {
    uint8_t* address = nullptr;
    EXPECT_TRUE(memPool->allocate(address, 1));
    EXPECT_EQ(memPool->getUsed(), 32);  // Aligned to 32 bytes
    EXPECT_TRUE(memPool->allocate(address, 31));
    EXPECT_EQ(memPool->getUsed(), 64);  // Aligned to 32 bytes
}

TEST_F(MemPoolTest, ZeroBytesAllocation) {
    uint8_t* address = nullptr;
    EXPECT_FALSE(memPool->allocate(address, 0));
    EXPECT_EQ(address, nullptr);
    EXPECT_EQ(memPool->getUsed(), 0);
}

TEST_F(MemPoolTest, FullPoolAllocation) {
    uint8_t* address = nullptr;
    // Allocate most of the pool minus one aligned block
    EXPECT_TRUE(memPool->allocate(address, 1024 - MEMPOOL_ALIGN_BYTES));
    EXPECT_EQ(memPool->getUsed(), 1024 - MEMPOOL_ALIGN_BYTES); // Aligned usage
    
    // Allocate the remaining aligned block
    EXPECT_TRUE(memPool->allocate(address, MEMPOOL_ALIGN_BYTES - 1));
    EXPECT_EQ(memPool->getUsedPercentage(), 100);  // Fully used
    
    // No more space should be available
    EXPECT_FALSE(memPool->allocate(address, 1));
}