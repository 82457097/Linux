# 多目标规则：
# 多目标规则可以简单的理解为是一种将多条具有 相同依赖 相同生命 的规则，合并成一条规则语句，其格式为
# targets...: prerequisites...
# 	commands
# 	...

all: target1 target2
	echo "This is a rule for $@"

target1: dep
	echo "This is a rule for $@"

target2: dep
	echo "This is a rule for $@"

dep:

# 利用多目标规则改写：

all: target1 target2
	echo "This is a rule for $@"

target1 target2: dep
	echo "This is a rule for $@"

dep:
