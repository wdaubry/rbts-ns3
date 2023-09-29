#ifndef PDC_APPLICATION
#define PDC_APPLICATION

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/traced-callback.h"
#include "ns3/random-variable-stream.h"

#include <map>
#include <string>

#include "PMU-application.h"

namespace ns3 {

class Socket;
class Packet;
class InetSocketAddress;
class Inet6SocketAddress;

/**
 * \ingroup helicsapplication
 * \brief A Helics Application
 *
 * Every packet sent should be returned by the server and received here.
 */
class PDCApplication : public PMUApplication
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  PDCApplication ();

  virtual ~PDCApplication ();

  virtual void SetSubscriptions (std::shared_ptr<helics::CombinationFederate> fed, std::vector<std::string> keys) override;
  void SetTimer (const double);
  double GetTimer (void) const;
  void SetNbAggregates (const unsigned int);
  virtual void StartSampling (helics::Time time) override;
  void SetSeed (int seed);

protected:
  virtual void DoDispose (void) override;
  virtual void StartApplication (void) override;
  virtual void StopApplication (void) override;
  virtual void DoRead (std::unique_ptr<helics::Message> message) override;
  virtual void GenerateMeasurement (helics::Time time) override;
  
  double timer;
  unsigned int nb_aggregates;
  std::map<std::string, std::vector<std::string>> payloads;
  std::map<std::string, EventId> send_events;
  std::map<std::string, double> first_arrival;
};

} // namespace ns3

#endif /* PDC_APPLICATION */
