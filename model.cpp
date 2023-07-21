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
		vertex.BoneTranSformMatrixIndex[i] = -1;//��Ĭ������ÿ�������Ӧ�Ĺ���������
		vertex.m_Weights[i] = 0.0f;//��Ӧ����Ӱ��Ȩ��
	}
}
void model::loadModel(std::string  path)
{
	Assimp::Importer importer;//����ģ���ļ�����
	const auto Pmodel = importer.ReadFile(path, aiProcess_Triangulate|aiProcess_CalcTangentSpace);//��·������ ����������Ϊ������Ԫ  ������ͼ
	Reading_heirarchyNode(RootNode, Pmodel->mRootNode);
	processNode(Pmodel->mRootNode, Pmodel);
}
void model::processNode(aiNode* node, const aiScene* scene)//std::unique_ptr<Node>ParseNode(const aiNode&node)
{   
		for (int i = 0; i < node->mNumMeshes; i++)//�˽ڵ��Ӧ�ж��ٸ����� ���α���
		{																			
			//����ģ��->����[����ģ�͵ڼ�������]   �˽ڵ��ӳ�ĵڼ�������//�洢����ģ�� ��ĳ���������첲����
			meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));//����Ҫ����������  
		}
		for (int i = 0; i < node->mNumChildren; i++)//���ж��ٸ��ӽڵ����εݹ鵽���һ���ӽڵ�
		{
			processNode(node->mChildren[i], scene);//���εݹ鵽���һ���ڵ�//����->�۹�->�������->��ָ����->��ָͷ
		}
}
Mesh model::processMesh(aiMesh* mesh)
{
	std::vector<SKeleTonMeshVertex>tempvertices;
	std::vector<unsigned int>tempindices;
	for (int i=0;i<mesh->mNumVertices;i++)//����mesh��vertex�Ķ��� �������� ����
	{
		SKeleTonMeshVertex tempvertex;
		setVertexBoneDataToDefault(tempvertex);//����ÿ������Ĺ��������Լ�Ȩ��
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
		//�������������ÿ�����Ӧ�ĵڼ����������
		for (int j=0;j<mesh->mFaces[i].mNumIndices;j++)
		{
            tempindices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
	ExtractBoneweightForVertices(tempvertices,mesh);//��ÿ�������ÿ�����������ÿ������������תƽ�ƾ���Ȩ��
	return Mesh(tempvertices,tempindices);
}
void model::Reading_heirarchyNode(SkeletonnodeData& destination, aiNode* src)
{
	destination.RelativeToparentTransform = AiMatrixGLMconverter(src->mTransformation);
	destination.NodeName = src->mName.data;///��ֵ����ڵ�����
	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		SkeletonnodeData destination_child;
		Reading_heirarchyNode(destination_child, src->mChildren[i]);//������ڵ��Ӧ��ÿ���ӽڵ㸳ֵ���������
		destination.children.push_back(destination_child);//�������Ŀ����ӽڵ�������
	}
}
void model::SetVertexBoneData(SKeleTonMeshVertex& vertex, int boneId, float weight)
{
	for (unsigned int i=0;i< Max_Bone_influences;i++)
	{
		if (vertex.BoneTranSformMatrixIndex[i] == -1)
		{
			vertex.m_Weights[i] = weight;//��ÿ�����㸴��Ȩ��
			vertex.BoneTranSformMatrixIndex[i] = boneId;//��ÿ�����㸴�ƹ�������
			return;
		}
	}
}
void model::ExtractBoneweightForVertices(std::vector<SKeleTonMeshVertex>& vertices, aiMesh* mesh)//��ÿ�������ÿ������������תƽ�ƾ���Ȩ��
{  
	for (int boneindex=0;boneindex<mesh->mNumBones;++boneindex)//���������ڵ��Ӧ��mesh�е�ÿ������ ��Ϊһ��mesh���ڶ������
	{    
		int boneId = -1;//�ȳ�ʼ����������
		std::string bonename = mesh->mBones[boneindex]->mName.C_Str();//������meshÿ��������Ӧ��ƥ�����ַ��� ��ֵ��bonename�������
		if (BoneInfomap.find(bonename)==BoneInfomap.end())//��������������ӳ���������˻�û�ҵ���˵��û��������� �����ǾͰ�����ӽ�ȥ
		{
		
			BoneInfo newBoneInfo;//��������ṹ�����
			newBoneInfo.id = Num_Bones;//��ֵ�����������Ϣ������
			newBoneInfo.offset = AiMatrixGLMconverter(mesh->mBones[boneindex]->mOffsetMatrix);//������ڵ��µ�mesh��Ӧ�����µĹ�������Ӧ��ƫ�ƾ���ֵ������ṹ�������Ա
			BoneInfomap[bonename] = newBoneInfo;//�������ӳ�����Ӧƥ�����¸�ֵ������¹�����Ϣ
			boneId = Num_Bones;//��������������ֵ�����boneid����
			Num_Bones++;//��������������һ
		}
		else
		{
			boneId = BoneInfomap[bonename].id;//�ҵ��˾Ͱ����ӳ�����������ֶ�Ӧ������ֵ��ֵ���������boneid
		}
		
		assert(boneId != -1);//ȷ���������ֵ�Ϸ����ǲ������½��г�������
		auto skinningVertinfo_AffectedBythisBone_Array = mesh->mBones[boneindex]->mWeights;//���������Ӧ�Ĺ��� ��Ӧ��Ȩ��������׵�ַ
		unsigned int Number_skinningVertinfo_AffectedBythisBone = mesh->mBones[boneindex]->mNumWeights;//Ȩ�������������
		for (unsigned int  skinningVertinfo_AffectedBythisBoneindex= 0; skinningVertinfo_AffectedBythisBoneindex < Number_skinningVertinfo_AffectedBythisBone; ++skinningVertinfo_AffectedBythisBoneindex)//�����������Ӱ��Ķ�����Ϣ����
		{
			unsigned int vertexid =skinningVertinfo_AffectedBythisBone_Array[skinningVertinfo_AffectedBythisBoneindex].mVertexId;//���Ȩ�غ��ĸ�����ƥ�� Ҳ����˵ĳ�����������Ȩ��ֵӰ��
			float weight= skinningVertinfo_AffectedBythisBone_Array[skinningVertinfo_AffectedBythisBoneindex].mWeight;//��ӦȨ����ֵ
			SetVertexBoneData(vertices[vertexid],boneId,weight);//����Ӧ����������ض��Ĺ�����Ȩ�ش���ȥ
		}
	}
}