#ifndef CT_ICP_TYPES_HPP
#define CT_ICP_TYPES_HPP

#include <map>
#include <unordered_map>
#include <list>

#include "Eigen/Dense"
#include "Eigen/StdVector"


namespace ct_icp {

    // A Point3D
    struct Point3D {
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        Eigen::Vector3d raw_pt; // Raw point read from the sensor
        Eigen::Vector3d pt; // Corrected point taking into account the motion of the sensor during frame acquisition
        double alpha_timestamp = 0.0; // Relative timestamp in the frame in [0.0, 1.0]
        int index_frame = -1; // The frame index

        Point3D() = default;
    };

    // A Trajectory Frame
    struct TrajectoryFrame {
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        Eigen::Matrix3d begin_R;
        Eigen::Vector3d begin_t;
        Eigen::Matrix3d end_R;
        Eigen::Vector3d end_t;

        TrajectoryFrame() = default;
    };


    // Voxel
    // Note: Coordinates range is in [-32 768, 32 767]
    struct Voxel {

        Voxel(short x, short y, short z) : x(x), y(y), z(z) {}

        bool operator==(const Voxel &vox) const { return x == vox.x && y == vox.y && z == vox.z; }

        inline bool operator<(const Voxel &vox) const {
            return x < vox.x || (x == vox.x && y < vox.y) || (x == vox.x && y == vox.y && z < vox.z);
        }

        short x;
        short y;
        short z;
    };

    typedef std::unordered_map<Voxel, std::list<Eigen::Vector3d>> VoxelHashMap;
    typedef std::vector<Eigen::Vector3d, EIGEN_ALIGNED_ALLOCATOR<Eigen::Vector3d>> ArrayVector3d;
    typedef std::vector<Eigen::Matrix4d, EIGEN_ALIGNED_ALLOCATOR<Eigen::Matrix4d>> ArrayMatrix4d;
    typedef ArrayMatrix4d ArrayPoses;

} // namespace CT_ICP


// Specialization of std::hash for our custom type Voxel
namespace std {


    template<>
    struct hash<ct_icp::Voxel> {
        std::size_t operator()(const ct_icp::Voxel &vox) const {
            // const std::hash<int32_t> hasher;
            const size_t kP1 = 73856093;
            const size_t kP2 = 19349669;
            const size_t kP3 = 83492791;

            // return ((hasher(vox.x) ^ (hasher(vox.y) << 1)) >> 1) ^ (hasher(vox.z) << 1) >> 1;
            return vox.x * kP1 + vox.y * kP2 + vox.z * kP3;
        }
    };
}

#endif //CT_ICP_TYPES_HPP