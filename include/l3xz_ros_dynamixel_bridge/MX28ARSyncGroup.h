/**
 * Copyright (c) 2022 LXRobotics GmbH.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/l3xz_ros_dynamixel_bridge/graphs/contributors.
 */

#ifndef L3XZ_HEAD_CTRL_MX28ARCONTROL_H
#define L3XZ_HEAD_CTRL_MX28ARCONTROL_H

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <tuple>

#include <dynamixel++/dynamixel++.h>

#include "MX28AR.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace l3xz::MX28AR
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class SyncGroup : private dynamixelplusplus::SyncGroup
{
public:
  /* CTOR/DTOR ************************************************************************/
  SyncGroup(dynamixelplusplus::SharedDynamixel dyn_ctrl,
            dynamixelplusplus::Dynamixel::IdVect const & dyn_id_vect)
  : dynamixelplusplus::SyncGroup{dyn_ctrl, dyn_id_vect}
  { }


  /* MEMBER FUNCTIONS *****************************************************************/
  void setTorqueEnable (TorqueEnable const torque_enable);
  void setOperatingMode(OperatingMode const operating_mode);
  void setGoalPosition (std::vector<float> const & angle_deg_vect);
  void setGoalVelocity (std::vector<float> const & velocity_rpm_vect);

  std::vector<float> getPresentPosition();
};

class HeadSyncGroup : public SyncGroup
{
public:
  HeadSyncGroup(dynamixelplusplus::SharedDynamixel dyn_ctrl,
                dynamixelplusplus::Dynamixel::Id const pan_servo_id,
                dynamixelplusplus::Dynamixel::Id const tilt_servo_id)
  : SyncGroup(dyn_ctrl, dynamixelplusplus::Dynamixel::IdVect{pan_servo_id, tilt_servo_id})
  { }

  void setGoalPosition(float const pan_angle_deg, float const tilt_angle_deg)
  {
    SyncGroup::setGoalPosition(std::vector<float>{pan_angle_deg, tilt_angle_deg});
  }
  void setGoalVelocity(float const pan_velocity_rpm, float const tilt_velocity_rpm)
  {
    SyncGroup::setGoalVelocity(std::vector<float>{pan_velocity_rpm, tilt_velocity_rpm});
  }
  std::tuple<float, float> getPresentPosition_head()
  {
    std::vector<float> const head_angle_vect = getPresentPosition();
    float const pan_angle_deg  = head_angle_vect.at(0);
    float const tilt_angle_deg = head_angle_vect.at(1);
    return std::make_tuple(pan_angle_deg, tilt_angle_deg);
  }
};

class CoxaSyncGroup : public SyncGroup
{
public:
  CoxaSyncGroup(dynamixelplusplus::SharedDynamixel dyn_ctrl,
                dynamixelplusplus::Dynamixel::Id const left_front_coxa_servo_id,
                dynamixelplusplus::Dynamixel::Id const left_middle_coxa_servo_id,
                dynamixelplusplus::Dynamixel::Id const left_back_coxa_servo_id,
                dynamixelplusplus::Dynamixel::Id const right_front_coxa_servo_id,
                dynamixelplusplus::Dynamixel::Id const right_middle_coxa_servo_id,
                dynamixelplusplus::Dynamixel::Id const right_back_coxa_servo_id)
    : SyncGroup(dyn_ctrl, dynamixelplusplus::Dynamixel::IdVect{left_front_coxa_servo_id, left_middle_coxa_servo_id, left_back_coxa_servo_id, right_front_coxa_servo_id, right_middle_coxa_servo_id, right_back_coxa_servo_id})
  { }

  enum class CoxaId
  {
    Left_Front, Left_Middle, Left_Back, Right_Front, Right_Middle, Right_Back
  };
  static std::array<CoxaId, 6> constexpr COXA_ID_ARRAY =
  {
    CoxaId::Left_Front, CoxaId::Left_Middle, CoxaId::Left_Back, CoxaId::Right_Front, CoxaId::Right_Middle, CoxaId::Right_Back
  };

  void setGoalPosition(std::map<CoxaId, float /* angle_deg */> const & coxa_angle_deg_map)
  {
    std::vector<float> coxa_angle_deg_vect;

    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Left_Front));
    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Left_Middle));
    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Left_Back));
    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Right_Front));
    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Right_Middle));
    coxa_angle_deg_vect.push_back(coxa_angle_deg_map.at(CoxaId::Right_Back));

    SyncGroup::setGoalPosition(coxa_angle_deg_vect);
  }
  void setGoalPosition(float const coxa_angle_deg)
  {
    std::map<CoxaId, float /* angle_deg */> coxa_angle_deg_map;
    for(auto id : COXA_ID_ARRAY)
      coxa_angle_deg_map[id] = coxa_angle_deg;
    setGoalPosition(coxa_angle_deg_map);
  }

  void setGoalVelocity(std::map<CoxaId, float /* velocity_rpm*/> const & coxa_velocity_rpm_map)
  {
    std::vector<float> coxa_velocity_rpm_vect;

    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Left_Front));
    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Left_Middle));
    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Left_Back));
    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Right_Front));
    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Right_Middle));
    coxa_velocity_rpm_vect.push_back(coxa_velocity_rpm_map.at(CoxaId::Right_Back));

    SyncGroup::setGoalVelocity(coxa_velocity_rpm_vect);
  }
  void setGoalVelocity(float const coxa_velocity_rpm)
  {
    std::map<CoxaId, float /* velocity_rpm*/> coxa_velocity_rpm_map;
    for(auto id : COXA_ID_ARRAY)
      coxa_velocity_rpm_map[id] = coxa_velocity_rpm;
    setGoalVelocity(coxa_velocity_rpm_map);
  }
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* l3xz */

#endif //L3XZ_HEAD_CTRL_MX28ARCONTROL_H
