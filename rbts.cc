/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

#include "../../examples/tutorial/PMU-application.h"
#include "../../examples/tutorial/PDC-application.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("RBTS_Network");


int
main(int argc, char* argv[])
{

  std::string path = std::filesystem::current_path();

  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  bool verbose = false;

	std::string dataRate_p2p = "800Mbps"; // Source : Frederic Sabot
  double sampling_period_PMU = 0.02;
  bool padding_b = true;
  char padding_c = '*';
  uint16_t padding_l = 500;  // Assumed that the aggregated packets also have a size of 500 bytes (like the PMU ones)
  uint16_t padding_SPDC = 500;

  double start_time = 1;
  double stop_time = 5;
  double cleanup_time = 5;

  /*
  HelicsHelper helicsHelper;

  NS_LOG_INFO ("Calling helicsHelper.SetupApplicationFederate");
  std::string path = std::filesystem::current_path();
  std::string json = path + "/contrib/helics/examples/IEEE39.json";
  helicsHelper.SetupApplicationFederateWithConfig(json);
  */

  std::string inputFile = path + "/examples/tutorial/rbts.csv";
  std::ifstream topologyStream(inputFile.c_str(), std::ifstream::in);
  std::string line, word;
  std::vector<std::vector<std::string>> topology;

  std::getline(topologyStream, line);
  if (line != "End 1; End 2; Length" && line != "End 1; End 2; Length;") {
    std::cout << line << std::endl;
    throw std::invalid_argument("Invalid csv header or missing file");
  }

  while (std::getline(topologyStream, line)) {
    std::vector<std::string> row;
    std::stringstream s(line);
    while (std::getline(s, word, ';')) {
      row.push_back(word);
    }
    topology.push_back(row);
    line.clear();
  }
  topologyStream.close();


  NodeContainer nodes;
  nodes.Create(7);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue (dataRate_p2p));
  NetDeviceContainer p2pDevices;

  Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));  // Only useful when global routing is used
  Ipv4GlobalRoutingHelper globalRouting;
  InternetStackHelper stack;
  stack.SetRoutingHelper (globalRouting);
  stack.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");


  for (auto &it : topology) {
    int node_1 = std::stoi(it[0]) - 1;
    int node_2 = std::stoi(it[1]) - 1;
    double length = std::stof(it[2]);
    double delay = length * 5000;  // 5000ns per km (i.e. assuming 1.5 refractive index)
    std::string delay_s = std::to_string(delay) + "ns";

    // delay_s = "5ms";  // Replace delay by value from GECO test case

    p2p.SetChannelAttribute ("Delay", StringValue (delay_s));
    NodeContainer n_links = NodeContainer (nodes.Get (node_1), nodes.Get (node_2));
    NetDeviceContainer n_devices = p2p.Install (n_links);
    ipv4.Assign (n_devices);

    ipv4.NewNetwork ();
  }

  ApplicationContainer PMUs;

  PMUs.Add (helicsHelper.InstallPMU (nodes.Get (std::stoi (name) - 1), name, destination, sampling_period_PMU, stop_time, helics_federate, keys, std::rand (), true, '*', 500));
  PMU_names.push_back (name);

  ApplicationContainer PDCs;


}
