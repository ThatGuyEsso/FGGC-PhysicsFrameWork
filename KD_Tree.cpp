#include "KD_Tree.h"

void KD_Tree::AddNodeRecursive(std::vector<GameObject*> gameObjectsInSet,Axis axis, KD_Node* parent)
{
    if (!_rootNode) {
        _rootNode = new KD_Node(0);
        _rootNode->objectSet = gameObjectsInSet;
        _rootNode->point = GetMedianPositionFormSet(gameObjectsInSet);
        axis = Axis::X_axis;

        if (_rootNode->depth < maxDepth&& gameObjectsInSet.size()>=minObjectsToSplit) {
            KD_Node* left = _rootNode->left = new KD_Node(_rootNode->depth++);
            KD_Node* right = _rootNode->left = new KD_Node(_rootNode->depth++);
       
            left->objectSet = GetObjectsInSplit(_rootNode->objectSet, false, axis, _rootNode->point.x);
            right->objectSet = GetObjectsInSplit(_rootNode->objectSet, true, axis, _rootNode->point.x);
            if (left->depth > maxDepth || left->objectSet.size() < minObjectsToSplit&&
                right->depth > maxDepth || right->objectSet.size() < minObjectsToSplit) {
                leaves.push_back(_rootNode);
            }
            else if (left->depth > maxDepth || left->objectSet.size() < minObjectsToSplit) {

                 AddNodeRecursive(right->objectSet, Axis::Y_axis, right);
            }
            else if (right->depth > maxDepth || right->objectSet.size() < minObjectsToSplit) {

                 AddNodeRecursive(left->objectSet, Axis::Y_axis, left);
            }
            else {
                AddNodeRecursive(right->objectSet, Axis::Y_axis, right);
                AddNodeRecursive(left->objectSet, Axis::Y_axis, left);
            }
        }
        else {
            leaves.push_back(_rootNode);
        }
   
    }
    else {
        if (parent == nullptr) {

         
            return;
        }
        KD_Node* currentNode = new KD_Node(parent->depth++);

        if (currentNode->depth > maxDepth || gameObjectsInSet.size() < minObjectsToSplit) {
            leaves.push_back(parent);
            return;

        }
        currentNode->point=(GetMedianPositionFormSet(gameObjectsInSet));
            switch (axis)
            {
            case Axis::X_axis:

                parent->left = new KD_Node(currentNode->depth);
                parent->left->point = currentNode->point;
                parent->left->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.x);
                AddNodeRecursive(parent->left->objectSet, Axis::Y_axis, parent->left);
                if (parent->left->depth < maxDepth || parent->left->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->left->objectSet, Axis::Y_axis, parent->left);
                }
                parent->right = new KD_Node(currentNode->depth);
                parent->right->point = currentNode->point;
                parent->right->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.x);
                if (parent->right->depth < maxDepth || parent->right->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->right->objectSet, Axis::Y_axis, parent->right);
                }
                break;
            case Axis::Y_axis:
                parent->left = new KD_Node(currentNode->depth);
                parent->left->point = currentNode->point;
                parent->left->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.y);
                if (parent->left->depth < maxDepth || parent->left->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->left->objectSet, Axis::Z_axis, parent->left);
                }
                parent->right = new KD_Node(currentNode->depth);
                parent->right->point = currentNode->point;
                parent->right->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.y);
                if (parent->right->depth < maxDepth || parent->right->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->right->objectSet, Axis::Z_axis, parent->right);
                }
                break;
            case Axis::Z_axis:
                parent->left = new KD_Node(currentNode->depth);
                parent->left->point = currentNode->point;
                parent->left->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.z);
                if (parent->left->depth < maxDepth || parent->left->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->left->objectSet, Axis::X_axis, parent->left);
                }
                parent->right = new KD_Node(currentNode->depth);
                parent->right->point = currentNode->point;
                parent->right->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.z);
                if (parent->right->depth < maxDepth || parent->right->objectSet.size() >= minObjectsToSplit) {

                    AddNodeRecursive(parent->right->objectSet, Axis::X_axis, parent->right);
                }
                break;
    
            }


    }

  
     //while (!current->IsLeaf()) {
     //    if (current->objectSet.size() < 2) return; //If there is less than 2 in object quit

     //    switch (axis)
     //    {
     //    case KD_Tree::X_axis:


     //        current->left = new KD_Node;
     //        break;
     //    case KD_Tree::Y_axis:
     //        break;
     //    case KD_Tree::Z_axis:
     //        break;
     //    default:
     //        break;
     //    }
     //}
    

}

Vector3D KD_Tree::GetMedianPositionFormSet(std::vector<GameObject*> gameObjectsInSet)
{
    Vector3D vectorSum= Vector3D();

    for (GameObject* object : gameObjectsInSet) {
        vectorSum += object->GetPosition();
    }

    return vectorSum/ gameObjectsInSet.size();
}

std::vector<GameObject*> KD_Tree::GetObjectsInSplit(std::vector<GameObject*> gameObjectsInSet, bool isGreater, Axis axis, float conditionVal)
{
    std::vector<GameObject*> newSet;
    SphereCollider* sphereCol;
    AABoxCollider* AABB;
    switch (axis)
    {
    case Axis::X_axis:
        if (isGreater) {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                        case ColliderType::Sphere:
                            sphereCol = (SphereCollider*)objCollider;
                            furhestPoint = objCollider->GetTransform()->GetPosition().x + sphereCol->GetRadius();
                            if (furhestPoint > conditionVal) {
                                newSet.push_back(object);
                            }
                        break;


                        case ColliderType::AABB:
                            AABB = (AABoxCollider*)objCollider;
                            furhestPoint = AABB->GetMaxSize().x;
                            if (furhestPoint > conditionVal) {
                                newSet.push_back(object);
                            }
                            break;
                    }
                }
            }
        }
        else {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                    case ColliderType::Sphere:
                        sphereCol = (SphereCollider*)objCollider;
                        furhestPoint = objCollider->GetTransform()->GetPosition().x - sphereCol->GetRadius();
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;


                    case ColliderType::AABB:
                        AABB = (AABoxCollider*)objCollider;
                        furhestPoint = AABB->GetMinSize().x;
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;
                    }
                }
            }
        }
        break;
    case Axis::Y_axis:
        if (isGreater) {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                    case ColliderType::Sphere:
                        sphereCol = (SphereCollider*)objCollider;
                        furhestPoint = objCollider->GetTransform()->GetPosition().y + sphereCol->GetRadius();
                        if (furhestPoint > conditionVal) {
                            newSet.push_back(object);
                        }
                        break;


                    case ColliderType::AABB:
                        AABB = (AABoxCollider*)objCollider;
                        furhestPoint = AABB->GetMaxSize().y;
                        if (furhestPoint > conditionVal) {
                            newSet.push_back(object);
                        }
                        break;
                    }
                }
            }
        }
        else {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                    case ColliderType::Sphere:
                        sphereCol = (SphereCollider*)objCollider;
                        furhestPoint = objCollider->GetTransform()->GetPosition().y - sphereCol->GetRadius();
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;


                    case ColliderType::AABB:
                        AABB = (AABoxCollider*)objCollider;
                        furhestPoint = AABB->GetMinSize().y;
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;
                    }
                }
            }
        }
        break;
    case Axis::Z_axis:
        if (isGreater) {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                    case ColliderType::Sphere:
                        sphereCol = (SphereCollider*)objCollider;
                        furhestPoint = objCollider->GetTransform()->GetPosition().z + sphereCol->GetRadius();
                        if (furhestPoint > conditionVal) {
                            newSet.push_back(object);
                        }
                        break;


                    case ColliderType::AABB:
                        AABB = (AABoxCollider*)objCollider;
                        furhestPoint = AABB->GetMaxSize().z;
                        if (furhestPoint > conditionVal) {
                            newSet.push_back(object);
                        }
                        break;
                    }
                }
            }
        }
        else {
            for (GameObject* object : gameObjectsInSet) {
                float furhestPoint;
                Collider* objCollider;
                //Check Collider exist
                if ((objCollider = object->GetComponent<RigidBody>()->GetCollider())) {
                    switch (objCollider->GetColliderType())
                    {

                    case ColliderType::Sphere:
                        sphereCol = (SphereCollider*)objCollider;
                        furhestPoint = objCollider->GetTransform()->GetPosition().z - sphereCol->GetRadius();
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;


                    case ColliderType::AABB:
                        AABB = (AABoxCollider*)objCollider;
                        furhestPoint = AABB->GetMinSize().z;
                        if (furhestPoint < conditionVal) {
                            newSet.push_back(object);
                        }
                        break;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return newSet;
}



