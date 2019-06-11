#include "ros_object_detection_viewer/viewer.h"

ViewerNode::ViewerNode(const ros::NodeHandle& nh)
  : nh_(nh)
  , it_(nh)
  , params()
  , image_sub(it_, params.image_topic, 1, image_transport::TransportHints("compressed"))
  , boxes_sub(nh_, params.boxes_topic, 1)
  , sync(image_sub, boxes_sub, 60)
{
  sync.registerCallback(boost::bind(&ViewerNode::syncCallback, this, _1, _2));
}

void ViewerNode::syncCallback(const sensor_msgs::ImageConstPtr& image_msg,
                              const object_msgs::ObjectsInBoxesConstPtr& boxes_msg)
{
  cv::Mat image = cv_bridge::toCvShare(image_msg, "bgr8")->image;
  for (auto obj : boxes_msg->objects_vector)
  {
    std::stringstream ss;
    ss << obj.object.object_name << ':' << obj.object.probability;
    cv::rectangle(image, cvPoint(obj.roi.x_offset, obj.roi.y_offset),
                  cvPoint(obj.roi.x_offset + obj.roi.width, obj.roi.y_offset + obj.roi.height),
                  cv::Scalar(255, 242, 35));
    cv::putText(image, ss.str(), cvPoint(obj.roi.x_offset, obj.roi.y_offset + 20), cv::FONT_HERSHEY_PLAIN, 1.0f,
                cv::Scalar(0, 255, 255));
  }

  cv::imshow("object_detection_viewer", image);
  cv::waitKey(30);
}

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "object_detection_viewer");

  ros::NodeHandle nh("~");
  ViewerNode viewer(nh);

  ros::spin();

  return 0;
}