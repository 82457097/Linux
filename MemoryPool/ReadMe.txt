//关于 typedef typename Alloc<T>::template rebind<Node>::other allocator 的解释

//我们用 typedef 给一个东西取了别名叫 allocator
//这个东西是

	typename Alloc<T>::template rebind<Node>::other 

//它其实是为了解决编译器不认识的代码的问题而出现的写法
//首先我们定义了 Alloc = std::allocator<T>，而 rebind 其实是 std::allocator 的一个成员。
//巧就巧在，rebind 本身又是另一个模板， C++ 称其为 dependent name。完整的形式本来应该是：

	std::allocator<T>::rebind<Node>::other

//但是模板的相关解析已经在 <T> 出现过了，后面的 <Node> 中的 < 只能被解释为小于符号，这会导致编译出错。
//为了表示 dependent name 是一个模板，就必须使用 template 前缀。
//如果没有 template 前缀，< 会被编译器解释为小于符号。所以，我们必须写成下面的形式：

	std::allocator<T>::template rebind<Node>::other

//最后，编译器在其实根本没有任何办法来区分 other 究竟是一个类型，还是一个成员。
//但我们其实知道 other 是一个类型（见这里），所以使用 typename 来明确指出这是一个类型，最终才有了：

	typename std::allocator<T>::template rebind<Node>::other

	template<typename U> 
	struct rebind {
		typedef allocator other; 
	}; 

//rebind的作用是，对于给定的类型T的分配器（typename Alloc = std::allocator<T>），
//想根据相同的策略得到另一个类型U的分配器（这里得到了std::allocator<StackNode_<T>>）。	
