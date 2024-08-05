#pragma once
#include <cstdint>

#define MEMPOOL_ALIGN_BYTES 32

class MemPool
{
public:
	MemPool();
	MemPool(uint8_t *startAddress, uint32_t poolSize);
	virtual ~MemPool();

	// Allocate memory for this new set of params, aligns to 32 bytes
	bool allocate(uint8_t *&address, uint32_t bytes);

	// Get Size
	uint32_t getSize() const;

	// Get number of used bytes
	uint32_t getUsed() const;

	// Get used percentage
	uint8_t getUsedPercentage() const;

	void reset();

private:
	uint8_t *mStartAdr;
	uint8_t *mCurrentAdr;
	uint32_t mPoolSize;
	uint32_t mSizeRemaining;

	uint32_t alignSize(uint32_t bytes) const;
};