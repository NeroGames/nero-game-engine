////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/ScriptHolder.h>
#include <Nero/core/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ScriptHolder::ScriptHolder()
	{
		m_Configuration = loadJson("setting/resource")["script"];
    }

    void ScriptHolder::load()
    {

    }
}

