#include "MemPool.h"

MemPool::MemPool() : mStartAdr(nullptr),
					 mCurrentAdr(nullptr),
					 mPoolSize(0),
					 mSizeRemaining(0)
{
}

MemPool::MemPool(uint8_t *startAddress, uint32_t poolSize) : mStartAdr(startAddress),
															 mCurrentAdr(startAddress),
															 mPoolSize(poolSize),
															 mSizeRemaining(poolSize)
{
	// Ensure the pool size is valid
	if (mPoolSize == 0 || mStartAdr == nullptr)
	{
		mPoolSize = 0;
		mSizeRemaining = 0;
		mCurrentAdr = nullptr;
	}
}

MemPool::~MemPool()
{
	// Nothing to clean up as we do not manage the memory
}

bool MemPool::allocate(uint8_t *&address, uint32_t bytes)
{
	if (bytes == 0 || mCurrentAdr == nullptr)
	{
		return false;
	}

	uint32_t sizeToAllocate = alignSize(bytes);

	if (mSizeRemaining >= sizeToAllocate)
	{
		address = mCurrentAdr;
		mCurrentAdr += sizeToAllocate;
		mSizeRemaining -= sizeToAllocate;
		return true;
	}
	else
	{
		return false;
	}
}

void MemPool::reset()
{
	mCurrentAdr = mStartAdr;
	mSizeRemaining = mPoolSize;
}

uint32_t MemPool::getSize() const
{
	return mPoolSize;
}

uint32_t MemPool::getUsed() const
{
	return mPoolSize - mSizeRemaining;
}

uint8_t MemPool::getUsedPercentage() const
{
	if (mPoolSize == 0)
	{
		return 0;
	}
	uint32_t used = getUsed();
	return static_cast<uint8_t>((used * 100) / mPoolSize);
}

uint32_t MemPool::alignSize(uint32_t bytes) const
{
	// Align size to the next multiple of MEMPOOL_ALIGN_BYTES
	return (bytes + MEMPOOL_ALIGN_BYTES - 1) & ~(MEMPOOL_ALIGN_BYTES - 1);
}
