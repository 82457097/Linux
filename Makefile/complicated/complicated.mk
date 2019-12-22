complicated: main.o complicated.o
	gcc -o complicated main.o complicated.o

main.o: main.c
	gcc -o main.o -c main.c

complicated.o: complicated.c
	gcc -o complicated.o -c complicated.c

# 修改记录：
# 12.22：使用变量改进我们complicated项目的makefile

executbale := complicated

sources := main.c complicated.c

objects := $(source:.c=.o)

CC := gcc

RM := rm -rf

$(executbale): $(objects)
	$(CC) -o $@ $^

main.o: main.c
	$(CC) -o $@ -c $<

complicated.o: complicated.c
	$(CC) -o $@ -c $<

# 修改记录：
# 12.22：使用静态模式简化

$(executbale): $(objects)
	$(CC) -o $@ $^

$(objects): %.o: %.c
	$(CC) -o $@ -c $<

# 伪目标 保持clean时间戳最新，clean永远能够被执行
.PHONY: clean
clean:
	$(RM) $(executbale) $(objects)

# 修改记录：
# 12.22：使用wildcard函数改进makefile

executbale := complicated

sources := $(wildcard *.c)

objects := $(sources:.c=.o)

CC := gcc

RM := rm -rf

$(executbale): $(objects)
	$(CC) -o $@ $^

$(objects): %.o:%.c
	$(CC) -o $@ -c $<

.PHONY: clean
clean: 
	$(Rm) $(executbale) $(objects)

# 修改记录：
# 12.22 完善头文件依赖关系

executbale := complicated

sources := $(wildcard *.c)

objects := $(sources:.c=.o)

deps := $(sources:.c=.d)

CC := gcc

RM := rm -rf

$(executbale): $(objects)
	$(CC) -o $@ $^

$(objects):%.o:%.c
	$(CC) -o $@ -c $<

sinclude $(deps)
$(deps):%.d:%.c
	$(CC) -MM $< > $@
