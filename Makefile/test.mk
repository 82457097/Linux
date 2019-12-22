bar1 = foo1
bar1 ?= foo2
bar2 ?= foo2

all:
	echo "bar1 is $(bar1),bar2 is $(bar2)"
