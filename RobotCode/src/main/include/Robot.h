#pragma once

#include <string>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <WheelDrive.h>
#include <SwerveDrive.h>
#include <frc/Joystick.h>
#include <frc/XboxController.h>
#include <frc/PowerDistribution.h>
#include <AHRS.h>
#include <frc/Timer.h>
#include <Trajectory.h>
#include <Constants.h>
#include <frc/Compressor.h>
#include "Intake.h"
#include "Shooter.h"
#include "Climber.h"
//#include "FramePeriod.h"
#include "AutoMode.h"


frc::Joystick l_joy{OIConstants::l_joy_port};
frc::Joystick r_joy{OIConstants::r_joy_port};
frc::XboxController xbox{OIConstants::O_joy_port};
//frc::Compressor m_compressor{frc::PneumaticsModuleType::REVPH};
frc::PowerDistribution PDH{1, frc::PowerDistribution::ModuleType::kRev};

double m_time = 0;
double m_timeStep = GeneralConstants::timeStep;

AHRS *navx;

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void DisabledInit() override;
  void DisabledPeriodic() override;
  void TestInit() override;
  void TestPeriodic() override;


 private:

  AutoMode m_auto;
  //FramePeriod m_frameperiod;
  SwerveDrive m_swerve;
  Intake m_intake;
  Shooter m_shooter;
  Climber m_climber;

  double out;

  frc::SendableChooser<std::string> m_chooser;
  const std::string blueAlliance = "BLUE";
  const std::string redAlliance = "RED";
  std::string m_autoSelected;
};