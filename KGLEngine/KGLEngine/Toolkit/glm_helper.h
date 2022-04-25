// Developed by Kelin Lyu.
#ifndef glm_helper_h
#define glm_helper_h
class glm_helper {
public:
    static inline glm::vec3 getPosition(const glm::mat4 transform) {
        return(glm::vec3(transform[3][0], transform[3][1], transform[3][2]));
    }
    static inline glm::vec3 getEulerAngles(const glm::mat4 transform) {
        float x, y, z;
        glm::extractEulerAngleYZX(transform, y, z, x);
        return(glm::vec3(glm::degrees(x), glm::degrees(y), glm::degrees(z)));
    }
    static inline glm::vec3 getScale(const glm::mat4 transform) {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, translation, skew, perspective);
        return(scale);
    }
};
#endif
