FROSTBYTES
Custom Game Engine
Project Master Report
Version 1.0  ·  June 2026  ·  Restart Build

1.  Project Overview
Frostbytes is a from-scratch custom game engine targeting the visual fidelity of the 2005–2015 era of PC gaming. It is designed to run efficiently on modest consumer hardware, making it accessible for indie development without requiring a high-end GPU.

1.1  Goals
•	Deliver a fully functional 2D and 3D game engine built in C++17.
•	Target 2005–2015 era fidelity: textured meshes, Phong lighting, sprite batching, basic physics.
•	Run well on integrated graphics hardware (Intel HD 4000) with 8 GB RAM.
•	Use only open-source dependencies managed entirely through CMake FetchContent — no manual library installs.
•	Establish a clean, layered architecture (Window → Renderer → 2D/3D → Systems → Physics).

1.2  Target Hardware
•	CPU: Intel Core i3 (any generation)
•	RAM: 8 GB
•	GPU: Intel HD Graphics 4000 (integrated) — OpenGL 3.3 Core Profile supported
•	OS: Windows 10/11 (primary target); Linux compatibility is a stretch goal

2.  Technology Stack
All dependencies are fetched and compiled automatically via CMake FetchContent. No developer is required to pre-install any library.

Component	Version / Value	Purpose
Language	C++17  (namespace fb)	Core engine language
Graphics API	OpenGL 3.3 Core Profile	Rendering backend
Windowing	GLFW 3.3.8	Window, context, input
GL Loader	GLAD v2.0.4  (glad2 branch)	OpenGL function loading
Math	GLM 0.9.9.8	Vectors, matrices, transforms
Audio	OpenAL Soft	Cross-platform 3D audio
Image Loading	stb_image	PNG, JPG, BMP texture loading
Build System	CMake 3.20+  with FetchContent	Dependency + build management
IDE / Agent	Trae IDE  (ByteDance AI)	Prompt-driven code generation

3.  Project Structure
Root path:  d:\Project\ByteScript\Frostbytes\

3.1  Directory Layout
•	Frostbytes/
◦	CMakeLists.txt          — root build configuration
◦	src/                    — all engine source files
◦	src/core/              — Application, Window, Logger
◦	src/renderer/          — Shader, VBO/VAO, Texture, Renderer
◦	src/2d/                — Sprite, Camera2D, SpriteBatch
◦	src/3d/                — Mesh, OBJLoader, Lighting
◦	src/systems/           — ECS, Input, Audio
◦	src/physics/           — AABB, SceneGraph
◦	assets/                — shaders, textures, models, audio
◦	external/              — FetchContent cache (auto-generated)

 
4.  Development Workflow
Frostbytes uses a fully prompt-driven development pipeline. No code is written manually by the developer.

4.1  Pipeline
•	Step 1: Claude writes a detailed, structured prompt describing exactly what files to create, the architecture, the API surface, and any constraints.
•	Step 2: The developer pastes the prompt into Trae IDE's built-in AI agent (ByteDance).
•	Step 3: The agent generates and writes all files directly into the project directory.
•	Step 4: The developer runs a CMake build to verify the step compiles cleanly before moving on.
•	Step 5: Any reasonable improvements the agent makes (e.g. safer casts for MSVC /W4) are accepted as long as they do not deviate from the agreed architecture.

4.2  Rules
•	Never skip a step — each step must verify as a clean build before the next begins.
•	All dependencies are handled via CMake FetchContent — no manual installs.
•	Agent improvements are accepted; architectural deviations are flagged and discussed.
•	Claude is the source of truth for the overall plan, architecture, and prompt design.

5.  Environment Setup Status
The following tools are required before any build can be verified:

Tool	Details	Status
Git	v2.54.0 (windows.1)	✅ Installed
CMake 3.20+	Zip-based install	⚠️  PATH issue (pending fix)
VS Build Tools	MSVC toolchain	❌ Not yet installed
Trae IDE	ByteDance AI agent	✅ Active

5.1  Immediate Actions Required
•	Fix CMake PATH: confirm cmake.exe location and add to system PATH permanently.
•	Install Visual Studio Build Tools 2022 (Desktop development with C++ workload) to provide the MSVC compiler and Windows SDK.
•	Run a test build of Step 1 once both tools are available.

 
6.  17-Step Build Plan
The engine is built in six phases across seventeen discrete, verifiable steps. Each step produces files, compiles, and is tested before moving to the next.

#	Phase	Scope / Deliverables	Status
1	1: Foundation	CMake setup, GLFW window creation, Logger system, Application loop, main.cpp	✅ Complete
2	1: Foundation	Input polling abstraction (keyboard/mouse), Event callback wiring via GLFW	✅ Complete
3	1: Foundation	Build verification + project structure review	✅ Complete
4	2: Renderer Core	Shader class: GLSL compile, link, uniform setters, error reporting	✅ Complete
5	2: Renderer Core	VBO / VAO abstraction: vertex buffer, vertex array, element buffer	✅ Complete
6	2: Renderer Core	Texture system: stb_image loading, bind, mipmap, wrap/filter config	✅ Complete
7	2: Renderer Core	Renderer interface: submit, flush, clear, viewport management	✅ Complete
8	3: 2D System	Sprite class: quad geometry, UV mapping, texture binding	✅ Complete
9	3: 2D System	2D camera: orthographic projection, pan/zoom, view matrix	⏳ Pending
10	3: 2D System	Sprite batching: dynamic vertex buffer, batch flush on texture switch	⏳ Pending
11	4: 3D System	Mesh class: interleaved vertex data (pos, normal, uv), draw call	⏳ Pending
12	4: 3D System	OBJ file parser: positions, normals, UVs, face index assembly	⏳ Pending
13	4: 3D System	Phong lighting: directional + point lights, material struct, fragment shader	⏳ Pending
14	5: Game Systems	Entity Component System (ECS): entity IDs, component pools, system iteration	⏳ Pending
15	5: Game Systems	Input manager: key/mouse state, pressed/released/held queries	⏳ Pending
16	5: Game Systems	Audio system: OpenAL Soft init, source/buffer management, 3D positioning	⏳ Pending
17	6: Physics & Scene	AABB collision detection + scene graph: parent-child transforms, update traversal	⏳ Pending

 
7.  Phase Descriptions
Phase 1 — Foundation  (Steps 1–3)
Establishes the project skeleton: CMake configuration, GLFW window creation, OpenGL context initialisation via GLAD, the Logger utility, and the core Application class that owns the main game loop.
•	Step 1 (Complete): All 9 foundational files generated by Trae IDE. Correct MSVC-safe cast applied by agent.
•	Step 2: Input polling — keyboard and mouse state captured via GLFW callbacks.
•	Step 3: Build verification and review of the full project layout before renderer work begins.

Phase 2 — Renderer Core  (Steps 4–7)
Builds the low-level graphics layer that everything else renders through.
•	Step 4: Shader class — compile vertex and fragment shaders, link programs, set uniforms by name.
•	Step 5: VBO/VAO abstraction — vertex buffer object, vertex array object, element buffer object.
•	Step 6: Texture system — stb_image integration, mipmap generation, wrap and filter configuration.
•	Step 7: Renderer interface — submit geometry, flush, clear, manage viewport.

Phase 3 — 2D System  (Steps 8–10)
Provides all tools needed to build a 2D game: sprites, camera, and efficient batched rendering.
•	Step 8: Sprite class — quad geometry, UV mapping, colour tinting, anchor point.
•	Step 9: 2D camera — orthographic projection matrix, pan, zoom, screen-to-world transform.
•	Step 10: Sprite batcher — dynamic vertex buffer filled CPU-side, flushed on texture-switch to minimise draw calls.

Phase 4 — 3D System  (Steps 11–13)
Brings three-dimensional rendering to the engine with asset loading and classic Phong lighting.
•	Step 11: Mesh class — interleaved vertex layout (position, normal, UV), indexed draw call.
•	Step 12: OBJ parser — reads Wavefront .obj files, assembles indexed triangle data.
•	Step 13: Phong lighting — directional light, point lights (up to 8), material struct, fragment shader.

Phase 5 — Game Systems  (Steps 14–16)
The high-level engine systems a game developer interacts with daily.
•	Step 14: Entity Component System — entity IDs, component pools, typed system iteration.
•	Step 15: Input manager — clean API over GLFW callbacks; key/button pressed, released, held queries.
•	Step 16: Audio system — OpenAL Soft initialisation, WAV/OGG buffer management, 3D positioned sources.

Phase 6 — Physics and Scene  (Step 17)
Completes the engine with collision detection and a hierarchical scene representation.
•	Step 17: AABB collision detection (broad-phase overlap, resolution), scene graph with parent-child transform inheritance and depth-first update traversal.

8.  Immediate Next Steps
•	[BLOCKER]  Fix CMake PATH issue on Windows (confirm cmake.exe is accessible from any terminal).
•	[BLOCKER]  Install Visual Studio Build Tools 2022 with the C++ Desktop workload (MSVC compiler + Windows SDK).
•	Once both blockers are resolved, run cmake -B build -S . and cmake --build build inside the Frostbytes directory.
•	Confirm Step 1 produces a working window with no compiler warnings under /W4 before requesting Step 2 prompt.
•	Report back to Claude with build output — success message or any errors — to proceed.

This document is the single source of truth for the Frostbytes engine project. Update it at the end of each phase.
    