from PyLidar3 import YdLidarX4
import time

lidar = YdLidarX4('/dev/ttyUSB0', chunk_size=6000)

try:
    while True:
        scan = lidar.GetScanData()
        if scan:
            print(scan)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Stopping LiDAR")
    lidar.Stop()
