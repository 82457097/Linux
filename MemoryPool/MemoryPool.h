#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include<climits>
#include<cstddef>

template<typename T, size_t BlockSize = 4096 >
class MemoryPool {
public:
	typedef T* Pointer;

	template<typename U>
	struct rebind {
		typedef MemoryPool<U> other;
	};

	MemoryPool() noexcept {
		m_currentBlock = nullptr;
		m_currentSlot = nullptr;
		m_lastSlot = nullptr;
		m_freeSlots = nullptr;
	}

	~MemoryPool() noexcept {
		m_slotPointer curr = m_currentBlock;
		while(curr != nullptr) {
			m_slotPointer next = curr->next;
			operator delete(reinterpret_cast<void*>(curr));
		curr = next;
		}
	}

	Pointer allocate(size_t n = 1, const T* hint = 0) {
		if(m_freeSlots != nullptr) {
			Pointer result = reinterpret_cast<Pointer>(m_freeSlots);
			m_freeSlots = m_freeSlots->next;
			return result;
		}
		else {
			//如果对象槽不够用了，则分配一个新的内存区块
			if(m_currentSlot >= m_lastSlot) {
				//分配一个新的内存块，并指向前一个内存区块；
				m_dataPointer newBlock = reinterpret_cast<m_dataPointer>(operator new(BlockSize));
				reinterpret_cast<m_slotPointer>(newBlock)->next = m_currentBlock;
				m_currentBlock = reinterpret_cast<m_slotPointer>(newBlock);
				//填补整个区块来满足元素内存区域的对齐要求；
				m_dataPointer body = newBlock + sizeof(m_slotPointer);
				uintptr_t result = reinterpret_cast<uintptr_t>(body);
				size_t bodyPadding = (alignof(m_slotType) - result) % alignof(m_slotType);
				m_currentSlot = reinterpret_cast<m_slotPointer>(body + bodyPadding);
				m_lastSlot = reinterpret_cast<m_slotPointer>(newBlock + BlockSize - sizeof(m_slotType));
			}
			return reinterpret_cast<Pointer>(m_currentSlot++);
		}
	}

	void deallocate(Pointer p, size_t n = 1) {
		if(p != nullptr) {
			reinterpret_cast<m_slotPointer>(p)->next = m_freeSlots;
			m_freeSlots = reinterpret_cast<m_slotPointer>(p);
		}
	}

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		new (p) U (std::forward<Args>(args)...);
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}

private:
	union Slot {
		T element;
		Slot* next;
	};

	typedef char* m_dataPointer;
	typedef Slot m_slotType;
	typedef Slot* m_slotPointer;

	m_slotPointer m_currentBlock;
	m_slotPointer m_currentSlot;
	m_slotPointer m_lastSlot;
	m_slotPointer m_freeSlots;
	static_assert(BlockSize >= 2 * sizeof(m_slotType), "BlockSize too small.");

};

#endif