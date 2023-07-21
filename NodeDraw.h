#pragma once
#include"shaderProgram.h"
#include"model_Node.h"
class Drawnode
{
public:
	void DrawNode(modelNode* n, shaderProgram* animationshader)//遍历子节点
	{
		if (n->GetMesh())
		{
			n->Draw(animationshader);
			// n->Getchildrenarray()[0]->Draw(animationshader);
		}
	
		for (std::vector < modelNode* >::const_iterator i = n->Getchildbegin(); i != n->Getchildend(); ++i)
		{
			DrawNode(*i, animationshader);
	
		}
	}
};