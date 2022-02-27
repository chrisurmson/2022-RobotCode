#include "FramePeriod.h"

FramePeriod::FramePeriod() {
    talons.clear();
}

void FramePeriod::addTalon(WPI_TalonFX& talon) {
   talons.push_back(&talon);
    setFramePeriods(talon);
}

void FramePeriod::periodic() {
    for (WPI_TalonFX* talon: talons) {
        checkFramePeriods(talon);
    }
}

void FramePeriod::setFramePeriods(WPI_TalonFX& talon) {
    ErrorCollection err;
    for (int i = 0; i < statusFrameAttempts; i++) {
     //   err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, st));
      //  err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_2_Feedback0, st));
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_3_Quadrature, lt));
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_4_AinTempVbat, lt));
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_8_PulseWidth, lt));
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_10_MotionMagic, lt));
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_12_Feedback1, lt));
     //   err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_13_Base_PIDF0, st)); //idk if we actually use
        err.NewError(talon.SetStatusFramePeriod(StatusFrameEnhanced::Status_14_Turn_PIDF1, lt));
        if (err.GetFirstNonZeroError() == ErrorCode::OK) return;
    }
    
}

void FramePeriod::checkFramePeriods(WPI_TalonFX* talon) {
    if ((*talon).HasResetOccurred()) setFramePeriods(*talon);   
}