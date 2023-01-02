#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class Component {
  public:
    // Constructor
    // (the lower the update order, the earlier the component updates)
    Component(class Actor *owner, int updateOrder = 100);
    // Destructor
    virtual ~Component();
    // Update this component by delta time
    virtual void Update(float deltaTime);

    int GetUpdateOrder() const { return mUpdateOrder; }

  protected:
    // Owning actor
    class Actor *mOwner;
    // Update order of component
    int mUpdateOrder;
};

#endif // __COMPONENT_H__