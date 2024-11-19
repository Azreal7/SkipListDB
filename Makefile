# 定义变量
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -g
TARGET = bin/main
OBJ_DIR = obj
SRC_DIR = .
INCLUDE_DIR = .

# 源文件和目标文件
SRCS = $(SRC_DIR)/main.cpp
OBJS = $(OBJ_DIR)/main.o

# 默认目标
all: $(TARGET)

# 目标文件生成规则
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # 如果目标目录不存在则创建
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 链接目标文件生成最终可执行文件
$(TARGET): $(OBJS)
	@mkdir -p bin  # 如果 bin 目录不存在则创建
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

# 清理目标文件和可执行文件
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# 伪目标
.PHONY: all clean
