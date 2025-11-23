#include "professor_time_node.hpp"
#include "section_time_node.hpp"
#include <graphalgorithms.hpp>

class FlowNetwork
{
private:
    Designar::Digraph<FlowNode, uint> flow_network;
};