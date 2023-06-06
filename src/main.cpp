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
#include "Entity.h"

// TODO: add slider to allow changing the amount of rects in the quadtree.
const int COUNT = 500;
int COUNT_SQUARED = COUNT*COUNT;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

SDL_Renderer* renderer;
SDL_Window* window;
ImGuiIO io; // idk what this is for rn, but imgui needs it

int setup();
void showImGui(int collisions, int entityCount, int checksThisFrame, int total);
bool detectAndResolveCollision(Entity* rect1, Entity* rect2);

// Main code
int main(int, char**)
{
    QuadTree* qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//,

    // TODO: rename to something less shit.
    // Rects that will exist in our tree.
    std::vector<Entity*> dots;

    int entSize = 35;
    for (int i = 0; i < COUNT; i++) {
        float numbX = rand() % WINDOW_WIDTH;
        float numbY = rand() % WINDOW_HEIGHT;
        float size = rand() % entSize;
        Entity* entity = new Entity(numbX, numbY, size, size);
        //Entity* dot = new Entity(numbX, numbY, 6, 6);
        qTree->insert(entity);
        dots.push_back(entity);
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
            if (event.type == SDL_MOUSEBUTTONDOWN){
              int xMousePos = event.button.x;
              int yMousePos = event.button.y;

              // Spawn an entity at position of mouseclick.
              float size = rand() % entSize;
              Entity* entity = new Entity(xMousePos, yMousePos, size, size);
              dots.push_back(entity);
            }
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
        qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//,
        //quadTreeLimit, quadTreeDepthLimit);
        for (int i = 0; i < dots.size(); i++) {
            qTree->insert(dots[i]);
        }

        // Hashtable to keep track of collisions which have already happened.
        // K: id of dot
        // V: vector containing all ids which this id has already collided
        // with.
        // Leaving commented because I'm switching to different method.
        //std::unordered_map<int, std::vector<int>> id_map;

        // TODO: put this into a function or something to make it easier to
        // stomach.
        // Check/handle collisions of rects
        int collisionsThisFrame = 0;
        int dotsCheckedThisFrame = 0;
        for(int i=0; i<dots.size(); i++){
          Entity* curr = dots[i];
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
              Entity* other = currLeaf->points[k];

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

        // Demo just detects collisions and no physics handling, so I don't
        // differentiate the same collision from both entities' perspective.
        // So just divide by 2 to show correct amount of collisions.
        collisionsThisFrame = collisionsThisFrame / 2;

        // Rendering //

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render entire quadtree (and the entities contained in it)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        qTree->draw(renderer);

        showImGui(collisionsThisFrame, dots.size(), dotsCheckedThisFrame, dots.size()*dots.size());


        SDL_RenderPresent(renderer);
    }

    // Cleanup
    qTree->~QuadTree();
    //TODO: figure this shit out.
    //while(!dots.empty()) delete dots.front(), dots.pop_back();

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// TODO: could make this better, but not a priority.
// Parameters are all the stuff ImGui needs to display not in scope.
void showImGui(int collisions, int entityCount, int checksThisFrame, int total) {
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

    collisionText += std::to_string(collisions);
    countText += std::to_string(entityCount);
    comparisonText += std::to_string(checksThisFrame);
    lazyText += std::to_string(total);

    ImGui::Text(countText.c_str());
    ImGui::Text(comparisonText.c_str());
    ImGui::Text(lazyText.c_str());
    ImGui::Text(collisionText.c_str());
    ImGui::Button("Delete all");

    // Sliders which change the value of static ints in QuadTree
    // which limit QuadTree max depth and max entities per leaf.
    ImGui::SliderInt("'Entity Limit':", &QuadTree::LIMIT, 0, 9);
    ImGui::SliderInt("'QuadTree Depth Limit':", &QuadTree::DEPTH_LIMIT, 0, 9);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

/* Setup required for sdl and imgui.
 * Mostly just ripped from the example imgui provides.
 */
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

bool checkCollision(Entity* dot1, Entity* dot2){
  SDL_FRect rect1 = *dot1->rect;
  SDL_FRect rect2 = *dot2->rect;

  bool xCollision = (((rect1.x + rect1.w) >= (rect2.x)) && ((rect2.x + rect2.w) >= (rect1.x)));
  bool yCollision = (((rect1.y + rect1.h) >= (rect2.y)) && ((rect2.y + rect2.h) >= (rect1.y)));

  return xCollision && yCollision;
}

// TODO: remove this and just use collision function above,
// I'm not resolving this shit in this project, will save for
// the game I make to try and decouple physics in its own module.
bool detectAndResolveCollision(Entity* dot1, Entity* dot2){
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
    } else {
      dot1->xVel = -dot1->xVel;
      dot2->xVel = -dot2->xVel;
    }
  }
  return collision;
}

