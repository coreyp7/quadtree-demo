#include "sorting.h"
#include <cstdlib>
#include <cassert>

// sdl/imgui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL.h>

#include "Tree.h"
#include "QuadTree.h"
#include "Dot.h"

const int COUNT = 1000;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

SDL_Renderer* renderer;
SDL_Window* window;
ImGuiIO io;

int setup();
void showImGui();

//int main(int, char**){
//
//	//// Make random array
//	//int items[COUNT];
//	//for (int i = 0; i < COUNT; i++) {
//	//	int numb = rand() % 1000;
//	//	items[i] = numb;
//	//}
//
//	//printf("before:");
//	//printArray(items, COUNT);
//
//	//sort(items, COUNT);
//
//	//printf("after:");
//	//printArray(items, COUNT);
//	//printf("Status: %s", (isSorted(items, COUNT)) ? "sorted" : "NOT SORTED");
//
//    return 0;
//}

// Main code
int main(int, char**)
{
    QuadTree* qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    /*SDL_FPoint point1 = { 250, 250 };
    SDL_FPoint point2 = { 1050, 250 };
    SDL_FPoint point3 = { 950, 250 };
    SDL_FPoint point4 = { 750, 250 };
    qTree->insert(point1);
    qTree->insert(point2);
    qTree->insert(point3);
    qTree->insert(point4);*/

    std::vector<Dot*> dots;

    for (int i = 0; i < 20; i++) {
        float numbX = rand() % WINDOW_WIDTH;
        float numbY = rand() % WINDOW_HEIGHT;
        //SDL_FPoint point = {numbX, numbY};
        Dot* dot = new Dot(numbX, numbY, 35, 35);
        qTree->insert(dot);
        dots.push_back(dot);
    }
    /*Dot* dot = new Dot(3, 3, 35, 35);
    dot->xVel = 500;
    dot->yVel = 500;
    qTree->insert(dot);
    dots.push_back(dot);*/
    printf("Everything inserted.\n");


    setup();

    float lastPhysicsUpdate = 0;

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        float dt = (SDL_GetTicks() - lastPhysicsUpdate)/1000;
        for (int i = 0; i < dots.size(); i++) {
            dots[i]->simulate(dt);
        }
        lastPhysicsUpdate = SDL_GetTicks();

        qTree->~QuadTree();
        qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        for (int i = 0; i < dots.size(); i++) {
            qTree->insert(dots[i]);
        }

        // Render quadtree

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        /*SDL_Rect rect = { qTree->x, qTree->y, qTree->width, qTree->height };
        SDL_RenderDrawRect(renderer, &rect);*/
        qTree->draw(renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //Tree* tree = new Tree(5);
    ///*for (int i = 0; i < 500; i++) {
    //    int numb = rand() % 1000;
    //    tree.insert(numb);
    //}*/

    ////tree.insert(3);
    //tree->insert(6);
    //tree->insert(87);
    //tree->insert(73);
    //tree->insert(543);
    //tree->insert(47);
    //tree->insert(3);

    //printf(tree->to_string().c_str());
    //printf("\n");

    //tree->remove(87);
    ////printf("%i\n", tree.searchTree(5)->value); // TODO: have search tree return tree value is in

    //printf("%s\n", (tree->valid() ? "valid" : "INVALID BST"));
    //printf("%i\n", tree->min()->value);
    //printf("%i\n", tree->max()->value);

    //printf(tree->to_string().c_str());



//    // Setup SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
//    {
//        printf("Error: %s\n", SDL_GetError());
//        return -1;
//    }
//
//    // From 2.0.18: Enable native IME.
//#ifdef SDL_HINT_IME_SHOW_UI
//    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
//#endif
//
//    // Create window with SDL_Renderer graphics context
//    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
//    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
//    if (renderer == nullptr)
//    {
//        SDL_Log("Error creating SDL_Renderer!");
//        return 0;
//    }
//    //SDL_RendererInfo info;
//    //SDL_GetRendererInfo(renderer, &info);
//    //SDL_Log("Current SDL_Renderer: %s", info.name);
//
//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//    //ImGui::StyleColorsLight();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
//    ImGui_ImplSDLRenderer_Init(renderer);
//
//    // Load Fonts
//    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
//    // - Read 'docs/FONTS.md' for more instructions and details.
//    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//    //io.Fonts->AddFontDefault();
//    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
//    //IM_ASSERT(font != nullptr);
//
//    // Our state
//    bool show_demo_window = false;
//    bool show_another_window = false;
//    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//    // Make random array
//	int arr[COUNT];
//	for (int i = 0; i < COUNT; i++) {
//		int numb = rand() % 1000;
//		arr[i] = numb;
//	}
//
//    // Main loop
//    bool done = false;
//    while (!done)
//    {
//        // Poll and handle events (inputs, window resize, etc.)
//        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
//        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
//        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
//        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
//        SDL_Event event;
//        while (SDL_PollEvent(&event))
//        {
//            ImGui_ImplSDL2_ProcessEvent(&event);
//            if (event.type == SDL_QUIT)
//                done = true;
//            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
//                done = true;
//        }
//
//
//
//        // Rendering
//        showImGui();
//        ImGui::Render();
//        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
//        //SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//        SDL_RenderClear(renderer);
//        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
//        SDL_RenderPresent(renderer);
//    }
//
//    // Cleanup
//    ImGui_ImplSDLRenderer_Shutdown();
//    ImGui_ImplSDL2_Shutdown();
//    ImGui::DestroyContext();
//
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();

    return 0;
}

void showImGui() {
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    /*ImGui::ShowDemoWindow(&show_demo_window);*/
    ImGui::Begin("Algorithm Visualizer");

    const char* items[] = { "Selection Sort", "Merge Sort", "etc" };
    static int item_current = 1;
    ImGui::ListBox("Pick algorithm", &item_current, items, IM_ARRAYSIZE(items), 4);

    if (ImGui::Button("Start")) {
        //printf(items[item_current]);
        /*printArray(arr, COUNT);
        selectionSort(arr, COUNT);
        printArray(arr, COUNT);
        printf("Status: %s", (isSorted(arr, COUNT)) ? "sorted" : "NOT SORTED");*/
    }
    ImGui::End();
}

/* Setup required for sdl and imgui. */
int setup() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    return 0;
}