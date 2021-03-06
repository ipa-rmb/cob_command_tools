// ROS includes
#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <cob_teleop/cob_teleop_v2Config.h>

// ROS message includes
#include <sensor_msgs/JoyFeedbackArray.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/Joy.h>

// other includes
#include <cob_teleop_v2_common.cpp>


class cob_teleop_v2_ros
{
    public:
    ros::NodeHandle n_;
    ros::NodeHandle np_;

    dynamic_reconfigure::Server<cob_teleop::cob_teleop_v2Config> server;
    dynamic_reconfigure::Server<cob_teleop::cob_teleop_v2Config>::CallbackType f;

    ros::Publisher joy_feedback_;
    ros::Publisher base_controller_command_;
    ros::Publisher arm_cart_left_;
    ros::Publisher arm_cart_right_;
    ros::Publisher arm_joint_right_;
    ros::Publisher arm_joint_left_;
    ros::Publisher head_controller_command_;
    ros::Publisher sensorring_controller_command_;
    ros::Publisher torso_controller_command_;
    ros::Publisher gripper_left_;
    ros::Publisher gripper_right_;
    ros::Subscriber joy_;

    cob_teleop_v2_data component_data_;
    cob_teleop_v2_config component_config_;
    cob_teleop_v2_impl component_implementation_;

    cob_teleop_v2_ros() : np_("~")
    {
        f = boost::bind(&cob_teleop_v2_ros::configure_callback, this, _1, _2);
        server.setCallback(f);


        joy_feedback_ = n_.advertise<sensor_msgs::JoyFeedbackArray>("joy_feedback", 1);
        base_controller_command_ = n_.advertise<geometry_msgs::Twist>("base_controller_command", 1);
        arm_cart_left_ = n_.advertise<geometry_msgs::Twist>("arm_cart_left", 1);
        arm_cart_right_ = n_.advertise<geometry_msgs::Twist>("arm_cart_right", 1);
        arm_joint_right_ = n_.advertise<std_msgs::Float64MultiArray>("arm_joint_right", 1);
        arm_joint_left_ = n_.advertise<std_msgs::Float64MultiArray>("arm_joint_left", 1);
        head_controller_command_ = n_.advertise<geometry_msgs::Twist>("head_controller_command", 1);
        sensorring_controller_command_ = n_.advertise<std_msgs::Float64MultiArray>("sensorring_controller_command", 1);
        torso_controller_command_ = n_.advertise<geometry_msgs::Twist>("torso_controller_command", 1);
        gripper_left_ = n_.advertise<std_msgs::Float64MultiArray>("gripper_left", 1);
        gripper_right_ = n_.advertise<std_msgs::Float64MultiArray>("gripper_right", 1);
        joy_ = n_.subscribe("joy", 1, &cob_teleop_v2_ros::topicCallback_joy, this);

        np_.param("button_deadman", component_config_.button_deadman, (int)11);
        np_.param("base_max_linear", component_config_.base_max_linear, (double)0.5);
        np_.param("base_max_angular", component_config_.base_max_angular, (double)1.5);
        np_.param("torso_max_angular", component_config_.torso_max_angular, (double)0.2);
        np_.param("head_max_angular", component_config_.head_max_angular, (double)0.3);
        np_.param("sensor_ring_max_angular", component_config_.sensor_ring_max_angular, (double)0.1);
        np_.param("arm_joint_velocity_max", component_config_.arm_joint_velocity_max, (double)0.3);
        np_.param("arm_cartesian_max_linear", component_config_.arm_cartesian_max_linear, (double)0.1);
        np_.param("arm_cartesian_max_angular", component_config_.arm_cartesian_max_angular, (double)0.1);
        np_.param("gripper_max_velocity", component_config_.gripper_max_velocity, (double)0.1);
        np_.param("base_x", component_config_.base_x, (int)1);
        np_.param("base_y", component_config_.base_y, (int)0);
        np_.param("base_yaw", component_config_.base_yaw, (int)2);
        np_.param("arm_x", component_config_.arm_x, (int)0);
        np_.param("arm_y", component_config_.arm_y, (int)1);
        np_.param("arm_yaw", component_config_.arm_yaw, (int)2);
        np_.param("arm_pitch_up", component_config_.arm_pitch_up, (int)4);
        np_.param("arm_pitch_down", component_config_.arm_pitch_down, (int)6);
        np_.param("arm_roll_right_and_ellbow", component_config_.arm_roll_right_and_ellbow, (int)5);
        np_.param("arm_roll_left_and_ellbow", component_config_.arm_roll_left_and_ellbow, (int)7);
        np_.param("arm_z_up", component_config_.arm_z_up, (int)12);
        np_.param("arm_z_down", component_config_.arm_z_down, (int)14);
        np_.param("gripper_open", component_config_.gripper_open, (int)15);
        np_.param("gripper_close", component_config_.gripper_close, (int)13);
        np_.param("arm_joint_up", component_config_.arm_joint_up, (int)4);
        np_.param("arm_joint_down", component_config_.arm_joint_down, (int)6);
        np_.param("arm_joint_left", component_config_.arm_joint_left, (int)7);
        np_.param("arm_joint_right", component_config_.arm_joint_right, (int)5);
        np_.param("arm_joint_12", component_config_.arm_joint_12, (int)15);
        np_.param("arm_joint_34", component_config_.arm_joint_34, (int)14);
        np_.param("arm_joint_56", component_config_.arm_joint_56, (int)13);
        np_.param("arm_joint_7_gripper", component_config_.arm_joint_7_gripper, (int)12);
        np_.param("axis_runfactor", component_config_.axis_runfactor, (int)9);
        np_.param("button_safety_override", component_config_.button_safety_override, (int)9);
        np_.param("button_init_recover", component_config_.button_init_recover, (int)3);
        np_.param("button_mode_switch", component_config_.button_mode_switch, (int)0);
        np_.param("torso_roll", component_config_.torso_roll, (int)0);
        np_.param("torso_pitch", component_config_.torso_pitch, (int)1);
        np_.param("torso_yaw_left", component_config_.torso_yaw_left, (int)15);
        np_.param("torso_yaw_right", component_config_.torso_yaw_right, (int)13);
        np_.param("sensorring_yaw_left", component_config_.sensorring_yaw_left, (int)4);
        np_.param("sensorring_yaw_right", component_config_.sensorring_yaw_right, (int)6);
        np_.param("head_roll", component_config_.head_roll, (int)2);
        np_.param("head_pitch", component_config_.head_pitch, (int)3);
        np_.param("head_yaw_left", component_config_.head_yaw_left, (int)7);
        np_.param("head_yaw_right", component_config_.head_yaw_right, (int)5);
        np_.param("head_home", component_config_.head_home, (int)4);
        np_.param("arm_left_home", component_config_.arm_left_home, (int)7);
        np_.param("arm_right_home", component_config_.arm_right_home, (int)5);
        np_.param("torso_home", component_config_.torso_home, (int)6);
        np_.param("sensorring_home", component_config_.sensorring_home, (int)12);
        np_.param("gripper_left_home", component_config_.gripper_left_home, (int)15);
        np_.param("gripper_right_home", component_config_.gripper_right_home, (int)13);
        np_.param("base_home", component_config_.base_home, (int)14);
        if(np_.hasParam("arm_left_uri"))
            np_.getParam("arm_left_uri", component_config_.arm_left_uri);
        else
            ROS_ERROR("Parameter arm_left_uri not set");
        if(np_.hasParam("components"))
            np_.getParam("components", component_config_.components);
        else
            ROS_ERROR("Parameter components not set");
        np_.param("home_time", component_config_.home_time, (double)5.0);
        np_.param("stop_time", component_config_.stop_time, (double)0.8);
        if(np_.hasParam("arm_right_uri"))
            np_.getParam("arm_right_uri", component_config_.arm_right_uri);
        else
            ROS_ERROR("Parameter arm_right_uri not set");
        if(np_.hasParam("led_mode"))
            np_.getParam("led_mode", component_config_.led_mode);
        else
            ROS_ERROR("Parameter led_mode not set");
        np_.param("gripper_1", component_config_.gripper_1, (int)3);
        np_.param("gripper_2", component_config_.gripper_2, (int)2);
        np_.param("gripper_max_angular", component_config_.gripper_max_angular, (double)0.2);
    }
    void topicCallback_joy(const sensor_msgs::Joy::ConstPtr& msg)
    {
        component_data_.in_joy = *msg;
    }

    void configure_callback(cob_teleop::cob_teleop_v2Config &config, uint32_t level)
    {
        component_config_.button_deadman = config.button_deadman;
        component_config_.base_max_linear = config.base_max_linear;
        component_config_.base_max_angular = config.base_max_angular;
        component_config_.torso_max_angular = config.torso_max_angular;
        component_config_.head_max_angular = config.head_max_angular;
        component_config_.sensor_ring_max_angular = config.sensor_ring_max_angular;
        component_config_.arm_joint_velocity_max = config.arm_joint_velocity_max;
        component_config_.arm_cartesian_max_linear = config.arm_cartesian_max_linear;
        component_config_.arm_cartesian_max_angular = config.arm_cartesian_max_angular;
        component_config_.gripper_max_velocity = config.gripper_max_velocity;
        component_config_.base_x = config.base_x;
        component_config_.base_y = config.base_y;
        component_config_.base_yaw = config.base_yaw;
        component_config_.arm_x = config.arm_x;
        component_config_.arm_y = config.arm_y;
        component_config_.arm_yaw = config.arm_yaw;
        component_config_.arm_pitch_up = config.arm_pitch_up;
        component_config_.arm_pitch_down = config.arm_pitch_down;
        component_config_.arm_roll_right_and_ellbow = config.arm_roll_right_and_ellbow;
        component_config_.arm_roll_left_and_ellbow = config.arm_roll_left_and_ellbow;
        component_config_.arm_z_up = config.arm_z_up;
        component_config_.arm_z_down = config.arm_z_down;
        component_config_.gripper_open = config.gripper_open;
        component_config_.gripper_close = config.gripper_close;
        component_config_.arm_joint_up = config.arm_joint_up;
        component_config_.arm_joint_down = config.arm_joint_down;
        component_config_.arm_joint_left = config.arm_joint_left;
        component_config_.arm_joint_right = config.arm_joint_right;
        component_config_.arm_joint_12 = config.arm_joint_12;
        component_config_.arm_joint_34 = config.arm_joint_34;
        component_config_.arm_joint_56 = config.arm_joint_56;
        component_config_.arm_joint_7_gripper = config.arm_joint_7_gripper;
        component_config_.axis_runfactor = config.axis_runfactor;
        component_config_.button_safety_override = config.button_safety_override;
        component_config_.button_init_recover = config.button_init_recover;
        component_config_.button_mode_switch = config.button_mode_switch;
        component_config_.torso_roll = config.torso_roll;
        component_config_.torso_pitch = config.torso_pitch;
        component_config_.torso_yaw_left = config.torso_yaw_left;
        component_config_.torso_yaw_right = config.torso_yaw_right;
        component_config_.sensorring_yaw_left = config.sensorring_yaw_left;
        component_config_.sensorring_yaw_right = config.sensorring_yaw_right;
        component_config_.head_roll = config.head_roll;
        component_config_.head_pitch = config.head_pitch;
        component_config_.head_yaw_left = config.head_yaw_left;
        component_config_.head_yaw_right = config.head_yaw_right;
        component_config_.head_home = config.head_home;
        component_config_.arm_left_home = config.arm_left_home;
        component_config_.arm_right_home = config.arm_right_home;
        component_config_.torso_home = config.torso_home;
        component_config_.sensorring_home = config.sensorring_home;
        component_config_.gripper_left_home = config.gripper_left_home;
        component_config_.gripper_right_home = config.gripper_right_home;
        component_config_.base_home = config.base_home;
        component_config_.home_time = config.home_time;
        component_config_.stop_time = config.stop_time;
        component_config_.gripper_1 = config.gripper_1;
        component_config_.gripper_2 = config.gripper_2;
        component_config_.gripper_max_angular = config.gripper_max_angular;
    }

    void configure()
    {
        component_implementation_.configure(component_config_);
    }

    void activate_all_output()
    {
        component_data_.out_joy_feedback_active = true;
        component_data_.out_base_controller_command_active = true;
        component_data_.out_arm_cart_left_active = true;
        component_data_.out_arm_cart_right_active = true;
        component_data_.out_arm_joint_right_active = true;
        component_data_.out_arm_joint_left_active = true;
        component_data_.out_head_controller_command_active = true;
        component_data_.out_sensorring_controller_command_active = true;
        component_data_.out_torso_controller_command_active = true;
        component_data_.out_gripper_left_active = true;
        component_data_.out_gripper_right_active = true;
    }

    void update()
    {
        activate_all_output();
        component_implementation_.update(component_data_, component_config_);
        if (component_data_.out_joy_feedback_active)
            joy_feedback_.publish(component_data_.out_joy_feedback);
        if (component_data_.out_base_controller_command_active)
            base_controller_command_.publish(component_data_.out_base_controller_command);
        if (component_data_.out_arm_cart_left_active)
            arm_cart_left_.publish(component_data_.out_arm_cart_left);
        if (component_data_.out_arm_cart_right_active)
            arm_cart_right_.publish(component_data_.out_arm_cart_right);
        if (component_data_.out_arm_joint_right_active)
            arm_joint_right_.publish(component_data_.out_arm_joint_right);
        if (component_data_.out_arm_joint_left_active)
            arm_joint_left_.publish(component_data_.out_arm_joint_left);
        if (component_data_.out_head_controller_command_active)
            head_controller_command_.publish(component_data_.out_head_controller_command);
        if (component_data_.out_sensorring_controller_command_active)
            sensorring_controller_command_.publish(component_data_.out_sensorring_controller_command);
        if (component_data_.out_torso_controller_command_active)
            torso_controller_command_.publish(component_data_.out_torso_controller_command);
        if (component_data_.out_gripper_left_active)
            gripper_left_.publish(component_data_.out_gripper_left);
        if (component_data_.out_gripper_right_active)
            gripper_right_.publish(component_data_.out_gripper_right);
    }
};

int main(int argc, char** argv)
{

    ros::init(argc, argv, "cob_teleop_v2");

    cob_teleop_v2_ros node;
    node.configure();

    ros::Rate loop_rate(50.0);

    while(node.n_.ok())
    {
        node.update();
        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
