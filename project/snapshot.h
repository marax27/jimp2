#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

// Representation of a single step of pedestrian simulation.

// The snapshot should contain following information:
// - position of pedestrians
// - static floor field essentials: walls, exits
// - state of dynamic floor field
// - simulation constants.

#include <vector>

class Snapshot{
public:
	std::vector<Pedestrian> pedestrians;

private:
	;
};

#endif  //_SNAPSHOT_H_