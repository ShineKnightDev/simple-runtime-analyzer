import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    sizes = []
    times = []
    for line in lines:
        size, time = line.strip().split()
        sizes.append(int(size))
        times.append(float(time))

    return sizes, times

def plot_metrics(sizes, times):
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, 'bo-', label='Execution Time')
    plt.xlabel('Sequence Size')
    plt.ylabel('Time (seconds)')
    plt.title('Execution Time Growth by Sequence Size')
    plt.grid(True)
    plt.legend()
    plt.savefig('runtime_metrics_plot.png')
    plt.show()

if __name__ == '__main__':
    sizes, times = read_data('runtime_metrics.txt')
    plot_metrics(sizes, times)