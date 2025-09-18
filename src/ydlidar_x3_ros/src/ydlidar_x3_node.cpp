#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "CYdLidar.h"

using namespace ydlidar;

class YDLidarX3Node : public rclcpp::Node {
public:
    YDLidarX3Node() : Node("ydlidar_x3_node") {
        scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);

        // Configure Lidar
        laser_ = std::make_shared<CYdLidar>();

        std::string port = "/dev/ttyUSB0";
        int baudrate = 115200;
        bool singleChannel = true;

        laser_->setlidaropt(LidarPropSerialPort, port.c_str(), port.size());
        laser_->setlidaropt(LidarPropSerialBaudrate, &baudrate, sizeof(int));
        laser_->setlidaropt(LidarPropSingleChannel, &singleChannel, sizeof(bool));

        if (!laser_->initialize()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to initialize YDLidar.");
            // rclcpp::shutdown();   // disable for debugging
        } else {
            RCLCPP_INFO(this->get_logger(), "YDLidar initialized OK");
        }

        if (!laser_->turnOn()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to start YDLidar.");
            // rclcpp::shutdown();   // disable for debugging
        } else {
            RCLCPP_INFO(this->get_logger(), "YDLidar scanning started OK");
        }

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&YDLidarX3Node::publish_scan, this)
        );
        RCLCPP_INFO(this->get_logger(), "Timer started, should call publish_scan every 100ms");
    }

    ~YDLidarX3Node() {
        laser_->turnOff();
        laser_->disconnecting();
    }

private:
    void publish_scan() {
        LaserScan scan;
        bool ok = laser_->doProcessSimple(scan);
        RCLCPP_DEBUG(this->get_logger(), "publish_scan called, doProcessSimple returned: %d", ok);

        if (ok) {
            RCLCPP_INFO(this->get_logger(), "Got a scan with %zu points", scan.points.size());

            auto msg = sensor_msgs::msg::LaserScan();
            msg.header.stamp = this->get_clock()->now();
            msg.header.frame_id = "laser_frame";
            msg.angle_min = scan.config.min_angle;
            msg.angle_max = scan.config.max_angle;
            msg.angle_increment = scan.config.angle_increment;
            msg.range_min = scan.config.min_range;
            msg.range_max = scan.config.max_range;

            size_t n = scan.points.size();
            msg.ranges.resize(n);
            msg.intensities.resize(n);

            for (size_t i = 0; i < n; i++) {
                msg.ranges[i] = scan.points[i].range;
                msg.intensities[i] = scan.points[i].intensity;
            }

            scan_pub_->publish(msg);
        } else {
            RCLCPP_WARN(this->get_logger(), "No scan data available this cycle");
        }
    }

    std::shared_ptr<CYdLidar> laser_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<YDLidarX3Node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
