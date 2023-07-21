#include "model.h"
#include<iostream>
model::model(std::string path)
{
	loadModel(path);
}
SkeletonnodeData& model::GetRootNode()
{
	return RootNode;
}
void model::draw()
{
	for (  Mesh permesh:meshes)
	{   
		permesh.Draw();
		
	}
}
void model::setVertexBoneDataToDefault(SKeleTonMeshVertex& vertex)
{
	for (int i = 0; i < Max_Bone_influences; i++)
	{
		vertex.BoneTranSformMatrixIndex[i] = -1;//先默认设置每个顶点对应的骨骼的索引
		vertex.m_Weights[i] = 0.0f;//相应骨骼影响权重
	}
}
void model::loadModel(std::string  path)
{
	Assimp::Importer importer;//导入模型文件声明
	const auto Pmodel = importer.ReadFile(path, aiProcess_Triangulate|aiProcess_CalcTangentSpace);//从路径加载 以三角形面为基本单元  法线贴图
	Reading_heirarchyNode(RootNode, Pmodel->mRootNode);
	processNode(Pmodel->mRootNode, Pmodel);
}
void model::processNode(aiNode* node, const aiScene* scene)//std::unique_ptr<Node>ParseNode(const aiNode&node)
{   
		for (int i = 0; i < node->mNumMeshes; i++)//此节点对应有多少个网格 依次遍历
		{																			
			//整个模型->网格[整个模型第几个网格]   此节点对映的第几个网格//存储整个模型 的某个网格比如胳膊网格
			meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));//放入要操作的网格  
		}
		for (int i = 0; i < node->mNumChildren; i++)//看有多少个子节点依次递归到最后一个子节点
		{
			processNode(node->mChildren[i], scene);//依次递归到最后一个节点//脖子->臂骨->手腕骨骼->手指骨骼->手指头
		}
}
Mesh model::processMesh(aiMesh* mesh)
{
	std::vector<SKeleTonMeshVertex>tempvertices;
	std::vector<unsigned int>tempindices;
	for (int i=0;i<mesh->mNumVertices;i++)//拷贝mesh的vertex的顶点 纹理坐标 法线
	{
		SKeleTonMeshVertex tempvertex;
		setVertexBoneDataToDefault(tempvertex);//设置每个顶点的骨骼索引以及权重
		tempvertex.position.x = mesh->mVertices[i].x;                         
		tempvertex.position.y = mesh->mVertices[i].y ;                 
		tempvertex.position.z = mesh->mVertices[i].z;
		
		tempvertex.normal.x = mesh->mNormals[i].x;
		tempvertex.normal.y = mesh->mNormals[i].y;
		tempvertex.normal.z = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0])
		{
			tempvertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			tempvertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			tempvertex.texcoord = glm::vec2(0, 0);
		}
		if (mesh->mTangents!=NULL)
		{
          tempvertex.Tangent.x = mesh->mTangents[i].x;
		  tempvertex.Tangent.y = mesh->mTangents[i].y;                                                                                
		  tempvertex.Tangent.z = mesh->mTangents[i].z;
		}
		tempvertices.push_back(tempvertex);
	}
	for (int i=0;i<mesh->mNumFaces;i++)
	{
		//放入此容器网格每个面对应的第几个点的索引
		for (int j=0;j<mesh->mFaces[i].mNumIndices;j++)
		{
            tempindices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	ExtractBoneweightForVertices(tempvertices,mesh);//对每个网格的每个顶点相对于每个骨骼设置旋转平移矩阵权重
	return Mesh(tempvertices,tempindices);
}
void model::Reading_heirarchyNode(SkeletonnodeData& destination, aiNode* src)
{
	destination.RelativeToparentTransform = AiMatrixGLMconverter(src->mTransformation);
	destination.NodeName = src->mName.data;///赋值这个节点名字
	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		SkeletonnodeData destination_child;
		Reading_heirarchyNode(destination_child, src->mChildren[i]);//将这个节点对应的每个子节点赋值给这个变量
		destination.children.push_back(destination_child);//放入这个目标的子节点容器中
	}
}
void model::SetVertexBoneData(SKeleTonMeshVertex& vertex, int boneId, float weight)
{
	for (unsigned int i=0;i< Max_Bone_influences;i++)
	{
		if (vertex.BoneTranSformMatrixIndex[i] == -1)
		{
			vertex.m_Weights[i] = weight;//对每个顶点复制权重
			vertex.BoneTranSformMatrixIndex[i] = boneId;//对每个顶点复制骨骼索引
			return;
		}
	}
}
void model::ExtractBoneweightForVertices(std::vector<SKeleTonMeshVertex>& vertices, aiMesh* mesh)//对每个网格的每个顶点设置旋转平移矩阵权重
{  
	for (int boneindex=0;boneindex<mesh->mNumBones;++boneindex)//遍历单个节点对应的mesh中的每个骨骼 因为一个mesh存在多个骨骼
	{    
		int boneId = -1;//先初始化骨骼索引
		std::string bonename = mesh->mBones[boneindex]->mName.C_Str();//将单个mesh每个骨骼对应的匹配名字符串 赋值给bonename这个变量
		if (BoneInfomap.find(bonename)==BoneInfomap.end())//如果遍历这个骨骼映射表都到最后了还没找到就说明没有这个骨骼 那我们就把他添加进去
		{
		
			BoneInfo newBoneInfo;//定义这个结构体变量
			newBoneInfo.id = Num_Bones;//赋值给这个骨骼信息的索引
			newBoneInfo.offset = AiMatrixGLMconverter(mesh->mBones[boneindex]->mOffsetMatrix);//将这个节点下的mesh相应索引下的骨骼所对应的偏移矩阵赋值给这个结构体变量成员
			BoneInfomap[bonename] = newBoneInfo;//这个骨骼映射表相应匹配名下赋值给这个新骨骼信息
			boneId = Num_Bones;//将骨骼计数器赋值给这个boneid变量
			Num_Bones++;//骨骼计数器自增一
		}
		else
		{
			boneId = BoneInfomap[bonename].id;//找到了就把这个映射表下相关名字对应的索引值赋值给这个变量boneid
		}
		
		assert(boneId != -1);//确保这个索引值合法我们才能往下进行程序运行
		auto skinningVertinfo_AffectedBythisBone_Array = mesh->mBones[boneindex]->mWeights;//这个索引对应的骨骼 对应的权重数组的首地址
		unsigned int Number_skinningVertinfo_AffectedBythisBone = mesh->mBones[boneindex]->mNumWeights;//权重数组的总数量
		for (unsigned int  skinningVertinfo_AffectedBythisBoneindex= 0; skinningVertinfo_AffectedBythisBoneindex < Number_skinningVertinfo_AffectedBythisBone; ++skinningVertinfo_AffectedBythisBoneindex)//遍历这个骨骼影响的顶点信息数组
		{
			unsigned int vertexid =skinningVertinfo_AffectedBythisBone_Array[skinningVertinfo_AffectedBythisBoneindex].mVertexId;//这个权重和哪个顶点匹配 也就是说某个顶点受这个权重值影响
			float weight= skinningVertinfo_AffectedBythisBone_Array[skinningVertinfo_AffectedBythisBoneindex].mWeight;//相应权重数值
			SetVertexBoneData(vertices[vertexid],boneId,weight);//对相应顶点相对于特定的骨骼的权重传进去
		}
	}
}