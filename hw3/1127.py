import random
import sys

def fcfs(requests, head):
    route = [head] + requests
    total_movement = sum(abs(route[i] - route[i-1]) for i in range(1, len(route)))
    return total_movement, route

def scan(requests, head):
    left = [request for request in requests if request <= head]
    right = [request for request in requests if request > head]
    left.sort(reverse=True)
    right.sort()
    route = [head] + left + [0] + right
    total_movement = sum(abs(route[i] - route[i-1]) for i in range(1, len(route)))
    return total_movement, route

def c_scan(requests, head):
    right = [request for request in requests if request >= head]
    left = [request for request in requests if request < head]
    right.sort()
    left.sort()
    route = [head] + right + [4999, 0] + left
    total_movement = sum(abs(route[i] - route[i-1]) for i in range(1, len(route)))
    return total_movement, route

if __name__ == "__main__":
    head = int(sys.argv[1])
    requests = [random.randint(0, 4999) for _ in range(1000)]

    print("\nFCFS:")
    total_movement_fcfs, route_fcfs = fcfs(requests.copy(), head)
    print("FCFS 磁頭移動路線:", route_fcfs)
    print("FCFS 磁頭移動總量:", total_movement_fcfs)

    print("\nSCAN:")
    total_movement_scan, route_scan = scan(requests.copy(), head)
    print("SCAN 磁頭移動路線:", route_scan)
    print("SCAN 磁頭移動總量:", total_movement_scan)

    print("\nC-SCAN:")
    total_movement_c_scan, route_c_scan = c_scan(requests.copy(), head)
    print("C-SCAN 磁頭移動路線:", route_c_scan)
    print("C-SCAN 磁頭移動總量:", total_movement_c_scan)
