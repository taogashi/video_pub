#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <string>
#include <termios.h>

int getch();

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "video_publisher");
    ros::NodeHandle nh;

    std::string filename;
    int frame_rate;
    bool local_display;
    int skip;

    if (!ros::param::get("~videosrc",filename))
    {
        ROS_WARN("no video file specified!");
        return 0;
    }
    ros::param::param<int>("~framerate",frame_rate, 30);
    ros::param::param<bool>("~localdisplay", local_display, false);
    ros::param::param<int>("~skip", skip, 0);

    image_transport::ImageTransport it(nh);
    image_transport::Publisher img_pub = it.advertise("/usb_cam/image_raw",1);
    ros::Rate loop_rate(frame_rate);

    cv::VideoCapture cap(filename);
    if (!cap.isOpened())
    {
        ROS_WARN("cannot open video file!");
        return 0;
    }

    int cnt = 0;
    while (cnt < skip) {
	if (!cap.grab()) {
		ROS_ERROR("shit...");
	}
	cnt++;
    }

    cv_bridge::CvImage cv_img;
    unsigned int frame_cnt = skip;
    cv_img.encoding = "rgb8";

    if (local_display)
    {
        cv::namedWindow("image", CV_WINDOW_NORMAL);
        cv::startWindowThread();
    }

    bool paused = false;
    while(ros::ok())
    {
        if (32 == getch())
        {
            if (!paused)
            {
                ROS_INFO_STREAM("paused.");
                paused = true;
            }
            else
                paused = false;
        }

        if (!paused)
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
        }
        loop_rate.sleep();
    }

    return 0;
}

int getch()
{
    static struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt);           // save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON);                 // disable buffering      
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

    int c = getchar();  // read character (non-blocking)

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
    return c;
}
