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

const int DEFAULT_COUNT = 500;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Globals because this just a demo project to implement a QuadTree.
SDL_Renderer* renderer;
SDL_Window* window;
ImGuiIO io; // idk what this is for rn, but imgui needs it

int setup();
void showImGui(int collisions, int entityCount, int checksThisFrame, int total);
bool detectAndResolveCollision(Entity* rect1, Entity* rect2);
void countCollisions(QuadTree *qTree, std::vector<Entity*> *entities, int* collisionsThisFrame, int* entitiesCheckedThisFrame);

// Main code
int main(int, char**)
{
    QuadTree* qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);//,

    // Entities (rects) that will exist in our tree.
    std::vector<Entity*> entities;

    int entSize = 35;
    for (int i = 0; i < DEFAULT_COUNT; i++) {
        float numbX = rand() % WINDOW_WIDTH;
        float numbY = rand() % WINDOW_HEIGHT;
        float size = rand() % entSize;
        Entity* entity = new Entity(numbX, numbY, size, size);
        qTree->insert(entity);
        entities.push_back(entity);
    }
    printf("Initial QuadTree setup.\n"); 

    // Sets up SDL and imgui
    setup();

    // Main game loop starts here
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
              entities.push_back(entity);
            }
        }

        // Simulate game state (motion of entities).
        // Use delta time to make motion consistent.
        float dt = (SDL_GetTicks() - lastPhysicsUpdate)/1000;
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->simulate(dt);
        }
        lastPhysicsUpdate = SDL_GetTicks();

        // Reconstruct QuadTree of all entities/rects
        // TODO: Should just call an update function to do this internally
        // so we don't have to construct one of these every frame.
        qTree->~QuadTree();
        qTree = new QuadTree(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        for (int i = 0; i < entities.size(); i++) {
            qTree->insert(entities[i]);
        }

        int collisionsThisFrame = 0;
        int entitiesCheckedThisFrame = 0;
        countCollisions(qTree, &entities, &collisionsThisFrame, &entitiesCheckedThisFrame); 

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

        showImGui(collisionsThisFrame, entities.size(), entitiesCheckedThisFrame, entities.size()*entities.size());

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

// Detects collisions using QuadTree.
// Will update int* params with info regarding detection.
void countCollisions(QuadTree *qTree, std::vector<Entity*> *entities, 
    int* collisionsThisFrame, int* entitiesCheckedThisFrame){

    for(int i=0; i<entities->size(); i++){
      Entity* currentEntity = entities->at(i);

      // Get all the leafs which contain curr entity. (max of 4)
      std::vector<QuadTree*> currentEntityLeafs = qTree->getLeafs(currentEntity);

      // Keep tracks of rects collided with to avoid duplicates.
      std::vector<int> alreadyCollidedEntities;

      for(int j=0; j<currentEntityLeafs.size(); j++){
        QuadTree* currLeaf = currentEntityLeafs[j];

        // Iterate through other entities inside of this leaf
        // and check for collision with 'curr' rect.
        for(int k=0; k<currLeaf->points.size(); k++){
          *entitiesCheckedThisFrame = *entitiesCheckedThisFrame + 1;
          Entity* otherEntity = currLeaf->points[k];

          if(currentEntity->id != otherEntity->id){

            // Ensure otherEntity hasn't been collided with by this rect already.
            if(std::count(alreadyCollidedEntities.begin(), alreadyCollidedEntities.end(), otherEntity->id) == 0){
              bool collision = detectAndResolveCollision(currentEntity, otherEntity);
              if(collision) {
                *collisionsThisFrame = *collisionsThisFrame+1;
                alreadyCollidedEntities.push_back(otherEntity->id);
              }
            }
          }
        }
      }
    }
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

// TODO: dot is leftover from early version with pixels instead of rects.
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

