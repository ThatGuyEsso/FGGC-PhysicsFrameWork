#include "KD_Tree.h"

KD_Tree::KD_Tree()
{
    _rootNode = nullptr;
}

KD_Tree::~KD_Tree()
{
    if (!leaves.empty()) leaves.clear();
}

void KD_Tree::AddNodeRecursive(std::vector<GameObject*> gameObjectsInSet,Axis axis, KD_Node* currentNode)
{
    if (!_rootNode) {
        _rootNode = new KD_Node(0,nullptr);
        _rootNode->objectSet = gameObjectsInSet;
        _rootNode->point = GetMedianPositionFormSet(gameObjectsInSet);

        axis = Axis::X_axis;
      
        if (_rootNode->depth < maxDepth&& gameObjectsInSet.size()>=minObjectsToSplit) {
            KD_Node* left = _rootNode->left = new KD_Node(_rootNode->depth + 1, _rootNode);
            KD_Node* right = _rootNode->right = new KD_Node(_rootNode->depth+1, _rootNode);
       
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
            _rootNode->ApplyDepthMat();//Add section mat
            leaves.push_back(_rootNode);
        }
   
    }
    else {
        //Check if the node is connected to tree and is valid
        if (currentNode->parent == nullptr|| currentNode==nullptr) return;
        
        //If valid check if it is valid to split further
        if (currentNode->depth >= maxDepth || gameObjectsInSet.size() < minObjectsToSplit) {
 

            leaves.push_back(currentNode);
       
            return;

        }
        //Create new node split 
        currentNode->point=(GetMedianPositionFormSet(gameObjectsInSet));
        currentNode->ApplyDepthMat();
        KD_Node* newLeft;
        KD_Node* newRight;
        switch (axis)
        {
        case Axis::X_axis:

                //Create branches 
            newLeft = currentNode->left = new KD_Node(currentNode->depth+1, currentNode);
            newRight = currentNode->right = new KD_Node(currentNode->depth + 1, currentNode);

            //Split to the left
            newLeft->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.x);
           
            if (newLeft->depth < maxDepth || newLeft->objectSet.size() >= minObjectsToSplit) {
                //If valid to split continue recursion
                AddNodeRecursive(newLeft->objectSet, Axis::Y_axis, newLeft);
            }
            else {

                leaves.push_back(newLeft);
            }

            //Split to the right
            newRight->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.x);
            //Apply slit mat
      
            if (newRight->depth < maxDepth || newRight->objectSet.size() >= minObjectsToSplit) {

                AddNodeRecursive(newRight->objectSet, Axis::Y_axis, newLeft);
            }
            else {
 
                leaves.push_back(newRight);
        
            }
        
            break;
        case Axis::Y_axis:

            //Create branches 
            newLeft = currentNode->left = new KD_Node(currentNode->depth + 1, currentNode);
            newRight = currentNode->right = new KD_Node(currentNode->depth + 1, currentNode);

            //Split to the left
            newLeft->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.y);

            if (newLeft->depth < maxDepth || newLeft->objectSet.size() >= minObjectsToSplit) {
                //If valid to split continue recursion
                AddNodeRecursive(newLeft->objectSet, Axis::Z_axis, newLeft);
            }
            else {
         
                leaves.push_back(newLeft);
            }

            //Split to the right
            newRight->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.y);
            //Apply slit mat
            newRight->ApplyDepthMat();
            if (newRight->depth < maxDepth || newRight->objectSet.size() >= minObjectsToSplit) {

                AddNodeRecursive(newRight->objectSet, Axis::Z_axis, newLeft);
            }
            else
            {
            
                leaves.push_back(newRight);
            }

            break;
        case Axis::Z_axis:

            //Create branches 
            newLeft = currentNode->left = new KD_Node(currentNode->depth + 1, currentNode);
            newRight = currentNode->right = new KD_Node(currentNode->depth + 1, currentNode);

            //Split to the left
            newLeft->objectSet = GetObjectsInSplit(gameObjectsInSet, false, axis, currentNode->point.z);
     
            if (newLeft->depth < maxDepth || newLeft->objectSet.size() >= minObjectsToSplit) {
                //If valid to split continue recursion
                AddNodeRecursive(newLeft->objectSet, Axis::X_axis, newLeft);
            }
            else {
          
                leaves.push_back(newLeft);
            }

            //Split to the right
            newRight->objectSet = GetObjectsInSplit(gameObjectsInSet, true, axis, currentNode->point.z);
            //Apply slit mat
      
            if (newRight->depth < maxDepth || newRight->objectSet.size() >= minObjectsToSplit) {

                AddNodeRecursive(newRight->objectSet, Axis::X_axis, newLeft);
            }
            else {
       
                leaves.push_back(newRight);
            }

            break;
            }
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
                if ((objCollider = object->GetComponent<Collider>())) {
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
                if ((objCollider = object->GetComponent<Collider>())) {
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
                if ((objCollider = object->GetComponent<Collider>())) {
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
                if ((objCollider = object->GetComponent<Collider>())) {
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
                if ((objCollider = object->GetComponent<Collider>())) {
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
                if ((objCollider = object->GetComponent<Collider>())) {
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



