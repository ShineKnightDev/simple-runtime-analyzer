import sys
import os
import csv
import json
import matplotlib.pyplot as plt


def read_csv(file_path):
    csv.field_size_limit(10**7)  # Increase field size limit to avoid overflow

    sizes = []
    times = []
    unit = "units"

    with open(file_path, 'r', newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                size = int(row.get('sample_size', 0))
                time = float(row.get('time_value', 0.0))
                unit = row.get('time_unit', unit)
                sizes.append(size)
                times.append(time)
            except (ValueError, TypeError) as e:
                print(f"⚠️ Skipping row due to error: {e}")
                continue

    return sizes, times, unit


def read_json(file_path):
    sizes = []
    times = []
    unit = "units"

    with open(file_path, 'r', encoding='utf-8') as f:
        try:
            data = json.load(f)
        except json.JSONDecodeError as e:
            print(f"❌ JSON decode error: {e}")
            return sizes, times, unit

        for entry in data:
            try:
                size = int(entry.get('sample_size', 0))
                time = float(entry.get('time_value', 0.0))
                unit = entry.get('time_unit', unit)
                sizes.append(size)
                times.append(time)
            except (ValueError, TypeError) as e:
                print(f"⚠️ Skipping entry due to error: {e}")
                continue

    return sizes, times, unit


def read_data(file_path):
    ext = os.path.splitext(file_path)[1]
    if ext == '.csv':
        return read_csv(file_path)
    elif ext == '.json':
        return read_json(file_path)
    else:
        raise ValueError(f"❌ Unsupported file extension: {ext}")


def plot_metrics(sizes, times, unit, source_path):
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, 'bo-', label='Execution Time')
    plt.xlabel('Sample Size')
    plt.ylabel(f'Time ({unit})')
    plt.title('Execution Time vs Sample Size')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    output_path = os.path.splitext(source_path)[0] + '.png'
    plt.savefig(output_path)
    print(f"✅ Plot saved to {output_path}")

    if os.getenv('SUPPRESS_PLOT_DISPLAY'):
        plt.close()
    else:
        plt.show()


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python plot.py <data_file>")
        print("Supported formats: .csv, .json")
        print("Example: python plot.py data/report.csv")
        print("Example: python plot.py data/report.json")
        sys.exit(1)

    file_path = sys.argv[1]
    
    # Verify that the file exists before processing
    if not os.path.exists(file_path):
        print(f"❌ Error: File '{file_path}' not found")
        sys.exit(1)
        
    # Verify that the extension is supported
    ext = os.path.splitext(file_path)[1].lower()
    if ext not in ['.csv', '.json']:
        print(f"❌ Error: Unsupported file format '{ext}'. Use .csv or .json")
        sys.exit(1)

    try:
        sizes, times, unit = read_data(file_path)
        plot_metrics(sizes, times, unit, file_path)
    except Exception as e:
        print(f"❌ Error processing file: {e}")
        sys.exit(1)
