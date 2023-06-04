#include "sorting.h"
#include <cstdlib>
#include <cassert>
#include <unordered_map>

// sdl/imgui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL.h>

#include "Tree.h"
#include "QuadTree.h"
#include "Dot.h"

const int COUNT = 50;
int COUNT_SQUARED = COUNT*COUNT;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

static int quadTreeLimit = 4;
static int quadTreeDepthLimit = 5;

SDL_Renderer* renderer;
SDL_Window* window;
ImGuiIO io;

int setup();
//void showImGui();
bool detectAndResolveCollision(Dot* rect1, Dot* rect2);

// Main code
int main(int, char**)
{
    QuadTree* qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        quadTreeLimit, quadTreeDepthLimit);

    // Rects that will exist in our tree.
    std::vector<Dot*> dots;

    for (int i = 0; i < COUNT; i++) {
        float numbX = rand() % WINDOW_WIDTH;
        float numbY = rand() % WINDOW_HEIGHT;
        Dot* dot = new Dot(numbX, numbY, 35, 35);
        //Dot* dot = new Dot(numbX, numbY, 6, 6);
        qTree->insert(dot);
        dots.push_back(dot);
    }
    printf("Initial QuadTree setup.\n"); 

    // Sets up SDL and imgui
    setup();


    // main game loop starts here
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

        // Simulate game state (motion of rects)
        float dt = (SDL_GetTicks() - lastPhysicsUpdate)/1000;
        for (int i = 0; i < dots.size(); i++) {
            dots[i]->simulate(dt);
        }
        lastPhysicsUpdate = SDL_GetTicks();

        // Reconstruct QuadTree of all entities/rects
        // TODO: Should just call an update function to do this internally
        // so we don't have to construct one of these every frame.
        qTree->~QuadTree();
        qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        quadTreeLimit, quadTreeDepthLimit);
        for (int i = 0; i < dots.size(); i++) {
            qTree->insert(dots[i]);
        }

        // Hashtable to keep track of collisions which have already happened.
        // K: id of dot
        // V: vector containing all ids which this id has already collided
        // with.
        // Leaving commented because I'm switching to different method.
        //std::unordered_map<int, std::vector<int>> id_map;

        // Check/handle collisions of rects
        int collisionsThisFrame = 0;
        int dotsCheckedThisFrame = 0;
        for(int i=0; i<dots.size(); i++){
          Dot* curr = dots[i];
          std::vector<QuadTree*> currLeafs = qTree->getLeafs(curr);

          // Keep tracks of rects collided with to avoid duplicates.
          std::vector<int> alreadyCollidedDots;

          // TODO: put all this into a function.
          // For each leaf this rect is inside of
          for(int j=0; j<currLeafs.size(); j++){
            QuadTree* currLeaf = currLeafs[j];
            // Iterate through other rects inside of this leaf
            // and check for collision with 'curr' rect.
            for(int k=0; k<currLeaf->points.size(); k++){
              dotsCheckedThisFrame++;
              Dot* other = currLeaf->points[k];

              // if they're the same dot ignore checking this one
              if(curr->id != other->id){

                // Ensure other hasn't been collided with by this rect already.
                // TODO: fix this counting every collision twice.
                if(std::count(alreadyCollidedDots.begin(), alreadyCollidedDots.end(), other->id) == 0){
                  // check if they're colliding and resolve it.
                  // for now, just detecting it.
                  bool collision = detectAndResolveCollision(curr, other);
                  if(collision) {
                    collisionsThisFrame++;
                    alreadyCollidedDots.push_back(other->id);
                  }
                }
              }
            }
          }
        }



        // Rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render entire quadtree (and the rects contained in it)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        qTree->draw(renderer);

        // ImGui stuff
        // MY WINDOW CREATED HERE
        // TODO: Could move this back into a function/separate file.
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        bool show = true;

        ImGui::ShowDemoWindow(&show);
        ImGui::Begin("Info");
        std::string comparisonText = "Number of comparisons this frame:";
        std::string lazyText = "Comparisons w/o tree (brute force):";
        std::string countText = "Total:";
        std::string collisionText = "Collisions this frame:";
        collisionText += std::to_string(collisionsThisFrame);
        countText += std::to_string(COUNT);
        comparisonText += std::to_string(dotsCheckedThisFrame);
        lazyText += std::to_string(COUNT_SQUARED);
        ImGui::Text(countText.c_str());
        ImGui::Text(comparisonText.c_str());
        ImGui::Text(lazyText.c_str());
        ImGui::Text(collisionText.c_str());

        // Sliders
        ImGui::SliderInt("'Rect Limit' slider", &quadTreeLimit, 1, 12);
        ImGui::SliderInt("'Depth Limit' slider", &quadTreeDepthLimit, 1, 12);

        ImGui::End();
        // MY WINDOW CREATED HERE

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// old function for handling imgui window.
void showImGui() {
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    bool show = true;

    ImGui::ShowDemoWindow(&show);
    //ImGui::Begin("Algorithm Visualizer");
    ImGui::Begin("Info");

    /*
    const char* items[] = { "Selection Sort", "Merge Sort", "etc" };
    static int item_current = 1;
    ImGui::ListBox("Pick algorithm", &item_current, items, IM_ARRAYSIZE(items), 4);

    if (ImGui::Button("Start")) {
        //selectionSort(arr, COUNT);
        //printArray(arr, COUNT);
        //printf("Status: %s", (isSorted(arr, COUNT)) ? "sorted" : "NOT SORTED");
    }
    */
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

bool checkCollision(Dot* dot1, Dot* dot2){
  SDL_FRect rect1 = *dot1->rect;
  SDL_FRect rect2 = *dot2->rect;

  bool xCollision = (((rect1.x + rect1.w) >= (rect2.x)) && ((rect2.x + rect2.w) >= (rect1.x)));
  bool yCollision = (((rect1.y + rect1.h) >= (rect2.y)) && ((rect2.y + rect2.h) >= (rect1.y)));

  return xCollision && yCollision;
}

bool detectAndResolveCollision(Dot* dot1, Dot* dot2){
  bool collision = checkCollision(dot1, dot2);
  SDL_FRect rect1 = *dot1->rect;
  SDL_FRect rect2 = *dot2->rect;

  if(collision){
    // find out which axis is easier to resolve (recall warehouse escape) 
    // should be fine for this i think.
    // First figure out which side the collision is happening relative to dot1.
    float xDist, yDist;
    if(rect1.x < rect2.x){
      xDist = ((rect1.x + rect1.w) - rect2.x);
    } else {
      xDist = ((rect2.x + rect2.w) - rect1.x);
    }

    if(rect1.y < rect2.y){
      yDist = ((rect1.y + rect1.h) - rect2.y);
    } else {
      yDist = ((rect2.y + rect2.h) - rect1.y);
    }

    if(xDist > yDist){
      dot1->yVel = -dot1->yVel; 
      dot2->yVel = -dot2->yVel; 
      printf("yvel flipped\n");
    } else {
      dot1->xVel = -dot1->xVel;
      dot2->xVel = -dot2->xVel;
      printf("xvel flipped\n");
    }
  }
  return collision;
}

