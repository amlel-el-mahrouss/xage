/*
 * =====================================================================
 *
 *			XAGE Audio System
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: XPX Sound backend.
 *
 * =====================================================================
 */

#pragma once

namespace RRMan
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Collections::Generic;

	public ref class XSD
	{
		File^ FileRef;
		String^ TexturePath;
		List<float>^ IndicesRef;
		List<float>^ VerticesRef;
	};
}

/*
* #! XSD
* 
* #SCENE_SYSTEM: Sedan
* #UUID: AD56738F-A55B-43CF-9283-2157D8C03882
* #WAVEFRONT: C:/Users/Something/Models/Sedan.obj
* ...
*/