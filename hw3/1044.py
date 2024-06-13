import random

def fifo(pages, frames, verbose=False):
    page_faults = 0
    queue = []
    for i, page in enumerate(pages):
        if verbose:
            print(f"Step {i+1}: Accessing page {page}")
        if page not in queue:
            if len(queue) == frames:
                if verbose:
                    print(f"  Page fault! Replacing page {queue[0]}")
                queue.pop(0)
            queue.append(page)
            page_faults += 1
            if verbose:
                print(f"  Frames: {queue}")
        else:
            if verbose:
                print("  Hit! No page fault")
    return page_faults

def lru(pages, frames, verbose=False):
    page_faults = 0
    queue = []
    for i, page in enumerate(pages):
        if verbose:
            print(f"Step {i+1}: Accessing page {page}")
        if page not in queue:
            if len(queue) == frames:
                if verbose:
                    print(f"  Page fault! Replacing page {queue[0]}")
                queue.pop(0)
            queue.append(page)
            page_faults += 1
        else:
            queue.remove(page)
            queue.append(page)
            if verbose:
                print("  Hit! No page fault")
        if verbose:
            print(f"  Frames: {queue}")
    return page_faults

def optimal(pages, frames, verbose=False):
    page_faults = 0
    queue = []
    for i, page in enumerate(pages):
        if verbose:
            print(f"Step {i+1}: Accessing page {page}")
        if page not in queue:
            if len(queue) == frames:
                farthest = next(j for j, p in enumerate(queue) if p not in pages[i+1:])
                if verbose:
                    print(f"  Page fault! Replacing page {queue[farthest]}")
                queue.pop(farthest)
            queue.append(page)
            page_faults += 1
        else:
            if verbose:
                print("  Hit! No page fault")
        if verbose:
            print(f"  Frames: {queue}")
    return page_faults

if __name__ == "__main__":
    frames = int(input("輸入分頁框數量: "))
    pages = [random.randint(0, 9) for _ in range(20)]
    print("頁面參考字串:", pages)

    print("\nFIFO:")
    page_faults_fifo = fifo(pages.copy(), frames, verbose=True)
    print("FIFO 頁面錯誤次數:", page_faults_fifo)

    print("\nLRU:")
    page_faults_lru = lru(pages.copy(), frames, verbose=True)
    print("LRU 頁面錯誤次數:", page_faults_lru)

    print("\nOptimal:")
    page_faults_optimal = optimal(pages.copy(), frames, verbose=True)
    print("Optimal 頁面錯誤次數:", page_faults_optimal)
