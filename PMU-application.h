/*
Copyright (c) 2017-2019, Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See the top-level NOTICE for additional details. All rights reserved.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef PMU_APPLICATION_H
#define PMU_APPLICATION_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/traced-callback.h"
#include "ns3/random-variable-stream.h"

#include <map>
#include <string>

#include <random>
#include <iomanip>

namespace ns3 {

class Socket;
class Packet;
class InetSocketAddress;
class Inet6SocketAddress;

/**
 * \ingroup HelicsApplication
 * \brief A Helics Application
 *
 * Every packet sent should be returned by the server and received here.
 */
class PMUApplication : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  PMUApplication ();

  virtual ~PMUApplication ();

  void SetEndpointName (const std::string &name, bool is_global);

  void SetPadding (const char, uint16_t);
  
  void SetSeed (int seed);

  /**
   * \brief set the name of the destination node
   * \param name name
   */
  void SetDestination (const std::string &name);

  std::string GetDestination (void) const;

  void SetSamplingPeriodSeconds (double period);

  void SetStopTimeSeconds (double stopTime);

  virtual void StartSampling ();

protected:
  virtual void DoDispose (void) override;
  virtual void StartApplication (void) override;
  virtual void StopApplication (void) override;
  virtual void GenerateMeasurement ();

  char padding_char;
  bool padding_b = false;
  u_int16_t padded_size;
  std::vector<helics::Input> subscriptions;
  std::default_random_engine m_eng;

  std::string m_destination; //!< name of this application
  double sampling_period;
  bool turn_on = false;
  double stop_time;
};

} // namespace ns3

#endif /* PMU_APPLICATION_H */
