#	$@ -- 代表规则中的目标文件名
#	$< -- 代表规则的第一个依赖文件名
#	$^ -- 代表规则中所依赖文件的列表，文件名用空格分割

all: first second third
	echo "\$$@ = $@"
	echo "$$< = $<"
	echo "$$^ = $^"

#	1.$字符在makefile中有特殊用途，因此如果要取消其特殊用途当成一个普通字符传递给echo命令执行，需要使用$$
#	2.$@在bash shell中也有特殊用途，因此如果希望echo命令在bash中正常输出$@，需要加上\字符
#	3.该makefile的最后一行first second third:看起来有点奇怪，这是一条没有依赖和命令的多目标规则，读者可自行将它删除看有什么效果，并思考原因。

first second third:
