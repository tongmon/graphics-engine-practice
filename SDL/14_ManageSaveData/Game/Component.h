#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "MyMath.h"
#include <rapidjson/document.h>

class Component {
  public:
    enum TypeID {
        TComponent = 0,
        TAudioComponent,
        TBallMove,
        TBoxComponent,
        TCameraComponent,
        TFollowCamera,
        TMeshComponent,
        TMoveComponent,
        TSkeletalMeshComponent,
        TSpriteComponent,
        TMirrorCamera,
        TPointLightComponent,
        TTargetComponent,

        NUM_COMPONENT_TYPES
    };

    static const char *TypeNames[NUM_COMPONENT_TYPES];

    // Constructor
    // (the lower the update order, the earlier the component updates)
    Component(class Actor *owner, int updateOrder = 100);
    // Destructor
    virtual ~Component();
    // Update this component by delta time
    virtual void Update(float deltaTime);
    // Process input for this component
    virtual void ProcessInput(const uint8_t *keyState) {}
    // Called when world transform changes
    virtual void OnUpdateWorldTransform();

    class Actor *GetOwner() { return mOwner; }
    int GetUpdateOrder() const { return mUpdateOrder; }

    virtual TypeID GetType() const = 0;

    // Load/Save
    virtual void LoadProperties(const rapidjson::Value &inObj);
    virtual void SaveProperties(rapidjson::Document::AllocatorType &alloc,
                                rapidjson::Value &inObj) const;

    // Create a component with specified properties
    template <typename T>
    static Component *Create(class Actor *actor, const rapidjson::Value &inObj) {
        // Dynamically allocate component of type T
        T *t = new T(actor);
        // Call LoadProperties on new component
        t->LoadProperties(inObj);
        return t;
    }

  protected:
    // Owning actor
    class Actor *mOwner;
    // Update order of component
    int mUpdateOrder;
};

#endif // __COMPONENT_H__