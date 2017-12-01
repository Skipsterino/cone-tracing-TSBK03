/**
* @file	GenericScene.cpp
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Abstract class for a generic scene object
*/

#include "GenericScene.h"
#include <iostream>


GenericScene::~GenericScene()
{
	for (auto i : sceneObjs) delete i.second;
	for (auto i : shaders) delete i.second;
	for (auto i : textures) delete i.second;
}

