#include "SelfdeFineSkeletonmesh.h"

SelfdeFineSkeletonmesh::SelfdeFineSkeletonmesh()
{
	for (unsigned int i=0;i<2;i++)
	{
		FinalTrandformationForVertices.push_back(glm::mat4(0.0f));
    }
	loadOurskeletonVertices();
	loadSkeleboneInfo();
	loadHeirarchyForBoneNode();
	ExtractBoneweightForourskeletonMeshvertices(ourskeletonMeshvertices);
	BindOurMeshToGPU();
}

void SelfdeFineSkeletonmesh::DebugVertices()
{
	//std::cout << ourskeletonMeshvertices[0].BoneIDs[0] << std::endl;
}

void SelfdeFineSkeletonmesh::UpdatefinalTransform()
{
	CalculateOurBoneNodeFinaltransformationForvertices(selfskeletonRootNode,glm::mat4(1.0f));
}

void SelfdeFineSkeletonmesh::DrawourskeletonMesh()
{
	glBindVertexArray(Vao);
	glDrawElements(GL_TRIANGLES, DrawingIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::mat4>& SelfdeFineSkeletonmesh::Gettrandformation()
{
	return FinalTrandformationForVertices;
}

void SelfdeFineSkeletonmesh::loadOurskeletonVertices()
{
	SelfdefineSkeletonVertex skeletonMeshvertices[12] = {
		{glm::vec3(-0.6,0.4,0.7),glm::vec2(0.5,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//0
		{glm::vec3(-0.6,0.0,0.7),glm::vec2(0.5,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//1
		{glm::vec3(-0.6,0.0,0.0),glm::vec2(0.0,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//2
		{glm::vec3(-0.6,0.4,0.0),glm::vec2(0.5,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//3
		{glm::vec3(-0.3,0.0,0.0),glm::vec2(0.0,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//4
		{glm::vec3(-0.3,0.4,0.0),glm::vec2(0.0,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//5
		{glm::vec3(-0.3,0.4,0.7),glm::vec2(0.5,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//6
		{glm::vec3(-0.3,0.0,0.7),glm::vec2(0.0,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//7
		{glm::vec3( 0.0,0.4,0.7),glm::vec2(0.5,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//8
		{glm::vec3( 0.0,0.0,0.7),glm::vec2(0.0,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//9
		{glm::vec3( 0.0,0.0,0.0),glm::vec2(0.0,0.0),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}},//10
		{glm::vec3( 0.0,0.4,0.0),glm::vec2(0.0,0.5),{-1,-1,-1,-1},{0.0f,0.0f,0.0f,0.0f}} //11

	};//骨骼网格坐标
	SelfdefineSkeletonFaces skeletonMeshFaces[20] = {
		{0,1,2},
		{0,2,3},

		//0356
		{0,5,6},
		{0,5,3},

		//3254
		{3,4,2},
		{3,4,5},

		//1247
		{1,4,2},
		{1,4,7},

		//0617
		{0,7,6},
		{0,7,1},

		//8 9 10 11
		{8,10,9},
		{8,10,11},
		//8 11 5 6
		{6,11,8},
		{6,11,5},
		//5 11  4 10
		{5,10,4},
		{5,10,11},

		//4 7 10 11
		{7,10,4},
		{7,10,11},

		//6 8 9 7
		{6,9,8},
		{6,9,7}
	};
	for (SelfdefineSkeletonVertex SKELETONVERTEX: skeletonMeshvertices)
	{
		ourskeletonMeshvertices.push_back(SKELETONVERTEX);
	}
	for (SelfdefineSkeletonFaces SKELETONFACE: skeletonMeshFaces)
	{
		for (unsigned int i=0;i<3;i++)//三角形
		{
			DrawingIndices.push_back(SKELETONFACE.SkeletonIndices[i]);
		}
	}
	
}

void SelfdeFineSkeletonmesh::loadSkeleboneInfo()
{
	BongNameArray.push_back("lowRightArm");
	BongNameArray.push_back("UpRightArm");
	//小臂
	skeleBone["lowRightArm"].BoneOFFset = glm::translate(glm::mat4(1.0f),glm::vec3(0.3,0,0));//和它本身位置反过来
	skeleBone["lowRightArm"].boneindex = 1;
	skeleBone["lowRightArm"].BONEName = "lowRightArm";
	VertaffectedByBoneInfo VertArrayAffectedBylowRightArm[8] = {
		{0,0.99492f},
		{1,0.99902f},
		{2,0.99992f},
		{3,0.999459f},
		{4,0.000f},
		{5,0.000f},
		{6,0.000f},
		{7,0.000f},


	};
	for (auto vertinfo: VertArrayAffectedBylowRightArm)
	{
		skeleBone["lowRightArm"].VertinfoArrayAffectedByThisBone.push_back(vertinfo);
	}
     skeleBone["lowRightArm"].NumVertaffectedthisByBone = skeleBone["lowRightArm"].VertinfoArrayAffectedByThisBone.size();
	 //上臂
	 skeleBone["UpRightArm"].BoneOFFset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0, 0));
	 skeleBone["UpRightArm"].boneindex = 0;
	 skeleBone["UpRightArm"].BONEName = "UpRightArm";
	 VertaffectedByBoneInfo VertArrayAffectedByUpRightArm[8] = {
		 {0,0.000f},
		 {1,0.000f},
		 {2,0.000f},
		 {3,0.000f},
		 {4,0.99492f},
		 {5,0.99902f},
		 {6,0.99992f},
		 {7,0.99945f},
		
	 };
	 for (auto vertinfo : VertArrayAffectedByUpRightArm)
	 {
		 skeleBone["UpRightArm"].VertinfoArrayAffectedByThisBone.push_back(vertinfo);
	 }
	 skeleBone["UpRightArm"].NumVertaffectedthisByBone = skeleBone["UpRightArm"].VertinfoArrayAffectedByThisBone.size();

}

void SelfdeFineSkeletonmesh::loadHeirarchyForBoneNode()
{
	lowRightArmBoneNode = new selfBoneNode;
	lowRightArmBoneNode->BoneNodename = "lowRightArm";
	lowRightArmBoneNode->RelativeToparentBindposeTransform =glm::translate(glm::mat4(1.0f), glm::vec3(-0.3, 0, 0));
	lowRightArmBoneNode->childCount = 0;
	lowRightArmBoneNode->RelativeToparentRotation= glm::rotate(glm::mat4(1.0f), glm::radians(720.4f), glm::vec3(0.0f, 1.0f, 1.0f));

	UpRightArmBoneNode =new selfBoneNode;
	UpRightArmBoneNode->BoneNodename = "UpRightArm";
	UpRightArmBoneNode->RelativeToparentBindposeTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	UpRightArmBoneNode->children.push_back(*lowRightArmBoneNode);
	UpRightArmBoneNode->childCount = UpRightArmBoneNode->children.size();
	UpRightArmBoneNode->RelativeToparentRotation= glm::rotate(glm::mat4(1.0f), glm::radians(20.4f), glm::vec3(1.0f, 0.0f, 1.0f));
	selfskeletonRootNode = UpRightArmBoneNode;//加载根节点
}
void SelfdeFineSkeletonmesh::CalculateOurBoneNodeFinaltransformationForvertices(selfBoneNode* eachNode, glm::mat4 ParentBoneNodeTransform)
{
	glm::mat4 accumulatedTransformationForEachvert;
	time++;
	if (eachNode->BoneNodename== "lowRightArm")
	{
		accumulatedTransformationForEachvert = ParentBoneNodeTransform * eachNode->RelativeToparentBindposeTransform* glm::rotate(glm::mat4(1.0f), glm::radians(43.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else
	{
		accumulatedTransformationForEachvert = ParentBoneNodeTransform * eachNode->RelativeToparentBindposeTransform * glm::rotate(glm::mat4(1.0f), glm::radians(32*sin(time/210)), glm::vec3(1.0f, 0.0f, 0.0f));;
	}
	std::string boneNodeNAME = eachNode->BoneNodename;
	if (skeleBone.find(boneNodeNAME) != skeleBone.end())//找到了
	{
		glm::mat4 BONEoffsetTransform = skeleBone[boneNodeNAME].BoneOFFset;
		auto finalTransformindex = skeleBone[boneNodeNAME].boneindex;
		////std::cout << finalTransformindex << std::endl;
		FinalTrandformationForVertices[finalTransformindex] = accumulatedTransformationForEachvert * BONEoffsetTransform;
	}

	for (unsigned int i = 0; i < eachNode->childCount; i++)
	{
		
		CalculateOurBoneNodeFinaltransformationForvertices(&eachNode->children[i], accumulatedTransformationForEachvert);
		
	}

}
void SelfdeFineSkeletonmesh::ExtractBoneweightForourskeletonMeshvertices(std::vector<SelfdefineSkeletonVertex>& selfdefineSkeletonvertices)
{
	for (std::string tempBoneName: BongNameArray)
	{
		for (unsigned int i=0;i<skeleBone[tempBoneName].NumVertaffectedthisByBone;i++)
		{
			auto vertindex=skeleBone[tempBoneName].VertinfoArrayAffectedByThisBone[i].vertexid;
			float weightforthisVert = skeleBone[tempBoneName].VertinfoArrayAffectedByThisBone[i].weightValue;
			SetSelfdefineSkeletonVertexBoneData(selfdefineSkeletonvertices[vertindex], skeleBone[tempBoneName].boneindex, weightforthisVert);
		}
	}
}

void SelfdeFineSkeletonmesh::SetSelfdefineSkeletonVertexBoneData(SelfdefineSkeletonVertex& ourvertex, int boneindex, float WeightValue)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (ourvertex.BoneTransformindex[i] == -1)
		{
			ourvertex.Weights[i] = WeightValue;//对每个顶点复制权重
			ourvertex.BoneTransformindex[i] = boneindex;//对每个顶点复制骨骼索引
			//std::cout << ourvertex.BoneIDs[i] << std::endl;
			return;
		}
	}
}

void SelfdeFineSkeletonmesh::BindOurMeshToGPU()
{
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SelfdefineSkeletonVertex) * ourskeletonMeshvertices.size(), &ourskeletonMeshvertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * DrawingIndices.size(), &DrawingIndices[0], GL_STATIC_DRAW);//索引

	glEnableVertexAttribArray(0);//position                                                                                         
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(SelfdefineSkeletonVertex), (GLvoid*)offsetof(SelfdefineSkeletonVertex, vertPos));

	glEnableVertexAttribArray(1);//texcoord														  
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(SelfdefineSkeletonVertex), (GLvoid*)offsetof(SelfdefineSkeletonVertex, Texelpos));


	glEnableVertexAttribArray(2);//boneIDs  骨骼索引
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(SelfdefineSkeletonVertex), (GLvoid*)offsetof(SelfdefineSkeletonVertex, BoneTransformindex));

	glEnableVertexAttribArray(3);//weights   骨骼权重
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(SelfdefineSkeletonVertex), (GLvoid*)offsetof(SelfdefineSkeletonVertex, Weights));
}