#include "SwerveDrive.h"

//Constructor
SwerveDrive::SwerveDrive(){}


//Drive function contains Swerve Inverse Kinematics
void 
SwerveDrive::Drive(double x1, double y1, double x2, double rot, bool fieldOrient){
    double r = sqrt(m_L*m_L + m_W*m_W);
    y1 *= -1;
    x1 *= -1;
    x2 *= -1;
    rot *= M_PI/180;

    if(fieldOrient){
        m_temp = y1*cos(rot) + x1*sin(rot);
        x1 = -y1*sin(rot) + x1*cos(rot);
        y1 = m_temp;
    }

    double a = x1 - x2 * (m_L/r);
    double b = x1 + x2 * (m_L/r);
    double c = y1 - x2 * (m_W/r);
    double d = y1 + x2 * (m_W/r);

    double backRightSpeed = sqrt((a*a) + (d*d));
    double backLeftSpeed = sqrt((a*a) + (c*c));
    double frontRightSpeed = sqrt((b*b) + (d*d));
    double frontLeftSpeed = sqrt((b*b) + (c*c));

    double backRightAngle = atan2(a, d) * 180/ M_PI;
    double backLeftAngle = atan2(a, c) *180/ M_PI;
    double frontRightAngle = atan2(b, d) *180/ M_PI;
    double frontLeftAngle = atan2(b, c) *180/ M_PI;

    frc::SmartDashboard::PutNumber("Rotation",rot);

    m_backRight.drive(-backRightSpeed, backRightAngle);
    m_backLeft.drive(-backLeftSpeed, backLeftAngle);
    m_frontRight.drive(-frontRightSpeed, frontRightAngle);
    m_frontLeft.drive(-frontLeftSpeed, frontLeftAngle);
}


//Updates the Odometry
void
SwerveDrive::UpdateOdometry(double theta){
    double BR_A = m_backRight.getAngle();
    double BR_V = m_backRight.getVelocity();
    double BL_A = m_backLeft.getAngle();
    double BL_V = m_backLeft.getVelocity();
    double FR_A = m_frontRight.getAngle();
    double FR_V = m_frontRight.getVelocity();
    double FL_A = m_frontLeft.getAngle();
    double FL_V = m_frontLeft.getVelocity();
    m_odometry.updateOdometry(BR_A, BR_V, BL_A, BL_V, FR_A, FR_V,
        FL_A, FL_V, theta);
}


//Trajectory Following for Swerve
void
SwerveDrive::TrajectoryFollow(double rot, bool vel){

    size_t index = m_trajectory_1.getIndex();
    if( (abs(GetYPosition() - m_trajectory_1.getY(index)) < 0.2)
        && (abs(GetXPostion() - m_trajectory_1.getX(index)) < 0.2)
        && (abs(rot - m_trajectory_1.getRotation(index))) < 3){
        m_trajectory_1.Progress();
    }

    frc::SmartDashboard::PutNumber("YError", abs(GetYPosition() - m_trajectory_1.getY(index)));
    frc::SmartDashboard::PutNumber("XError", abs(GetXPostion() - m_trajectory_1.getX(index)));
    frc::SmartDashboard::PutNumber("RotError", (abs(rot - m_trajectory_1.getRotation(index))));

    m_swerveController.updatePosition(GetYPosition(), GetXPostion(), rot);
    double forward = m_swerveController.calculateForward(m_trajectory_1.getY(index));
    double strafe = m_swerveController.calculateStrafe(m_trajectory_1.getX(index));
    double angle = m_swerveController.calculateRotation(m_trajectory_1.getRotation(index));
    
    // frc::SmartDashboard::PutNumber("fwdOut", forward);
    // frc::SmartDashboard::PutNumber("strOut", strafe);
    // frc::SmartDashboard::PutNumber("rotOut", angle);
    // frc::SmartDashboard::PutNumber("index", index);
    Drive(strafe, forward, angle + calcYawStraight(m_trajectory_1.getRotation(index), rot), rot, true);
}


//Function for generating trajectory_1
void
SwerveDrive::GenerateTrajectory_1(){
    m_trajectory_1.clearTrajectory();
    Trajectory::Waypoint p1(8, 4, 0, 0);
    Trajectory::Waypoint p2(1, 4, 0, 90);
    Trajectory::Waypoint p3(8, 3, 0, -90);
    Trajectory::Waypoint p4(0, 0, 0, 0);

    m_trajectory_1.addWaypoint(p1);
    m_trajectory_1.addWaypoint(p2);
    m_trajectory_1.addWaypoint(p3);
    m_trajectory_1.addWaypoint(p4);
}


void
SwerveDrive::GeneratePath(){
    m_trajectory_2.clearTrajectory();
    Trajectory::Waypoint p1(0, 1, 2, 0);
    Trajectory::Waypoint p2(1, 3, 3, 0);
    Trajectory::Waypoint p3(3, 6, 5, 0);
    Trajectory::Waypoint p4(8, 5, 0, 0);

    m_trajectory_2.addWaypoint(p1);
    m_trajectory_2.addWaypoint(p2);
    m_trajectory_2.addWaypoint(p3);
    m_trajectory_2.addWaypoint(p4);    
}


//Return y position of the robot
double
SwerveDrive::GetYPosition(){
    return m_odometry.getY();
}


//Return x position of the robot
double
SwerveDrive::GetXPostion(){
    return m_odometry.getX();
}


double
SwerveDrive::GetYSpeed(){
    return m_odometry.getYSpeed();
}


double
SwerveDrive::GetXSpeed(){
    return m_odometry.getXSpeed();
}


//Rotation correction - P controller
double 
SwerveDrive::calcYawStraight(double targetAngle, double currentAngle){
    double error = targetAngle - currentAngle;
    return 0.01*error;
}


//Function to set PID for Swerve Drive Controller
void
SwerveDrive::SetDriveControllerPID(){
    m_swerveController.setPID();
}


void
SwerveDrive::SetDriveControllerROTPID(){
    m_swerveController.setRotPID();
}


//REDUNDANT!! FIX
//Function to Set PID for Swerve Modules
void 
SwerveDrive::SetPID(){
    m_backRight.setPID();
    m_backLeft.setPID();
    m_frontRight.setPID();
    m_frontLeft.setPID();
}


//Reset Odometry
void
SwerveDrive::ResetOdometry(){
    m_odometry.Reset();
}


//For initialization of Swerve Modules
void
SwerveDrive::Initialize(){
    m_backLeft.initialization(m_BL_Offset);
    m_backRight.initialization(m_BR_Offset);
    m_frontRight.initialization(m_FR_Offset);
    m_frontLeft.initialization(m_FL_Offset);    
}


//Reset Swerve Module Encoders
void
SwerveDrive::ResetEncoders(){
    m_backLeft.resetEncoder();
    m_backRight.resetEncoder();
    m_frontLeft.resetEncoder();
    m_frontRight.resetEncoder();
}


//Helper Function
void 
SwerveDrive::debug(){
    //frc::SmartDashboard::PutNumber("backrightVelocity", m_backRight.getVelocity());
    //m_trajectory_1.getX(0);
    m_backLeft.Debug();
}