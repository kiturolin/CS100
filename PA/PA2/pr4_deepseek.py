import random
import sys
from itertools import permutations

def cyclic_left_shift(s, i):
    i %= 32
    return ((s << i) | (s >> (32 - i))) & 0xFFFFFFFF

class WorldLineSimulator:
    def __init__(self, n):
        self.n = n
        self.p = list(range(n+1))  # 1-based
        self.s = [random.randint(0, (1<<32)-1) for _ in range(n+1)]  # 1-based
        self.original_s = self.s.copy()
    
    def generate_complex_events(self, Q):
        """生成具有复杂路径的事件序列"""
        events = []
        # 生成链式结构
        chain_length = min(n, Q//2)
        for i in range(chain_length):
            x = random.randint(1, self.n)
            y = (x + random.randint(1, 5)) % self.n
            if y == 0: y = self.n
            events.append((x, y))
        
        # 生成环状结构
        nodes = random.sample(range(1, self.n+1), 3)
        events.append((nodes[0], nodes[1]))
        events.append((nodes[1], nodes[2]))
        events.append((nodes[2], nodes[0]))
        
        # 填充剩余事件
        while len(events) < Q:
            x = random.randint(1, self.n)
            y = random.choice([
                (x + 1) % self.n,
                random.randint(1, self.n),
                events[-1][1] if events else x
            ])
            if y == 0: y = self.n
            events.append((x, y))
        
        return events[:Q]
    
    def process_event(self, x, y):
        """处理单个事件并更新状态"""
        original_sx = self.s[x]
        self.p[x] = y
        
        current = y
        cnt = 1
        visited = set()
        
        while True:
            if current in visited or current == x:
                break
            visited.add(current)
            
            # 计算异或值
            delta = cyclic_left_shift(original_sx, cnt)
            self.s[current] ^= delta
            
            # 移动到下一个节点
            next_node = self.p[current]
            if next_node == current or self.s[next_node] != self.original_s[next_node]:
                break
            current = next_node
            cnt += 1
        
        # 更新原始值快照
        self.original_s = self.s.copy()
    
    def find_min(self):
        """找到最小奇异值"""
        min_s = float('inf')
        min_idx = -1
        for i in range(1, self.n+1):
            if self.s[i] < min_s or (self.s[i] == min_s and i < min_idx):
                min_s = self.s[i]
                min_idx = i
        return min_idx, min_s

def generate_test_case(n=1000, Q=4000):
    # 初始化模拟器
    wl = WorldLineSimulator(n)
    
    # 生成复杂事件序列
    events = wl.generate_complex_events(Q)
    
    # 处理所有事件
    for x, y in events:
        wl.process_event(x, y)
    
    # 获取结果
    min_idx, min_s = wl.find_min()
    
    # 构建输入输出
    input_data = [
        f"{n} {Q}",
        " ".join(map(str, wl.original_s[1:]))
    ]
    input_data += [f"{x} {y}" for x, y in events]
    
    output_data = [str(min_idx), str(min_s)]
    
    return input_data, output_data

def save_test_case(case_id, input_data, output_data):
    with open(f"test_{case_id}.in", "w") as f:
        f.write("\n".join(input_data))
    with open(f"test_{case_id}.out", "w") as f:
        f.write("\n".join(output_data))

# 生成测试用例示例
if __name__ == "__main__":
    for i in range(1, 6):  # 生成5个测试用例
        n = random.randint(50, 100)  # 控制测试规模
        Q = random.randint(300, 500)
        input_data, output_data = generate_test_case(n, Q)
        save_test_case(i, input_data, output_data)
        print(f"Generated test case {i} with {n} nodes and {Q} events")
