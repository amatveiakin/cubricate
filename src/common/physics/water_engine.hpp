#ifndef WATER_ENGINE_CPP
#define WATER_ENGINE_CPP


#include <cassert>
#include <cmath>
#include <set>

#include "common/linear_algebra.hpp"
#include "common/cube_geometry.hpp"


namespace WaterParams {
  const float NORMAL_VERTICAL_SATURATION_RATIO = 1.2;

//   const float SPLITTING_THRESHOLD   = M_SQRT2   * sqrt (NORMAL_VERTICAL_SATURATION_RATIO);
//   const float MERGING_THRESHOLD     = M_SQRT2   / sqrt (NORMAL_VERTICAL_SATURATION_RATIO);

  const float SPLITTING_THRESHOLD   = 1.1;
  const float MERGING_THRESHOLD     = M_SQRT2;

  const float MAX_SATURATION        = M_SQRT2   * NORMAL_VERTICAL_SATURATION_RATIO;
  const float MIN_SATURATION        = M_SQRT1_2 / NORMAL_VERTICAL_SATURATION_RATIO;
}


class WaterEngine {
public:
  WaterEngine ();
  ~WaterEngine ();

  void addWaterCube (Vec3i pos) {
    assert (waterCubes.find (pos) == waterCubes.end ());
    waterCubes.insert (pos);
  }
  void removeWaterCube (Vec3i pos) {
    // TODO: That's quite a dirty trick. Try to fix.
    auto it = waterCubes.find (pos);
    assert (it != waterCubes.end ());
    if (it == nextCubeIter)
      nextCubeIter++;
    waterCubes.erase (it);
  }

  void processWater ();

private:
  std::set <Vec3i, LexicographicCompareVec3i> waterCubes;
  std::set <Vec3i, LexicographicCompareVec3i >::iterator nextCubeIter;


  void processLowerNeighbour (Vec3i cube);
  void processUpperNeighbour (Vec3i cube);

  void processHorizontalNeighbour (Vec3i cube, Vec3i neighbourCube);
  void processVerticalWaterPair (Vec3i lowerCube, Vec3i upperCube);
  void processHorizontalWaterPair (Vec3i firstCube, Vec3i secondCube);

  // Returns true if splitting succeeded
  bool tryToSplit (Vec3i cube, Direction dir);
};


#endif