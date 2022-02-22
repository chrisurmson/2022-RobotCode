#include "frc2/command/SubsystemBase.h"
#include <frc/geometry/Rotation2d.h>
#include <frc/kinematics/SwerveDriveOdometry.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc/controller/PIDController.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <frc/controller/SimpleMotorFeedforward.h>
#include <frc/kinematics/SwerveModuleState.h>
#include <ctre/Phoenix.h>
#include "Constants.h"

class SwerveModule : public frc2::SubsystemBase {
    public: 
        SwerveModule(int moduleNum, int turningMotorID, int drivingMotorID, double zeroOffset, bool invertTurn, bool invertThrottle);
        void resetEncoders();
        
        //getters and setters
        double getTurningRadians();
        double getTurningDegrees() {return getTurningRadians()*57.2958;}
        frc::Rotation2d getHeading() { return frc::Rotation2d{units::radian_t{getTurningRadians()}};}
        double getVelocity();
        frc::SwerveModuleState getState() {return frc::SwerveModuleState{units::velocity::meters_per_second_t{getVelocity()}, getHeading()};}
        void setDesiredState(frc::SwerveModuleState state);
        void setPercentOutput(double output);
        void setBrakeMode(bool on); //true is brake, false is coast
        frc::Pose2d getPose() {return swerveModulePose;}
        void setPose(frc::Pose2d pose) {swerveModulePose = pose;}
        //update sdb if we want
        //can write a sim periodic class

        //motor stuff
        WPI_TalonFX mTurningMotor;
        WPI_TalonFX mDrivingMotor;
        double mZeroOffset;
        bool mInverted;

        //pid stuff
        static double kF;
        static double kP;
        static double kI;
        static double kD;
        int kI_Zone = 900;
        int kMaxIAccum = 1000000;
        int kErrorBand = 50;
        //idk if these values are true...
        int kCruiseVelocity = 14000;
        int kMotionAccel = kCruiseVelocity*10;

        frc::Pose2d swerveModulePose;

    private:
        int moduleNum;

        const static long STALL_TIMEOUT = 2000;
        long mStallTimeBegin = LLONG_MAX;

        double mTurnOutput;
        double mDriveOutput;

        //pid controllers
        frc::PIDController mDrivePID{ModuleConstants::kPDrivingController, 0, 0};        
        //frc::ProfiledPIDController</*reee idk the distancceee*/> mTurningPID{ModuleConstants::kpTurningController, 0, 0, /*trapezoid thing idk how to instentiate*/};
        //frc::SimpleMotorFeedforward</*reee distance*/> mDriveFeedFwd{0, 0, 0}; //todo tune
        //frc::SimpleMotorFeedforward</*reee distance*/> mTurnFeedFwd{0, 0, 0}; //todo tune

        //he defines some sim stuff that i'm not gonna do just yet


};