#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <string>

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "video_publisher");
    ros::NodeHandle nh;

    std::string filename;
    int frame_rate;
    bool local_display;

    if (!ros::param::get("~videosrc",filename))
    {
        ROS_WARN("no video file specified!");
        return 0;
    }
    ros::param::param<int>("~framerate",frame_rate, 30);
    ros::param::param<bool>("~localdisplay", local_display, false);

    image_transport::ImageTransport it(nh);
    image_transport::Publisher img_pub = it.advertise("/usb_cam/image_raw",1);

    cv::VideoCapture cap(filename);
    if (!cap.isOpened())
    {
        ROS_WARN("cannot open video file!");
        return 0;
    }

    cv_bridge::CvImage cv_img;

    if (local_display)
    {
        cv::namedWindow("image", CV_WINDOW_NORMAL);
        cv::startWindowThread();
    }

    ros::Rate loop_rate(frame_rate);
    unsigned int frame_cnt=0;

    cv_img.encoding = "rgb8";

    while(ros::ok())
    {
        cap >> cv_img.image;
        if (cv_img.image.empty())
        {
            ROS_INFO("end.");
            return 0;
        }
        cv_img.header.stamp = ros::Time::now();
        img_pub.publish(cv_img.toImageMsg());
        ROS_INFO_STREAM("frame: "<<frame_cnt++);
        if (local_display)
            cv::imshow("image", cv_img.image);
        loop_rate.sleep();
    }

    return 0;
}
