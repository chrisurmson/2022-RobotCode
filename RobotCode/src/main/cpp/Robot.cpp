#include "Robot.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>


void 
Robot::RobotInit() {
  m_chooser.SetDefaultOption("Blue", blueAlliance);
  m_chooser.AddOption("RED", redAlliance);
  frc::SmartDashboard::PutData("Alliance Color", &m_chooser);
  try{
    navx = new AHRS(frc::SPI::Port::kMXP);
  } catch(const std::exception& e){
    std::cout << e.what() <<std::endl;
  }
  m_swerve.debug(*navx);
}


void 
Robot::RobotPeriodic() {
  //m_compressor.EnableDigital();
}


void 
Robot::AutonomousInit() {
  m_auto.ResetAuto();
  m_time = 0;
  m_swerve.ResetOdometry();
  m_swerve.GenerateTrajectory_1();
  m_swerve.Initialize();
  m_shooter.setState(Shooter::State::IDLE);
  m_intake.setState(Intake::State::IDLE);
  m_shooter.Zero();
  PDH.ClearStickyFaults();
  navx->Reset();
}


void 
Robot::AutonomousPeriodic() {
  m_time += m_timeStep;
  frc::SmartDashboard::PutNumber("time", m_time);
  m_auto.Periodic(m_time);
  // frc::SmartDashboard::PutNumber("Y", m_swerve.GetYPosition());
  // frc::SmartDashboard::PutNumber("X", m_swerve.GetXPostion());
  //m_swerve.UpdateOdometry(navx->GetYaw());

  switch(m_auto.getState()){
    case AutoMode::State::IDLE:
      m_intake.setState(Intake::IDLE);
      m_shooter.setState(Shooter::IDLE);
      m_swerve.Drive(0, 0, 0, 0, true);
      break;
    case AutoMode::State::DRIVEnINTAKE:
      m_intake.setState(Intake::State::RUN);
      m_shooter.setState(Shooter::State::LOAD);
      m_swerve.TrajectoryFollow(navx->GetYaw(), m_auto.getWaypointIndex());
      break;
    case AutoMode::State::SHOOT:
      m_shooter.setState(Shooter::State::SHOOT);
      break;
    case AutoMode::State::DRIVE:
      m_swerve.TrajectoryFollow(navx->GetYaw(), m_auto.getWaypointIndex());
      break;
    case AutoMode::State::INTAKE:
      // m_intake.setState(Intake::State::RUN);
      // m_shooter.setState(Shooter::State::LOAD);
      break;
    default:
      break;
  }

  m_intake.Periodic();
  m_shooter.Periodic();
}


void
Robot::TeleopInit() {
  //Put this in robotinit?
  if(m_chooser.GetSelected() == blueAlliance){
    m_shooter.setColor(true);
  } else {
    m_shooter.setColor(false);
  }

  m_time = 0;
  navx->Reset();
  m_shooter.setState(Shooter::State::IDLE);
  m_intake.setState(Intake::State::IDLE);
  m_shooter.Periodic();
  m_intake.Periodic();
  m_swerve.Initialize();
  m_shooter.Zero();
  PDH.ClearStickyFaults();
  m_intake.Deploy();
}


void 
Robot::TeleopPeriodic() {
  m_time += m_timeStep;
  double x1, y1, x2;
  x1 = l_joy.GetRawAxis(0) * 0.7;
  y1 = l_joy.GetRawAxis(1);
  x2 = r_joy.GetRawAxis(0);
  x1 = abs(x1) < 0.05 ? 0.0: x1;
  y1 = abs(y1) < 0.05 ? 0.0: y1;
  x2 = abs(x2) < 0.05 ? 0.0: x2;
  
  m_swerve.Drive(-x1*0.6, -y1, -x2, navx->GetYaw(), true);
  //m_swerve.UpdateOdometry(navx->GetYaw());
  // frc::SmartDashboard::PutNumber("Y", m_swerve.GetYPosition());
  // frc::SmartDashboard::PutNumber("X", m_swerve.GetXPostion());
  // frc::SmartDashboard::PutNumber("rot", navx->GetYaw());
  
  if(r_joy.GetTrigger()){
    m_intake.setState(Intake::State::RUN);
    m_shooter.setState(Shooter::State::LOAD);
  }
  else if(l_joy.GetTrigger()){
    m_shooter.setState(Shooter::State::SHOOT);
  }
  else if(abs(xbox.GetRawAxis(4)) > 0.2 ){
    m_shooter.Manual(xbox.GetRawAxis(4));
    m_shooter.setState(Shooter::State::MANUAL);
  }


    else if(xbox.GetRawButton(1)){
      //m_swerve.ResetOdometry();
      //m_swerve.SetDriveControllerPID();
      //m_swerve.SetDriveControllerROTPID();
      //m_shooter.setPID();
      //m_shooter.Calibrate();
      //m_swerve.SetPID();
    }
    else if(xbox.GetRawButton(2)){
      m_climber.ExtendsecondStage();
      std::cout <<"extend second" << std::endl;
    }
    else if(xbox.GetRawButton(3)){
      m_climber.ExtendfirstStage();
      std::cout << "retract first" << std::endl;
    }
    else if(xbox.GetRightBumper()){
      out = 0;
      m_climber.armExtension(out);
    }
    else if(abs(xbox.GetRawAxis(1)) > 0.1){
      out = xbox.GetRawAxis(1);
      if(abs(out) < 0.3){
        out = 0;
      }
      frc::SmartDashboard::PutNumber("output", out);
      m_climber.armExtension(out);
    } 
    


  else {
    m_shooter.setState(Shooter::State::IDLE);
    m_intake.setState(Intake::State::IDLE);
  }

  m_intake.Periodic(); 
  m_shooter.Periodic();
}


void 
Robot::TestInit() {
  m_time = 0;
  navx->Reset();
  m_swerve.ResetOdometry();
  m_swerve.GenerateTrajectory_1();
  m_shooter.setState(Shooter::State::IDLE);
  m_intake.setState(Intake::State::IDLE);
  m_shooter.Periodic();
  m_intake.Periodic();
  m_shooter.Zero();
  m_swerve.Initialize();
}


void 
Robot::TestPeriodic() {
  m_time += m_timeStep;
  frc::SmartDashboard::PutNumber("Y", m_swerve.GetYPosition());
  frc::SmartDashboard::PutNumber("X", m_swerve.GetXPostion());

  //m_swerve.UpdateOdometry(navx->GetYaw());
  m_swerve.TrajectoryFollow(navx->GetYaw(), false);

}


void 
Robot::DisabledInit() {}


void 
Robot::DisabledPeriodic() {}



#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif