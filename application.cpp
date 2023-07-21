#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<iostream>
#include "shaderProgram.h"
#include "gtc/matrix_transform.hpp"
#include<glm.hpp>
#include "mesh.h"
#include"imgui.h"
#include"imgui_impl_glfw_gl3.h"
#include"Texture2Dimage.h"
#include"cubemap.h"
#include "shadowmap.h"
#include"lightrender.h"
#include"model.h"//骨骼动画  法线贴图  物理引擎  水面倒影  喷泉 烟雾  体积云   第一人称鼠标摄像机  光线追踪
#include"Animator.h"
#include"Camara.h"
#include"model_Node.h"
#include"robot.h"
#include"Minigun.h"
#include"Bird.h"
#include"waterframe.h"
#include"MD5FileData.h"
#include"MD5Node.h"
#include"Drawcube.h"
#include"Animator.h"
#include"selfdefineSkele/SelfdeFineSkeletonmesh.h"
#define N 100
float yaw, pitch = 0.0f;//摄像机视野旋转角
void glfw_onKey(GLFWwindow* window, int key,int scancode,int action,int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void mouse_callback(GLFWwindow*window,double xpos,double ypos)//鼠标输入
{
	static double lastxpos = 0.0,lastypos=0.0;
	double xoffset = xpos - lastxpos;//获取x偏移量
	double yoffset = lastypos -ypos ;//获取y偏移量
    lastxpos = xpos; lastypos = ypos;
	double sensitivity=0.33;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
        xoffset *= sensitivity;
	    yoffset *=sensitivity;
	    yaw += xoffset;
	    pitch += yoffset;
	}
	
}
//void scorll_callback(GLFWwindow* window, double scrollX, double scrollY)
//{
//	double sensitivity = 3.33;
//	static double lastxpos = 0.0, lastypos = 0.0;
//	double yoffset = lastypos - scrollY;//获取y偏移量
//	double sense = 2.4;
//
//	
//        scorllYoffset+=yoffset*sense;
//	    FOV += scorllYoffset;
//	
//	
//	
//	lastypos = scrollY;
//}
int main()
{

	/*bool RobotRun=false, RobotPunch =false,Robotflip=false,wireframe=false;
	bool movesingle_skeleton = false, set_skeleton_map = false, play_anim = false;
	int set_animationmap_time_index = 0, playanimation_time_index=0;
	bool fire = false;*/
	float  left = -100.0f, right = 100.0f, bottom = -40.0f, top = 70.0f, lnear = 2.0f, lfar = 100.5f;
	float FOV = 34.0f;
	float znear = 0.3f;//近平面
	float zfar = 1000000.0f;//远平面
	float clearcolor[4];//背景颜色
	float materia_shineness = 1.0f;
	clearcolor[0] = 0.0; clearcolor[1] = 0.0; clearcolor[2] = 0.0; clearcolor[3] = 0.001;
	glm::vec3 skeletonscale = glm::vec3(1,1,1);
	//glm::vec3 Rootposition=glm::vec3(0, 68, 0);
	//glm::vec3 RootRotation = glm::vec3(0, 0, 0);//机器人位置
	//

	//glm::vec3 minigunRootposition = glm::vec3(0, 0, 0);
	//glm::vec3 minigunRootRotation = glm::vec3(0, 0, 0);//手枪位置


	//glm::vec3 birdRootposition = glm::vec3(0, 0, 0);
	//glm::vec3 birdRootRotation = glm::vec3(0, 0, 0);//小鸟位置 旋转角度

	//glm::vec3  mirrorposition = glm::vec3(156.296, 431.852, -302.963);
	//glm::vec3  mirrorRotation = glm::vec3(213, 0, 293);
	
	GLFWwindow* pwindow;
	if (!glfwInit())
		return -1;
	
		pwindow = glfwCreateWindow(900, 800, "3D test", NULL, NULL);//创建一个窗口
	
	glfwMakeContextCurrent(pwindow);
	glewInit();
	glViewport(0, 0, 800, 600);//视野大小

	shaderProgram shaderprorgam, shadowshader,watershader,geometry_shader,framebuffercolor_shader ,skeletonshader, SkinningShader, selfskeletonShader;
	lightrender lightshader;
	lightshader.init();
	//geometry_shader.Add_Geometry_shader(
	//	    "shaderGLSL\\geometry.vert",
	//	"shaderGLSL\\geometry.frag",
	//	"shaderGLSL\\geometry.gs");//加载几何着色器
	
	//shadowshader.loadShaders("shaderGLSL\\shadow.vert", "shaderGLSL\\shadow.frag");//加载阴影着色器 从光的角度投影获取各个物体深度
	//watershader.loadShaders("shaderGLSL\\water.vert", "shaderGLSL\\water.frag");//加载水面反射着色器
	//framebuffercolor_shader.loadShaders("shaderGLSL\\framebuffercolor.vert", "shaderGLSL\\framebuffercolor.frag");//加载水面反射着色器//shaderprorgam.loadShaders("shaderGLSL\\basic.vert", "shaderGLSL\\basic.frag");//场景着色器
	SkinningShader.loadShaders("shaderGLSL\\skinningShader.vert", "shaderGLSL\\skinningShader.frag");
	skeletonshader.loadShaders("shaderGLSL\\animationshader.vert", "shaderGLSL\\animationshader.frag");//加载骨骼动画shader
	//selfskeletonShader.loadShaders("shaderGLSL\\Art.vert", "shaderGLSL\\Art.frag");




	texture2D tex[N];//加载每个物体对应的纹理材质
	mesh Mesh[N];//初始化模型网格
	Camara Camera;//初始化摄像机类
 //   cubemap skybox;//天空盒初始化                                        
	//shadowmap shadowmap;//初始化阴影映射
	//texture2D nanosuittex[N];
	//water Water;
	//robot robot;
	model*Model= new model("model\\res\\Dragon.dae");
	//模型加载
	//Animation ouranimation("model\\res\\Wolf_dae.dae", *Model);//动画
	//Animator ouranimator(&ouranimation);//执行动画
	MD5FileData*skeletonData=new MD5FileData("animation_file\\hellknight.md5mesh");
	MD5Node* skeletonnode = new MD5Node(skeletonData);//骨骼关节节点
    skeletonnode->AddAnim("animation_file\\attack2.md5anim");//加入动画

	auto AnimatSkeleton = new Animator(Model);


	//robot.Robot_Create();//创建机器人身体各个部位
	//robot.initNodeMatrix();
	//robot.Set_animationmap();//初始化骨骼映射表

	//Minigun minigun;
	//minigun.Create_MiniGun();//创建枪身体各个部位
	//minigun.initGunNodeMatrix();

	//Bird sqirrle;
	//sqirrle.Create_Bird();
	//sqirrle.initBirdNodeMatrix();//创建鸟身体各个部位

	//skybox.init();
	//skybox.loadtexture();//天空盒加载纹理
	//shadowmap.Shadowmapinit();//初始化阴影映射
	//Water.init();

   

	glm::vec3 modelpos[N] = {
		glm::vec3(-5.291f, -37.657f, 24.444f),
		glm::vec3(1.538f, -6.500f,25.639f),
		glm::vec3(-3.846f, -6.923f,25.588f),
		glm::vec3(-2.308f, -10.0f,30.333f),
		glm::vec3(-1.494f, -4.730f,24.944f),
		glm::vec3(-1.494f, -6.923f,24.944f),
		glm::vec3(-2.996f, 0.432f,16.476f)
	};//每个物体摆放位置
	glm::vec3 objectangle[N] = {
		glm::vec3(40.500f, 168.877f, 176.250f),
		glm::vec3(13.562f, 180.000f,0.0f),
		glm::vec3(13.562f, 180.000f,0.0f),
		glm::vec3(213.018f, 183.195f,173.647f),
		glm::vec3(13.562f, 180.000f,0.0f),
		glm::vec3(172.562f, 180.000f,0.0f),
		glm::vec3(172.562f, 180.000f,70.0f)
	};//每个物体旋转欧拉角
	



	
	
	

	//md5filedata.loadMesh("animation_file\\hellknight.md5mesh");


	//Mesh[5].loadobj("birdmesh\\body.obj");
	//Mesh[1].loadobj("mesh\\bird.obj");
	//mesh[2].loadobj("mesh\\sphere.obj");
	//Mesh[3].loadobj("mesh\\fan.obj");
	//mesh[4].loadobj("mesh\\Teapot.obj");
	//Mesh[0].loadobj("mesh\\floor1.obj");


	//tex[0].loadtexture("teximage\\duol.png");
	//tex[0].loadtexture("teximage\\bird2.jpg");
	//tex[2].loadtexture("teximage\\billard.jpg");
	/*tex[3].loadtexture("teximage\\Wolf_Body.jpg");*/
	//tex[4].loadtexture("teximage\\front.jpg");
	tex[0].loadtexture("teximage\\ktn.jpg");
	tex[6].loadtexture("teximage\\Dragon.jpg");
	tex[7].loadtexture("teximage\\DragonBack.jpg");
	//tex[8].loadtexture("teximage\\DragonNormal.jpg");//法线贴图
	glm::vec3 camera(-3.699f, -1.299f, 38.712f);//摄像机
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//摄像机向前方向向量
	glm::vec3 lightpos(-2.390f, -0.712f, 32.474f);//光源位置
	glm::vec4 lightcolor(1.0f, 1.0f, 1.0f, 1.0f);//光颜色
	float lightshineness = 30.0f;//光泽因子
	glm::vec3 targetpos(0.0f, 0.0f, 1.0f);//目标
	glm::vec3 up(0.0f, 1.0f, 0.0f);//向上向量
	glm::vec3 environmentlight(0.3f, 0.3f, 0.0f);//环境光
	ImVec4 clear_color = ImVec4(1.0f, 1.55f, 1.00f, 1.00f);
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(pwindow, true);
	ImGui::StyleColorsDark();

	float mpitch = 0.0f, myaw = 0.0f;//摄像机视角旋转角


	glm::mat4 lightspacematrix;//光线视野矩阵
	glm::mat4  view, projection;//视野矩阵  投影矩阵
	glm::mat4 model = glm::mat4(1.0f);//初始化模型矩阵
	float angle = 0.0f, deltatime;
	double currenttime, lasttime = 0;
	int Timeindex= 0;
	int animation_frameindex = 3;



	
    std::string* skeleton_string_array;//const char*
	std::string skeleton_name;
	bool modelNodeflag = false;
	int selectedModeNodeindex = 0;
	skeletonnode->init_bonespace_matrix();//初始化骨骼空间矩阵
	skeletonnode->update(animation_frameindex);

	
	//auto cube= new Drawcube;
	//cube->PushVerticesToGPU();//加载顶点
//auto selfdefineSkele=new SelfdeFineSkeletonmesh();

	
	//glPolygonMode(GL_FRONT, GL_LINE);

	while (!glfwWindowShouldClose(pwindow))
	{
		//selfdefineSkele->DebugVertices();
		   glfwSetKeyCallback(pwindow, glfw_onKey);
           glfwSetCursorPosCallback(pwindow, mouse_callback);//更新第一人称鼠标yaw pitch
		   Camera.UpdateCameraFront(yaw,pitch);//更新摄像机向前向量
		   Camera.UpdataCamerapos(pwindow);//更新摄像机位置
		   camera=Camera.position;//获取更新后的摄像机位置
		   cameraFront = Camera.GetCameraFront();//获取更新后的摄像机向前的方向向量
		   currenttime = glfwGetTime();
		   deltatime = currenttime - lasttime;
		   angle += deltatime;
		   lasttime = currenttime; 
           glClearColor(clearcolor[0], clearcolor[1], clearcolor[2], clearcolor[3]);
		   //绑定阴影帧缓冲
		//lightspacematrix = shadowmap.Shadowmapsetlight(lightpos, left, right, bottom, top, lnear, lfar);//光线视野矩阵//glm::mat4 Shadowmapsetlight(glm::vec3&lightposition,float&left,float&right,float&bottom,float&top,float&near,float&far);//设置光线视野矩阵let
		//glViewport(0, 0, 4096, 4096);//设置阴影视野大小
		//shadowmap.Shadowmapbindfbo();//绑定阴影深度贴图帧缓冲
		//glClear(GL_DEPTH_BUFFER_BIT);//清空深度缓冲
		//projection = glm::ortho(left, right, bottom, top, lnear, lfar);
		//view = glm::lookAt(lightpos, glm::vec3(0, 0, 0), up);//视野矩阵
		//shadowshader.use();
		//shadowshader.setUniform("projection", projection);
		//shadowshader.setUniform("view", view);
		//robot.DrawNode(robot.GetRootNode(), &shadowshader);//绘制节点
		//minigun.DrawNode(minigun.GetRootNode(), &shadowshader);
		//sqirrle.DrawNode(sqirrle.GetbirdRoot(),&shadowshader);
		//model = glm::mat4(1.0f);//全部置model矩阵数据为1
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);//解绑帧缓冲
		//阴影帧缓冲



		//绑定水面帧缓冲
		// 从镜子方向为视角
		//开始刻录当前帧图像的颜色信息启动水面着色器刻录
		//glViewport(0, 0, 1920, 1920);
		//glBindFramebuffer(GL_FRAMEBUFFER, Water.waterframebuffer());
		//glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲
		//projection = glm::perspective(glm::radians(FOV), (float)(1024 / 1011), znear, zfar);//透视投影矩阵 
		//glm::vec3 mirror_front = glm::normalize(glm::vec3(cos(glm::radians(mirrorRotation.z)) * cos(glm::radians(mirrorRotation.x)),
		//	                                              sin(glm::radians(mirrorRotation.x)),
		//	                                              sin(glm::radians(mirrorRotation.z)) * cos(glm::radians(mirrorRotation.x))));
		//view = glm::lookAt(mirrorposition,mirrorposition+mirror_front,up);//视野矩阵
		//framebuffercolor_shader.use();
		//framebuffercolor_shader.setUniform("projection", projection);
		//framebuffercolor_shader.setUniform("lightpos", lightpos);
		//framebuffercolor_shader.setUniform("view", view);
		//framebuffercolor_shader.setUniform("viewpos", glm::vec3(camera));
		//framebuffercolor_shader.setUniform("lightspacematrix", lightspacematrix);
		//framebuffercolor_shader.setUniform("light.color", lightcolor);
		//framebuffercolor_shader.setUniform("light.shineness", lightshineness);
		//framebuffercolor_shader.setUniform("environmentlight", environmentlight);//设置环境光
		//robot.DrawNode(robot.GetRootNode(), &shaderprorgam);//绘制节点
		//minigun.DrawNode(minigun.GetRootNode(), &shaderprorgam);
		//sqirrle.DrawNode(sqirrle.GetbirdRoot(), &shaderprorgam);//正常顶点颜色着色器
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);//解绑帧缓冲
		////水面帧缓冲

		glm::mat4 DAEmodelmatrix= glm::mat4(1.0f);//DAE模型矩阵

		model = glm::mat4(1.0f);
		//绑定到默认帧缓冲 屏幕帧缓冲
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲
		ImGui_ImplGlfwGL3_NewFrame();
		glViewport(0, 0, 1924, 1924);//窗口视野大小
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空颜色 深度缓冲
		glEnable(GL_DEPTH_TEST);//裁剪一些深度
	    projection = glm::perspective(glm::radians(FOV), (float)(1024 / 1011), znear, zfar);//透视投影矩阵 
		view = glm::lookAt(camera, camera + cameraFront, up);//视野矩阵
		//shaderprorgam.use();
		//shaderprorgam.setUniform("projection", projection);
		//shaderprorgam.setUniform("lightpos", lightpos);
		//shaderprorgam.setUniform("view", view);
		//shaderprorgam.setUniform("viewpos", glm::vec3(camera));
		//shaderprorgam.setUniform("lightspacematrix", lightspacematrix);
		//shaderprorgam.setUniform("light.color", lightcolor);
		//shaderprorgam.setUniform("light.shineness", lightshineness);
		//shaderprorgam.setUniform("environmentlight", environmentlight);//设置环境光
		//tex[0].bindtexture(0);
		//glUniform1i(glGetUniformLocation(shaderprorgam.getprogram(), "Texbuffer0"), 0);//绑定纹理图片		
		//shadowmap.setsamplershadowmap(&shaderprorgam);//渲染阴影	
		//robot.DrawNode(robot.GetRootNode(), &shaderprorgam);//绘制节点
		//minigun.DrawNode(minigun.GetRootNode(), &shaderprorgam);
		//sqirrle.DrawNode(sqirrle.GetbirdRoot(), &shaderprorgam);//正常顶点颜色着色器
		//skybox.render(camera,cameraFront);//天空盒渲染
		//渲染当前各个物体

		//骨骼动画绘制
		//skeletonshader.use();
		//skeletonshader.setUniform("projection", projection);
		//skeletonshader.setUniform("view", view);
		//skeletonshader.setUniform("lightpos", lightpos);
		//skeletonshader.setUniform("viewpos", glm::vec3(camera));
		//model = glm::scale(glm::mat4(1.0f), skeletonscale);
		//model = glm::translate(model, modelpos[0]) * glm::rotate(model, glm::radians(objectangle[0].y), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(objectangle[0].z), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(objectangle[0].x), glm::vec3(1.0f, 0.0f, 0.0f));//transform
		//skeletonshader.setUniform("model", model);
		//tex[0].bindtexture(0);
		//glUniform1i(glGetUniformLocation(skeletonshader.getprogram(), "Texbuffer0"), 0);//绑定纹理图片
		//skeletonnode->update_single_skeletonMatrix(skeleton_name);
		//skeletonnode->draw(skeletonshader);
		//
		
		//skeletonData->update_transformTBO(angle);
		//skeletonData->BindTextureBuffers();
		/*glUniform1i(glGetUniformLocation(skeletonshader.getprogram(), "weightTex"), 0);
		glUniform1i(glGetUniformLocation(skeletonshader.getprogram(), "transformTex"), 1);
		skeletonData->Draw();*/

		
		/*skeletonnode->update(animation_frameindex);
		skeletonnode->draw(animationshader);*/


  //      if (wireframe)
		//{
  //       	geometry_shader.use();
		//}
		//geometry_shader.setUniform("projection", projection);
		//geometry_shader.setUniform("view", view);
		//geometry_shader.setUniform("Time",angle);
		//robot.DrawNode(robot.GetRootNode(), &geometry_shader);//在几何着色器下绘制节点
		//minigun.DrawNode(minigun.GetRootNode(), &geometry_shader);
		//sqirrle.DrawNode(sqirrle.GetbirdRoot(), &geometry_shader);//几何着色器显示
		//geometry_shader.setUniform("model", model);
		//skeletonnode->draw(geometry_shader);


		////水面渲染
		//watershader.use();
		//watershader.setUniform("projection", projection);
		//watershader.setUniform("view", view);
		//watershader.setUniform("lightpos",lightpos);
		//watershader.setUniform("viewpos",glm::vec3(camera));
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::translate(model, mirrorposition+glm::vec3(0,3,0)) * glm::rotate(model, glm::radians(mirrorRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mirrorRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(mirrorRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));//transform
		//watershader.setUniform("model",model);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Water.reflectiontexture());
		//glUniform1i(glGetUniformLocation(watershader.getprogram(), "reflection_Texture"), 0);
		//tex[0].bindtexture(1);
		//glUniform1i(glGetUniformLocation(watershader.getprogram(), "floor_texture"), 1);//绑定纹理图片
		//tex[1].bindtexture(2);
		//glUniform1i(glGetUniformLocation(watershader.getprogram(), "dudvmap"), 2);//绑定纹理图片
		//Mesh[0].draw();
		////水面渲染

		////开始画立方体
		//cubeShader.use();
		//glm::mat4 MOdel = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(70.0f), 1024.0f/ 1011, 0.3f, 10000.0f);
		//view = glm::lookAt(camera, camera + cameraFront, up);//视野矩阵

		//MOdel=glm::rotate(MOdel, glm::radians(150.0f), glm::vec3(1.0f, 0.0f, 0.0f));//让立方体旋转60度
		//MOdel = glm::translate(MOdel, glm::vec3(-5.0f, 0.0f,1.0f));
	 //  //CPU to GPU 顶点操作数据发送
		//cubeShader.setUniform("model", MOdel); 
		//cubeShader.setUniform("view", view);
		//cubeShader.setUniform("projection", projection);
		//cube->draw();
  //    
	//
	//	if (RobotPunch)
	//	{
 //         robot.punch(deltatime);//更新机器人每个节点的变换矩阵
	//	}
	//	if (RobotRun)
	//	{
	//		robot.Run(deltatime);
	//	}
	//	if (Robotflip)
	//	{
	//		robot.flip(deltatime);
	//	}
	//	if (fire)
	//	{
	//		minigun.fire(deltatime);

	////	}
	////	sqirrle.flying(deltatime);
	////   Timeindex++;
	////if (glfwGetKey(pwindow, GLFW_KEY_R) == GLFW_PRESS)
	////{
	////	Timeindex = 0;
	////}
	////	
	////	robot.GetRootNode()->Set_Transform(glm::translate(glm::mat4(1.0f), Rootposition)
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(RootRotation.x), glm::vec3(1, 0, 0))
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(RootRotation.y), glm::vec3(0, 1, 0))//改变机器人根节点变换矩阵
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(RootRotation.z), glm::vec3(0, 0, 1)));
	////	minigun.GetRootNode()->Set_Transform(glm::translate(glm::mat4(1.0f), minigunRootposition)
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(minigunRootRotation.x), glm::vec3(1, 0, 0))
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(minigunRootRotation.y), glm::vec3(0, 1, 0))//改变手枪根节点变换矩阵
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(minigunRootRotation.z), glm::vec3(0, 0, 1)));
	////	sqirrle.GetbirdRoot()->Set_Transform(glm::translate(glm::mat4(1.0f), birdRootposition)
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(birdRootRotation.x), glm::vec3(1, 0, 0))
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(birdRootRotation.y), glm::vec3(0, 1, 0))//改变小鸟根节点变换矩阵
	////		* glm::rotate(glm::mat4(1.0f), glm::radians(birdRootRotation.z), glm::vec3(0, 0, 1)));
 ////       robot.animationPlay(robot.GetRootNode(),Timeindex,&shaderprorgam);//播放骨骼动画
	////	robot.GetRootNode()->update(angle);//机器人节点累乘矩阵
	////	minigun.GetRootNode()->update(angle);//枪节点累乘矩阵
	////	sqirrle.GetbirdRoot()->update(angle);
 ////     
		//DAE
		SkinningShader.use();
		SkinningShader.setUniform("projection", projection);
		SkinningShader.setUniform("view", view);
		SkinningShader.setUniform("lightpos", lightpos);
		SkinningShader.setUniform("viewpos", glm::vec3(camera));
		model = glm::translate(model, modelpos[1]) * glm::rotate(model, glm::radians(objectangle[1].y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(objectangle[1].z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(objectangle[1].x), glm::vec3(1.0f, 0.0f, 0.0f));//transform
		SkinningShader.setUniform("model", model);
		tex[6].bindtexture(0);
		tex[7].bindtexture(1);
		glUniform1i(glGetUniformLocation(SkinningShader.getprogram(), "Texbuffer0"), 0);//绑定纹理图片
		glUniform1i(glGetUniformLocation(SkinningShader.getprogram(), "Texbuffer1"), 1);//绑定纹理图片
		AnimatSkeleton->Bindtexbuffer();
	    AnimatSkeleton->UpdateAnimation(deltatime);
		glUniform1i(glGetUniformLocation(SkinningShader.getprogram(), "transformTex"), 0);//矩阵缓存
		Model->draw();
		////画自己定义的骨骼
		//selfskeletonShader.use();
		//selfskeletonShader.setUniform("projection", projection);
		//selfskeletonShader.setUniform("view", view);
		//
		//model = glm::scale(glm::mat4(1.0f), glm::vec3(240, 240, 240));
  // 
		//model = glm::translate(model, glm::vec3(1, 1, 1));
		//selfskeletonShader.setUniform("model", model);
		//selfdefineSkele->UpdatefinalTransform();
		//std::vector<glm::mat4> Transformation=selfdefineSkele->Gettrandformation();
		//
		//for (int i = 0; i < Transformation.size(); ++i)
		//{   
		//	selfskeletonShader.setUniform("finaltranform[" + std::to_string(i) + "]", Transformation[i]);	
		//	/*std::cout << "第几个" << i << std::endl;
		//	std::cout << Transformation[i][0][0] << " " << Transformation[i][0][1] << " " << Transformation[i][0][2] << " " << Transformation[i][0][3] << " " << std::endl;
		//std::cout << Transformation[i][1][0] << " " << Transformation[i][1][1] << " " << Transformation[i][1][2] << " " << Transformation[i][1][3] << " " << std::endl;
		//std::cout << Transformation[i][2][0] << " " << Transformation[i][2][1] << " " << Transformation[i][2][2] << " " << Transformation[i][2][3] << " " << std::endl;
		//std::cout << Transformation[i][3][0] << " " << Transformation[i][3][1] << " " << Transformation[i][3][2] << " " << Transformation[i][3][3] << " " << std::endl; */
		//}
	
		//selfdefineSkele->DrawourskeletonMesh();
		lightshader.render(camera, lightpos, lightcolor, cameraFront);
		
		//待转换的骨骼节点矩阵
		/*glm::mat4 tempmodel(1.0f);
		modify_skeleton_transform=  glm::translate(tempmodel, modify_skeleton_translation) *
			glm::rotate(tempmodel, glm::radians(modify_skeleton_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(tempmodel, glm::radians(modify_skeleton_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(tempmodel, glm::radians(modify_skeleton_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));*/
		//if (movesingle_skeleton)//是否用GUI改变骨骼节点矩阵
		//{
  //       skeletonnode->update_single_skeletonMatrix(skeleton_name);
		//}
		//if (set_skeleton_map)//如果往骨骼映射表对勾勾上
		//{
  //           skeletonnode->Setanimation_skeleton_Bone_map(animation_frameindex,skeleton_name,modify_skeleton_transform);
		//
		//}
		//if (play_anim)//播放骨骼动画对勾勾上是否勾上
		//{
		//	skeletonnode->playanim(playanimation_time_index);
		//	skeletonnode->update_boneMatrix();
		//	playanimation_time_index++;
		//}
		
		skeleton_string_array=skeletonnode->Get_skeleton_nameArray();
		for (int currentskeletonindex=0; currentskeletonindex <110; currentskeletonindex++)
		{
			modelNodeflag = selectedModeNodeindex == currentskeletonindex ? true : false;
			ImGui::TreeNodeEx(skeleton_string_array[currentskeletonindex].c_str(), modelNodeflag);
			if (ImGui::IsItemClicked())
			{
				skeleton_name = skeleton_string_array[currentskeletonindex];//传进去选中的骨骼节点名字
				selectedModeNodeindex = currentskeletonindex;
				
			}
			
		}
		
			//更新骨骼矩阵
		/*ImGui::Text("Root");
		ImGui::SliderFloat3("position", &(Rootposition.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("rotation", &(RootRotation.x), 0.0f, 360.0f);
		ImGui::NextColumn();
	
		ImGui::Text("frameindex");
		ImGui::SliderInt("time", &animation_frameindex, 0.0f, 40.0f);


		ImGui::Checkbox("Run",&RobotRun);	ImGui::Checkbox("punch", &RobotPunch);	ImGui::Checkbox("Flip_over", &Robotflip);
		ImGui::Checkbox("fire", &fire); ImGui::Checkbox("wireframe", &wireframe);
		ImGui::Text("minigun");
		ImGui::SliderFloat3("minigunposition", &(minigunRootposition.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("minigunrotation", &(minigunRootRotation.x), 0.0f, 360.0f);
		
		ImGui::Text("bird");
		ImGui::SliderFloat3("birdposition", &(birdRootposition.x), -100.0f, 100.0f);
		ImGui::SliderFloat3("birdrotation", &(birdRootRotation.x), 0.0f, 360.0f);
	*/

		ImGui::Begin("scene");
		ImGui::SliderFloat("FOV", &FOV, 30.3f, 321.0f);
		ImGui::SliderFloat("znear", &znear, 0.3f, 10.0f);
		ImGui::SliderFloat("zfar", &zfar, 100.0f, 30000000.0f);



		ImGui::Text("light");
		ImGui::SliderFloat3("lightpos", &(lightpos.x), -110.7f, 700.0f);
		ImGui::SliderFloat("lightshineness", &lightshineness, 0.0f, 79.0f);
		ImGui::SliderFloat3("lightcolor", &(lightcolor.x), 0.0f, 1.0f);

		ImGui::Text("base");
		ImGui::SliderFloat3("baseposition", &(modelpos[0].x), -200.0f, 260.0f);
		ImGui::SliderFloat3("baseangle", &(objectangle[0].x), -360.0f, 360.0f);
		ImGui::SliderFloat3("basescale", &(skeletonscale.x), 0.0f, 30.0f);
		
		auto bone_Matrix = skeletonnode->GetbonespaceMatrix(skeleton_name);
		ImGui::Text("skeleton");
		ImGui::SliderFloat3("skeletonmposition", &(bone_Matrix->translation.x), -200.0f, 609.0f);
		ImGui::SliderFloat3("skeletonmangle", &(bone_Matrix->rotation.x), -360.0f, 360.0f);

		//ImGui::Text("setanimationmap");
		//ImGui::SliderInt("frameindex", &animation_frameindex, 0, 3800);
		//ImGui::SliderFloat3("bonemposition", &(modify_skeleton_translation.x), -200.0f, 609.0f);
		//ImGui::SliderFloat3("boneangle", &(modify_skeleton_rotation.x), -360.0f, 360.0f);

		//ImGui::Checkbox("movesingleskeleton", &movesingle_skeleton);	ImGui::Checkbox("setanim", &set_skeleton_map);	
		//ImGui::Checkbox("playanim", &play_anim);
		ImGui::Text("DAEMODEL");
		ImGui::SliderFloat3("DAEposition", &(modelpos[1].x), -200.0f, 260.0f);
		ImGui::SliderFloat3("DAEangle", &(objectangle[1].x), -360.0f, 360.0f);
	//	/*ImGui::Text("mirror position");
	//	ImGui::SliderFloat3("mirror position", &(mirrorposition.x), -900.0f, 960.0f);
	//	ImGui::Text("mirror angle");
	//	ImGui::SliderFloat3("mirror angle", &(mirrorRotation.x), 0.0f, 360.0f);
	//*/
		ImGui::Text("camera");
		ImGui::SliderFloat3("camera", &(camera.x), -100.0f, 1000.0f);
	/*	ImGui::SliderFloat("left", &(left), -100.0f, 140.0f);
		ImGui::SliderFloat("right", &right, 110.0f, 270.0f);
		ImGui::SliderFloat("bottom", &bottom, -400.0f, 400.0f);
		ImGui::SliderFloat("top", &top, 100.0f, 700.0f);
		ImGui::SliderFloat("near", &lnear, -100.0f, 1.0f);
		ImGui::SliderFloat("far", &lfar, 7.5f, 1000.0f);

		ImGui::Text("shadersetting");
		ImGui::SliderFloat3("environmentlight", &(environmentlight.x), 0.0, 1.0f);*/
	

		ImGui::ColorEdit3("clear color", (float*)clearcolor); // Edit 3 floats representing a color



		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(pwindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}