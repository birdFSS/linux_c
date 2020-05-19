#author : birdFSS
#date : 2020/2/1
#结构如下所示
#[project]
#|-src
#  |-main.c
#|-include
#  |-main.h
#|-obj
#  |-main.o
#  |-main.d 	//用来依赖头文件的，就是头文件修改后，只需编译使用该头文件的源文件即可
#|-Makefile

#源文件路径
SRC_PATH=src
#目标文件路径
OBJ_PATH=obj
#头文件路径
INC_PATH=include

#搜索源文件路径下所有.c文件
SRCS=$(wildcard $(SRC_PATH)/*.c)

#将.c文件后缀换成.o
OBJS_TMP=$(patsubst %.c, %.o, $(SRCS))

#先去掉目录项(这时是.c文件的目录)，添加.o文件的目录项
OBJS=$(addprefix $(OBJ_PATH)/, $(notdir $(OBJS_TMP)))


#gcc标志
CC=gcc
version=test

ifeq ($(version), release)

CFLAG=-Wall -I$(INC_PATH) -O2
DLIB=

#主目标
TARGET=main

else

#CUnit gcov lcov测试程序使用下面参数
#CFLAG=-Wall -g -I$(INC_PATH) -fprofile-arcs -ftest-coverage	
#DLIB=-lrt -lcunit
CFLAG=-Wall -g -I$(INC_PATH) -O1 -pg
DLIB=

#主目标
TARGET=test

endif

#设定依赖关系
#执行顺序是由下至上
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(CFLAG)

# % 表示对应字符串匹配，分别匹配不同目录下的.c和.o文件
# 设定.o文件的依赖关系
$(OBJS):$(OBJ_PATH)/%.o:$(SRC_PATH)/%.c
	$(CC) -c $(CFLAG) $< -o $@ $(DLIB)

#创建头文件依赖
$(OBJ_PATH)/%.d:$(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJ_PATH)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

#将生成的头文件的依赖关系，包含进来
#如下
#obj/test.o obj/test.d : src/test.c src/../include/test.h
-include $(OBJS:.o=.d)

#.PHONY是避免与Makefile同一目录下存在同名(如clean文件)导致失败
.PHONY:clean show
#展示所有配置信息
#如果不加@，会将echo也输出
show:
	@echo SRC_PATH=$(SRC_PATH)
	@echo OBJ_PATH=$(OBJ_PATH)
	@echo SRCS=$(SRCS)
	@echo OBJS=$(OBJS)
	@echo CFLAG=$(CFLAG)

clean:
	rm -rf $(OBJ_PATH)/* $(TARGET)

gprof:
	./test
	gprof test gmon.out > analysis.txt
	vim analysis.txt





























