#pragma once

#include "Vector3D.h"
#include <vector>
#include "GameObject.h"
#include "Commons.h"
class KD_Tree {

public:
	struct KD_Node
	{
		Vector3D point;
		KD_Node* parent;
		KD_Node* left, * right;
		std::vector<GameObject*> objectSet;
		int depth;
		KD_Tree::KD_Node(int Depth) {
			depth = Depth;
		}
		bool IsLeaf() {
			return left && right == nullptr;
		}
		
		void ApplyDepthMat() {
			if (depth == 0) {
				for (GameObject* object : objectSet) {
					Appearance* app;

					if (app = object->GetComponent<Appearance>()) {
						app->SetMaterial(depth1Mat);
					}
				}
			}
			if (depth == 1) {
				for (GameObject* object : objectSet) {
					Appearance* app;

					if (app = object->GetComponent<Appearance>()) {
						app->SetMaterial(depth2Mat);
					}
				}
			}
			if (depth == 2) {
				for (GameObject* object : objectSet) {
					Appearance* app;

					if (app = object->GetComponent<Appearance>()) {
						app->SetMaterial(depth3Mat);
					}
				}
			}

			
		}
	};

	enum class Axis
	{
		X_axis = 0,
		Y_axis = 1,
		Z_axis = 2,
	};

public:
	KD_Tree();
	~KD_Tree();
	void AddNodeRecursive(std::vector<GameObject*> gameObjectsInSet, Axis axis, KD_Node* parent);
	std::vector<KD_Node*> GetLeaves() { return leaves; }

private:
	Vector3D GetMedianPositionFormSet(std::vector<GameObject*> gameObjectsInSet);
	std::vector<GameObject*>  GetObjectsInSplit(std::vector<GameObject*> gameObjectsInSet, bool isGreater, Axis axis,float conditionVal);


private:
	KD_Node* _rootNode;

	std::vector<KD_Node*> leaves;
	int minObjectsToSplit = 4;
	int maxDepth=6;
};