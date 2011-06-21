#include <cassert>

#ifdef CLIENT_APP
  #include "client/client_world.hpp"
#else // !CLIENT_APP
  #include "server/server_world.hpp"
#endif // !CLIENT_APP

#include "common/player.hpp"



Player::Player () {
  m_pos = m_viewFrame.origin ();
}

Player::~Player () {

}

#ifdef CLIENT_APP  // TODO: delete
// static inline Vec3i getCubeByPoint (Vec3d point, Vec3d direction) {
//   const float EPSILON = 0.00001;
//   Vec3i cube;
//   for (int i = 0; i < 3; ++i)
//     cube[i] = (int) floor (point[i] + xSgn (direction[i]) * EPSILON + 0.5);
//   return cube;
// }

static inline Vec3i getCubeByPoint (Vec3d point, Vec3d direction) {
  return worldToCube (point + direction * 0.00001);
}


CubeWithFace Player::getHeadOnCube() {
  Vec3d currentPoint = m_viewFrame.origin ();
  Vec3d forwardVector = m_viewFrame.dirForward ();
  Vec3i mapCenter = Vec3i::replicated (MAP_SIZE / 2);

  Vec3i cube = getCubeByPoint (currentPoint, forwardVector);
  Vec3i prevCube = cube;
  while  (  cubeValid (cube + mapCenter)
         && !cubeArray.cube_presents (XYZ_LIST (cube + mapCenter))) {
    Vec3d parameter;
    Vec3d nearestInt;
    for (int i = 0; i < 3; ++i) {
      nearestInt[i] = (forwardVector[i] > 0) ? (floor (currentPoint[i] - 0.5) + 1.5) : (ceil (currentPoint[i] + 0.5) - 1.5);
      parameter[i] = (nearestInt[i] - currentPoint[i]) / forwardVector[i];
      assert (parameter[i] >= 0);
    }
    float t = xMin (parameter[0], parameter[1], parameter[2]);
    assert (t >= 0);

    for (int i = 0; i < 3; ++i)
      currentPoint[i] += forwardVector[i] * t;

    prevCube = cube;
    cube = getCubeByPoint (currentPoint, forwardVector);
  }
  return CubeWithFace (cube, getAdjacentFace (cube, prevCube));
}
#endif // CLIENT_APP
