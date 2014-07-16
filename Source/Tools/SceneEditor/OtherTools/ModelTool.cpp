#include "stdafx.h"
#include "ModelTool.h"

#include "WorkQueue.h"

#include <assimp/cimport.h>	//导入器
#include <assimp/scene.h>	//读取到的模型数据都放在scene中
#include <assimp/postprocess.h>	//包含后处理的标志位定义

#include <assimp/DefaultLogger.hpp>

ModelTool::ModelTool(QWidget* parent)
{
	initUi();

	context_ = new Context();
	context_->RegisterSubsystem(new FileSystem(context_));
	context_->RegisterSubsystem(new ResourceCache(context_));
	context_->RegisterSubsystem(new WorkQueue(context_));

	RegisterSceneLibrary(context_);
	RegisterGraphicsLibrary(context_);
	RegisterPhysicsLibrary(context_);

	unsigned flags = 
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_GenUVCoords |
		aiProcess_FindInstances |
		aiProcess_OptimizeMeshes;

}

ModelTool::~ModelTool()
{

}

void ModelTool::initUi()
{
	this->setWindowTitle("Model Translate Tool");
	this->setWindowIcon(QIcon("Images/ModelTool.png"));

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	this->setLayout(layout);

	//导出类型
	radModel = new QRadioButton("Output a model");
	radModel->setChecked(true);
	layout->addWidget(radModel);
	radScene = new QRadioButton("Output a scene");
	layout->addWidget(radScene);
	radNode = new QRadioButton("Output a node and its children (prefab)");
	layout->addWidget(radNode);

	//
	chkB = new QCheckBox("-b Save scene in binary format, default format is XML");
	layout->addWidget(chkB);
	chkH = new QCheckBox("-h Generate hard instead of smooth normals if input file has no normals");
	layout->addWidget(chkH);

	chkI = new QCheckBox("-i Use local ID's for scene nodes");
	layout->addWidget(chkI);

	chkL = new QCheckBox("-l Output a material list file for models");
	layout->addWidget(chkL);

	chkNA = new QCheckBox("-na Do not output animations");
	layout->addWidget(chkNA);

	chkNM = new QCheckBox("-nm Do not output materials");
	layout->addWidget(chkNM);

	chkNT = new QCheckBox("-nt Do not output material textures");
	layout->addWidget(chkNT);

	chkNC = new QCheckBox("-nc Do not use material diffuse color value, instead output white");
	layout->addWidget(chkNC);

	chkNH = new QCheckBox("-nh Do not save full node hierarchy (scene mode only)");
	layout->addWidget(chkNH);

	chkNS = new QCheckBox("-ns Do not create subdirectories for resources");
	layout->addWidget(chkNS);

	chkNZ = new QCheckBox("-nz Do not create a zone and a directional light (scene mode only)");
	layout->addWidget(chkNZ);

	chkNF = new QCheckBox("-nf Do not fix infacing normals");
	layout->addWidget(chkNF);

	chkT = new QCheckBox("-t Generate tangents");
	layout->addWidget(chkT);

	chkV = new QCheckBox("-v Enable verbose Assimp library logging");
	layout->addWidget(chkV);

	chkEAO = new QCheckBox("-eao Interpret material emissive texture as ambient occlusion");
	layout->addWidget(chkEAO);

	chkCM = new QCheckBox("-cm Check and do not overwrite if material exists");
	layout->addWidget(chkCM);

	chkCT = new QCheckBox("-ct Check and do not overwrite if texture exists");
	layout->addWidget(chkCT);

	chkCTN = new QCheckBox("-ctn Check and do not overwrite if texture has newer timestamp");
	layout->addWidget(chkCTN);

	btnExport = new QPushButton("export");
	layout->addWidget(btnExport);
}

void ModelTool::SetModelPath(const char* pPath)
{

}

bool ModelTool::import(const char* pPath)
{
//	Assimp::Importer importer;
	return false;
}
