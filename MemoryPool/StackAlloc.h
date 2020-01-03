#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H
#include<memory>

template<typename T>
struct StackNode {
	T data;
	StackNode* next;
};

template<typename T,typename Alloc = std::allocator<T>>
class StackAlloc {
public:
	typedef StackNode<T> Node;
	typedef typename Alloc::template rebind<Node>::other allocator;

	StackAlloc() { m_head = nullptr; }
	~StackAlloc() { Clear(); }

	void Clear() {
		Node* curr = m_head;

		while(curr != nullptr) {
			Node* temp = curr->next;
			m_allocator.destroy(curr);
			m_allocator.deallocate(curr, 1);
			curr = temp;
		}
		m_head = nullptr;
	}

	bool Empty() { return (m_head == nullptr); }

	void Push(T element) {
		Node* curr = m_allocator.allocate(1);
		m_allocator.construct(curr, Node());
		curr->data = element;
		curr->next = m_head;
		m_head = curr;
	}

	T Pop() {
		T result = m_head->data;
		Node* temp = m_head->next;
		m_allocator.destroy(m_head);
		m_allocator.deallocate(m_head, 1);
		m_head = temp;

		return result;
	}

	T Top() { return m_head->data; }
	
private:
	Node* m_head;
	allocator m_allocator;
};

#endif










