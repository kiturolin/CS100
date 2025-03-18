import random

def cyclic_shift_left(x, k, bit_width=32):
    # Perform cyclic left shift
    return ((x << k) & (2**bit_width - 1)) | (x >> (bit_width - k))

def generate_test_case(n, Q, max_value=2**32-1):
    # Generate initial singularity values for n world lines
    singularity_values = [random.randint(0, max_value) for _ in range(n)]
    
    # Generate Q events as tuples (x, y) where 1 <= x, y <= n
    events = [(random.randint(1, n), random.randint(1, n)) for _ in range(Q)]
    
    return singularity_values, events

def process_events(n, singularity_values, events):
    # Initialize outgoing edges and singularity copies
    p = list(range(n))  # using 0-based index
    s = singularity_values[:]
    
    for x, y in events:
        s_org = s[:]
        situation_changed = [False] * n
        p[x-1] = y-1  # set the outgoing edge from x to y (convert to 0-based)
        
        next_line = x - 1  # Convert to 0-based for processing
        cnt = 0
        
        while True:
            next_line = p[next_line]
            if next_line == x-1 or situation_changed[next_line]:
                break
            situation_changed[next_line] = True
            cnt += 1
            s[next_line] ^= cyclic_shift_left(s_org[x-1], cnt)
    
    # Find the world line with the smallest singularity value
    min_s = min(s)
    min_i = s.index(min_s) + 1  # Convert back to 1-based index
    return min_i, min_s

def main():
    n = 900# Number of world lines
    Q = 900# Number of events

    # Generate test cases
    singularity_values, events = generate_test_case(n, Q)

    # Output the generated test case
    print(n, Q)
    print(' '.join(map(str, singularity_values)))
    for event in events:
        print(f"{event[0]} {event[1]}")

    # Compute the correct result for the generated test case
    min_i, min_s = process_events(n, singularity_values, events)

    # Output the correct result
    print(min_i)  # World line with the smallest singularity value
    print(min_s)  # Corresponding singularity value

if __name__ == "__main__":
    main()
