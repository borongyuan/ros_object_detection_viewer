#include "ros/ros.h"
#include "message_filters/subscriber.h"
#include "message_filters/time_synchronizer.h"
#include "image_transport/subscriber_filter.h"
#include "opencv2/highgui/highgui.hpp"
#include "cv_bridge/cv_bridge.h"
#include "object_msgs/ObjectsInBoxes.h"

class ViewerNode
{
public:
  ViewerNode(const ros::NodeHandle& nh);
  //   ~ViewerNode();

  struct parameters
  {
    std::string boxes_topic;
    std::string image_topic;

    parameters()
    {
      ros::param::param<std::string>(ros::this_node::getName() + "/boxes_topic", boxes_topic, "inference");
      ros::param::param<std::string>(ros::this_node::getName() + "/image_topic", image_topic, "image_raw");
    }
  };
  const struct parameters params;

private:
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;

  message_filters::Subscriber<object_msgs::ObjectsInBoxes> boxes_sub;
  image_transport::SubscriberFilter image_sub;

  message_filters::TimeSynchronizer<sensor_msgs::Image, object_msgs::ObjectsInBoxes> sync;

  void syncCallback(const sensor_msgs::ImageConstPtr& image_msg, const object_msgs::ObjectsInBoxesConstPtr& boxes_msg);
};