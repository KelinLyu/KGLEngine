// Developed by Kelin Lyu.
#ifndef assimp_helper_h
#define assimp_helper_h
class assimp_helper {
public:
    static inline glm::mat4 getMat4(const aiMatrix4x4& matrix) {
        glm::mat4 to;
        to[0][0] = matrix.a1; to[1][0] = matrix.a2; to[2][0] = matrix.a3; to[3][0] = matrix.a4;
        to[0][1] = matrix.b1; to[1][1] = matrix.b2; to[2][1] = matrix.b3; to[3][1] = matrix.b4;
        to[0][2] = matrix.c1; to[1][2] = matrix.c2; to[2][2] = matrix.c3; to[3][2] = matrix.c4;
        to[0][3] = matrix.d1; to[1][3] = matrix.d2; to[2][3] = matrix.d3; to[3][3] = matrix.d4;
        return to;
    }
    static inline glm::vec3 getVec3(const aiVector3D& vector) {
        return(glm::vec3(vector.x, vector.y, vector.z));
    }
    static inline glm::quat getQuat(const aiQuaternion& orientation) {
        return(glm::quat(orientation.w, orientation.x, orientation.y, orientation.z));
    }
};
#endif
