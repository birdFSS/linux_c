#下一步需要学习如何与头文件产生依赖

#源文件路径
SRC_PATH=./

#目标文件路径
OBJ_PATH=../obj

#搜索源文件路径下所有.c文件
SRCS=$(wildcard $(SRC_PATH)/*.c)

#将.c文件后缀换成.o
OBJS_TMP=$(patsubst %.c, %.o, $(SRCS))

#先去掉目录项(这时是.c文件的目录)，添加.o文件的目录项
OBJS=$(addprefix $(OBJ_PATH)/, $(notdir $(OBJS_TMP)))

#gcc标志
CC=gcc
CFLAG=-Wall -g 

#主目标
TARGET=Test

#设定依赖关系
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(CFLAG)

# % 表示对应字符串匹配，分别匹配不同目录下的.c和.o文件
# 设定.o文件的依赖关系
$(OBJS):$(OBJ_PATH)/%.o:$(SRC_PATH)/%.c
	$(CC) -c $^ -o $@ $(CFLAG)

#.PHONY是避免与Makefile同一目录下存在同名(如clean文件)导致失败
.PHONY:clean show
clean:
	rm -rf $(OBJS) $(TARGET)

#展示所有配置信息
#如果不加@，会将echo也输出
show:
	@echo SRC_PATH=$(SRC_PATH)
	@echo OBJ_PATH=$(OBJ_PATH)
	@echo SRCS=$(SRCS)
	@echo OBJS=$(OBJS)
	@echo CFLAG=$(CFLAG)
