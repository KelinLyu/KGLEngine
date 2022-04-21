// Developed by Kelin Lyu.
#ifndef Node_hpp
#define Node_hpp
#include "../Engine.hpp"
#define LIGHTS_LIMIT 50
struct FontCharacter;
class SpriteShader;
class ParticleShader;
class Geometry;
class ParticleRenderer;
class Sprite;
class TextRenderer;
class Animator;
class CameraNode;
class LightNode;
class ParticleNode;
class UINode;
class SpriteNode;
class TextNode;
class Font;
class Node {
protected:
    mat4 worldTransform;
    map<string, Node*> boneNodes;
    vector<Animator*> animators;
    int geometryInstancingIndex;
    map<Geometry*, vector<unsigned int>> frozenNodeGeometryInstancingIndices;
    bool hasUnfreezableGeometries;
    CameraNode* currentCameraNode;
    LightNode* currentLightNode;
    ParticleNode* currentParticleNode;
    UINode* currentUINode;
    SpriteNode* currentSpriteNode;
    TextNode* currentTextNode;
public:
    string name;
    Node* parent;
    vector<Node*> childNodes;
    bool isDisabled;
    unsigned int renderingBitMask;
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
    Node* orientationTargetNode;
    vector<Geometry*> geometries;
    Node();
    void addChildNode(Node* node);
    void removeFromParentNode();
    void loadUnitCube();
    void loadModelFile(string file);
    Animator* loadAnimator(string name, string file);
    Node* generateBoneNode(string boneName);
    virtual Node* copy();
    virtual Node* clone();
    void freeze();
    Animator* getAnimator(string name);
    void playAnimators(unsigned int mask, float fadeIn, float fadeOut = 0.0f);
    void stopAnimators(unsigned int mask, float fadeOut);
    void updateTransform();
    Node* getChildNode(string name);
    mat4 getWorldTransform();
    vec3 getWorldPosition();
    vec3 getWorldEulerAngles();
    vec3 getWorldScale();
    vec3 convertLocalPositionToWorld(vec3 localPosition);
    vec3 convertLocalVectorToWorld(vec3 localVector);
    vec3 getFrontVectorInWorld();
    vec3 getBackVectorInWorld();
    vec3 getLeftVectorInWorld();
    vec3 getRightVectorInWorld();
    vec3 getUpVectorInWorld();
    vec3 getDownVectorInWorld();
    vec3 getPositionOnScreen();
    CameraNode* convertToCameraNode();
    LightNode* convertToLightNode();
    ParticleNode* convertToParticleNode();
    UINode* convertToUINode();
    SpriteNode* convertToSpriteNode();
    TextNode* convertToTextNode();
    ~Node();
    void engineInitializeNode();
    void engineProcessNode(aiNode* node, const aiScene* scene);
    void engineUpdateNodeAnimators(mat4 parentWorldTransform);
    virtual void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data);
    virtual void engineCalculateNodeWorldTransform(mat4 parentWorldTransform);
    void engineRecursivelyFreezeChildNodes(vector<Geometry*>* allGeometries, map<Geometry*, vector<unsigned int>>* indices);
};
class CameraNode final: public Node{
public:
    float field;
    float near;
    float far;
    CameraNode(float field, float near, float far);
    Node* copy() override;
    Node* clone() override;
    mat4 getOrthogonalProjectionTransform();
    mat4 getProjectionTransform();
    mat4 getViewTransform();
    ~CameraNode() = default;
};
class LightNode final: public Node {
private:
    unsigned int lightType;
    float cameraNodeDistance;
public:
    vec3 colorFactor;
    float highlightIntensity;
    float attenuationExponent;
    float range;
    float penetrationRange;
    float innerAngle;
    float outerAngle;
    unsigned int lightingBitMask;
    LightNode(vec3 color);
    Node* copy() override;
    Node* clone() override;
    void setAmbientLight();
    void setDirectionalLight();
    void setPointLight(float attenuationExponent, float range);
    void setSpotLight(float attenuationExponent, float range, float innerAngle, float outerAngle);
    ~LightNode() = default;
    void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) override;
    unsigned int engineGetLightType();
    void engineConfigurateShader(Shader* shader, int index);
};
class ParticleNode final: public Node {
private:
    bool isPlaying;
    bool hasLimit;
    unsigned int maxAmount;
    unsigned int currentAmount;
    unsigned int birthrate;
    float duration;
    float durationVariation;
    ParticleRenderer* renderer;
    ParticleShader* shader;
    unsigned int productionAmount;
    float productionTime;
    float productionTimeInterval;
    float particleDuration;
    float particleDurationVariation;
    unsigned int emissionShape;
    float innerSphereRadius;
    float outerSphereRadius;
    vec3 boxSize;
    vector<vec4> colorKeys;
    vector<float> progressKeys;
    unsigned int spriteSheetAnimationInitialFrameRange;
    unsigned int spriteSheetAnimationFPS;
    unsigned int spriteSheetAnimationFPSVariation;
public:
    float renderingOrder;
    bool useLocalSpace;
    bool isAdditive;
    bool youngestFirst;
    float spreadingAngle;
    float initialSpeed;
    float initialSpeedVariation;
    float speedAcceleration;
    float speedAccelerationVariation;
    vec3 acceleration;
    vec3 accelerationVariation;
    float initialRotation;
    float initialRotationVariation;
    float rotatingSpeed;
    float rotatingSpeedVariation;
    bool randomizeRotatingDirection;
    float initialScale;
    float initialScaleVariation;
    float scalingSpeed;
    float scalingSpeedVariation;
    Texture* texture;
    vec4 color;
    ParticleNode(unsigned int birthrate, float duration, float durationVariation);
    Node* copy() override;
    Node* clone() override;
    void setUnlimitedAmount();
    void setMaxAmount(unsigned int amount);
    void setEmissionSphere(float innerRadius, float outerRadius);
    void setEmissionBox(vec3 size);
    void setColorAnimation(vec4 color, float progress);
    void setSpriteSheetAnimation(unsigned int rows, unsigned int columns,
                                 unsigned int initialFrameRange,
                                 unsigned int FPS, unsigned int FPSVariation);
    void play();
    void stop();
    void reset();
    ~ParticleNode();
    void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) override;
};
class UINode: public Node {
private:
    mat4 renderingTransform;
public:
    vec2 screenPosition;
    vec2 position;
    float rotation;
    vec2 scale;
    vec2 size;
    float alpha;
    float renderingOrder;
    UINode();
    Node* copy() override;
    Node* clone() override;
    void loadSprite(Sprite* sprite);
    vec2 convertScreenPositionToLocal(vec2 screenPosition);
    bool checkSizeIncludesScreenPosition(vec2 screenPosition);
    ~UINode() = default;
    void engineInitializeUINode();
    void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) override;
    void engineCalculateNodeWorldTransform(mat4 parentWorldTransform) override;
};
class SpriteNode: public UINode {
private:
    SpriteShader* spriteShader;
public:
    vec4 color;
    Texture* texture;
    vec3 multiplyColor;
    Texture* multiplyTexture;
    float multiplyIntensity;
    vec3 emissionColor;
    Texture* emissionTexture;
    float emissionIntensity;
    SpriteNode(vec2 size);
    Node* copy() override;
    Node* clone() override;
    void setSemitransparent();
    void setAdditive();
    ~SpriteNode();
    void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) override;
    void engineCalculateNodeWorldTransform(mat4 parentWorldTransform) override;
};
class TextNode final: public UINode {
private:
    Font* font;
    float lineHeight;
    float lineLength;
    float lineDistance;
    int alignmentX;
    int alignmentY;
    float currentAlpha;
    vec4 currentColor;
    string currentText;
    TextRenderer* textRenderer;
public:
    vec4 color;
    string text;
    TextNode(Font* font, float lineHeight, float lineLength, float lineDistance);
    Node* copy() override;
    Node* clone() override;
    void setLeftHorizontalAlignment();
    void setCenterHorizontalAlignment();
    void setRightHorizontalAlignment();
    void setTopVerticalAlignment();
    void setCenterVerticalAlignment();
    void setBottomVerticalAlignment();
    ~TextNode();
    void enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) override;
    void engineCalculateNodeWorldTransform(mat4 parentWorldTransform) override;
};
#endif
