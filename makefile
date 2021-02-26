src = $(wildcard *.c)
targets = test

CC = gcc
CFLAGS = -Wall -g

# all:$(targets)

$(targets):$(src)
	$(CC) $^ -o $@ $(CFLAGS)
# 静态模式：
# $(targets):%:%.c ---- 表示目标从$(targets)中获取，而依赖模式“%.c”则取模式“%”匹配

.PHONY:clean all
clean:
	-rm -rf $(targets)

# Makefile
# 
# 基本规则：
# target：src
# 	command
# 
# obj = ~~~ ---- 给变量obj赋值
# $(obj) ---- 取obj中的值
# 
# 模式规则：
# %.o:%.c             | main.o:main.c
# 	gcc -c $< -o $@   |	  gcc -c main.c -o main.o
# 
# 自动变量：只能在命令中使用
# $< ---- 依赖中的第一个依赖
# $@ ---- 规则中的目标
# $^ ---- 规则中的所有依赖
# 
# makefile中自己维护的变量：
# CC = gcc ---- 默认值(CC = cc = gcc)
# CPPFLAGS = -I ---- 预编译需要的选项
# CFLAGS ---- 编译的时候使用的参数，如：-Wall -g -c
# LDFLAGS ---- 链接库使用的选项，如： -L -l
# 
# makefile中的函数：函数都有返回值
# src = $(wildcard ./*.c) ---- 从当前目录下查找所有的.c文件
# obj = $(patsubst ./%.c, ./%.o, $(src)) ---- 模式匹配替换：将$(src)中的*.c，模式匹配替换为*.o
# 
# .PHONY:clean (添加伪目标)
# clean:
# 	rm $(obj) $(target) -rf
# ***用 make clean 调用(-rf 强制执行)***
# -rm -f $(obj) ---- 命令前加“-”表示如果执行失败，则忽略该命令继续向下执行
