// Copyright 2021, Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cv_bridge/cv_bridge.hpp"
#include "image_transport/image_transport.hpp"
#include "opencv2/highgui.hpp"
#include "rclcpp/logging.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

#include <unistd.h>

void imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr & msg)
{
  try {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::waitKey(10);
  } catch (const cv_bridge::Exception & e) {
    auto logger = rclcpp::get_logger("my_subscriber");
    RCLCPP_ERROR(logger, "Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void imageCallback2(const sensor_msgs::msg::Image::ConstSharedPtr & msg)
{
  auto logger = rclcpp::get_logger("my_subscriber");
  sleep(10);
  RCLCPP_INFO(logger, "Hello there !");
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions options;
  rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("image_listener", options);

  auto cb_group = node->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
  rclcpp::SubscriptionOptions sub_options;
  sub_options.callback_group = cb_group;

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node);

  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(node);

  auto sub = image_transport::create_subscription(node.get(), "camera/image", imageCallback, "raw", rmw_qos_profile_default , sub_options);
  auto sub2 = image_transport::create_subscription(node.get(), "camera/image", imageCallback2, "raw", rmw_qos_profile_default , sub_options);

  executor.spin();
  cv::destroyWindow("view");

  return 0;
}
